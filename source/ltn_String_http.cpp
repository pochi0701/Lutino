// ==========================================================================
//code=UTF8	tab=4
//
// Lutino:	Application SErver.
//
// 		ltn_String_http.cpp
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

namespace {
struct HttpUrlParts {
	bool    use_tls = false;
	wString host;
	wString target;
	int     port = HTTP_SERVER_PORT;
};

bool parse_http_url(const wString& url, HttpUrlParts& info)
{
	if (url.starts_with("https://")) {
		info.use_tls = true;
		info.port = 443;
	}
	else if (url.starts_with("http://")) {
		info.use_tls = false;
		info.port = HTTP_SERVER_PORT;
	}
	else {
		return false;
	}

	int host_pos = url.Pos("://");
	if (host_pos < 0) {
		return false;
	}
	host_pos += 3;

	int path_pos = url.Pos("/", host_pos);
	wString authority;
	if (path_pos >= 0) {
		info.target = url.substr(path_pos, url.length() - path_pos);
		authority = url.substr(host_pos, path_pos - host_pos);
	}
	else {
		info.target = "/";
		authority = url.substr(host_pos, url.length() - host_pos);
	}

	int port_pos = authority.Pos(":");
	if (port_pos >= 0) {
		info.port = atoi(authority.c_str() + port_pos + 1);
		info.host = authority.substr(0, port_pos);
	}
	else {
		info.host = authority;
	}

	return info.host.length() > 0;
}

SOCKET open_http_socket(const HttpUrlParts& info)
{
	auto socket = wString::sock_connect(info.host.c_str(), info.port);
	if (SERROR(socket)) {
		return INVALID_SOCKET;
	}
	if (info.use_tls && !transport_attach_tls_client(socket, info.host.c_str())) {
		sClose(socket);
		return INVALID_SOCKET;
	}
	return socket;
}

class HttpSocketGuard
{
private:
	SOCKET socket;

public:
	explicit HttpSocketGuard(SOCKET socket) : socket(socket) {}
	~HttpSocketGuard()
	{
		transport_close(socket);
	}

