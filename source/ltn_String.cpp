// ==========================================================================
//code=UTF8	tab=4
//
// Lutino:	Application SErver.
//
// 		ltn_String.cpp
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
//#include <dir.h>
#include <direct.h>
#include <io.h>
#endif
#include "ltn_String.h"
#include "ltn.h"
#include "ltn_tools.h"
#include <time.h>
#include "define.h"
#include <fstream>
#include "libnkf.hpp"
#ifndef __linux__
#include <ipmib.h>
#include <iphlpapi.h>
#endif
#ifndef strrstr
///---------------------------------------------------------------------------
/// <summary>
/// strrstr()
///   文字列 から 文字列 を検索する。
///   最後 に現れた文字列の先頭を指す ポインタ を返す。
///   文字列が見つからない場合は null を返す。
///   pattern が 空文字列 の場合は常に 文字列終端 を返す。
///   文字列終端文字 '\0' は検索対象とならない。
///   半角英字 の 大文字と小文字 を区別する。
/// </summary>
/// <param name="string">検索対象となる文字列</param>
/// <param name="pattern">文字列から検索する文字列</param>
/// <returns></returns>
static char* strrstr (const char* string, const char* pattern)
{
	// 文字列終端に達するまで検索を繰り返す。
	const char* last = NULL;
	for (const char* p = string; ; p++) {
		if (0 == *p) {
			return (char*)last;
		}
		p = strstr (p, pattern);
		if (p == NULL) {
			break;
		}
		last = p;
	}
	return (char*)last;
}//strrstr
#endif

/// <summary>検索失敗時の値</summary>
const int wString::npos = (int)(-1);
///---------------------------------------------------------------------------
/// <summary>
/// 文字列クラス　ディフォールトコンストラクタ
/// </summary>
wString::wString (void)
{
	//初期化
	len = 0;
	capa = 1;
	String = static_cast<char*>(new char[1]);
	*String = 0;
}
///---------------------------------------------------------------------------
/// <summary>
/// 文字列クラス文字数指定（内容は０）
/// </summary>
/// <param name="mylen">初期文字数指定</param>
wString::wString (int mylen)
{
	//初期化
	if (mylen < 0) mylen = 0;
	len = 0;
	//末尾０の分
	capa = mylen + 1;
	String = static_cast<char*>(new char[mylen + 1]);
	*String = 0;
}
///---------------------------------------------------------------------------
/// <summary>
/// 文字列クラス文字列コンストラクタ
/// </summary>
/// <param name="str">元となる文字列</param>
wString::wString (const char* str)
{
	//初期化
	len = static_cast<unsigned int>(strlen (str));
	if (len) {
		capa = len + 1;
		String = static_cast<char*>(new char[capa]);
		strcpy (String, str);
	}
	else {
		capa = 1;
		String = static_cast<char*>(new char[1]);
		String[0] = 0;
	}
}
///---------------------------------------------------------------------------
/// <summary>
/// コピーコンストラクタ
/// </summary>
/// <param name="str">コピーする文字列</param>
wString::wString (const wString& str)
{
	//初期化
	len = str.len;
	if (str.len) {
		capa = str.capa;
		String = new char[str.capa];
		memcpy (String, str.String, str.capa);
		//*String = 0;
	}
	else {
		capa = 1;
		String = static_cast<char*>(new char[1]);
		String[0] = 0;
	}
}
///---------------------------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
wString::~wString ()
{
	delete[] String;
}
///---------------------------------------------------------------------------
/// <summary>
/// プラスメソッドa+b
/// </summary>
/// <param name="str">プラスするwString</param>
/// <returns>プラス結果</returns>
wString wString::operator+(const wString& str) const
{
	wString temp (*this);
	temp += str;
	return temp;
}
///---------------------------------------------------------------------------
/// <summary>
/// プラスメソッドa+b
/// </summary>
/// <param name="str">プラスする文字列</param>
/// <returns>プラス結果</returns>
wString wString::operator+(const char* str) const
{
	wString temp (*this);
	temp += str;
	return temp;
}
///---------------------------------------------------------------------------
/// <summary>
/// プラスメソッド+(a,b)
/// </summary>
/// <param name="str1">元文字列</param>
/// <param name="str2">プラスするwString</param>
/// <returns>プラス結果</returns>
wString operator+(const char* str1, const wString& str2)
{
	wString temp (str1);
	temp += str2;
	return temp;
}
///---------------------------------------------------------------------------
/// <summary>
/// プラスメソッドa+=b
/// </summary>
/// <param name="str">プラスするwString</param>
void wString::operator+=(const wString& str)
{
	unsigned int newLen = len + str.len;
	resize (newLen + 1);
	memcpy (String + len, str.String, str.len);
	String[newLen] = 0;
	len = newLen;
	return;
}
///---------------------------------------------------------------------------
/// <summary>
/// プラスメソッドa+=b
/// </summary>
/// <param name="str">プラスする文字列</param>
void wString::operator+=(const char* str)
{
	auto slen = static_cast<unsigned int>(strlen (str));
	auto newLen = slen + len;
	resize (newLen);
	strcpy (String + len, str);
	len = newLen;
	return;
}
///---------------------------------------------------------------------------
/// <summary>
/// バイナリ文字列の設定a.set_binary(mem,len)
/// </summary>
/// <param name="mem">設定バイナリ</param>
/// <param name="addLen">設定長さ</param>
void wString::set_binary (const void* mem, int binaryLength)
{
	auto newLen = binaryLength + len;
	resize (newLen);
	memcpy (static_cast<void*>(String + len), mem, binaryLength);
	len = newLen;
	return;
}
///---------------------------------------------------------------------------
/// <summary>
/// 先頭文字列の比較（バイナリ非対応）
/// </summary>
/// <param name="needle">検索する文字列</param>
/// <param name="pos">検査開始位置</param>
/// <returns>先頭と一致すればtrue</returns>
bool wString::starts_with (const char* needle, int pos) const
{
	auto nlen = strlen (needle);
	if (nlen == 0 || nlen > len) {
		return false;
	}
	if (pos >= 0) {
		if (pos >= len - nlen + 1) {
			return false;
		}
		return memcmp (String + pos, needle, nlen) == 0;
	}
	else {
		return memcmp(String, needle, nlen) == 0;
	}
}
//---------------------------------------------------------------------------
/// <summary>
/// 末尾文字列の比較
/// 非バイナリ
/// </summary>
/// <param name="needle">検索する文字列</param>
/// <returns>先頭と一致すればtrue</returns>
bool wString::ends_with (const char* needle, int end_len) const
{
	/// TODO:end_lenは無意味
	int temp_len = len;
	if (end_len >= 0) {
		if (end_len == 0 || end_len > temp_len) {
			return false;
		}
		//temp_len = end_len;
	}
	auto nlen = static_cast<unsigned int>(strlen (needle));
	if (nlen == 0 || nlen > len) {
		return false;
	}
	return memcmp(String + len - nlen, needle,nlen) == 0;
}
///---------------------------------------------------------------------------
/// <summary>
/// 文字列に1バイト文字加算
/// バイナリ対応
/// </summary>
/// <param name="ch">加算する1バイト文字</param>
void wString::operator+=(const char ch)
{
	int tmpl = ((len >> 4) + 1) << 4;
	resize (tmpl);
	String[len++] = ch;
	String[len] = 0;
	return;
}
///---------------------------------------------------------------------------
/// <summary>
/// wStringとwStringの比較
/// 非バイナリ
/// </summary>
/// <param name="str">比較する文字列</param>
/// <returns>ture:同一、false:異なる場合</returns>
bool wString::operator==(const wString& str) const
{
	if (len != str.len) {
		return false;
	}
	return(strncmp (String, str.String, len) == 0);
}
///---------------------------------------------------------------------------
/// <summary>
/// 文字列の比較
/// 非バイナリ
/// </summary>
/// <param name="str">比較する文字列</param>
/// <returns>true:同一、false:不一致</returns>
bool wString::operator==(const char* str) const
{
	auto mylen = static_cast<unsigned int>(strlen (str));
	if (len != mylen) {
		return false;
	}
	return(strncmp (String, str, len) == 0);
}
///---------------------------------------------------------------------------
/// <summary>
/// 文字列の比較
/// 非バイナリ
/// </summary>
/// <param name="str">比較する文字列</param>
/// <returns>true:不一致、false:一致</returns>
bool wString::operator!=(const wString& str) const
{
	if (len != str.len) {
		return true;
	}
	return(strncmp (String, str.String, len) != 0);
}
//---------------------------------------------------------------------------
bool wString::operator!=(const char* str) const
{
	if (len != strlen (str)) {
		return true;
	}
	return(strncmp (String, str, len) != 0);
}
//---------------------------------------------------------------------------
bool wString::operator>=(const wString& str) const
{
	unsigned int maxlen = (len > str.len) ? len : str.len;
	return(strncmp (String, str.String, maxlen) >= 0);
}
//---------------------------------------------------------------------------
bool wString::operator>=(const char* str) const
{
	auto mylen = static_cast<unsigned int>(strlen (str));
	auto maxlen = (len > mylen) ? len : mylen;
	return(strncmp (String, str, maxlen) >= 0);
}
//---------------------------------------------------------------------------
bool wString::operator<=(const wString& str) const
{
	unsigned int maxlen = (len > str.len) ? len : str.len;
	return(strncmp (String, str.String, maxlen) <= 0);
}
//---------------------------------------------------------------------------
bool wString::operator<=(const char* str) const
{
	auto mylen = static_cast<unsigned int>(strlen (str));
	auto maxlen = (len > mylen) ? len : mylen;
	return(strncmp (String, str, maxlen) <= 0);
}
//---------------------------------------------------------------------------
bool wString::operator>(const wString& str) const
{
	auto maxlen = (len > str.len) ? len : str.len;
	return(strncmp (String, str.String, maxlen) > 0);
}
//---------------------------------------------------------------------------
bool wString::operator>(const char* str) const
{
	auto mylen = static_cast<unsigned int>(strlen (str));
	auto maxlen = (len > mylen) ? len : mylen;
	return(strncmp (String, str, maxlen) > 0);
}
//---------------------------------------------------------------------------
bool wString::operator<(const wString& str) const
{
	auto maxlen = (len > str.len) ? len : str.len;
	return(strncmp (String, str.String, maxlen) < 0);
}
//---------------------------------------------------------------------------
bool wString::operator<(const char* str) const
{
	auto mylen = static_cast<unsigned int>(strlen (str));
	auto maxlen = (len > mylen) ? len : mylen;
	return(strncmp (String, str, maxlen) < 0);
}
//---------------------------------------------------------------------------
void wString::operator=(const wString& str)
{
	resize (str.capa);
	memcpy (String, str.String, str.capa);
	len = str.len;
	return;
}
//---------------------------------------------------------------------------
void wString::operator=(const char* str)
{
	auto newLen = static_cast<unsigned int>(strlen (str));
	resize (newLen);
	strcpy (String, str);
	len = newLen;
	return;
}
///---------------------------------------------------------------------------
/// <summary>
/// 整数の文字列化
/// </summary>
/// <param name="num">文字列にする整数</param>
void wString::operator=(const int num)
{
	this->sprintf ("%d", num);
	return;
}
///---------------------------------------------------------------------------
/// <summary>
/// 実数の文字列化
/// </summary>
/// <param name="num">文字列にする実数</param>
void wString::operator=(const double num)
{
	this->sprintf ("%f", num);
	return;
}
///---------------------------------------------------------------------------
/// <summary>
/// 文字列の配列アクセス
/// バイナリ対応
/// </summary>
/// <param name="index">参照する添え字</param>
/// <returns>参照した1バイトの値</returns>
char wString::operator[](int index) const
{
	if (index >= 0 && index < static_cast<int>(len)) {
		return String[index];
	}
	else {
		throw new CScriptException ("Out bound.Operator[].");
		//perror("out bound");
		//return 0;
	}
}

