// ==========================================================================
//code=UTF8	tab=4
//
// Lutino:	Application SErver.
//
// 		ltn_String_file.cpp
//		$Revision: 1.0 $
//		$Date: 2018/02/12 21:11:00 $
//
// ==========================================================================
//---------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <assert.h>
#include <sys/types.h>
#include <vector>
#include <algorithm>
#include <limits>
#include <time.h>
#include <fstream>
#ifdef __linux__
#include <dirent.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <process.h>
#include "dirent.h"
#include <direct.h>
#include <io.h>
#endif
#include "ltn_String.h"
#include "ltn.h"
#include "ltn_tools.h"
#include "define.h"
#include "libnkf.hpp"
#ifndef __linux__
#include <ipmib.h>
#include <iphlpapi.h>
#endif

const char* MIME_TYPE_NAME[] = {
	"TYPE_UNKNOWN",
	"TYPE_DIRECTORY",
	"TYPE_MOVIE",
	"TYPE_MUSIC",
	"TYPE_IMAGE",
	"TYPE_DOCUMENT",
	"TYPE_PLAYLIST",
	"TYPE_MUSICLIST",
	"TYPE_URL",
	"TYPE_SCRIPT",
};

/// <summary>
/// ファイル読み込み
/// </summary>
/// <param name="str">読み込むファイルのフルファイル名</param>
int wString::load_from_file (const char* str)
{
	return load_from_file (wString (str));
}
//---------------------------------------------------------------------------
// ファイル読み込み
//---------------------------------------------------------------------------
wString wString::replace_env(const wString& path)
{
	//文字列から%%で囲まれた部分を抽出
	wString result = path;
	int start = 0;

	while ((start = result.find('%', start)) != wString::npos) {
		int end = result.find('%', start + 1);
		if (end == wString::npos) {
			break;  // 閉じる % がない場合は終了
		}

		wString varName = result.substr(start + 1, end - start - 1);
		char* ptr  = getenv(varName.c_str());
		wString varValue = ptr;

		if (varValue.length()) {
			varValue = varValue.nkfcnv("sW");
			result.replace(start, end - start + 1, varValue);
		}
		else {
			result.replace(start, end - start + 1, "");  // 未定義の環境変数は削除
		}
	}
	return result;
}

/// <summary>
/// ファイル読み込み
/// </summary>
/// <param name="FileName">ファイル名（フルパス）またはURL</param>
/// <returns>０</returns>
int wString::load_from_file (const wString& FileName)
{
	long flen;
	int  handle;
	if (FileName.starts_with ("http://") || FileName.starts_with ("https://")) {
		try {
			wString tmp = wString::http_get(FileName);
			*this = tmp;
		}
		catch (const HttpRequestException& e) {
			debug_log_output("%s(%d):load_from_file() HTTP error: %s", __FILE__, __LINE__, e.what());
			return -1;
		}
	}
	else {
		wString FileName2 = replace_env(FileName);
#ifdef __linux__
		handle = open (FileName2.c_str (), O_RDONLY | S_IREAD);
#else
		handle = myopen (FileName2, O_RDONLY | O_BINARY, S_IREAD);
#endif
		if (handle < 0) {
			debug_log_output ("%s(%d):load_from_file(%s) Error.", __FILE__, __LINE__, FileName2.c_str ());
			return -1;
		}
		flen = lseek (handle, 0, SEEK_END);
		lseek (handle, 0, SEEK_SET);
		set_length (flen + 1);
		len = read (handle, String, flen);
		close (handle);
		// アプリケーションレベルでやるべき
		//// BOM?
		//// 「BOM」は「バイトオーダーマーク」の略で、
		//// テキストファイルの文字コードが「UTF - 8」であることを示すものです。
		//// UTF - 8の場合、テキストデータの先頭に「0xEF 0xBB 0xBF」が付きます。
		//if (len >= 3) {
		//	if (this->starts_with ("\xef\xbb\xbf")) {
		//		// 先頭3バイトを削る
		//		len -= 3;
		//		memmove (String, String + 3, len);
		//	}
		//}
		String[len] = 0;
		//\0がある場合を考えればstrlenとってはいけない
		//len = strlen(String);
	}
	return 0;
}

