#include "HTTPServer.h" // Incluimos la estructura de los métodos.
#include "FileManager.h" /// Igual con el FileManager. para poder acceder a esa estructuración de los archivos y sus operaciones
#include <iostream> //Para acceder a rutas y archivos.
#include <stdexcept> // Para poder usar excepciones estándar de C++
#include <nlohmann/json.hpp> //Biblioteca JSON for Modern C++; Se usa para convertir listas y objetos a JSON y viceversa.
#include <sstream> // Para manipular y construir cadenas de texto de manera más cómoda.
using namespace std;

using json = nlohmann::json; // Creación de alias para no tener que escribir nlohmann::json cada vez.

HTTPServer* HTTPServer::instance = nullptr; // Onstacia del server.

HTTPServer::HTTPServer(const std::string& cert_path, const std::string& key_path): server(cert_path.c_str(), key_path.c_str()) {
    /*
        Lo que se hace acá es inicializar el server HTTPS usando la ruta del sertificado y la clave privada, luego se verifica la parte del
        server(osea la instancia de la librería httplib) que efectivamente son válidos.
    */
    if (!server.is_valid()) {
        throw std::runtime_error("Error: certificado o clave inválidos, error de enlace");
    }
    setUpRoutes(); // responsable de definir y registrar las rutas HTTP(endingPoints) del server, para soportar las operaciones y responder.
}

HTTPServer* HTTPServer::getInstance(const std::string& cert_path, const std::string& key_path) {
    if (!instance) { // Patrón de diseño Singleton
        instance = new HTTPServer(cert_path, key_path); //permite realizar una instancia del server para poder configurarlo, pero solo permite hacerla una vez por el patrón Singleton
    }
    return instance;
}

void HTTPServer::setUpRoutes() {
    // POST /upload
    server.Post("/upload", [&](const httplib::Request& req, httplib::Response& res) {
        cout << "Se ha solicitado cargar un archivo a la carpeta de carga" << endl;
        auto file_header = req.get_file_value("file"); // Para obtener la información necesaria del archivo, tal como el nombre y el contenido.
        bool ok = FileManager::saveFile(file_header.filename,file_header.content);
        if (ok) {
            res.set_content("{ \"success\": true }", "application/json");
            cout << "Se ha logrado cargar exitosamente" << endl;
        } else {
            res.status = 500;
            cout << "No se subió el archivo." << endl;
            res.set_content("{ \"success\": false }", "application/json");
        }
    });

    // GET /list
    server.Get("/list", [&](const httplib::Request&, httplib::Response& res) {
        auto files = FileManager::listFiles();
        json j = files; // La lista de los archivos son convertidos a Json para su transmición.
        res.set_content(j.dump(), "application/json"); // transmite la lista de nombres.
        cout << "Se ha solicitado un listado de los archivos almacenados en el server, los cuales son: " << j <<endl;
    });

    // GET /download/{filename}
    server.Get(R"(/download/(.+))", [&](const httplib::Request& req, httplib::Response& res) {
        std::string filename = req.matches[1]; // Obtiene el nombre del archivo ingresado por consola
        cout << "Se ha solicitado un la descarga de un archivo en el server, el cual es: "<< filename << endl;
        std::string data = FileManager::readFile(filename); // Extrae el contenido del archivo guardado para poder crear un nuevo archivo intacto con el nombre y contenido del original.
        if (data == "El archivo no existe") {
            res.status = 404;
            res.set_content("File not found", "text/plain");
            cout << "No se le descargó al cliente, puesto que el arrchivo no tiene contenido." << endl;
        } else { //Puede que el archivo esté vacío pero igual se envía y todo.
            res.set_header("Content-Disposition", "attachment; filename=" + filename);
            res.set_content(data, "application/octet-stream");
            cout << "Archivo descargado exitosamente para el cliente" << endl;
        }
    });
}

void HTTPServer::start(int port) { //se pasa el puerto que viene desde main para luego inicializarlo aquí.
    std::cout << "Servidor HTTPS iniciado en https://0.0.0.0:" << port << std::endl;
    server.listen("0.0.0.0", port);
}
