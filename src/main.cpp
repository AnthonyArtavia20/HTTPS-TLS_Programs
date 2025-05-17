#include "HTTPServer.h"
#include <iostream>

int main() {
    try {
        auto* server = HTTPServer::getInstance("cert.pem", "key.pem");
        server->start(8443);
        std::cout << "Servidor HTTPS iniciado en https://0.0.0.0:8443" << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error al iniciar servidor: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
