#pragma once
#include <httplib.h> // Biblioteca para HTTP/HTTPS
#include "FileManager.h"
using namespace std;

class HTTPServer {

  private:
    httplib::SSLServer server; // Server HTTPS usando OpenSSL
    static HTTPServer* instance; // Patrón Singleton (Solo una insntancia, pues... ¿para que quedríamos más de una instancia en el mismo puerto?)

    // Constructor privado para Singleton
    HTTPServer(const string& cert_path, const string& key_path);

    void setUpRoutes();

  public:
    //Obtener la única instancia del server
    static HTTPServer* getInstance(const string& cert_path, const string& key_path);

    // Inicia el servidor en un puerto específico
    void start(int startingPort)
};