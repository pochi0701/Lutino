#define _CRT_SECURE_NO_WARNINGS

#include "ltn_tools.h"
#include "ltn_tls.h"

bool transport_attach_tls(SOCKET socket)
{
	return ltn_tls_attach(socket);
}

bool transport_attach_tls_client(SOCKET socket, const char* server_name)
{
	return ltn_tls_attach_client(socket, server_name);
}

void transport_shutdown_all(void)
{
	ltn_tls_shutdown_all();
}

int transport_send(SOCKET socket, const char* buffer, unsigned int length, int mode)
{
	(void)mode;
	if (ltn_tls_active(socket)) {
		return ltn_tls_send(socket, buffer, length);
	}
#ifdef linux
	return ::send(socket, buffer, length, 0);
#else
	return ::send(socket, buffer, static_cast<int>(length), 0);
#endif
}

int transport_recv(SOCKET socket, char* buffer, unsigned int length, int mode)
{
	(void)mode;
	if (ltn_tls_active(socket)) {
		return ltn_tls_recv(socket, buffer, length);
	}
#ifdef linux
	return ::recv(socket, buffer, length, 0);
#else
	return ::recv(socket, buffer, static_cast<int>(length), 0);
#endif
}

int transport_close(SOCKET& socket)
{
	if (socket == 0 || socket == INVALID_SOCKET) {
		socket = 0;
		return 0;
	}
	if (ltn_tls_active(socket)) {
		auto target = socket;
		socket = 0;
		return ltn_tls_close(target);
	}
	return sClose(socket);
}
