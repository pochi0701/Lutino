// ==========================================================================
//code=UTF8	tab=4
//
// Lutino:	Application SErver.
//
// 		ltn_io.h
//		TLS透過I/Oラッパー
//
// ==========================================================================
#ifndef _LTN_IO_H
#define _LTN_IO_H

#include "ltn_tools.h"

class TlsConn;

// スレッドローカルTLS接続の設定・取得
void      ltn_set_tls(TlsConn* tls);
TlsConn*  ltn_get_tls(void);

// TLS透過ラッパー関数
// t_tls != nullptr のときはTLS経由、nullptr のときは通常socket API
int  ltn_recv(SOCKET sock, void* buf, size_t len, int flags);
int  ltn_send(SOCKET sock, const void* buf, size_t len, int flags);
int  ltn_close(SOCKET& sock);

#endif /* _LTN_IO_H */
