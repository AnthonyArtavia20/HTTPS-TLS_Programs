#include "include/HTTPServer.h"
#include <nlohmann/json.hpp> // Para devolver JSON en /list

HTTPServer* HTTPServer::instance = nullptr; // Inicializa el Singleon

// Constructor: Configurar el servidor HTTPS con los certificados planteados
HTTPServer::HTTPServer(string& cert_path, const string& key_path) : server(cert_path.c_str(), key_path.c_str()) {
  //se usa cert.pem y key.pem
  setUpRoutes(); //Define las rutas.
}

// Configura las rutas del servidor
void HTTPServer::setupRoutes() {
    // POST /upload -> Sube un archivo
    server.Post("/upload", [](const httplib::Request& req, httplib::Response& res) {
        auto file = req.get_file_value("file"); // Obtiene el archivo enviado

        // Intenta guardarlo
        if (FileManager::saveFile(file.filename, file.content)) {
            res.set_content("Archivo subido.", "text/plain");
        } else {
            res.status = 400; // Error
            res.set_content("Error al subir el archivo.", "text/plain");
        }
    });

    // GET /list -> Lista archivos en JSON
    server.Get("/list", [](const httplib::Request&, httplib::Response& res) {
        nlohmann::json files = FileManager::listFiles(); // Obtiene la lista
        res.set_content(files.dump(), "application/json"); // Devuelve JSON
    });

    // GET /download/<filename> -> Descarga un archivo
    server.Get("/download/(.*)", [](const httplib::Request& req, httplib::Response& res) {
        std::string filename = req.matches[1]; // Nombre del archivo
        std::string content = FileManager::readFile(filename); // Lee el contenido

        if (!content.empty()) {
            // Envía el archivo como binario
            res.set_content(content, "application/octet-stream");
            // Fuerza la descarga (no lo abre en el navegador)
            res.set_header("Content-Disposition", "attachment; filename=" + filename);
        } else {
            res.status = 404; // No encontrado
            res.set_content("Archivo no encontrado.", "text/plain");
        }
    });
}

// Singleton: Devuelve la instancia única del servidor
HTTPServer* HTTPServer::getInstance(const std::string& cert_path, const std::string& key_path) {
    if (!instance) {
        instance = new HTTPServer(cert_path, key_path);
    }
    return instance;
}

// Inicia el servidor en el puerto especificado
void HTTPServer::start(int port) {
    server.listen("0.0.0.0", port); // Escucha en todas las interfaces
}