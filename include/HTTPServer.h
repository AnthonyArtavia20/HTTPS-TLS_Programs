#pragma once

// Habilitar soporte SSL en cpp-httplib
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <string>

class HTTPServer {
public:
    // Obtiene la instancia singleton del servidor HTTPS
    static HTTPServer* getInstance(const std::string& cert_path,
                                   const std::string& key_path);
    void start(int port);

private:
    httplib::SSLServer server;
    static HTTPServer* instance;

    HTTPServer(const std::string& cert_path, const std::string& key_path);
    void setUpRoutes();
};
