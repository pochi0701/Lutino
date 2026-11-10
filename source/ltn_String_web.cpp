// ==========================================================================
//code=UTF8	tab=4
//
// Lutino:	Application SErver.
//
// 		ltn_String_web.cpp
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
//#include <dir.h>
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

#ifdef WEB
/// <summary>
/// httpヘッダを作成する
/// </summary>
/// <param name="content_length">送出するコンテンツ長さ。０なら省略</param>
/// <param name="expire">０以外で作成。１時間で期限切れ</param>
/// <param name="mime_type">mime_type</param>
void wString::init_header (size_t content_length, int expire, const char* mime_type)
{
	sprintf ("%s", HTTP_OK);
	cat_sprintf ("%s", HTTP_CONNECTION);
	cat_sprintf (HTTP_SERVER_NAME, SERVER_NAME);
	cat_sprintf (HTTP_CONTENT_TYPE, mime_type);
	//Date
	time_t timer;
	time (&timer);
	//struct tm *utc;
	//utc = gmtime(&timer);
	struct tm utc;
	gmtime_r (&timer, &utc);
	char work[80] = {};
	const static char* dow[] = { "Sun", "Mon","Tue", "Wed", "Thu", "Fri", "Sat" };
	const static char* mon[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	::sprintf (work, "%s, %d %s %d %02d:%02d:%02d",
			   dow[utc.tm_wday], utc.tm_mday, mon[utc.tm_mon], utc.tm_year + 1900, utc.tm_hour, utc.tm_min, utc.tm_sec);
	this->cat_sprintf ("Date: %s GMT\r\n", work);
	//expire
	if (expire) {
		timer += 60 * 60;
		//utc = gmtime(&timer);
		gmtime_r (&timer, &utc);
		::sprintf (work, "%s, %d %s %d %02d:%02d:%02d",
				   dow[utc.tm_wday], utc.tm_mday, mon[utc.tm_mon], utc.tm_year + 1900, utc.tm_hour, utc.tm_min, utc.tm_sec);
		this->cat_sprintf ("Expires: %s GMT\r\n", work);
	}
	if (content_length) {
		this->cat_sprintf (HTTP_CONTENT_LENGTH, content_length);
	}
}

/// <summary>
/// ヘッダー文字列をsocketに送出
/// </summary>
/// <param name="socket">送出ソケット</param>
/// <param name="endflag">指定するとHTTPヘッダ終了(\r\n\r\n)</param>
void wString::send_header (SOCKET socket, int endflag) const
{
	transport_send (socket, String, len, 0);
	if (endflag) {
		transport_send (socket, HTTP_END, (int)strlen (HTTP_END), 0);
	}
}
wString wString::headerPrintMem (void) const
{
	return *this + HTTP_END;
}

/// <summary>
/// BorlandのTStringList対応
/// ヘッダ作成
/// 301(恒久的)は転送先のURLが表示され、302(一時的)では元のURLが表示される
/// </summary>
/// <param name="str">header文字列</param>
/// <param name="flag">true:新規作成,false:継続作成</param>
/// <param name="status">301それ以外は302</param>
/// <returns></returns>
int wString::header (const char* str, int flag, int status)
{
	char head[80] = {};
	char body[80] = {};
	if (*str) {
		if (!flag) {
			Add (str);
			return 0;
		}
		for (auto i = 0; i < (int)strlen (str); i++) {
			if (str[i] == ' ') {
				memcpy (head, str, i + 1);
				head[i + 1] = 0;
				strcpy (body, str + i + 1);
				break;
			}
		}
		if (*head && *body) {
			if (strcmp (head, "Location: ") == 0) {
				if (status == 301) {
					header ("HTTP/1.0 301 Moved Permanetry", true);
				}
				else {
					header ("HTTP/1.0 302 Found", true);
				}
			}
			int count = lines ();
			wString str2;
			str2.sprintf ("%s%s", head, body);
			//あれば入れ替え
			for (int i = 0; i < count; i++) {
				wString tmp = get_list_string (i);
				if (strncmp (tmp.c_str (), head, strlen (head)) == 0) {
					insert_list_string (str2.c_str (), i);
					return 0;
				}
			}
			//なければ追加
			Add (str2);
			return 0;
		}
	}
	return 1;
}
#endif
extern GLOBAL_PARAM_T global_param;
/// <summary>
/// ローカルアドレスを文字列として返す
/// </summary>
/// <returns>ローカルアドレス</returns>
wString wString::get_local_address (void)
{
#ifdef __linux__
	//linux番の自IP/ホスト名を設定する
	struct ifaddrs* ifa_list;
	struct ifaddrs* ifa;
	char addrstr[256] = {};

	// 機器リスト取得
	if (getifaddrs (&ifa_list) == 0) {
		// 一覧作成
		for (ifa = ifa_list; ifa != NULL; ifa = ifa->ifa_next) {
			if (ifa->ifa_addr->sa_family == AF_INET) {
				// eth0の場合のみ
				if (*ifa->ifa_name == 'e') {
					inet_ntop (AF_INET, &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr, addrstr, sizeof (addrstr));
					break;
				}
			}
		}
		/* (7) */
		freeifaddrs (ifa_list);
		return addrstr;
	}
	else {
		return "";
	}
#else
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
	//ホスト名を取得する
	char hostname[256];
	if (gethostname (hostname, sizeof (hostname)) != 0) {
		throw new CScriptException ("get_local_address:GetHostName error.");
		//perror ("ディレクトリのオープンエラー");
		//return "";
	}
	//puts(hostname);

	// 有効なルーティングを検索
	// アクセス対象はDNS1.1.1.1
	DWORD dwDestAddr = 0x1111;
	PMIB_IPFORWARDROW pBestRoute = (PMIB_IPFORWARDROW)malloc (sizeof (MIB_IPFORWARDROW));
	if (pBestRoute == NULL) {
		// TODO:Throwする
		throw new CScriptException ("get_local_address:memory access error.");
		//exit (1);
	}
	DWORD dwRetVal = GetBestRoute (dwDestAddr, 0, pBestRoute);
	if (dwRetVal == NO_ERROR) {
		dwDestAddr = pBestRoute->dwForwardNextHop;
		//printf("Interface Index: %d\n", pBestRoute->dwForwardIfIndex);
		//printf("Next Hop: %s\n", inet_ntoa(*(struct in_addr*)&pBestRoute->dwForwardNextHop));
	}
	else {
		dwDestAddr = 0;
	}
	free (pBestRoute);

	//ホスト名からIPアドレスを取得する
	HOSTENT* hostend = gethostbyname (hostname);
	if (hostend == NULL) {
		return "";
	}
	IN_ADDR inaddr = {};
	// GetBestRouteで外に出られるアドレスを正とする。
	// もし全てのインターフェースが外に出られる場合はGetBestRouteが選択したルートとする。
	////for (auto i = 0; i < hostend->h_length; i++) {
	auto i = 0;
	while (hostend->h_addr_list[i] != NULL) {
		//DWORD aaa = (Dhostend->h_addr_list[i];
		// TODO:３バイト比較。厳密にすべき。
		if (memcmp (static_cast<void*>(hostend->h_addr_list[i]), static_cast<void*>(&dwDestAddr), 3) == 0) {
			inaddr.S_un.S_un_b.s_b1 = hostend->h_addr_list[i][0];
			inaddr.S_un.S_un_b.s_b2 = hostend->h_addr_list[i][1];
			inaddr.S_un.S_un_b.s_b3 = hostend->h_addr_list[i][2];
			inaddr.S_un.S_un_b.s_b4 = hostend->h_addr_list[i][3];
			break;
		}
		i += 1;
	}
	//ip.resize(256);
	//static char ip[256];
	//strcpy(ip, inet_ntoa(inaddr));
	return inet_ntoa (inaddr);
#endif
}
/// <summary>
/// ローカルポートを文字列として返す
/// </summary>
/// <returns>ローカルポート</returns>
int wString::get_local_port (void)
{
	return global_param.server_port;
}
/// <summary>
/// skinに名称を設定
/// config_file_read()と同様に、末尾がDELIMITERでなければ付与する
/// (skin_root + skin_name + file のパス結合と整合を取るため)
/// </summary>
/// <returns>成功失敗</returns>
bool wString::set_skin_name(char* str)
{
	wString name = str;
	if (name.ends_with(DELIMITER) == false) {
		name += DELIMITER;
	}
	global_param.skin_name = name;
	return true;
}
/// <summary>
/// accept_socketから、１行(CRLFか、LF単独が現れるまで)受信
/// CRLFは削除する。
/// </summary>
/// <param name="accept_socket"></param>
/// <returns>受信したサイズ,エラーは-1を返す</returns>
int wString::line_receive (SOCKET accept_socket)
{
	char 	byte_buf;
	clear ();
	// １行受信実行
	while (1) {
		int recv_len = transport_recv (accept_socket, &byte_buf, 1, 0);
		if (recv_len == 0) {
			if (len) {
				return len;
			}
			else {
				return -1;
			}
		}
		else if (recv_len < 0) { // 受信失敗チェック
#ifdef __linux__
			debug_log_output ("header read error cnt = %d error=%s\n", recv_len, strerror (errno));
#else
			debug_log_output ("header read error cnt = %d socket=%d error=%d\n", recv_len, accept_socket, WSAGetLastError ());
#endif
			return (-1);
		}
		else {
			// CR/LFチェック
			if (byte_buf == '\r') {
				continue;
			}
			else if (byte_buf == '\n') {
				break;
			}
			// バッファにセット
			*this += (char)byte_buf;
		}
	}
	return len;
}