/// <summary>
/// CSVファイル読み込み
/// </summary>
/// <param name="str">CSVファイル名</param>
void wString::load_from_csv (const char* str)
{
	load_from_csv (wString (str));
}

/// <summary>
/// CSVファイル読み込み
/// 文字コードはファイル依存
/// 1行目はタイトル行固定
/// </summary>
/// <param name="FileName">CSVファイル名</param>
bool wString::load_from_csv (const wString& FileName)
{
	int  fd;
	char s[1024] = {};
	char t[1024] = {};
	int first = 1;
	fd = myopen (FileName, O_RDONLY | O_BINARY, S_IREAD);
	if (fd < 0) {
		debug_log_output ("%s(%d):load_from_csv(%s) Error.", __FILE__, __LINE__, FileName.c_str ());
		return false;
	}

	wString work;

	work = "[";
	//１行目はタイトル
	while (true) {
		auto ret = readLineCSV (fd, s, sizeof (s));
		if (ret < 0) break;
		//分解する
		int ptr = 0;
		int ptr2 = 0;
		char* p;
		char* comma = const_cast <char*>("");
		while ((p = strtok_csv (s, ptr2)) != 0) {
			if (is_number (p)) {
				ptr += ::sprintf (t + ptr, "%s%s", comma, p);
			}
			else {
				// ""->\"
				// cr ->なし
				// lf ->"\n"
				ptr += ::sprintf (t + ptr, "%s\"%s\"", comma, p);
			}
			comma = const_cast <char*>(",");
		}
		if (first) {
			first = 0;
		}
		else {
			work += ",";
		}
		work += wString ("[") + t + "]";
	}
	work += "]";
	*this = work;
	close (fd);
	return true;
}
//---------------------------------------------------------------------------
// ファイル書き込み
//---------------------------------------------------------------------------
int wString::save_to_file (const char* str) const
{
	return save_to_file (wString (str));
}
//---------------------------------------------------------------------------
// ファイル書き込み
//---------------------------------------------------------------------------
int wString::save_to_file (const wString& FileName) const
{
	wString FileName2 = replace_env(FileName);
#ifdef __linux__
	int handle = myopen (FileName2, O_CREAT | O_TRUNC | O_RDWR, S_IREAD | S_IWRITE);
#else
	int handle = myopen (FileName2, O_CREAT | O_TRUNC | O_RDWR | O_BINARY, S_IREAD | S_IWRITE);
#endif
	if (handle < 0) {
		debug_log_output ("%s(%d):save_to_file(%s) Error.", __FILE__, __LINE__, FileName2.c_str ());
		return handle;
	}
	write (handle, String, len);
	close (handle);
	return 0;
}

