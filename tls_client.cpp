#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509v3.h>

#define HOST "www.google.com"
#define PORT "443"

int main() {
    //Inicializar OpenSSL
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    //Crear contexto TLS
    const SSL_METHOD* method = TLS_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) {
        std::cerr << "Error al crear SSL_CTX\n";
        ERR_print_errors_fp(stderr);
        return 1;
    }

    // CArgar certificados raiz del sistema
    if (SSL_CTX_set_default_verify_paths(ctx) != 1){
        std::cerr << "NO se pudieron cargar los certificados raiz del sistema\n";
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(ctx);
        return 1;
    }

    // Crear socket
    struct addrinfo hints{}, *res;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(HOST, PORT, &hints, &res) !=0){
        std::cerr << "Error en getaddrinfo\n";
        SSL_CTX_free(ctx);
        return 1;
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) {
        std::cerr << "Error al crear el socket\n";
        freeaddrinfo(res);
        SSL_CTX_free(ctx);
        return 1;
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) != 0){
        std::cerr << "Error al conectar al servidor\n";
        close(sockfd);
        freeaddrinfo(res);
        SSL_CTX_free(ctx);
        return 1;
    }

    //Crear estructura SLL y vincularla al socket
    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);

    //Activar verificacion del certificado del servidor
    SSL_set_verify(ssl, SSL_VERIFY_PEER, nullptr);

    // Inicar handshake TLS
    if (SSL_connect(ssl) !=1){
        std::cerr << "Error al hacer handshake TLS\n";
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        close(sockfd);
        freeaddrinfo(res);
        SSL_CTX_free(ctx);
        return 1;
    }

    long verify_result = SSL_get_verify_result(ssl);
    if (verify_result != X509_V_OK){
        std::cerr << "Error de verificacion del certificado: " << X509_verify_cert_error_string(verify_result) << "\n";
        SSL_free(ssl);
        close(sockfd);
        freeaddrinfo(res);
        SSL_CTX_free(ctx);
        return 1;
    }

    //Verificar que el nombre del host coincida con el certificado
    X509* cert = SSL_get_peer_certificate(ssl);
    if (!cert) {
        std::cerr << "No se pudo obtener el certificado del servidor \n";
        SSL_free(ssl);
        close(sockfd);
        freeaddrinfo(res);
        SSL_CTX_free(ctx);
        return 1;
    }

    //Verificacion de nombre de host
    char peer_CN[256];
    X509_NAME_get_text_by_NID(X509_get_subject_name(cert), NID_commonName, peer_CN, sizeof(peer_CN));
    std::string cert_name(peer_CN);
    X509_free(cert);

    if (cert_name !=HOST) {
        std::cerr << "Nombre del certificado (" << cert_name << ") no coincide con el host esperado (" << HOST << ")\n";
        SSL_free(ssl);
        close(sockfd);
        freeaddrinfo(res);
        SSL_CTX_free(ctx);
        return 1;
    }

    std::cout << "Conexion TLS establecida con " << HOST << "\n";

    //Enviar solicitud HTTP GET
    const char* request = "GET / HPPT/1.1\nHost: www.google.com\r\nConnection: close\r\n\r\n";
    SSL_write(ssl, request, strlen(request));

    //Leer respuesta
    char buffer[4096];
    int bytes;
    while ((bytes = SSL_read(ssl, buffer, sizeof(buffer))) > 0) {
        std::cout.write(buffer, bytes);
    }

    //Cerrar
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sockfd);
    freeaddrinfo(res);
    SSL_CTX_free(ctx);
    EVP_cleanup();

    return 0;

}