/// <summary>
/// 文字列の配列アクセス左辺値
/// </summary>
/// <param name="index">参照する添え字</param>
/// <returns>参照した1バイトの実体</returns>
char& wString::operator[](int index)
{
	return String[index];
}

///---------------------------------------------------------------------------
/// <summary>
/// 文字列の位置指定アクセス
/// </summary>
/// <param name="index">参照する添え字</param>
/// <returns>参照した1バイトの値</returns>
char wString::at (unsigned int index) const
{
	if (index < len) {
		return String[index];
	}
	else {
		throw new CScriptException ("Out bound.at().");
		//perror ("out bound");
		//return -1;
	}
}
//---------------------------------------------------------------------------
wString& wString::set_length (const unsigned int num)
{
	resize (num);
	return *this;
}
//---------------------------------------------------------------------------
// 比較
//---------------------------------------------------------------------------
int wString::compare (const wString& str) const
{
	return strcmp (String, str.String);
}
//---------------------------------------------------------------------------
// 比較
//---------------------------------------------------------------------------
int wString::compare (const char* str) const
{
	return strcmp (String, str);
}
//---------------------------------------------------------------------------
// クリア
//---------------------------------------------------------------------------

/// <summary>
/// 文字列のクリア
/// 設定した領域は解放しない
/// バイナリ対応
/// </summary>
void  wString::clear (void)
{
	len = 0;
	if (String != nullptr) {
		*String = 0;
	}
}
//---------------------------------------------------------------------------
// 部分文字列
//---------------------------------------------------------------------------

///// <summary>
///// 部分文字列
///// バイナリ対応
///// </summary>
///// <param name="start">開始位置(0スタート)</param>
///// <param name="mylen">指定バイト数。0以下で残り全文字列</param>
///// <returns>指定した文字列</returns>
//wString  wString::SubString(int start, int mylen) const
//{
//	// TODO エラーチェック
//	if (start < 0 || start >= static_cast<int>(len)) {
//		//throw error
//	}
//	if (mylen <= 0 || start + mylen > (int)len) {
//		mylen = (int)len - start;
//		// if( mylen < 0 )
//	}
//	wString temp(mylen);
//	if (mylen > 0) {
//		memcpy(temp.String, String + start, mylen);
//		temp.String[mylen] = 0;
//		//長さ不定。数えなおす
//		temp.len = mylen;
//	}
//	return temp;
//}
//---------------------------------------------------------------------------
// substr
//---------------------------------------------------------------------------
/// <summary>
/// 部分文字列抽出
/// バイナリ対応
/// </summary>
/// <param name="start">開始バイト</param>
/// <param name="cut_len">切り取りバイト数</param>
/// <returns>抽出した文字列</returns>
wString wString::substr (int start, int cut_len) const
{
	// TODO エラーチェック
	if (start < 0 || start >= static_cast<int>(len)) {
		// TODO:throw error
	}
	if (cut_len < 0) cut_len = len;
	if (start + cut_len > (int)len) {
		cut_len = (int)len - start;
	}

	wString temp (cut_len);
	if (cut_len > 0) {
		memcpy (temp.String, String + start, cut_len);
		temp.String[cut_len] = 0;
		//長さ不定。数えなおす
		temp.len = cut_len;
	}
	return temp;
}
//---------------------------------------------------------------------------
// 位置
//---------------------------------------------------------------------------
int wString::find (const wString& str, int index) const
{
	const char* ptr = strstr (String + index, str.String);
	if (ptr == NULL) {
		return npos;
	}
	else {
		return static_cast<int>((ptr - String));
	}
}
///---------------------------------------------------------------------------
/// 位置
///---------------------------------------------------------------------------
/// <summary>
/// 文字列の検索
/// </summary>
/// <param name="ch">検索文字列</param>
/// <param name="index">検索開始位置</param>
/// <returns>見つかった場合先頭からの文字位置（0スタート)、見つからなかった場合wString:npos</returns>
int wString::find (const char* str, int index) const
{
	const char* ptr = strstr (String + index, str);
	if (ptr == NULL) {
		return npos;
	}
	else {
		return static_cast<int>((ptr - String));
	}
}
/// <summary>
/// 文字の検索
/// </summary>
/// <param name="ch">検索文字</param>
/// <param name="index">検索開始位置</param>
/// <returns>見つかった場合先頭からの文字位置（0スタート)、見つからなかった場合wString:npos</returns>
int wString::find (char ch, int index) const
{
	const char* ptr = strchr (String + index, ch);
	if (ptr == NULL) {
		return npos;
	}
	else {
		return static_cast<int>((ptr - String));
	}
}
/// <summary>
/// 行末からの文字列の検索
/// </summary>
/// <param name="ch">検索文字</param>
/// <param name="index">検索開始位置</param>
/// <returns>見つかった場合先頭からの文字位置（0スタート)、見つからなかった場合wString:npos</returns>
int wString::rfind (const wString& str, int index) const
{
	const char* ptr = strrstr (String + index, str.String);
	if (ptr == NULL) {
		return npos;
	}
	else {
		return static_cast<int>((ptr - String));
	}
}
///---------------------------------------------------------------------------
/// <summary>
/// 行末からの検索
/// </summary>
/// <param name="str">検索文字列</param>
/// <param name="index">検索開始位置(省略可能)</param>
/// <returns>成功：文字位置 失敗:wString::npos</returns>
int wString::rfind (const char* str, int index) const
{
	const char* ptr = strrstr (String + index, str);
	if (ptr == NULL) {
		return npos;
	}
	else {
		return static_cast<int>((ptr - String));
	}
}
///---------------------------------------------------------------------------
/// <summary>
/// 行末からの検索
/// </summary>
/// <param name="ch">検索文字</param>
/// <param name="index">検索開始位置(省略可能)</param>
/// <returns>成功：文字位置 失敗:wString::npos</returns>
int wString::rfind (char ch, int index) const
{
	const char* ptr = strrchr (String + index, ch);
	if (ptr == NULL) {
		return npos;
	}
	else {
		return static_cast<int>((ptr - String));
	}
}

