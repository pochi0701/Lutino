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
#ifdef __linux__
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
//int volatile child_count = 0;
void    thread_process(const ACCESS_INFO& ac_in);
namespace {
struct LISTENER_CONTEXT {
	SOCKET listen_socket;
	int use_tls;
	int port;
	const char* name;
};

SOCKET create_listener_socket(int port, const char* name)
{
	int ret;
	int sock_opt_val = 1;
	struct sockaddr_in saddr = {};
	auto socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (SERROR(socket_fd)) {
		debug_log_output("%s socket() error.", name);
		perror("socket");
		return INVALID_SOCKET;
	}
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&sock_opt_val), sizeof(sock_opt_val));
	memset(reinterpret_cast<char*>(&saddr), 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons((u_short)port);
	ret = bind(socket_fd, reinterpret_cast<struct sockaddr*>(&saddr), sizeof(saddr));
	if (ret < 0) {
		debug_log_output("%s bind() error. ret=%d", name, ret);
		perror("bind");
		sClose(socket_fd);
		return INVALID_SOCKET;
	}
	ret = listen(socket_fd, LISTEN_BACKLOG);
	if (ret < 0) {
		debug_log_output("%s listen() error. ret=%d", name, ret);
		perror("listen");
		sClose(socket_fd);
		return INVALID_SOCKET;
	}
	return socket_fd;
}
}
#ifdef __linux__
void* accessloop(void* arg);
#else
unsigned int __stdcall   accessloop(void* arg);
#endif
SOCKET	listen_socket;	//待ち受けソケット
SOCKET	listen_socket_tls;	// HTTPS待ち受けソケット
/////////////////////////////////////////////////////////////////////////
// HTTPサーバ 待ち受け動作部
/////////////////////////////////////////////////////////////////////////
void	server_listen(void)
{
	//int ret = 0;
	LISTENER_CONTEXT listeners[2] = {};
	int listener_count = 0;

	listen_socket = create_listener_socket(global_param.server_port, "HTTP");
	if (SERROR(listen_socket)) {
		return;
	}
	listeners[listener_count++] = { listen_socket, FALSE, global_param.server_port, "HTTP" };

	listen_socket_tls = INVALID_SOCKET;
	if (global_param.server_tls_port > 0 && global_param.server_tls_port != global_param.server_port) {
		listen_socket_tls = create_listener_socket(global_param.server_tls_port, "HTTPS");
		if (!SERROR(listen_socket_tls)) {
			listeners[listener_count++] = { listen_socket_tls, TRUE, global_param.server_tls_port, "HTTPS" };
		}
	}
	// =====================
	// BLOCKING MODE設定
	// =====================
	debug_log_output("THREAD MODE START");
	//TODO:worker数で設定出来るようにすること
#ifdef __linux__
	pthread_t hdl[MAXTHREAD * 2];
	int thread_count = 0;
	for (int listener_index = 0; listener_index < listener_count; listener_index++) {
		for (int i = 0; i < MAXTHREAD; i++) {
			pthread_create(&hdl[thread_count++], NULL, accessloop, static_cast<void*>(&listeners[listener_index]));
		}
	}
	for (int i = 0; i < thread_count; i++) {
		int ret = pthread_join(hdl[i], NULL);
	}
#else

	HANDLE thread_handle[MAXTHREAD * 2] = {};
	unsigned int  id[MAXTHREAD * 2] = {};
	int thread_count = 0;

	for (int listener_index = 0; listener_index < listener_count; listener_index++) {
		for (auto i = 0; i < MAXTHREAD; i++) {
			thread_handle[thread_count] = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, accessloop, static_cast<void*>(&listeners[listener_index]), 0, &id[thread_count]));
			thread_count++;
		}
	}

	WaitForMultipleObjects(thread_count, thread_handle, TRUE, INFINITE);
	for (auto i = 0; i < thread_count; i++) {
		if (thread_handle[i]) {
			CloseHandle(thread_handle[i]);
		}
	}
#endif
	transport_shutdown_all();
	transport_close(listen_socket);
	transport_close(listen_socket_tls);
	return;
}
/////////////////////////////////////////////////////////////////////////
// 複数アクセス対応
/////////////////////////////////////////////////////////////////////////
#ifdef __linux__
void* accessloop(void* arg)
#else
unsigned int __stdcall accessloop(void* arg)
#endif
{
	auto listener = static_cast<LISTENER_CONTEXT*>(arg);
	SOCKET             lis_soc = listener->listen_socket;
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
		ac_in.use_tls = listener->use_tls;
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
	int use_tls = ac_in.use_tls;
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
		transport_close(accept_socket);     // Socketクローズ
	}
	else {
		if (use_tls && !transport_attach_tls(accept_socket)) {
			debug_log_output("TLS attach failed for socket=%d\n", accept_socket);
			transport_close(accept_socket);
			return;
		}
		// HTTP鯖として、仕事実行
		server_http_process(accept_socket, access_host, client_addr_str);
		//debug_log_output ("HTTP Process done. From %s:%d\n", inet_ntoa (caddr.sin_addr), ntohs (caddr.sin_port));

		//sClose(accept_socket);
	}
}
