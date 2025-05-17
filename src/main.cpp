// Punto de entrada del programa, donde se ejecuta todo

#include "HTTPServer.h"
#include <iostream>
using namespace std;

int main () {

  //crear la carpeta para almacenar si no lo está actualmente
  system("mkdir -p uploads");

  //Obtiene la instancia del server, como solo es una instancia esto es aplicado por Singleton
  auto*  server = HTTPServer::getInstance("./cert.pem","./key.pem");

  cout << "Servidor HTTPS iniciado en https.//localhost8443\n";
  cout << "Operaciones disponibles: ";
  cout << "  POST /upload    - Sube un archivo\n";
  cout << "  GET  /list      - Lista archivos\n";
  cout << "  GET  /download/<file> - Descarga un archivo\n";

  //Inicializar el server en el puerto 8443, esto está definido así para poder evitar una vulnerabilidad
  //el poder usar sudo para ataques malisiosos
  server->start(8443);
  return 0;
}