/********************************************************************************/
// sentence文字列内のunique_charが連続しているところを、unique_char1文字だけにする。
/********************************************************************************/
void wString::duplex_character_to_unique(char unique_char)
{
	for(int i = 0; i < len - 1; i++) {
		if (String[i] == unique_char && String[i + 1] == unique_char) {
			// 連続している場合、後ろの文字を削除
			memmove(String + i + 1, String + i + 2, len - i - 1);
			len--;
			String[len] = '\0'; // 終端文字を更新
			i--; // 次の文字もチェックするためにインデックスを戻す
		}
	}
	//wString fromStr;
	//wString toStr;
	//fromStr = unique_char;
	//fromStr += unique_char;
	//toStr = unique_char;
	//replace_character(fromStr, toStr);
	return;
}
//---------------------------------------------------------------------------
// 位置
//---------------------------------------------------------------------------
//int wString::Pos(const char* pattern)
//{
//    char* ptr = strstr(String,pattern);
//    if( ptr == NULL ){
//        return npos;
//    }else{
//        return (int)(ptr-String);
//    }
//}
//---------------------------------------------------------------------------
// 位置
//---------------------------------------------------------------------------

/// <summary>
/// 文字列中の位置
/// </summary>
/// <param name="pattern">存在を検査する文字列</param>
/// <param name="pos">検査開始位置</param>
/// <returns>見つからない場合wString::npos,存在する場合、先頭を０とした位置情報</returns>
int wString::Pos (const char* pattern, int pos) const
{
	const char* ptr = strstr (String + pos, pattern);
	if (ptr == NULL) {
		return npos;
	}
	else {
		return (int)(ptr - String);
	}
}

//---------------------------------------------------------------------------
// 位置
//---------------------------------------------------------------------------
int wString::Pos (const wString& pattern, int pos) const
{
	return Pos (pattern.String, pos);
}
//---------------------------------------------------------------------------
//　Size
//---------------------------------------------------------------------------

/// <summary>
/// 文字長
/// </summary>
/// <returns>文字列の長さ</returns>
unsigned int wString::size (void) const
{
	return len;
}

/// <summary>
/// 文字長
/// </summary>
/// <param name=""></param>
/// <returns>文字列の長さ</returns>
unsigned int wString::length (void) const
{
	return len;
}

/// <summary>
/// 文字列が空かどうか判定
/// </summary>
/// <param name=""></param>
/// <returns>空の場合true、それ以外false</returns>
bool wString::empty (void) const
{
	return len == 0;
}

/// <summary>
/// 文字列の数値判断(0123456789.-+)
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
int wString::is_number (const char* str)
{
	for (auto i = static_cast<int>(strlen (str)) - 1; i >= 0; i--) {
		auto ch = static_cast<unsigned char>(str[i]);
		if ((!isdigit (ch)) && ch != '.' && ch != '-' && ch != '+') {
			return 0;
		}
	}
	return 1;
}

/// <summary>
/// CSV用のstrtok
/// </summary>
/// <param name="str">対象文字列</param>
/// <param name="ptr">切り出しポインター</param>
/// <returns>切り出し文字列</returns>
char* wString::strtok_csv (char* str, int& ptr)
{
	int start = ptr;
	char* token = str + start;
	int quoted = 0;
	if (ptr && str[ptr] == 0) {
		return NULL;
	}
	// 区切り文字または\0まで
	while (str[ptr]) {
		if (str[ptr] == '\"') {
			quoted++;
		}else if ((quoted & 1) == 0 && str[ptr] == ',') {
			break;
		}
		// 1文字増加
		ptr++;
	}

	str[ptr++] = 0;
	//先頭と末尾の"を除去
	if (token[0] == '\"' && token[ptr - start-2] == '\"') {
		token[ptr - start-2] = '\0';
		token++;
	}
	return token;
}

