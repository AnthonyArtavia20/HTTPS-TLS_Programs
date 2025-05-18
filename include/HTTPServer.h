#pragma once

// Habilitar soporte SSL en cpp-httplib
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h> // La librería importada para la comunicación de HTTP y HTTPS
#include <string>

class HTTPServer { //Clase del server
    public:
        // Obtiene la instancia singleton del servidor HTTPS
        static HTTPServer* getInstance(const std::string& cert_path, const std::string& key_path);
        void start(int port); //Inicializa el server en el puerto solicitado.

    private:
        httplib::SSLServer server; //Instancia de la librería para la comunicación.
        static HTTPServer* instance; //Instancia de la clase HTTPServer

        HTTPServer(const std::string& cert_path, const std::string& key_path);
        void setUpRoutes(); //definimos el header de la función que configurará las opciones
};
