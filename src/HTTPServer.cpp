#include "HTTPServer.h"
#include "FileManager.h"
#include <iostream>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <sstream>

using json = nlohmann::json;

HTTPServer* HTTPServer::instance = nullptr;

HTTPServer::HTTPServer(const std::string& cert_path,
                       const std::string& key_path)
    : server(cert_path.c_str(), key_path.c_str()) {
    if (!server.is_valid()) {
        throw std::runtime_error("Error: certificado o clave inválidos");
    }
    setUpRoutes();
}

HTTPServer* HTTPServer::getInstance(const std::string& cert_path,
                                    const std::string& key_path) {
    if (!instance) {
        instance = new HTTPServer(cert_path, key_path);
    }
    return instance;
}

void HTTPServer::setUpRoutes() {
    // POST /upload
    server.Post("/upload", [&](const httplib::Request& req,
                                 httplib::Response& res) {
        auto file_header = req.get_file_value("file");
        bool ok = FileManager::saveFile(file_header.filename,
                                        file_header.content);
        if (ok) {
            res.set_content("{ \"success\": true }", "application/json");
        } else {
            res.status = 500;
            res.set_content("{ \"success\": false }", "application/json");
        }
    });

    // GET /list
    server.Get("/list", [&](const httplib::Request&,
                              httplib::Response& res) {
        auto files = FileManager::listFiles();
        json j = files;
        res.set_content(j.dump(), "application/json");
    });

    // GET /download/{filename}
    server.Get(R"(/download/(.+))", [&](const httplib::Request& req,
                                          httplib::Response& res) {
        std::string filename = req.matches[1];
        std::string data = FileManager::readFile(filename);
        if (data.empty()) {
            res.status = 404;
            res.set_content("File not found", "text/plain");
        } else {
            res.set_header("Content-Disposition",
                           "attachment; filename=" + filename);
            res.set_content(data, "application/octet-stream");
        }
    });

void HTTPServer::start(int port) {
    std::cout << "Servidor HTTPS iniciado en https://0.0.0.0:" << port << std::endl;
    server.listen("0.0.0.0", port);
}