/// <summary>
/// fdから、１行(CRLFか、LF単独が現れるまで)受信
/// CRLFは削除する。
/// CSV用にダブルクォーテーションをエスケープする
/// 受信したサイズをreturnする。
/// \rは無視する
/// </summary>
/// <param name="fd">ファイルディスクリプタ</param>
/// <param name="line_buf_p">ラインバッファ</param>
/// <param name="line_max">最大文字数</param>
/// <returns></returns>
int wString::readLineCSV (int fd, char* line_buf_p, int line_max)
{
	char byte_buf;
	int  line_len = 0;
	int  quoted = 0;
	// １行受信実行
	while (1) {
		auto recv_len = read (fd, &byte_buf, 1);
		if (recv_len != 1) { // 受信失敗チェック
			return (-1);
		}
		// CR/LFチェック
		if (byte_buf == '\"') {
			quoted++;
		}else if ((quoted & 1) == 0) {
			if (byte_buf == '\r') {
				continue;
			}
			else if (byte_buf == '\n') {
				*line_buf_p = 0;
				break;
			}
		}

		// バッファにセット
		*line_buf_p++ = byte_buf;
		// 受信バッファサイズチェック
		if (++line_len >= line_max) {
			// バッファオーバーフロー検知
			return (-1);
		}
	}
	return line_len;
}
//---------------------------------------------------------------------------
// 大文字化
//---------------------------------------------------------------------------
wString wString::ToUpper (void) const
{
	wString temp (*this);
	for (auto i = 0U; i < length (); i++) {
		temp.String[i] = (char)toupper (temp.String[i]);
	}
	return temp;
}
//---------------------------------------------------------------------------
// 大文字化
//---------------------------------------------------------------------------
wString wString::ToLower (void) const
{
	wString temp (*this);
	for (auto i = 0U; i < length (); i++) {
		temp.String[i] = (char)tolower (temp.String[i]);
	}
	return temp;
}
//---------------------------------------------------------------------------
// トリム
//---------------------------------------------------------------------------
wString wString::trim (void) const
{
	wString temp (*this);
	if (temp.len) {
		//先頭の空白等を抜く
		while (temp.len && *reinterpret_cast<unsigned char*>(temp.String) <= ' ') {
#ifdef __linux__
			char* src = temp.String;
			char* dst = src + 1;
			while (*src) {
				*src++ = *dst++;
			}
#else
			strcpy (temp.String, temp.String + 1);
#endif
			temp.len--;
		}
		//末尾の空白等を抜く
		while (temp.len && reinterpret_cast<unsigned char*>(temp.String)[temp.len - 1] <= ' ') {
			temp.String[--temp.len] = 0;
		}
	}
	return temp;
}
//---------------------------------------------------------------------------
// トリム
//---------------------------------------------------------------------------
wString wString::rtrim (void) const
{
	wString temp (*this);
	if (temp.len) {
		//末尾の空白等を抜く
		while (temp.len && reinterpret_cast<unsigned char*>(temp.String)[temp.len - 1] <= ' ') {
			temp.String[--temp.len] = 0;
		}
	}
	return temp;
}
///---------------------------------------------------------------------------
/// <summary>
/// sentence文字列の行末に、cut_charがあったとき、削除
/// </summary>
/// <param name="sentence">対象文字列</param>
/// <param name="cut_char">削除文字列（省略値スペース）</param>
void wString::rtrim_chr (char* sentence, unsigned char cut_char)
{
	if (sentence == NULL || *sentence == 0) return;
	char* source_p;
	auto slength = static_cast<int>(strlen (sentence));		// 文字列長Get
	source_p = sentence;
	source_p += slength;									// ワークポインタを文字列の最後にセット。
	for (auto i = 0; i < slength; i++) {					// 文字列の数だけ繰り返し。
		source_p--;											// 一文字ずつ前へ。
		if (*source_p == cut_char) {						// 削除キャラ ヒットした場合削除
			*source_p = '\0';
		}
		else {												// 違うキャラが出てきたところで終了。
			break;
		}
	}
	return;
}
/// <summary>
/// 複数文字列のtrim実装
/// </summary>
/// <param name="sentence"></param>
/// <param name="cut_char"></param>
void wString::trim_chr(char* sentence, unsigned char* cut_char)
{
	if (sentence == NULL || *sentence == '\0' || cut_char == NULL || *cut_char == '\0') return;

	unsigned char* s = reinterpret_cast<unsigned char*>(sentence);
	const int cut_len = static_cast<int>(strlen(reinterpret_cast<const char*>(cut_char)));
	int slength = static_cast<int>(strlen(sentence));

	// 先頭トリム
	bool changed = true;
	while (*s != '\0' && changed) {
		changed = false;
		for (int j = 0; j < cut_len; ++j) {
			if (s[0] == cut_char[j]) {
				memmove(s, s + 1, static_cast<size_t>(slength));
				--slength;
				changed = true;
				break;
			}
		}
	}

	// 末尾トリム
	changed = true;
	while (slength > 0 && changed) {
		changed = false;
		unsigned char* p = s + slength - 1;
		for (int j = 0; j < cut_len; ++j) {
			if (*p == cut_char[j]) {
				*p = '\0';
				--slength;
				changed = true;
				break;
			}
		}
	}
}
///---------------------------------------------------------------------------
/// <summary>
/// 文字列の行末に、cut_charがあったとき、削除
/// </summary>
/// <param name="cut_char">対象文字列</param>
void wString::rtrim_chr (unsigned char cut_char)
{
	if (len) {
		//末尾の空白等を抜く
		while (len && String[len - 1] == cut_char) {
			String[--len] = 0;
		}
	}
	return;
}
/// <summary>
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
wString wString::ltrim_chr (unsigned char cut_char) const
{
	wString temp (*this);
	if (temp.len) {
		//先頭の空白等を抜く
		while (temp.len && *reinterpret_cast<unsigned char*>(temp.String) == cut_char) {
			char* src = temp.String;
			char* dst = src + 1;
			while (*src) {
				*src++ = *dst++;
			}
			temp.len--;
		}
	}
	return temp;
}
//---------------------------------------------------------------------------
// トリム
//---------------------------------------------------------------------------
wString wString::ltrim (void) const
{
	wString temp (*this);
	if (temp.len) {
		//先頭の空白等を抜く
		while (temp.len && *reinterpret_cast<unsigned char*>(temp.String) <= ' ') {
			char* src = temp.String;
			char* dst = src + 1;
			while (*src) {
				*src++ = *dst++;
			}
			temp.len--;
		}
	}
	return temp;
}
///---------------------------------------------------------------------------
/// <summary>
/// 指定文字数で文字列を初期化
/// </summary>
/// <param name="num"></param>
void wString::reset_length (unsigned int num)
{
	assert (capa > (unsigned int)num);
	String[num] = 0;
	len = num;
}
//---------------------------------------------------------------------------
char* wString::c_str (void) const
{
	return String;
}
/// <summary>
/// 文字領域キャパシティ
/// </summary>
/// <param name=""></param>
/// <returns></returns>
unsigned int wString::capacity (void) const
{
	return capa;
}
//---------------------------------------------------------------------------
int wString::last_delimiter (const char* delim) const
{
	auto pos = -1;
	auto dlen = (int)strlen (delim);
	for (auto i = (int)len - dlen; i > 0; i--) {
		if (strncmp (String + i, delim, dlen) == 0) {
			pos = i;
			break;
		}
	}
	return pos;
}
//---------------------------------------------------------------------------
/// <summary>
/// sentence文字列内のkey文字列を置換先文字列で置換する。
/// </summary>
/// <param name="sentence">置換対象文字列</param>
/// <param name="slen">置換対象文字列の長さ</param>
/// <param name="p">置換の位置</param>
/// <param name="klen">置換する長さ</param>
/// <param name="rep">置換先文字列</param>
void wString::replace_character_len (const char* sentence, int slen, const char* p, int klen, const char* rep)
{
	auto rlen = (int)strlen (const_cast<char*>(rep));
	int num;
	if (klen == rlen) {
		memcpy (const_cast<char*>(p), rep, rlen);
		//前詰め置換そのままコピーすればいい
	}
	else if (klen > rlen) {
		num = klen - rlen;
		strcpy (const_cast<char*>(p), p + num);
		memcpy (const_cast<char*>(p), rep, rlen);
		//置換文字が長いので後詰めする
	}
	else {
		num = rlen - klen;
		//pからrlen-klenだけのばす
		for (auto str = const_cast<char*>(sentence + slen + num); str > p + num; str--) {
			*str = *(str - num);
		}
		memcpy (const_cast<char*>(p), rep, rlen);
	}
	return;
}
/// <summary>
/// sentence文字列内のkey文字列をrep文字列で置換する。ｎ回
/// </summary>
/// <param name="sentence">置換元文字列</param>
/// <param name="key">置換文字列</param>
/// <param name="rep">置換後の文字列</param>
bool wString::replace_character(wString key, wString rep)
{
	int klen = key.length();
	int rlen = rep.length();
	if (klen == 0 || len == 0) {
		return false;
	}
	auto p = find(key);
	//前詰め置換そのままコピーすればいい
	while (p != wString::npos) {
		replace(p, klen, rep);
		p = find(key, rlen);
	}
	return true;
}
/// <summary>
/// 文字列（バイナリ含む）ダンプ
/// </summary>
/// <returns>ダンプした文字列</returns>
wString wString::dump () const
{
	wString tmp;
	void* ptr = String;
	int vlen = len;
	unsigned char* out = static_cast<unsigned char*>(ptr);
	for (int start = 0; start < vlen; start += 16) {
		tmp.cat_sprintf ("%04d:", start);
		for (int line = start; line < start + 16 && line < vlen; line++) {
			tmp.cat_sprintf ("%02X ", out[line]);
		}
		for (int line = start; line < start + 16 && line < vlen; line++) {
			if (out[line] >= ' ' && out[line] < 0x7f) {
				tmp.cat_sprintf ("%c", out[line]);
			}
			else {
				tmp.cat_sprintf (".");
			}
		}
		tmp.cat_sprintf ("\r\n");
	}
	return tmp;
}
#ifndef va_copy