	SOCKET get() const
	{
		return socket;
	}
};

bool is_http_header_name(const wString& name);
wString trim_http_whitespace(const wString& value);

bool read_http_response(SOCKET socket, wString& response, bool head_request = false, bool framed_response = false)
{
	char buffer[4096];
	response.clear();
	if (!framed_response) {
		while (loop_flag) {
			auto recv_len = transport_recv(socket, buffer, sizeof(buffer), 0);
			if (recv_len > 0) {
				response.set_binary(buffer, recv_len);
			}
			else if (recv_len == 0) {
				return true;
			}
			else {
				return false;
			}
		}
		return response.length() > 0;
	}

	bool headers_received = false;
	bool has_content_length = false;
	size_t content_length = 0;
	size_t header_length = 0;
	while (loop_flag) {
		auto recv_len = transport_recv(socket, buffer, sizeof(buffer), 0);
		if (recv_len > 0) {
			response.set_binary(buffer, recv_len);
			if (!headers_received) {
				auto delimiter = response.Pos(HTTP_DELIMITER);
				if (delimiter < 0) continue;
				header_length = static_cast<size_t>(delimiter) + 4;
				auto headers = response.substr(0, delimiter + 4);
				auto line_end = headers.find("\r\n");
				auto status_line = headers.substr(0, line_end);
				if (!status_line.starts_with("HTTP/")) return false;
				auto status_pos = status_line.find(' ');
				if (status_pos == wString::npos) return false;
				int status = atoi(status_line.c_str() + status_pos + 1);
				if (head_request || status == 204 || status == 304) return true;
				auto line_start = line_end + 2;
				while (line_start < delimiter) {
					line_end = headers.find("\r\n", line_start);
					auto colon = headers.find(':', line_start);
					if (colon == wString::npos || colon >= line_end) return false;
					auto name = headers.substr(line_start, colon - line_start);
					if (!is_http_header_name(name)) return false;
					name = name.ToLower();
					auto value = trim_http_whitespace(headers.substr(colon + 1, line_end - colon - 1));
					if (name == "transfer-encoding") return false;
					if (name == "content-length") {
						if (value.empty()) return false;
						size_t length = 0;
						for (unsigned int i = 0; i < value.length(); ++i) {
							char digit = value[i];
							if (digit < '0' || digit > '9'
								|| length > ((std::numeric_limits<size_t>::max)() - (digit - '0')) / 10) return false;
							length = length * 10 + (digit - '0');
						}
						if (has_content_length && length != content_length) return false;
						content_length = length;
						has_content_length = true;
					}
					line_start = line_end + 2;
				}
				headers_received = true;
			}
			if (has_content_length && response.length() - header_length >= content_length) {
				return response.length() - header_length == content_length;
			}
		}
		else if (recv_len == 0) {
			return headers_received && !has_content_length;
		}
		else {
			// HTTP/1.0 peers may delimit the body with TCP EOF without TLS close_notify.
			if (headers_received && !has_content_length && transport_peer_closed(socket)) {
				debug_log_output("HTTP close-delimited response ended without TLS close_notify.");
				return true;
			}
			return false;
		}
	}
	return false;
}

bool contains_http_control(const wString& value)
{
	for (unsigned int i = 0; i < value.length(); ++i) {
		if (value[i] == '\r' || value[i] == '\n' || value[i] == '\0') {
			return true;
		}
	}
	return false;
}

bool is_http_token_char(unsigned char ch)
{
	return (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')
		|| ch == '!' || ch == '#' || ch == '$' || ch == '%' || ch == '&' || ch == '\''
		|| ch == '*' || ch == '+' || ch == '-' || ch == '.' || ch == '^' || ch == '_'
		|| ch == '`' || ch == '|' || ch == '~';
}

wString trim_http_whitespace(const wString& value)
{
	unsigned int first = 0;
	unsigned int end = value.length();
	while (first < end && (value[first] == ' ' || value[first] == '\t')) {
		++first;
	}
	while (end > first && (value[end - 1] == ' ' || value[end - 1] == '\t')) {
		--end;
	}
	return value.substr(first, end - first);
}

bool is_http_header_name(const wString& name)
{
	if (name.empty()) {
		return false;
	}
	for (unsigned int i = 0; i < name.length(); ++i) {
		unsigned char ch = static_cast<unsigned char>(name[i]);
		if (!is_http_token_char(ch)) {
			return false;
		}
	}
	return true;
}

bool is_reserved_request_header(const wString& name)
{
	auto lowercase = name.ToLower();
	return lowercase == "host" || lowercase == "content-length" || lowercase == "connection";
}

int get_http_status_code(const wString& response)
{
	auto pos = response.Pos(" ");
	if (pos < 0) {
		return -1;
	}
	return atoi(response.c_str() + pos + 1);
}

bool split_http_response(const wString& response, wString& body)
{
	auto pos = response.Pos(HTTP_DELIMITER);
	if (pos == wString::npos) {
		return false;
	}
	body = response.substr(pos + 4);
	return true;
}

bool parse_http_response(const wString& response, HttpRestResponse& parsed)
{
	auto delimiter = response.Pos(HTTP_DELIMITER);
	if (delimiter == wString::npos) {
		return false;
	}
	auto status_end = response.Pos("\r\n");
	if (status_end < 0 || status_end > delimiter) {
		return false;
	}
	auto status_line = response.substr(0, status_end);
	auto first_space = status_line.Pos(" ");
	auto second_space = first_space < 0 ? -1 : status_line.Pos(" ", first_space + 1);
	if (first_space < 0 || !status_line.starts_with("HTTP/")) {
		return false;
	}
	auto status_code = status_line.substr(first_space + 1,
		(second_space < 0 ? status_line.length() : static_cast<unsigned int>(second_space)) - first_space - 1);
	if (status_code.length() != 3 || status_code[0] < '0' || status_code[0] > '9'
		|| status_code[1] < '0' || status_code[1] > '9'
		|| status_code[2] < '0' || status_code[2] > '9') {
		return false;
	}
	parsed.status = atoi(status_code.c_str());
	if (second_space >= 0) {
		parsed.statusText = status_line.substr(second_space + 1);
	}

	int line_start = status_end + 2;
	while (line_start < delimiter) {
		int line_end = response.Pos("\r\n", line_start);
		if (line_end < 0 || line_end > delimiter) {
			return false;
		}
		auto line = response.substr(line_start, line_end - line_start);
		int colon = line.Pos(":");
		if (colon <= 0) {
			return false;
		}
		auto name = line.substr(0, colon);
		if (!is_http_header_name(name)) {
			return false;
		}
		auto value = line.substr(colon + 1);
		auto normalized_name = name.ToLower();
		auto normalized_value = trim_http_whitespace(value);
		auto existing = std::find_if(parsed.headers.begin(), parsed.headers.end(),
			[&normalized_name](const HttpRestHeader& header) { return header.name == normalized_name; });
		if (existing == parsed.headers.end()) {
			parsed.headers.push_back({ normalized_name, normalized_value });
		}
		else {
			existing->value += ", ";
			existing->value += normalized_value;
		}
		line_start = line_end + 2;
	}
	parsed.body = response.substr(delimiter + 4);
	parsed.ok = true;
	return true;
}
}
#if 1
constexpr auto HTTP_STR_BUF_SIZE = (1024);
/// <summary>
/// HTTPより文字列データを取得
/// </summary>
/// <param name="url">URL</param>
/// <param name="offset">読み込みオフセット</param>
/// <returns>HTTP応答本文。空本文も正常な応答として返る。</returns>
/// <exception cref="HttpRequestException">URL、接続、送受信、HTTP応答のいずれかに失敗した場合。</exception>
wString wString::http_get (const wString& url, off_t offset)
{
	wString     buf;
	wString     request;
	wString     body;
	HttpUrlParts target_info;
	SOCKET      server_socket;                  //サーバーソケット
	if (!parse_http_url(url, target_info)) {
		throw HttpRequestException("Invalid HTTP URL");
	}
	server_socket = open_http_socket(target_info);
	if (SERROR(server_socket)) {
		throw HttpRequestException("Unable to connect to HTTP server");
	}
	HttpSocketGuard socket_guard(server_socket);

	request.sprintf("GET %s HTTP/1.0\r\n"
					"Accept: */*\r\n"
					"User-Agent: %s%s\r\nHost: %s\r\nRange: bytes=%llu-\r\nConnection: close\r\n\r\n",
					target_info.target.c_str(),
					USERAGENT,
					MACADDR,
					target_info.host.c_str(),
					static_cast<unsigned long long>(offset));
	if (transport_send(socket_guard.get(), request.c_str(), request.length(), 0) == SOCKET_ERROR) {
		throw HttpRequestException("Failed to send HTTP request");
	}
	if (!read_http_response(socket_guard.get(), buf)) {
		throw HttpRequestException("Failed to receive HTTP response");
	}
	auto status = get_http_status_code(buf);
	if (status < 200 || 300 <= status) {
		throw HttpRequestException("HTTP server returned an unsuccessful status");
	}
	if (!split_http_response(buf, body)) {
		throw HttpRequestException("Malformed HTTP response");
	}
	return body;
}
///---------------------------------------------------------------------------
HttpRestResponse wString::http_rest(const wString& methods, const wString& url, const wString& data,
	const std::vector<HttpRestHeader>& headers)
{
	if (contains_http_control(url)) {
		throw HttpRequestException("HTTP URL contains a forbidden control character");
	}
	if (!is_http_header_name(methods)) {
		throw HttpRequestException("Invalid HTTP method");
	}

	bool has_accept = false;
	bool has_user_agent = false;
	bool has_content_type = false;
	for (const auto& header : headers) {
		if (!is_http_header_name(header.name)) {
			throw HttpRequestException("Invalid HTTP header name");
		}
		if (is_reserved_request_header(header.name)) {
			throw HttpRequestException("HTTP header is managed internally");
		}
		if (contains_http_control(header.value)) {
			throw HttpRequestException("HTTP header value contains a forbidden control character");
		}
		auto lowercase_name = header.name.ToLower();
		has_accept = has_accept || lowercase_name == "accept";
		has_user_agent = has_user_agent || lowercase_name == "user-agent";
		has_content_type = has_content_type || lowercase_name == "content-type";
	}

	HttpUrlParts target_info;
	if (!parse_http_url(url, target_info)) {
		throw HttpRequestException("Invalid HTTP URL");
	}
	auto server_socket = open_http_socket(target_info);
	if (SERROR(server_socket)) {
		throw HttpRequestException("Unable to connect to HTTP server");
	}
	HttpSocketGuard socket_guard(server_socket);

	wString request;
	request.sprintf("%s %s HTTP/1.0\r\n", methods.c_str(), target_info.target.c_str());
	if (!has_accept) {
		request += "Accept: */*\r\n";
	}
	if (!has_user_agent) {
		request += "User-Agent: ";
		request += USERAGENT;
		request += MACADDR;
		request += "\r\n";
	}
	request += "Host: ";
	request += target_info.host;
	if ((target_info.use_tls && target_info.port != 443)
		|| (!target_info.use_tls && target_info.port != HTTP_SERVER_PORT)) {
		request += ":";
		wString port;
		port.sprintf("%d", target_info.port);
		request += port;
	}
	request += "\r\n";
	for (const auto& header : headers) {
		request += header.name;
		request += ": ";
		request += header.value;
		request += "\r\n";
	}
	if (methods != "GET") {
		request += "Content-Length: ";
		wString content_length;
		content_length.sprintf("%u", data.length());
		request += content_length;
		request += "\r\n";
		if (!has_content_type) {
			request += "Content-Type: text/plain;charset=UTF-8\r\n";
		}
	}
	request += "Connection: close\r\n\r\n";
	if (methods != "GET") {
		request += data;
	}

	unsigned int sent_total = 0;
	while (sent_total < request.length()) {
		auto sent = transport_send(socket_guard.get(), request.c_str() + sent_total,
			request.length() - sent_total, 0);
		if (sent <= 0) {
			throw HttpRequestException("Failed to send HTTP request");
		}
		sent_total += static_cast<unsigned int>(sent);
	}

	wString raw_response;
	if (!read_http_response(socket_guard.get(), raw_response, methods == "HEAD", true)) {
		throw HttpRequestException("Failed to receive HTTP response");
	}
	HttpRestResponse parsed;
	if (!parse_http_response(raw_response, parsed)) {
		throw HttpRequestException("Malformed HTTP response");
	}
	return parsed;
}
///---------------------------------------------------------------------------
/// <summary>
/// ソケットを作成し、相手に接続するラッパー
/// </summary>
/// <param name="host">ホスト名</param>
/// <param name="port">ポート</param>
/// <returns>成功時:ソケット番号、失敗時:-1</returns>
SOCKET wString::sock_connect (const wString& host, const int port)
{
	return sock_connect (host.String, port);
}
///---------------------------------------------------------------------------
/// <summary>
/// ソケット接続
/// </summary>
/// <param name="host">ホスト名</param>
/// <param name="port">ポート</param>
/// <returns>成功時:ソケット番号、失敗時:-1</returns>
SOCKET wString::sock_connect (const char* host, const int port)
{
	//  int sock;
	SOCKET sock;
	struct sockaddr_in sockadd = {};     //ＳＯＣＫＥＴ構造体
	struct hostent* hent;
	debug_log_output ("sock_connect: %s:%d", host, port);
	//ＳＯＣＫＥＴ作成
	if (SERROR (sock = socket (PF_INET, SOCK_STREAM, 0))) {
		debug_log_output ("sock_connect_error:");
		return INVALID_SOCKET;
	}
	//debug_log_output("sock: %d", sock);
	if (NULL == (hent = gethostbyname (host))) {
		sClose (sock);
		return INVALID_SOCKET;
	}
	debug_log_output ("hent: %p", hent);
	//ソケット構造体へアドレス設定
	memcpy (&sockadd.sin_addr, hent->h_addr, hent->h_length);
	//ソケット構造体へポート設定
	sockadd.sin_port = htons ((u_short)port);
	//ＩＰＶ４アドレスファミリを設定
	sockadd.sin_family = AF_INET;
	//接続
	if (SERROR (connect (sock, reinterpret_cast<struct sockaddr*>(&sockadd), sizeof (sockadd)))) {
		sClose (sock);
		return INVALID_SOCKET;
	}
	return sock;
}
///---------------------------------------------------------------------------
/// <summary>
/// URLがアクセス可能かチェックする
/// /形式はwww.make-it.co.jp/index.html
/// </summary>
/// <param name="url">アクセス先url</param>
/// <returns>アクセス可能ならtrue</returns>
bool wString::check_url (const wString& url)
{
	wString buf;
	bool    access_flag = false;
	HttpUrlParts target_info;
	if (url.starts_with("http://") || url.starts_with("https://")) {
		if (!parse_http_url(url, target_info)) {
			return false;
		}
	}
	else {
		auto ptr = url.Pos("/");
		target_info.host = url.substr(0, ptr);
		target_info.target = url.substr(ptr, url.length() - ptr);
		target_info.port = HTTP_SERVER_PORT;
		if (target_info.target.length() == 0) {
			target_info.target = "/";
		}
	}
	SOCKET server_socket = open_http_socket(target_info);
	if (!SERROR(server_socket)) {
		buf.sprintf ("HEAD %s HTTP/1.0\r\n"
					 "User%cAgent: "
					 USERAGENT
					 "\r\n"
					 "Host: %s\r\n"
					 "Connection: close\r\n"
					 "\r\n",
					 target_info.target.c_str (),
					 '-',
					 target_info.host.c_str ()
		);
		int dd = transport_send(server_socket, buf.c_str(), buf.length(), 0);
		if (dd != SOCKET_ERROR) {
			buf.set_length (1024);
			auto recv_len = transport_recv(server_socket, buf.c_str(), buf.capacity() - 1, 0);
			buf.reset_length (recv_len);
			//見つからない
			auto ptr = atoi (buf.String + (buf.Pos (" ") + 1));
			// 受信データありならば(ファイル有り）、データを解析する。
			if (200 <= ptr && ptr < 300) {
				access_flag = true;
			}
		}
		transport_close(server_socket);
	}
	return access_flag;
}
///---------------------------------------------------------------------------
/// <summary>
/// 対象httpのサイズ取得
/// </summary>
/// <param name="url">参照先URL</param>
/// <returns>取得した場合コンテンツ長さ、エラー時-1</returns>
int wString::http_size (const wString& url)
{
	wString     buf;
	HttpUrlParts target_info;
	int         content_length = -1;
	SOCKET      server_socket;                          //サーバーソケット
	if (!parse_http_url(url, target_info)) {
		return -1;
	}
	server_socket = open_http_socket(target_info);
	if (!SERROR(server_socket)) {
		buf.sprintf ("HEAD %s HTTP/1.0\r\n"
					 "Accept: */*\r\n"
					 "User-Agent: %s%s\r\n"
					 "Host: %s\r\n"
					 "Connection: close\r\n\r\n",
					 target_info.target.uri_encode ().c_str (),
					 USERAGENT,
					 MACADDR,
					 target_info.host.c_str ()
		);
		if (transport_send(server_socket, buf.c_str(), buf.length(), 0) != SOCKET_ERROR) {
			buf.set_length (HTTP_STR_BUF_SIZE);
			auto recv_len = transport_recv(server_socket, buf.c_str(), buf.capacity() - 1, 0);
			buf.reset_length (recv_len);      //糸止め
			auto work1 = atoi (buf.String + (buf.Pos (" ") + 1));
			int pos = buf.Pos ("Content-Length:");
			if (pos >= 0) {
				if (200 <= work1 && work1 < 300) {
					content_length = atoi (buf.c_str () + pos + 16);
				}
			}
			else if (work1 == 302) {
				//Location:----\r\n
				work1 = buf.Pos ("Location:");
				if (work1) {
					int num = buf.Pos ("\r\n", work1) - work1 - 10;
					buf = buf.substr (work1 + 10, num);
					content_length = http_size (buf);
				}
			}
		}
		transport_close(server_socket);
	}
	//終了
	return content_length;
}
#endif