//--------------------------------------------------------------------
/// <summary>
/// ファイル情報
/// </summary>
/// <param name="str">ファイル名</param>
/// <param name="mode">1:ファイル情報,0:日付のみ</param>
/// <returns>ファイル情報(JSON {"permission":permittion,"size":size,"date":date}) 存在しないときundefined</returns>
wString wString::file_stats (const char* str, int mode)
{
	struct stat      stat_buf;
	wString buf = "undefined";
	wString path = str;
	if (stat (path.nkfcnv ("Ws").c_str (), &stat_buf) == 0) {
		if (mode == 0) {
			/* ファイル情報を表示 */

#ifdef __linux__
			buf.sprintf ("{\"permission\":\"%o\",\"size\":%d,\"date\":\"%s\"}", stat_buf.st_mode, stat_buf.st_size, ctime (&stat_buf.st_mtime));
#else
			wString* time_data = ctimew (&stat_buf.st_mtime);
			buf.sprintf ("{\"permission\":\"%o\",\"size\":%d,\"date\":\"%s\"}", stat_buf.st_mode, stat_buf.st_size, time_data->c_str ());
			delete time_data;
#endif
			//printf("デバイスID : %d\n",stat_buf.st_dev);
			//printf("inode番号 : %d\n",stat_buf.st_ino);
			//printf("アクセス保護 : %o\n",stat_buf.st_mode );
			//printf("ハードリンクの数 : %d\n",stat_buf.st_nlink);
			//printf("所有者のユーザID : %d\n",stat_buf.st_uid);
			//printf("所有者のグループID : %d\n",stat_buf.st_gid);
			//printf("デバイスID（特殊ファイルの場合） : %d\n",stat_buf.st_rdev);
			//printf("容量（バイト単位） : %d\n",stat_buf.st_size);
			//printf("ファイルシステムのブロックサイズ : %d\n",stat_buf.st_blksize);
			//printf("割り当てられたブロック数 : %d\n",stat_buf.st_blocks);
			//printf("最終アクセス時刻 : %s",ctime(&stat_buf.st_atime));
			//printf("最終修正時刻 : %s",ctime(&stat_buf.st_mtime));
			//printf("最終状態変更時刻 : %s",ctime(&stat_buf.st_ctime));
		}
		else {
			//date
			if (mode == 1) {
				//char s[128] = {};
				//time_t timer;
				//struct tm *timeptr;
				//timer = time(NULL);
				//timeptr = localtime(&stat_buf.st_mtime);
				//strftime(s, 128, "%Y/%m/%d %H:%M:%S", timeptr);
				buf.sprintf ("%d", stat_buf.st_mtime);
			}
		}
	}
	else {
		debug_log_output ("%s(%d):file_stats(%s) Error.", __FILE__, __LINE__, str);
	}
	return buf;
}
//---------------------------------------------------------------------------

/// <summary>
/// ファイル情報
/// </summary>
/// <param name="str">ファイル名</param>
/// <param name="mode">1:ファイル情報,0:日付のみ</param>
/// <returns>ファイル情報(JSON {"permission":permittion,"size":size,"date":date}) 存在しないときundefined</returns>
wString wString::file_stats (const wString& str, int mode)
{
	return file_stats (str.String, mode);
}

//---------------------------------------------------------------------------
int wString::file_exists (const char* str)
{
#ifdef __linux__
	struct stat send_filestat;
	int  result = stat (str, &send_filestat);
	if ((result == 0) && (S_ISREG (send_filestat.st_mode) == 1)) {
		return 1;
	}
#else
	wString tmp (str);
	wString tmp2 = tmp.nkfcnv ("Ws");
	std::ifstream file (tmp2.String);
	if (file.is_open ()) {
		return 1;
	}
#endif
	//debug_log_output("%s(%d):file_exists(%s) Error.", __FILE__, __LINE__, tmp.c_str());
	return 0;
}
//---------------------------------------------------------------------------
int wString::file_exists (const wString& str)
{
	return file_exists (str.String);
}
//---------------------------------------------------------------------------
int wString::file_exists (void) const
{
	return file_exists (String);
}
//---------------------------------------------------------------------------
//パス部分を抽出
wString wString::extract_file_dir (const wString& str)
{
	//todo SJIS/EUC対応するように
	wString temp (str);
	int ptr = temp.last_delimiter (DELIMITER);
	temp.len = ptr;
	temp.String[ptr] = 0;
	return temp;
}
//---------------------------------------------------------------------------