/// <summary>
/// 可変フォーマッティング
/// </summary>
/// <param name="fmt"></param>
/// <param name="arg"></param>
/// <returns></returns>
int wString::vtsprintf (const char* fmt, va_list arg)
{
	int vlen = 0;
	int vsize = 0;
	int zeroflag, width;
	bool long_long;


	while (*fmt) {
		if (*fmt == '%') {        /* % に関する処理 */
			zeroflag = width = 0;
			long_long = false;
			fmt++;
			if (*fmt == '0') {
				fmt++;
				zeroflag = 1;
			}
			while ((*fmt >= '0') && (*fmt <= '9')) {
				width *= 10;
				width += *(fmt++) - '0';
			}

			/* printf ("zerof = %d,width = %d\n",zeroflag,width); */

			//lluもluもuも同じ
			while (*fmt == 'l' || *fmt == 'z') {
				if (*fmt == 'l' && fmt[1] == 'l') {
					long_long = true;
					fmt += 2;
				}
				else {
				fmt++;
			}
			}

			switch (*fmt) {
			case 'd':        /* 10進数 */
				vsize = tsprintf_decimal (va_arg (arg, signed long), zeroflag, width);
				break;
			case 'u':        /* 10進数 */
				if (long_long) {
					vsize = tsprintf_decimalu(va_arg(arg, unsigned long long), zeroflag, width);
				}
				else {
				vsize = tsprintf_decimalu (va_arg (arg, unsigned long), zeroflag, width);
				}
				break;
			case 'o':        /* 8進数 */
				vsize = tsprintf_octadecimal (va_arg (arg, unsigned long), zeroflag, width);
				break;
			case 'x':        /* 16進数 0-f */
				vsize = tsprintf_hexadecimal (va_arg (arg, unsigned long), 0, zeroflag, width);
				break;
			case 'X':        /* 16進数 0-F */
				vsize = tsprintf_hexadecimal (va_arg (arg, unsigned long), 1, zeroflag, width);
				break;
			case 'c':        /* キャラクター */
				vsize = tsprintf_char (va_arg (arg, int));
				break;
			case 's':        /* ASCIIZ文字列 */
				vsize = tsprintf_string (va_arg (arg, char*));
				break;
			default:        /* コントロールコード以外の文字 */
				/* %%(%に対応)はここで対応される */
				vlen++;
				*this += *fmt;
				break;
			}
			vlen += vsize;
			fmt++;
		}
		else {
			*this += *(fmt++);
			vlen++;
		}
	}
	va_end (arg);
	return (vlen);
}
/// <summary>
/// 数値 => 10進文字列変換
/// </summary>
/// <param name="val"></param>
/// <param name="zerof"></param>
/// <param name="width"></param>
/// <returns></returns>
int wString::tsprintf_decimal (signed long val, int zerof, int width)
{
	//末尾０を保証
	char  tmp[22] = {};
	char* ptmp = tmp + 20;
	int   vlen = 0;
	int   minus = 0;

	if (!val) {        /* 指定値が0の場合 */
		*(ptmp--) = '0';
		vlen++;
	}
	else {
		/* マイナスの値の場合には2の補数を取る */
		if (val < 0) {
			val = ~val;
			val++;
			minus = 1;
		}
		while (val && vlen < 19) {
			/* バッファアンダーフロー対策 */
			//if (vlen >= 19){
			//    break;
			//}

			*ptmp = (char)((val % 10) + '0');
			val /= 10;
			ptmp--;
			vlen++;
		}

	}

	/* 符号、桁合わせに関する処理 */
	if (zerof) {
		if (minus) {
			width--;
		}
		while (vlen < width) {
			*(ptmp--) = '0';
			vlen++;
		}
		if (minus) {
			*(ptmp--) = '-';
			vlen++;
		}
	}
	else {
		if (minus) {
			*(ptmp--) = '-';
			vlen++;
		}
		while (vlen < width) {
			*(ptmp--) = ' ';
			vlen++;
		}
	}
	*this += (ptmp + 1);
	return (vlen);
}
/// <summary>
/// 符号なし数値 => 10進文字列変換
/// </summary>
/// <param name="val"></param>
/// <param name="zerof">先頭がゼロサプレスなら1</param>
/// <param name="width"></param>
/// <returns></returns>
int wString::tsprintf_decimalu (unsigned long long val, int zerof, int width)
{
	char tmp[22] = {};
	char* ptmp = tmp + 20;
	int vlen = 0;
	// TODO:minusが設定されない
	int minus = 0;

	if (!val) {        /* 指定値が0の場合 */
		*(ptmp--) = '0';
		vlen++;
	}
	else {
		while (val) {
			/* バッファアンダーフロー対策 */
			if (vlen >= 20) {
				break;
			}

			*ptmp = (char)((val % 10) + '0');
			val /= 10;
			ptmp--;
			vlen++;
		}
	}

	/* 符号、桁合わせに関する処理 */
	if (zerof) {
		if (minus) {
			width--;
		}
		while (vlen < width) {
			*(ptmp--) = '0';
			vlen++;
		}
		if (minus) {
			*(ptmp--) = '-';
			vlen++;
		}
	}
	else {
		while (vlen < width) {
			*(ptmp--) = ' ';
			vlen++;
		}
	}

	*this += (ptmp + 1);
	return (vlen);
}
/// <summary>
/// 数値 => 8進文字列変換
/// </summary>
/// <param name="val"></param>
/// <param name="zerof"></param>
/// <param name="width"></param>
/// <returns></returns>
int wString::tsprintf_octadecimal (unsigned long val, int zerof, int width)
{
	char tmp[22] = {};
	char* ptmp = tmp + 20;
	int vlen = 0;
	// TODO:minusが設定されない
	int minus = 0;

	if (!val) {        /* 指定値が0の場合 */
		*(ptmp--) = '0';
		vlen++;
	}
	else {
		while (val) {
			/* バッファアンダーフロー対策 */
			if (vlen >= 19) {
				break;
			}

			*ptmp = (char)((val % 8) + '0');
			val /= 8;
			ptmp--;
			vlen++;
		}
	}

	/* 符号、桁合わせに関する処理 */
	if (zerof) {
		if (minus) {
			width--;
		}
		while (vlen < width) {
			*(ptmp--) = '0';
			vlen++;
		}
		if (minus) {
			*(ptmp--) = '-';
			vlen++;
		}
	}
	else {
		while (vlen < width) {
			*(ptmp--) = ' ';
			vlen++;
		}
	}

	*this += (ptmp + 1);
	return (vlen);
}
/// <summary>
/// 数値 => 16進文字列変換
/// </summary>
/// <param name="val"></param>
/// <param name="capital"></param>
/// <param name="zerof"></param>
/// <param name="width"></param>
/// <returns></returns>
int wString::tsprintf_hexadecimal (unsigned long val, int capital, int zerof, int width)
{
	char tmp[22] = {};
	char* ptmp = tmp + 20;
	int vlen = 0;
	char str_a;

	/* A～Fを大文字にするか小文字にするか切り替える */
	if (capital) {
		str_a = 'A';
	}
	else {
		str_a = 'a';
	}

	if (!val) {        /* 指定値が0の場合 */
		*(ptmp--) = '0';
		vlen++;
	}
	else {
		while (val) {
			/* バッファアンダーフロー対策 */
			if (vlen >= 18) {
				break;
			}

			*ptmp = (char)(val % 16);
			if (*ptmp > 9) {
				*ptmp += (char)(str_a - 10);
			}
			else {
				*ptmp += '0';
			}

			val >>= 4;        /* 16で割る */
			ptmp--;
			vlen++;
		}
	}
	while (vlen < width) {
		*(ptmp--) = zerof ? '0' : ' ';
		vlen++;
	}

	*this += (ptmp + 1);
	return(vlen);
}
/// <summary>
/// 数値 => 1文字キャラクタ変換
/// </summary>
/// <param name="ch"></param>
/// <returns></returns>
int wString::tsprintf_char (int ch)
{
	*this += (char)ch;
	return(1);
}
/// <summary>
/// 数値 => ASCIIZ文字列変換
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
int wString::tsprintf_string (char* str)
{
	*this += str;
	return(static_cast<int>(strlen (str)));
}
#endif
//---------------------------------------------------------------------------
//wString 


