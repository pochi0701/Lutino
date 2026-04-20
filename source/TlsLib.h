/*
 * TlsLib.h  –  Minimal BearSSL TLS server wrapper
 *
 * Wraps an accepted TCP SOCKET with TLS.
 * Drop-in replacements for recv() / send():
 *   tls.read(buf, len)   ← recv(sock, buf, len, 0)
 *   tls.write(buf, len)  ← send(sock, buf, len, 0)
 *   tls.close()          ← sClose(sock)
 */
#ifndef TLS_LIB_H
#define TLS_LIB_H

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#else
#include <sys/socket.h>
#include <unistd.h>
typedef int SOCKET;
#define INVALID_SOCKET (-1)
#define closesocket close
#endif

#include "bearssl/bearssl.h"

/* ================================================================
 * Embedded test certificate (CN=127.0.0.1, RSA-2048, self-signed)
 * – for development / loopback only
 * ================================================================ */
namespace TlsTestCert {
extern const unsigned char *CERT;
extern const size_t        CERT_LEN;
extern br_rsa_private_key  KEY;
}

/* ================================================================
 * TlsCertFile – PEMファイルから証明書+秘密鍵を読み込む
 * ================================================================ */
class TlsCertFile {
public:
    TlsCertFile();
    ~TlsCertFile();

    TlsCertFile(const TlsCertFile &) = delete;
    TlsCertFile &operator=(const TlsCertFile &) = delete;

    /* PEMファイルから証明書と秘密鍵を読み込む。
     * certPath: 証明書PEMファイルパス
     * keyPath:  秘密鍵PEMファイルパス
     * 成功時 true を返す。 */
    bool load(const char *certPath, const char *keyPath);

    bool isLoaded() const { return valid_; }

    /* BearSSL用の証明書データ */
    const unsigned char *certDer() const { return cert_der_; }
    size_t certDerLen() const { return cert_der_len_; }
    const br_rsa_private_key *rsaKey() const { return valid_ ? &rsa_key_ : nullptr; }

private:
    unsigned char      *cert_der_;
    size_t              cert_der_len_;
    unsigned char      *key_buf_;
    br_rsa_private_key  rsa_key_;
    bool                valid_;

    static bool readFile(const char *path, unsigned char **out, size_t *outLen);
    static bool decodePem(const unsigned char *pem, size_t pemLen,
                          const char *objName,
                          unsigned char **derOut, size_t *derLen);
};

/* ================================================================
 * x509_noVerify – 証明書検証をスキップするX.509エンジン
 * (開発/内部利用向け。リーフ証明書から公開鍵を抽出するが検証は行わない)
 * ================================================================ */
struct x509_noVerify_context {
    const br_x509_class *vtable;
    br_x509_decoder_context decoder;
    br_x509_pkey pkey;
    bool first_cert;      // リーフ証明書のみデコードするフラグ
    bool key_available;
};

extern const br_x509_class x509_noVerify_vtable;

void x509_noVerify_init(x509_noVerify_context *ctx);

/* ================================================================
 * TlsClientConn – outgoing TLS client connection
 * ================================================================ */
class TlsClientConn {
public:
    TlsClientConn();
    ~TlsClientConn();

    TlsClientConn(const TlsClientConn &) = delete;
    TlsClientConn &operator=(const TlsClientConn &) = delete;

    /* Connect TLS on an already-connected socket.
     * server_name: SNI hostname (or NULL).
     * Returns true if setup succeeded. */
    bool tlsOpen(SOCKET sock, const char *server_name = nullptr);

    int tlsRead(void *buf, size_t len);
    int tlsWrite(const void *buf, size_t len);
    int tlsFlush();
    void tlsClose();
    int lastError() const;
    bool isActive() const { return active_; }

private:
    br_ssl_client_context      cc_;
    x509_noVerify_context      xn_;
    br_sslio_context           ioc_;
    unsigned char              iobuf_[BR_SSL_BUFSIZE_BIDI];
    SOCKET                     sock_;
    bool                       active_;

    static int sockRead (void *ctx, unsigned char *buf, size_t len);
    static int sockWrite(void *ctx, const unsigned char *buf, size_t len);
};

/* ================================================================
 * TlsConn – one TLS connection on top of an accepted SOCKET
 * ================================================================ */
class TlsConn {
public:
    TlsConn();
    ~TlsConn();

    TlsConn(const TlsConn &) = delete;
    TlsConn &operator=(const TlsConn &) = delete;

    /* Attach TLS to an already-accepted socket and start handshake.
     * cert/certLen : DER-encoded X.509 server certificate
     * key          : RSA private key (BearSSL format)
     * Returns true if setup succeeded (handshake completes lazily). */
    bool open(SOCKET sock,
              const unsigned char *cert, size_t certLen,
              const br_rsa_private_key *key);

    /* Convenience: open with the built-in test certificate. */
    bool open(SOCKET sock);

    /* Open with a loaded TlsCertFile. */
    bool open(SOCKET sock, const TlsCertFile &certFile);

    /* Read decrypted data.  Returns bytes read (>0), or -1 on error/EOF. */
    int read(void *buf, size_t len);

    /* Write plaintext (buffered).  Returns 0 on success, -1 on error. */
    int write(const void *buf, size_t len);

    /* Flush the TLS write buffer to the network. */
    int flush();

    /* Graceful TLS shutdown + closesocket(). */
    void close();

    /* Last BearSSL error code (0 = no error). */
    int lastError() const;

    bool isActive() const { return active_; }

private:
    br_ssl_server_context sc_;
    br_sslio_context      ioc_;
    unsigned char         iobuf_[BR_SSL_BUFSIZE_BIDI];
    SOCKET                sock_;
    bool                  active_;

    /* Must outlive the TLS session (BearSSL stores pointers) */
    br_x509_certificate   chain_;
    br_rsa_private_key    sk_;

    static int sockRead (void *ctx, unsigned char *buf, size_t len);
    static int sockWrite(void *ctx, const unsigned char *buf, size_t len);
};

#endif /* TLS_LIB_H */