#include "FileManager.h"
#include <fstream>
#include <dirent.h> //Para leer los archivos en la carpeta de carga.
#include <string>
using namespace std;

const string FileManager::UPLOAD_DIR = "../uploads/";

//guarda un archivo en ./uploads/
bool FileManager::saveFile(const string& filename, const string& content) {
  //Para evitar ataqiues como "../virus.txt"
  if (filename.find("..") != string::npos) {
    return false;
  }

  // Abre el archivo en modo binario y escribe el contenido.
  ofstream file(UPLOAD_DIR + filename, std::ios::binary);
  if (!file) return false; //error porque no hay archivo a abrir
  file << content;
  return true;
}

  // Lista los archivos en ./uploads/
vector<string> FileManager::listFiles() {
  vector<string> files;
  DIR* dir = opendir(UPLOAD_DIR.c_str()); //Abre la carpeta.
  if (dir) {
    dirent* entry;
    while ((entry = readdir(dir)) != nullptr)
    {
      if (entry->d_type == DT_REG) { // Si es un archivo (no unna carpeta)
        files.push_back(entry->d_name); //Agrega el nombre a la estructura
      }
    }
    closedir(dir); //cierra la carpeta
  }
  return files;
}

  // Lee un archivo y devuelve su contenido como string
string FileManager::readFile(const string& filename) {
  ifstream file(UPLOAD_DIR + filename, ios::binary);
  if (!file) return ""; //Si no existe, retorna vacio.
  // Lee todo el contenido
  return std::string(
      (std::istreambuf_iterator<char>(file)),
      std::istreambuf_iterator<char>()
  );
}