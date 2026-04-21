#ifndef LTN_TLS_H
#define LTN_TLS_H

#include "const.h"

bool ltn_tls_attach(SOCKET socket);
bool ltn_tls_attach_client(SOCKET socket, const char* server_name);
bool ltn_tls_active(SOCKET socket);
int ltn_tls_send(SOCKET socket, const char* buffer, size_t length);
int ltn_tls_recv(SOCKET socket, char* buffer, size_t length);
int ltn_tls_close(SOCKET socket);
void ltn_tls_shutdown_all(void);

#endif /* LTN_TLS_H */