/// <summary>
/// 可変引数フォーマッティング
/// </summary>
/// <param name="format"></param>
/// <param name=""></param>
/// <returns></returns>
int wString::sprintf (const char* format, ...)
{
#ifndef va_copy
	String[0] = 0;
	vlen = 0;
	va_list ap;
	va_start (ap, format);
	vtsprintf (format, ap);
	va_end (ap);
	return vlen;
#else
	int stat;
	//可変引数を２つ作る
	va_list ap1, ap2;
	va_start (ap1, format);
	va_copy (ap2, ap1);
	//最初はダミーで文字列長をシミュレート
	stat = vsnprintf (String, 0, format, ap1);
	set_length (stat + 1);
	//実際に出力
	stat = vsprintf (String, format, ap2);
	va_end (ap1);
	va_end (ap2);
	len = stat;
	return stat;
#endif
}
/// <summary>
/// 可変引数追加フォーマッティング
/// </summary>
/// <param name="format"></param>
/// <param name=""></param>
/// <returns></returns>
int wString::cat_sprintf (const char* format, ...)
{
#ifndef va_copy
	int status;
	va_list ap;
	va_start (ap, format);
	status = this->vtsprintf (format, ap);
	va_end (ap);
	return status;
#else
	int stat;
	//可変引数を２つ作る
	va_list ap1, ap2;
	va_start (ap1, format);
	va_copy (ap2, ap1);
	//最初はダミーで文字列長をシミュレート
	stat = vsnprintf (String, 0, format, ap1);
	set_length (stat + len + 1);
	//実際に出力
	stat = vsprintf (String + len, format, ap2);
	va_end (ap1);
	va_end (ap2);
	len += stat;
	return stat;
#endif
}
//---------------------------------------------------------------------------
/// <summary>
/// 文字列をデリミタで切って、デリミタ後の文字を返す
/// 元文字列は破壊
/// </summary>
/// <param name="delimstr">分解したTStringList文字列</param>
/// <returns>切断後の文字列。見つからない場合は長さ０の文字列</returns>
wString wString::strsplit (const char* delimstr) const
{
	wString tmp;
	char* ptr = c_str ();
	auto delimlen = static_cast<int>(strlen (delimstr));
	int start = 0;
	for (auto i = 0; i < static_cast<int>(len); i++) {
		for (auto j = 0; j < delimlen; j++) {
			if (ptr[i] == delimstr[j]) {
				ptr[i] = 0;
				tmp.Add (&ptr[start]);
				start = i + 1;
				break;
			}
		}
	}
	if (start < static_cast<int>(len)) {
		tmp.Add (&ptr[start]);
	}
	return tmp;
}
/// <summary>
/// 文字列容量の変更
/// </summary>
/// <param name="newsize"></param>
void wString::resize (const int newsize)
{
	if (len >= capa) {
		wString temp;
		temp.sprintf ("Over capacity.[len:%u capa:%u]", len, capa);
		throw new CScriptException (temp);
		//printf ("not good %u %u", len, capa);
		//exit (1);
	}
	if ((int)capa <= newsize) {
		// capaは古いサイズ＋新しいサイズ。必ず１は増やす。フィボナッチと同じ
		capa += newsize + ((capa <= 0) ? 1 : 0);
		char* tmp = new char[capa];
		memcpy (tmp, String, len);
		tmp[len] = 0;
		delete[] String;
		String = tmp;
	}
	else {
		//指定サイズが元より小さいので何もしない
	}
}
///---------------------------------------------------------------------------
/// <summary>
/// HTML特殊文字に変換
/// </summary>
/// <param name=""></param>
/// <returns>HTML特殊文字</returns>
wString wString::htmlspecialchars (void)
{
	wString temp;
	// 引数チェック
	char* ptr = String;
	for (unsigned int is = 0; is < len; is++) {
		switch (*ptr) {
		case '&':temp += "&amp;"; break;
		case '\"':temp += "&quot;"; break;
			//case '\'':temp += "&#39;";break;
		case '<':temp += "&lt;";  break;
		case '>':temp += "&gt;";  break;
		default:temp += *ptr;    break;
		}
		ptr++;
	}
	return temp;
}
///---------------------------------------------------------------------------
/// <summary>
/// URIエンコードを行う
/// </summary>
/// <param name="">変換元文字列</param>
/// <returns>URIエンコードした文字列</returns>
wString wString::uri_encode (void)
{
	unsigned int is;
	wString dst;
	// 引数チェック
	for (is = 0; is < len; is++) {
		auto datum = static_cast<unsigned char>(String[is]);
		/* ' '(space) はちと特別扱いにしないとまずい */
		if (datum == ' ') {
			dst += "%20";
			/* エンコードしない文字全員集合 */
			//        }else if ( strchr("!$()*,-./:;?@[]^_`{}~", String[is]) != NULL ){
		}
		else if (strchr ("!$()*,-.:;/?@[]^_`{}~", datum) != NULL) {
			dst += datum;
			/* アルファベットと数字はエンコードせずそのまま */
		}
		else if (isalnum (datum)) {
			dst += datum;
		}
		/* \マークはエンコード */
		else if (datum == '\\') {
			dst += "%5c";
			/* それ以外はすべてエンコード */
		}
		else {
			char work[8];
			::sprintf (work, "%%%2X", datum);
			dst += work;
		}
	}
	return dst;
}
///---------------------------------------------------------------------------
/// <summary>
/// QUOTE,ESCAPE,NULLをエスケープする
/// </summary>
/// <returns>エスケープした文字列</returns>
wString wString::add_slashes (void)
{
	wString tmp;
	for (unsigned int i = 0; i < len; i++) {
		if (String[i] == '\'' || String[i] == '\"' || String[i] == '\\' || String[i] == 0) {
			tmp += '\\';
		}
		tmp += String[i];
	}
	return tmp;
}
/// <summary>
/// 1文字の１６進文字を１０進数値に変換する
/// </summary>
/// <param name="x">変換する文字</param>
/// <returns>１０進の数値</returns>
static unsigned char htoc (unsigned char x)
{
	if ('0' <= x && x <= '9') return (unsigned char)(x - '0');
	if ('a' <= x && x <= 'f') return (unsigned char)(x - 'a' + 10);
	if ('A' <= x && x <= 'F') return (unsigned char)(x - 'A' + 10);
	return 0;
}
/// <summary>
/// URIデコードを行います.
/// </summary>
/// <returns>デコードした文字</returns>
wString wString::uri_decode ()
{
	unsigned char   code;
	wString dst;
	// =================
	// メインループ
	// =================
	for (auto i = 0U; i < len && String[i] != '\0'; i++) {
		if (String[i] == '%') {
			if (i + 2 >= len) {
				break;
			}
			code = (unsigned char)(htoc (String[++i]) << 4);
			code += htoc (String[++i]);
			dst += code;
		}
		else if (String[i] == '+') {
			dst += ' ';
		}
		else {
			dst += String[i];
		}
	}
	return dst;
}
#ifdef WEB

/// <summary>
/// 日本語文字コード変換
/// (libnkfのラッパー関数)
/// サポートされている形式は以下の通り。
///      in :S:SJIS E:EUC W:UTF-8
///      out:s:SJIS w:UTF-8
/// </summary>
/// <param name="option"></param>
/// <returns></returns>
wString wString::nkfcnv (const wString& option)
{
	auto ptr2 = 0U;
	auto kanji = false;
	while (ptr2 < len) {
		if (String[ptr2++] & 0x80) {
			kanji = true;
			break;
		}
	}
	if (!kanji) {
		return *this;
	}

	wString ptr (len * 3);
	//=================================================
	// libnkf 実行
	//=================================================
	nkf_cnv* nkfcnv = new nkf_cnv ();
	nkfcnv->nkf (static_cast<const char*>(String), static_cast<char*>(ptr.c_str ()), len * 3, static_cast<const char*>(option.c_str ()));
	// 文字列の長さの調整
	ptr.len = (unsigned int)strlen (ptr.c_str ());
	delete nkfcnv;
	return ptr;
}
#endif
///---------------------------------------------------------------------------
/// <summary>
/// BorlandのTStringList対応
/// 行の追加
/// </summary>
/// <param name="str">追加する文字列</param>
void wString::Add (const char* str)
{
	*this += str;
	*this += "\r\n";
}
///---------------------------------------------------------------------------
/// <summary>
/// BorlandのTStringList対応
/// 行の追加
/// </summary>
/// <param name="str">追加する文字列</param>
void wString::Add (const wString& str)
{
	*this += str;
	*this += "\r\n";
}

///---------------------------------------------------------------------------
/// <summary>
/// BorlandのTStringList風実装
/// 格納した行数を返す
/// </summary>
/// <returns>行数</returns>
int wString::lines (void)
{
	int count = 0;
	char* ptr = String;
	while (*ptr) {
		if (*ptr == '\r') {
			ptr++;
			continue;
		}
		if (*ptr++ == '\n') {
			count++;
		}
	}
	return count;
}