/// <summary>
/// フォルダ作成
/// </summary>
/// <param name="str">utf-8フォルダ名</param>
/// <returns>成功:true 失敗:false</returns>
bool wString::create_dir (const wString& str)
{
	bool flag = false;
#ifdef __linux__
	//0x777ではちゃんとフォルダできない
	flag = (mkdir (str.String, 0777) != -1);
#else
	wString str2 (str);

	if (!directory_exists (str2)) {
		flag = (mkdir (str2.nkfcnv ("Ws").windows_file_name ().c_str ()) != -1);
		if (!flag) {
			debug_log_output ("%s(%d):create_dir(%s) Error.", __FILE__, __LINE__, str2.c_str ());
		}
	}
#endif
	return flag;
}
//---------------------------------------------------------------------------
bool wString::rename_file (const wString& src, const wString& dst)
{
	wString src2 = src;
	wString dst2 = dst;
#ifdef __linux__
#else
	src2 = src2.nkfcnv ("Ws");
	dst2 = dst2.nkfcnv ("Ws");
#endif
	if (rename (src2.c_str (), dst2.c_str ()) >= 0) {
		return true;
	}
	else {
		//debug_log_output("%s(%d):rename_file(%s,%s) Error.", __FILE__, __LINE__, src.c_str(),dst.c_str());
		return false;
	}
}
//---------------------------------------------------------------------------
unsigned long wString::file_size_by_name (char* str)
{
	unsigned long pos;
	int handle;
#ifdef __linux__
	handle = open (str, 0);
#else
	wString temp = str;
	handle = open (temp.nkfcnv ("Ws").c_str (), O_BINARY);
#endif
	pos = lseek (handle, 0, SEEK_END);
	close (handle);
	return pos;
}
//---------------------------------------------------------------------------
unsigned long wString::file_size_by_name (wString& str)
{
	return file_size_by_name (str.String);
}
//---------------------------------------------------------------------------
wString wString::extract_file_name (const char* str, const char* delim)
{
	wString tmp (str);
	return extract_file_name (tmp, delim);
}
//---------------------------------------------------------------------------
wString wString::extract_file_name (const wString& str, const char* delim)
{
	int pos = str.last_delimiter (delim);
	return str.substr (pos + 1);
}
//---------------------------------------------------------------------------
wString wString::extract_file_ext (const wString& str)
{
	int pos = str.last_delimiter (".");
	return str.substr (pos + 1);
}
//---------------------------------------------------------------------------
wString wString::extract_file_ext (void) const
{
	return extract_file_ext (*this);
}
/// <summary>
/// JSON {"mineType":"mimetype","fileType":"filetype"}が返還される。
/// </summary>
/// <param name="file_name">検査する</param>
/// <returns></returns>
wString wString::find_mime_type (const wString& file_name)
{
	int pos = file_name.last_delimiter (".");
	wString tmp = file_name.substr (pos + 1).ToLower ();
	if (tmp.length () > 0) {
		int ptr = 0;
		while (mime_list[ptr].mime_type != NULL) {
			if (tmp == mime_list[ptr].file_extension) {
				tmp.wString::sprintf ("{\"mimeType\":\"%s\",\"fileType\":\"%s\"}", mime_list[ptr].mime_type, MIME_TYPE_NAME[static_cast<int>(mime_list[ptr].menu_file_type)]);
				return tmp;
			}
			ptr++;
		}
	}
	return "";
}

//---------------------------------------------------------------------------
wString wString::change_file_ext (const wString& str, const char* ext)
{
	int pos = str.last_delimiter (".");
	return str.substr (0, pos + 1) + ext;
}
//---------------------------------------------------------------------------
int wString::delete_file (const wString& str)
{
	int flag = 0;
#ifdef __linux__
	flag = (unlink (str.String) == 0);
#else
	wString str2 = str;
	if (file_exists (str2)) {
		char work[2048];
		// MS932に変換して実行
		strcpy (work, str2.nkfcnv ("Ws").c_str ());
		windows_file_name (work);
		if (unlink (work) == 0) {
			flag = 1;
		}
	}
#endif
	if (flag == 0) {
		debug_log_output ("%s(%d):delete_file(%s) Error.", __FILE__, __LINE__, str.c_str ());
	}
	return flag;
}
//---------------------------------------------------------------------------
int wString::delete_folder (const wString& str)
{
	int flag = 0;
#ifdef __linux__
	flag = (rmdir (str.String) == 0);
#else
	wString str2 = str;
	// str2はUTF-8
	if (directory_exists (str2)) {
		char work[2048];
		// MS932に変換して実行
		strcpy (work, str2.nkfcnv ("Ws").c_str ());
		windows_file_name (work);
		if (rmdir (work) == 0) {
			flag = 1;
		}
	}
#endif
	debug_log_output ("%s(%d):delete_file(%s) Error.", __FILE__, __LINE__, str.c_str ());
	return flag;
}
//---------------------------------------------------------------------------
int wString::directory_exists (const char* str)
{
	int flag = 0;
#ifdef __linux__
	struct stat send_filestat;
	int  result = stat (str, &send_filestat);
	if ((result == 0) && (S_ISDIR (send_filestat.st_mode) == 1)) {
		flag = 1;
	}
#else

	//char tmp[1024];
	// TODO SJIS変換
	wString tmp (str);
	wString tmp2 = tmp.nkfcnv ("Ws");

	//strcpy(tmp, str);
	// フォルダの末尾は'/'以外
	if (tmp2.ends_with ("/")) {
		tmp2 = tmp2.substr (0, tmp2.len - 1);
	}
	WIN32_FIND_DATAA fd; //検索データ
	HANDLE result = ::FindFirstFileA (tmp2.String, &fd); //最初の検索で使用する関数
	if (result != INVALID_HANDLE_VALUE) {
		if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
			flag = 1;
		}
		FindClose (result); //ハンドルを閉じる
	}
