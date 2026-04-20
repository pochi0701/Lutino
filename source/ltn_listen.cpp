// ==========================================================================
//code=UTF8	tab=4
//
// Lutino:	Application SErver.
//
// 		ltn_listen.cpp
//		$Revision: 1.0 $
//		$Date: 2018/02/12 21:11:00 $
//
// ==========================================================================
//---------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <sys/types.h>
#include <signal.h>
#ifdef linux
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <error.h>
#include <cerrno>
#include <pthread.h>
#else
#include <io.h>
#endif
#include "ltn.h"
#include "ltn_tools.h"
#include "ltn_io.h"
#include "TlsLib.h"
//int volatile child_count = 0;
void    thread_process(const ACCESS_INFO& ac_in);
#ifdef linux
void* accessloop(void* arg);
#else
unsigned int __stdcall   accessloop(void* arg);
#endif
SOCKET	listen_socket;		//HTTP待ち受けソケット
SOCKET	listen_socket_ssl;	//HTTPS待ち受けソケット
static TlsCertFile g_certFile;	// HTTPS用証明書（ファイルから読み込み時）

// accessloopへ渡す引数
typedef struct {
	SOCKET* listen_socket;
	int     is_https;
} LISTEN_ARG;
/////////////////////////////////////////////////////////////////////////
// listenソケットを生成してbind/listenする共通関数
/////////////////////////////////////////////////////////////////////////
static SOCKET create_listen_socket(int port)
{
	struct sockaddr_in saddr = {};
	int sock_opt_val;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (SERROR(sock)) {
		debug_log_output("socket() error for port %d.", port);
		return INVALID_SOCKET;
	}
	sock_opt_val = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&sock_opt_val), sizeof(sock_opt_val));

	memset(reinterpret_cast<char*>(&saddr), 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons((u_short)port);

	int ret = bind(sock, reinterpret_cast<struct sockaddr*>(&saddr), sizeof(saddr));
	if (ret < 0) {
		debug_log_output("bind() error for port %d. ret=%d", port, ret);
		sClose(sock);
		return INVALID_SOCKET;
	}
	ret = listen(sock, LISTEN_BACKLOG);
	if (ret < 0) {
		debug_log_output("listen() error for port %d. ret=%d", port, ret);
		sClose(sock);
		return INVALID_SOCKET;
	}
	return sock;
}
/////////////////////////////////////////////////////////////////////////
// HTTPサーバ 待ち受け動作部
/////////////////////////////////////////////////////////////////////////
void	server_listen(void)
{
	// =============================
	// HTTP listenソケット生成
	// =============================
	listen_socket = create_listen_socket(global_param.server_port);
	if (SERROR(listen_socket)) {
		debug_log_output("HTTP listen socket creation failed.");
		return;
	}
	debug_log_output("HTTP listening on port %d...", global_param.server_port);

	// =============================
	// HTTPS listenソケット生成 (ssl_port > 0 のとき)
	// =============================
	int ssl_enabled = (global_param.server_ssl_port > 0);
	if (ssl_enabled) {
		listen_socket_ssl = create_listen_socket(global_param.server_ssl_port);
		if (SERROR(listen_socket_ssl)) {
			debug_log_output("HTTPS listen socket creation failed. SSL disabled.");
			ssl_enabled = 0;
		} else {
			debug_log_output("HTTPS listening on port %d...", global_param.server_ssl_port);
			// 証明書ファイル読み込み
			if (global_param.ssl_cert_file[0] && global_param.ssl_key_file[0]) {
				if (g_certFile.load(global_param.ssl_cert_file, global_param.ssl_key_file)) {
					debug_log_output("SSL cert loaded: %s", global_param.ssl_cert_file);
				} else {
					debug_log_output("SSL cert load FAILED: %s / %s  (using test cert)",
						global_param.ssl_cert_file, global_param.ssl_key_file);
				}
			} else {
				debug_log_output("No ssl_cert_file/ssl_key_file configured. Using test cert.");
			}
		}
	}

	// =====================
	// BLOCKING MODE設定
	// =====================
	debug_log_output("THREAD MODE START");

	// accessloop引数
	LISTEN_ARG http_arg  = { &listen_socket, 0 };
	LISTEN_ARG https_arg = { &listen_socket_ssl, 1 };

	int total_threads = MAXTHREAD + (ssl_enabled ? MAXTHREAD : 0);

#ifdef linux
	pthread_t* hdl = new pthread_t[total_threads];
	int idx = 0;
	for (int i = 0; i < MAXTHREAD; i++) {
		pthread_create(&hdl[idx++], NULL, accessloop, (void*)&http_arg);
	}
	if (ssl_enabled) {
		for (int i = 0; i < MAXTHREAD; i++) {
			pthread_create(&hdl[idx++], NULL, accessloop, (void*)&https_arg);
		}
	}
	for (int i = 0; i < total_threads; i++) {
		pthread_join(hdl[i], NULL);
	}
	delete[] hdl;
#else
	HANDLE* thread_handle = new HANDLE[total_threads]{};
	unsigned int* id = new unsigned int[total_threads]{};
	int idx = 0;

	for (auto i = 0; i < MAXTHREAD; i++) {
		thread_handle[idx] = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, accessloop, static_cast<void*>(&http_arg), 0, &id[idx]));
		idx++;
	}
	if (ssl_enabled) {
		for (auto i = 0; i < MAXTHREAD; i++) {
			thread_handle[idx] = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, accessloop, static_cast<void*>(&https_arg), 0, &id[idx]));
			idx++;
		}
	}

	WaitForMultipleObjects(total_threads, thread_handle, TRUE, INFINITE);
	for (auto i = 0; i < total_threads; i++) {
		if (thread_handle[i]) {
			CloseHandle(thread_handle[i]);
		}
	}
	delete[] thread_handle;
	delete[] id;