//---------------------------------------------------------------------------
/// <summary>
/// BorlandのTStringList風実装
/// \r\nで区切られた文字列のpos行目にdstを挿入
/// </summary>
/// <param name="dst">挿入文字列</param>
/// <param name="pos">挿入行位置</param>
/// <returns>成功:true,失敗:false</returns>
bool wString::insert_list_string (wString& src, int pos)
{
	bool flag;
	flag = insert_list_string (src.String, pos);
	return flag;
}
///---------------------------------------------------------------------------
/// <summary>
/// BorlandのTStringList風実装
/// \r\nで区切られた文字列のpos行目にdstを挿入
/// </summary>
/// <param name="dst">挿入文字列</param>
/// <param name="pos">挿入行位置</param>
/// <returns>成功:true,失敗:false</returns>
bool wString::insert_list_string (const char* dst, int pos)
{
	//行数が多い
	std::vector<wString> work;
	int count = 0;
	int ptr = 0;
	int last = 0;
	int cchar = 0;
	while (String[ptr]) {
		switch (String[ptr]) {
		case '\r':
			ptr++;
			cchar++;
			continue;
		case '\n':
			ptr++;
			cchar++;
			count++;
			work.push_back (substr (last, ptr - last - cchar));
			cchar = 0;
			last = ptr;
			break;
		default:
			cchar = 0;
			ptr++;
		}
	}
	//行末にcrlfがなかった場合に対応
	if (ptr - last - cchar) {
		count++;
		work.push_back (substr (last, ptr - last - cchar));
	}
	//要素追加
	if (0 <= pos && pos <= count) {
		work[pos] = dst;
		//元を消す
		clear ();
		for (unsigned int i = 0; i < (unsigned int)work.size (); i++) {
			*this += work[i] + "\r\n";
		}
		return true;
	}
	else {
		//なにもしない
		return false;
	}
}
///---------------------------------------------------------------------------
/// <summary>
/// BorlandのTStringList風実装
/// \r\nで区切られた文字列リストのpos行目のデータを取得
/// </summary>
/// <param name="pos">取得行位置</param>
/// <returns>取得した文字列、失敗時区別なし</returns>
wString wString::get_list_string (int pos)
{
	int   lcount = 0;
	int   ptr = 0;
	int   curptr = 0;
	int   lstptr;
	int   spcnt = 0;//\r\nまたは\nの文字数
	while (String[ptr]) {
		if (String[ptr] == '\r') {
			ptr++;
			spcnt++;
			continue;
		}
		if (String[ptr++] == '\n') {
			lstptr = curptr;
			curptr = ptr;
			spcnt++;
			if (lcount++ == pos) {
				return substr (lstptr, curptr - lstptr - spcnt);
			}
		}
		spcnt = 0;
	}
	return wString ();
}
//---------------------------------------------------------------------------

/// <summary>
/// コピー先文字列に元文字列のindexからslen分コピーする
/// バイナリ非対応
/// </summary>
/// <param name="str">コピー先文字列</param>
/// <param name="slen">コピーする文字数</param>
/// <param name="index">開始位置</param>
/// <returns></returns>
unsigned int wString::copy (char* str, unsigned int slen, int index) const
{
	strncpy (str, String + index, slen);
	str[slen] = 0;
	return slen;
}
///---------------------------------------------------------------------------
/// <summary>
/// 開始位置から指定文字数分変更先文字列に置換する
/// </summary>
/// <param name="index">開始位置</param>
/// <param name="slen">変更する文字数</param>
/// <param name="repstr">変更先文字列</param>
/// <returns></returns>
wString& wString::replace (int index, unsigned int slen, const wString& repstr)
{
	auto rlen = repstr.len;
	//同じ
	if (slen == rlen) {
		memcpy (String + index, repstr.String, rlen);
		//前詰め置換そのままコピーすればいい
	}
	else if (slen > rlen) {
		int num = slen - rlen;
		char* p = String + index;
		char* q = p + num;
		while (*q) {
			*p++ = *q++;
		}
		memcpy (String + index, repstr.String, rlen);
		len -= num;
		String[len] = 0;
		//置換文字が長いので後詰めする
	}
	else {
		int num = rlen - slen;
		resize (len + num + 1);
		for (char* p = (String + len + num); p > String + index + num; p--) {
			*p = *(p - num);
		}
		memcpy (String + index, repstr.String, rlen);
		len += num;
		String[len] = 0;
	}
	return *this;
}

constexpr auto ToBase64tbl = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
/// <summary>
/// BASE64エンコード
/// </summary>
/// <param name="">エンコードする文字列</param>
/// <returns>エンコードした文字列</returns>
wString wString::base64 (void)
{
	wString tmpout;
	wString work (len + 3);
	memcpy (reinterpret_cast<void*>(work.String), String, len);
	work.String[len] = 0;
	work.String[len + 1] = 0;
	work.String[len + 2] = 0;

	unsigned char* instr = reinterpret_cast<unsigned char*>(work.String);
	int count = 0;
	/*
	ABC -> 414243 -> 0100 0001  0100 0010  0100 0011
		-> 010000 010100 001001 000011
		   10 14 09 03
	*/
	/// 文字数*8ビットから1回あたり６ビットとる。残りビットがある間処理を行う
	for (int i = len * 8; i > 0; i -= 6) {
		unsigned char ch = 0;
		switch (count) {
		case 0:
			ch = (unsigned char)((*instr >> 2) & 0x3f);
			break;
		case 1:
			ch = (unsigned char)((*instr++ << 4) & 0x3f);
			ch |= (unsigned char)((*instr >> 4) & 0x3f);
			break;
		case 2:
			ch = (unsigned char)((*instr++ << 2) & 0x3f);
			ch |= (unsigned char)((*instr >> 6) & 0x3f);
			break;
		case 3:
			ch = (unsigned char)((*instr++) & 0x3f);
			break;
		}
		tmpout += ToBase64tbl[ch];
		count++;
		count %= 4;
	}
	count = (4 - count) % 4;
	while (count-- > 0) {
		tmpout += '=';
	}
	//tmpout += '\0';
	return tmpout;
}
// TODO range chack
/// <summary>
/// BASE64デコード
/// </summary>
/// <param name="">デコードする文字列</param>
/// <returns>デコードされた文字列</returns>
wString wString::unbase64 (void)
{
	const unsigned char* instr = reinterpret_cast<unsigned char*>(String);

	char FromBase64tbl[256] = {};
	wString tmpout;
	tmpout.clear ();
	//逆テーブルの作成,=も０になる
	for (unsigned int i = 0; i < sizeof (ToBase64tbl); i++) {
		FromBase64tbl[(int)ToBase64tbl[i]] = (unsigned char)i;
	}

	/*
	ABC -> 414243 -> 0100 0001  0100 0010  0100 0011
	->   010000 010100 001001 000011
	10 14 09 03
	*/
	int ll = len;
	while (ll > 0) {
		int s1 = FromBase64tbl[*instr++];
		int s2 = FromBase64tbl[*instr++];
		char ch = (char)((s1 << 2) | (s2 >> 4));
		if (ch) {
			int s3 = FromBase64tbl[*instr++];
			tmpout += ch;
			ch = (char)(((s2 & 0x0f) << 4) | (s3 >> 2));
			if (ch) {
				int s4 = FromBase64tbl[*instr++];
				tmpout += ch;
				ch = (char)(((s3 & 0x03) << 6) | s4);
				if (ch) {
					tmpout += ch;
				}
			}
		}
		ll -= 4;
	}
	//tmpout += '\0';
	return tmpout;
}

/// <summary>
/// 文字列中の引用符、エスケープを二重エスケープ
/// </summary>
/// <param name="str">エスケープする文字列</param>
/// <returns>エスケープ結果</returns>
wString wString::escape (const wString& str)
{

	wString dst;
	unsigned int len = str.length ();
	// 引数チェック
	for (auto is = 0U; is < len; is++) {
		if (str[is] == '\"') {
			dst += "\\\"";
		}
		else if (str[is] == '\\') {
			dst += "\\\\";
		}
		else {
			dst += str[is];
		}
	}
	return dst;
}