#endif
	//if (flag == 0)
	//{
	//	debug_log_output("%s(%d):directory_exists(%s) Error.", __FILE__, __LINE__, tmp2.c_str());
	//}
	return flag;
}
//---------------------------------------------------------------------------
int wString::directory_exists (const wString& str)
{
	return directory_exists (str.String);
}
///---------------------------------------------------------------------------
/// <summary>
/// フォルダ内のファイルを列挙する
/// </summary>
/// <param name="Path">フォルダパス</param>
/// <returns>一覧のJSON文字列。コピーしてほしい</returns>
wString wString::enum_folder_json (const wString& Path)
{
#ifdef __linux__
	struct dirent** namelist;
	int n;
	wString temp;
	wString Path2;
	Path2 = Path;
	int first = 1;
	//Directoryオープン
	n = scandir (Path.String, &namelist, NULL, alphasort);
	if (n >= 0) {
		temp = "[";
		for (int i = 0; i < n; i++) {
			const char* name = namelist[i]->d_name;
			if (strcmp (name, ".") != 0 && strcmp (name, "..") != 0) {
			if (first) {
				first = 0;
			}
			else {
				temp += ",";
			}
				temp += "\"" + Path2 + DELIMITER + name + "\"";
			}
			free (namelist[i]);
		}
		temp += "]";
		free (namelist);
	}
	else {
		throw new CScriptException ("Directory Open Error.[" + Path2 + "]");
		//perror ("ディレクトリのオープンエラー");
		//exit (1);
	}
	return temp;
#else
	DIR* dir;
	wString              temp;
	wString              Path2;

	Path2 = Path;
	Path2 = Path2.nkfcnv ("Ws");
	//Directoryオープン
	if ((dir = opendir (Path2.String)) != NULL) {
		struct dirent* ent;
		std::vector<wString> list;
		//ファイルリスト
		while ((ent = readdir (dir)) != NULL) {
			if (strcmp (ent->d_name, ".") != 0 &&
				strcmp (ent->d_name, "..") != 0) {
				list.push_back ("\"" + Path2 + DELIMITER + ent->d_name + "\"");
			}
		}
		closedir (dir);

		//sort
		if (list.size () > 0) {
			for (unsigned int ii = 0; ii < list.size () - 1; ii++) {
				for (unsigned int jj = ii + 1; jj < list.size (); jj++) {
					if (strcmp (list[ii].c_str (), list[jj].c_str ()) > 0) {
						std::swap (list[ii], list[jj]);
					}
				}
			}
		}        temp = "[";
		for (unsigned int i = 0; i < list.size (); i++) {
			if (i) temp += ",";
			temp += list[i];
		}
		temp += "]";
	}
	else {
		throw new CScriptException ("Directory Open Error.Full Path Required.[" + Path2 + "]");
		//perror ("ディレクトリのオープンエラー");
		//exit (1);
	}
	wString temp2 = temp.nkfcnv ("Sw");
	return temp2;
#endif
}

