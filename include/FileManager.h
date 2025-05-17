// Se encarga de almacenar los datos, pdfs, imágenes, etc
// Además se encarga de listar y leer

#pragma once
#include <string>
#include <vector>
#include <string>
using namespace std;

class FileManager {

  public:
    static const string UPLOAD_DIR; // Carpeta donde se guardan los archivos (./uploads/)

    // Guardar un archiivo. En caos de que el nombre del entrante sea el mismo a uno ya existente, entonces retorna flase.
    static bool saveFile(const string& filename, const string& content);

    // Hace un listado de todos los archivos subidos hasta el momento.
    static vector<string> listFiles();

    // Lee un archivo. retornando contenido vacio si no existe nada.
    static string readFile(const string& filename);

};