#endif
	sClose(listen_socket);
	if (ssl_enabled) {
		sClose(listen_socket_ssl);
	}
	return;
}
/////////////////////////////////////////////////////////////////////////
// 複数アクセス対応
/////////////////////////////////////////////////////////////////////////
#ifdef linux
void* accessloop(void* arg)
#else
unsigned int __stdcall accessloop(void* arg)
#endif
{
	LISTEN_ARG* la = static_cast<LISTEN_ARG*>(arg);
	int                lis_soc = (int)*la->listen_socket;
	int                is_https = la->is_https;
	struct sockaddr_in caddr = {};					// クライアントソケットアドレス構造体
	socklen_t          caddr_len = sizeof(caddr);   // クライアントソケットアドレス構造体のサイズ
	char               access_host[256] = {};


	// =====================
	// メインループ
	// =====================
	//ready_flag += 1;
	while (loop_flag) {
		// ====================
		// Accept待ち.
		// ====================

		//debug_log_output("Waiting for a new client...");
		// 接続Socket
		auto accept_socket = accept(lis_soc, reinterpret_cast<struct sockaddr*>(&caddr), &caddr_len);
		if (SERROR(accept_socket)) // accept失敗チェック
		{
			debug_log_output("accept() error. ret=%d\n", accept_socket);
			continue;           // 最初に戻る。
		}
		// 停止時Unit1.cpp で listen_socketをclosesocketし、loop_flagに0を入れています。
		if (!loop_flag) {
			debug_log_output("%s(%d) accept_socet\n", __FILE__, __LINE__);
			sClose(accept_socket);       // Socketクローズ
			break;
		}
		ACCESS_INFO ac_in = {};
		ac_in.accept_socket = (unsigned int)accept_socket;
		ac_in.access_host = access_host;
		ac_in.caddr = caddr;
		ac_in.is_https = is_https;
		thread_process(ac_in);
	}
	//_endthreadex(0);
	return NULL;
}
/////////////////////////////////////////////////////////////////////////
// 複数アクセス対応
/////////////////////////////////////////////////////////////////////////
void thread_process(const ACCESS_INFO& ac_in)
{
	int        access_check_ok;
	char       client_addr_str[32] = {};
	char       client_address[4] = {};
	char       masked_client_address[4] = {};
	char       access_host[256] = {};
	//ローカルに保存して縁を切る
	SOCKET accept_socket = (unsigned int)ac_in.accept_socket;
	struct sockaddr_in  caddr = ac_in.caddr;         // クライアントソケットアドレス構造体
	strcpy(access_host, ac_in.access_host);
	//debug_log_output("\n\n=============================================================\n");
	//debug_log_output("Socket Accept!!(accept_socket=%d)\n", accept_socket);
	//child_count ++;
	std::ignore = rand();
	// caddr 情報表示
	//debug_log_output("client addr = %s\n", inet_ntoa(caddr.sin_addr));
	//debug_log_output("client port = %d\n", ntohs(caddr.sin_port));
	// ==============================
	// アクセスチェック
	// ==============================
	access_check_ok = FALSE;
	// クライアントアドレス
	strncpy(static_cast<char*>(client_addr_str), inet_ntoa(caddr.sin_addr), sizeof(client_addr_str));
	// -------------------------------------------------------------------------
	// Access Allowチェック
	//  リストが空か、クライアントアドレスが、リストに一致したらＯＫとする。
	// -------------------------------------------------------------------------
	if (access_allow_list[0].flag == FALSE) { // アクセスリストが空。
		// チェックＯＫとする。
		//debug_log_output("No Access Allow List. No Check.\n");
		access_check_ok = TRUE;
	}
	else {
		debug_log_output("Access Check.\n");
		// クライアントアドレス
		strncpy(client_addr_str, inet_ntoa(caddr.sin_addr), sizeof(client_addr_str) - 1);
		// client_addr_strをchar[4]に変換
		strncat(client_addr_str, ".", sizeof(client_addr_str) - 1);
		for (auto i = 0; i < 4; i++) {
			char       work1[32];
			char       work2[32];
			sentence_split(client_addr_str, '.', work1, work2);
			client_address[i] = (unsigned char)atoi(work1);
			strncpy(client_addr_str, work2, sizeof(client_addr_str));
		}
		// リストの存在する数だけループ
		for (auto i = 0; i < ACCESS_ALLOW_LIST_MAX; i++) {
			if (access_allow_list[i].flag == FALSE) { // リスト終了
				break;
			}
			// masked_client_address 生成
			masked_client_address[0] = client_address[0] & access_allow_list[i].netmask[0];
			masked_client_address[1] = client_address[1] & access_allow_list[i].netmask[1];
			masked_client_address[2] = client_address[2] & access_allow_list[i].netmask[2];
			masked_client_address[3] = client_address[3] & access_allow_list[i].netmask[3];
			// 比較実行
			if ((masked_client_address[0] == access_allow_list[i].address[0]) &&
				(masked_client_address[1] == access_allow_list[i].address[1]) &&
				(masked_client_address[2] == access_allow_list[i].address[2]) &&
				(masked_client_address[3] == access_allow_list[i].address[3]))
			{
				debug_log_output("[%d.%d.%d.%d] == [%d.%d.%d.%d] accord!!",
					masked_client_address[0], masked_client_address[1], masked_client_address[2], masked_client_address[3],
					access_allow_list[i].address[0], access_allow_list[i].address[1], access_allow_list[i].address[2], access_allow_list[i].address[3]);
				access_check_ok = TRUE;
				break;
			}
			else {
				debug_log_output("[%d.%d.%d.%d] == [%d.%d.%d.%d] discord!!",
					masked_client_address[0], masked_client_address[1], masked_client_address[2], masked_client_address[3],
					access_allow_list[i].address[0], access_allow_list[i].address[1], access_allow_list[i].address[2], access_allow_list[i].address[3]);
			}
		}
	}
	if (access_check_ok == FALSE) {
		debug_log_output("Access Denied.\n");
		debug_log_output("%s(%d) accept_socet\n", __FILE__, __LINE__);
		sClose(accept_socket);     // Socketクローズ
	}
	else if (ac_in.is_https) {
		// HTTPS: TLSハンドシェイク後にHTTP処理
		TlsConn tls;
		bool ok;
		if (g_certFile.isLoaded()) {
			ok = tls.open(accept_socket, g_certFile);
		} else {
			ok = tls.open(accept_socket);
		}
		if (!ok) {
			debug_log_output("TLS handshake failed. err=%d", tls.lastError());
			sClose(accept_socket);
		} else {
			debug_log_output("TLS handshake OK from %s", client_addr_str);
			// TLSラッパーを設定してserver_http_processを呼ぶ
			ltn_set_tls(&tls);
			server_http_process(accept_socket, access_host, client_addr_str);
			// server_http_process内のltn_closeでTLSはクローズされるが、念のためクリア
			ltn_set_tls(nullptr);
		}
	}
	else {
		// HTTP鯖として、仕事実行
		server_http_process(accept_socket, access_host, client_addr_str);
		//debug_log_output ("HTTP Process done. From %s:%d\n", inet_ntoa (caddr.sin_addr), ntohs (caddr.sin_port));

		//sClose(accept_socket);
	}
}