wString wString::get_current_dir ()
{
	char cwd[FILENAME_MAX];
	if (getcwd (cwd, sizeof (cwd)) == NULL) {
		debug_log_output ("get_current_dir: error %s", strerror (errno));
		throw new CScriptException ("get_current_dir");
		//exit(-1);
	}
#ifdef __linux__
	return wString (cwd);
#else
	return wString (cwd).nkfcnv ("Sw");
#endif
}

///---------------------------------------------------------------------------
/// <summary>
/// フォルダ内のファイルを列挙する
/// </summary>
/// <param name="Path">フォルダパス</param>
/// <returns>一覧のJSON文字列。コピーしてほしい</returns>
wString wString::enum_folder (const wString& Path)
{
#ifdef __linux__
	struct dirent** namelist;
	int n;
	wString temp;
	wString Path2;
	Path2 = Path;
	int first = 1;

	n = scandir (Path.String, &namelist, NULL, alphasort);
	if (n < 0) {
		temp = "";
	}
	else
	{
		temp = "[";
		for (int i = 0; i < n; i++) {
			if (first) {
				first = 0;
			}
			else {
				temp += ",";
			}
			temp += "\"" + Path2 + DELIMITER + namelist[i]->d_name + "\"";
			free (namelist[i]);
		}
		temp += "]";
		free (namelist);
	}
#else
	DIR* dir;
	wString              temp;
	wString              Path2;
	Path2 = Path;
	Path2 = Path2.nkfcnv ("Ws");
	//Directoryオープン
	if ((dir = opendir (Path2.String)) != NULL) {
		struct dirent* ent;
		std::vector<wString> list;
		//ファイルリスト
		while ((ent = readdir (dir)) != NULL) {
			if (strcmp (ent->d_name, ".") != 0 &&
				strcmp (ent->d_name, "..") != 0) {
				list.push_back (Path2 + DELIMITER + ent->d_name);
			}
		}
		closedir (dir);
		if (list.size () > 0) {
			for (unsigned int ii = 0; ii < list.size () - 1; ii++) {
				for (unsigned int jj = ii + 1; jj < list.size (); jj++) {
					if (strcmp (list[ii].c_str (), list[jj].c_str ()) > 0) {
						std::swap (list[ii], list[jj]);
					}
				}
			}
		}
		for (unsigned int i = 0; i < list.size (); i++) {
			temp.Add (list[i]);
		}
	}
	else {
		throw new CScriptException ("Directory Open Error.[" + Path2 + "]");
		//perror ("ディレクトリのオープンエラー");
		//exit (1);
	}
#endif
	wString temp2 = temp.nkfcnv ("Sw");
	return temp2;
}

/// <summary>
/// ファイルコピー
/// </summary>
/// <param name="fname_r">ファイルコピー元</param>
/// <param name="fname_w">ファイルコピー先</param>
/// <returns>成功:0,失敗:-1</returns>
int   wString::FileCopy (const wString& fname_r, const wString& fname_w)
{
	constexpr auto FILE_BUFFER = 8000;
	int fpr;
	int fpw;
	unsigned char buf[FILE_BUFFER];

	fpr = myopen (fname_r, O_RDONLY | O_BINARY, S_IREAD);
	if (fpr < 0) {
		return -1;
	}
	fpw = myopen (fname_w, O_CREAT | O_TRUNC | O_WRONLY | O_BINARY, S_IREAD | S_IWRITE);
	if (fpw < 0) {
		close (fpr);
		return -1;
	}
	while (1) {
		auto vsize = read (fpr, buf, sizeof (buf));
		if (vsize <= 0) {
			break;
		}
		write (fpw, buf, vsize);
	}
	close (fpr);
	close (fpw);

	return 0;
}

