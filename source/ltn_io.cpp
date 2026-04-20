// ==========================================================================
//code=UTF8	tab=4
//
// Lutino:	Application SErver.
//
// 		ltn_io.cpp
//		TLS透過I/Oラッパー実装
//
// ==========================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "ltn_io.h"
#include "TlsLib.h"

#ifdef linux
#include <unistd.h>
#include <sys/socket.h>
#else
#include <io.h>
#endif

// スレッドローカル: 現在のスレッドに関連付けられたTLS接続
static thread_local TlsConn* t_tls = nullptr;

void ltn_set_tls(TlsConn* tls)
{
	t_tls = tls;
}

TlsConn* ltn_get_tls(void)
{
	return t_tls;
}

int ltn_recv(SOCKET sock, void* buf, size_t len, int flags)
{
	(void)flags;
	if (t_tls) {
		return t_tls->read(buf, len);
	}
	return recv(sock, reinterpret_cast<char*>(buf), (int)len, flags);
}

int ltn_send(SOCKET sock, const void* buf, size_t len, int flags)
{
	(void)flags;
	if (t_tls) {
		int ret = t_tls->write(buf, len);
		if (ret < 0) return -1;
		// write_allは全バイト書き込むので、成功時はlenを返す
		return (int)len;
	}
	return send(sock, reinterpret_cast<const char*>(buf), (int)len, flags);
}

int ltn_close(SOCKET& sock)
{
	if (t_tls) {
		t_tls->flush();
		t_tls->close();
		t_tls = nullptr;
		sock = 0;
		return 0;
	}
	return sClose(sock);
}