/// <summary>
/// utf8文字列の長さ
/// range       8bit       5bit    bytes
/// 0xxx - xxxx:00 - 7f -> 00 - 0f 1
/// 110x - xxxx:c0 - df -> 18 - 1b 2
/// 1110 - xxxx:e0 - ef -> 1c - 1d 3
/// 1111 - 0xxx:f0 - f7 -> 1e - 1f 4
/// </summary>
/// <param name="str">utf-8文字列</param>
/// <returns>文字数</returns>
int wString::strlen_utf8 (char* str)
{
	const static int hb[32] = {
		//0 1 2 3 4 5 6 7 8 9 A B C D E F
		  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,//0x00
		  1,1,1,1,1,1,1,1,2,2,2,2,3,3,4,1,//0x00
	};

	int cnt = 0;
	while (*str) {
		auto target = *(reinterpret_cast<unsigned char*>(str));
		auto current_len = hb[target >> 3];
		str += current_len;
		cnt++;
	}
	return cnt;
}

/// <summary>
/// sentence文字列の、cut_charが最初に出てきた所から前を削除
/// もし、cut_charがsentence文字列に入っていなかった場合、文字列全部削除
/// </summary>
/// <param name="cut_char">分離文字</param>
/// <returns></returns>
bool wString::cut_before_character (const char cut_char)
{
	// 削除対象キャラクターが最初に出てくる所を探す。
	int ptr = find (cut_char);
	if (ptr == wString::npos) {
		// 発見できなかった場合、文字列全部削除。
		clear ();
		return false;
	}
	memmove(String, String + ptr + 1, len - ptr);
	len -= (ptr + 1);
	return true;
}
/// <summary>
/// sentence文字列より、cut_charから後ろを削除
///      見つからなければ何もしない。
/// </summary>
/// <param name="cut_char">分離文字</param>
/// <returns>成功:true 失敗:false</returns>
bool wString::cut_after_character (const char cut_char)
{
	int ptr = find (cut_char);
	if (ptr == wString::npos) {
		return false;
	}
	String[ptr] = 0;
	len = ptr;
	return true;
}

/// <summary>
///  sentence文字列の、cut_charが最後に出てきた所から後ろをCUT
/// もし、cut_charがsentence文字列に入っていなかった場合、文字列全部削除。
/// </summary>
/// <param name="cut_char"></param>
/// <returns></returns>
bool wString::cut_after_last_character(const char cut_char)
{
	int ptr = rfind(cut_char);
	if( ptr == wString::npos) {
		return false;
	}

	String[ptr] = 0;
	len = ptr;
	return true;
}

//*********************************************************
// sentence文字列より、最初に出て来たcut_charの前後を分割。
//
//      sentence        (IN) 分割対象の文字列を与える。
//      cut_char        (IN) 分割対象の文字を入れる。
//      split1          (OUT)カットされた前の部分が入る。sentenceと同等のサイズが望ましい。
//      split2          (OUT)カットされた後ろの部分が入る。sentenceと同等のサイズが望ましい。
//
//
// return
//              0:                      正常終了。
//              それ以外：      エラー。分割失敗などなど。
//*********************************************************
bool wString::sentence_split(char cut_char, wString& split1, wString& split2)
{
	split1.clear();
	split2.clear();
	char* p = String;
	char* ptr = nullptr;
	// エラーチェック。
	if (len == 0 ) {
		return false;       // 引数にNULLまじり。
	}
	int pos = find(cut_char);
	// 分割文字発見できず。
	if (pos == wString::npos) {
		return false;
	}
	ptr = p+pos;//分割文字の位置をポインタに変換
	//比較しながら複写
	while (*p) {
		//分割文字より前半
		if (p < ptr) {
			split1 += *p++;
			//分割文字より後半
		}
		else if (p > ptr) {
			split2 += *p++;
			//分割文字位置
		}
		else {
			p++;
		}
	}
	return 0; // 正常終了。
}

//////////////////////////////////////////////////////////////////////////
/// <summary>
/// pos位置に指定文字を挿入(バイナリ単位）
/// </summary>
/// <param name="pos">挿入位置</param>
/// <param name="fill">挿入文字</param>
/// <returns>挿入された文字</returns>
wString wString::insert (int pos, const wString& fill) const
{
	wString str1;
	wString str2;
	str1 = substr (0, pos);
	str2 = substr (pos);
	return str1 + fill + str2;
}

/// <summary>
/// Get BIOS UUID in a 16 byte array.
/// </summary>
/// <returns>Return BIOS UUID string</returns>
wString wString::bios_uuid()
{
	wString tmp;
#ifndef __linux__
	unsigned char uuid[16] = {};
	int cnt = 0;
	/// <summary>
    /// SMBIOS Structure header as described at
    /// https://www.dmtf.org/sites/default/files/standards/documents/DSP0134_3.3.0.pdf 
    /// (para 6.1.2)
    /// </summary>
	struct dmi_header
	{
		BYTE type;
		BYTE length;
		WORD handle;
		BYTE data[1];
	};

	/// <summary>
    /// Structure needed to get the SMBIOS table using GetSystemFirmwareTable API.
    /// see https ://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getsystemfirmwaretable
    /// </summary>
	struct RawSMBIOSData {
		BYTE  _Used20CallingMethod;
		BYTE  _SMBIOSMajorVersion;
		BYTE  _SMBIOSMinorVersion;
		BYTE  _DmiRevision;
		DWORD  Length;
		BYTE  SMBIOSTableData[1];
	};

	bool result = false;

	RawSMBIOSData* smb = nullptr;
	BYTE* data;

	DWORD size = 0;

	// Get size of BIOS table
	size = GetSystemFirmwareTable('RSMB', 0, smb, size);
	smb = static_cast<RawSMBIOSData*>(malloc(size));

	// Get BIOS table
	GetSystemFirmwareTable('RSMB', 0, smb, size);

	// Go through BIOS structures
	data = smb->SMBIOSTableData;
	while (data < smb->SMBIOSTableData + smb->Length)
	{
		BYTE* next;
		const dmi_header* h = reinterpret_cast<dmi_header*>(data);

		if (h->length < 4) {
			break;
		}

		//Search for System Information structure with type 0x01 (see para 7.2)
		if (h->type == 0x01 && h->length >= 0x19)
		{
			data += 0x08; //UUID is at offset 0x08

			// check if there is a valid UUID (not all 0x00 or all 0xff)
			bool all_zero = true, all_one = true;
			for (int i = 0; i < 16 && (all_zero || all_one); i++)
			{
				if (data[i] != 0x00) all_zero = false;
				if (data[i] != 0xFF) all_one = false;
			}
			if (!all_zero && !all_one)
			{
				/* As off version 2.6 of the SMBIOS specification, the first 3 fields
				of the UUID are supposed to be encoded on little-endian. (para 7.2.1) */
				uuid[cnt++] = data[3];
				uuid[cnt++] = data[2];
				uuid[cnt++] = data[1];
				uuid[cnt++] = data[0];
				uuid[cnt++] = data[5];
				uuid[cnt++] = data[4];
				uuid[cnt++] = data[7];
				uuid[cnt++] = data[6];
				for (int i = 8; i < 16; i++) {
					uuid[cnt++] = data[i];
				}

				result = true;
			}
			break;
		}

		//skip over formatted area
		next = data + h->length;

		//skip over unformatted area of the structure (marker is 0000h)
		while (next < smb->SMBIOSTableData + smb->Length && (next[0] != 0 || next[1] != 0)) {
			next++;
		}
		next += 2;

		data = next;
	}
	free(smb);


	tmp.sprintf("%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid[0], uuid[1], uuid[2], uuid[3], uuid[4], uuid[5], uuid[6], uuid[7],
		uuid[8], uuid[9], uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]);
#else
    const char* cmd = "sudo dmidecode -t system|grep UUID";
	char buffer[1024] = {};
	FILE* pipe = popen(cmd, "r");
	if (pipe) {
		while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
			tmp += buffer;
			memset(buffer, 0, sizeof(buffer));
		}
		// 頭をカット
		if (tmp.starts_with("UUID: ")) {
			tmp = tmp.substr(6);
		}
	}
#endif
	return tmp;
}