bool wString::path_sanitize()
{
	if (empty()) {
		*this = "/";
		return true;
	}

	wString drive;   // Windows の "C:" を保持する
	int pos = 0;
	const size_t n = this->size();

	// --- Windows ドライブレター判定 ---
	// 例: "C:/Users/user"
	if (n >= 2 && (*this)[1] == ':' &&
		(((*this)[0] >= 'A' && (*this)[0] <= 'Z') ||
			((*this)[0] >= 'a' && (*this)[0] <= 'z')))
	{
		drive = this->substr(0, 2);  // "C:"
		pos = 2;

		// ドライブの後が "/" ならスキップ
		if (pos < n && (*this)[pos] == '/')
			pos++;
	}

	// --- パス部分の正規化 ---
	std::vector<wString> stack;

	while (pos < n) {
		// skip leading '/'
		while (pos < n && (*this)[pos] == '/') pos++;

		if (pos >= n) break;

		// extract one component
		size_t start = pos;
		while (pos < n && (*this)[pos] != '/') pos++;
		wString part = this->substr(start, pos - start);

		if (part == "..") {
			if (stack.empty()) {
				// Windows の場合は "C:" があるのでそこまでは戻れる
				if (!drive.empty()) {
					// 例: C:/.. → C:
					// ただし C: から上には行けない
					// stack が空なら何もしない
					continue;
				}

				// Linux の場合は不正
				this->clear();
				return false;
			}
			stack.pop_back();
		}
		else if (part != ".") {
			stack.push_back(part);
		}
	}

	// --- 再構築 ---
	wString out;

	if (!drive.empty()) {
		out = drive;  // "C:"
	}

	if (stack.empty()) {
		// Windows: "C:/"
		// Linux:   "/"
		out += "/";
	}
	else {
		for (auto& s : stack) {
			out += "/";
			out += s;
		}
	}

	*this = out;
	return true;
}


//---------------------------------------------------------------------------
// linux用ファイル名に変換
//---------------------------------------------------------------------------

/// <summary>
/// Windows用パス名に変更(副作用あり）
/// </summary>
/// <param name="FileName">変換するパス名</param>
/// <returns>変換した文字列</returns>
char* wString::windows_file_name (char* FileName)
{
#ifdef __linux__
	return FileName;
#else
	char* work = FileName;
	int ptr = 0;
	while (work[ptr]) {
		if (work[ptr] == '/') {
			work[ptr] = '\\';
		}
		ptr++;
	}
	return work;
#endif
}
/// <summary>
/// Windows用パス名に変更(副作用あり）
/// </summary>
/// <param name="FileName">変換するパス名</param>
/// <returns>変換した文字列</returns>
wString wString::windows_file_name ()
{
#ifdef __linux__
	return *this;
#else

	//char* work = FileName;
	for (auto i = 0U; i < len; i++) {
		if (String[i] == '/') {
			String[i] = '\\';
		}
	}
	return *this;
#endif
}
/// <summary>
/// Linux用パス名に変換（自データを変更）
/// </summary>
/// <param name="FileName">変換するパス名</param>
/// <returns>変換した文字列</returns>
char* wString::linux_file_name (char* FileName)
{
#ifdef __linux__
	return FileName;
#else
	char* work = FileName;
	int ptr = 0;
	while (work[ptr]) {
		if (work[ptr] == '\\') {
			work[ptr] = '/';
		}
		ptr++;
	}
	return work;
#endif
}

//////////////////////////////////////////////////////////////////////////
/// <summary>
/// PNGフォーマットファイルから、画像サイズを得る。
/// </summary>
/// <param name="png_filename"></param>
/// <returns>json:{"x":</returns>
wString wString::png_size (const wString& png_filename)
{
	int         fd;
	unsigned char       buf[255] = {};
	ssize_t     read_len;
	auto x = 0;
	auto y = 0;
	fd = myopen (png_filename, O_BINARY | O_RDONLY);
	if (fd < 0) {
		return "null";
	}
	// ヘッダ+サイズ(0x18byte)  読む
	//memset(buf, 0, sizeof(buf));
	read_len = read (fd, reinterpret_cast<char*>(buf), 0x18);
	if (read_len == 0x18) {
		x = (buf[0x10] << 24) +
			(buf[0x11] << 16) +
			(buf[0x12] << 8) +
			(buf[0x13]);
		y = (buf[0x14] << 24) +
			(buf[0x15] << 16) +
			(buf[0x16] << 8) +
			(buf[0x17]);
	}
	close (fd);
	wString tmp;
	tmp.sprintf ("{\"x\":%d,\"y\":%d}", x, y);
	return tmp;
}
/// <summary>
///  GIFフォーマットファイルから、画像サイズを得る
/// </summary>
/// <param name="gif_filename"></param>
/// <returns></returns>
wString wString::gif_size (const wString& gif_filename)
{
	int         fd;
	unsigned char       buf[255] = {};
	ssize_t     read_len;
	auto x = 0;
	auto y = 0;
	fd = myopen (gif_filename, O_BINARY | O_RDONLY);
	if (fd < 0) {
		return "null";
	}
	// ヘッダ+サイズ(10byte)  読む
	//memset(buf, 0, sizeof(buf));
	read_len = read (fd, reinterpret_cast<char*>(buf), 10);
	if (read_len == 10) {
		x = buf[6] + (buf[7] << 8);
		y = buf[8] + (buf[9] << 8);
	}
	close (fd);
	wString tmp;
	tmp.sprintf ("{\"x\":%d,\"y\":%d}", x, y);
	return tmp;
}

/// <summary>
///  JPEGフォーマットファイルから、画像サイズを得る。
/// </summary>
/// <param name="jpeg_filename"></param>
/// <returns></returns>
wString wString::jpeg_size (const wString& jpeg_filename)
{
	int                 fd;
	unsigned char       buf[255];
	off_t               vlength;
	auto x = 0;
	auto y = 0;
	//debug_log_output("jpeg_size: '%s'.", jpeg_filename);
	fd = myopen (jpeg_filename, O_BINARY | O_RDONLY);
	if (fd < 0) {
		return "null";
	}
	while (1) {
		// マーカ(2byte)  読む
		auto read_len = read (fd, buf, 2);
		if (read_len != 2) {
			//debug_log_output("fraed() EOF.\n");
			break;
		}
		// Start of Image.
		if ((buf[0] == 0xFF) && (buf[1] == 0xD8)) {
			continue;
		}
		// Start of Frame 検知
		if ((buf[0] == 0xFF) && (buf[1] >= 0xC0) && (buf[1] <= 0xC3)) { // SOF 検知
			//debug_log_output("SOF0 Detect.");
			// sof データ読み込み
			memset (buf, 0, sizeof (buf));
			read_len = read (fd, buf, 0x11);
			if (read_len != 0x11) {
				debug_log_output ("fraed() error.\n");
				break;
			}
			y = (buf[3] << 8) + buf[4];
			x = (buf[5] << 8) + buf[6];
			break;
		}
		// SOS検知
		if ((buf[0] == 0xFF) && (buf[1] == 0xDA)) { // SOS 検知
			//debug_log_output("Start Of Scan.\n");
			// 0xFFD9 探す。
			while (1) {
				// 1byte 読む
				read_len = read (fd, buf, 1);
				if (read_len != 1) {
					//debug_log_output("fraed() error.\n");
					break;
				}
				// 0xFFだったら、もう1byte読む
				if (buf[0] == 0xFF) {
					buf[0] = 0;
					if (read (fd, buf, 1) != 1) {
						break;
					}
					// 0xD9だったら 終了
					if (buf[0] == 0xD9) {
						//debug_log_output("End Of Scan.\n");
						break;
					}
				}
			}
			continue;
		}
		// length 読む
		memset (buf, 0, sizeof (buf));
		if (read (fd, buf, 2) != 2) {
			close (fd);
			return "";
		}
		vlength = (buf[0] << 8) + buf[1];
		// length分とばす
		lseek (fd, vlength - 2, SEEK_CUR);
	}
	close (fd);
	wString tmp;
	tmp.sprintf ("{\"x\":%d,\"y\":%d}", x, y);
	return tmp;
}
