/*
  Construye la lógica del FileManager, dándole la lógica a los métodos definidos en el header.
*/

#include "FileManager.h" //Incluimos el header del manager y sus métodos.
#include <fstream> //Para poder escribir y leer sobre los archivos.
#include <dirent.h> //Para leer los archivos en la carpeta de carga.
#include <string>
using namespace std;

const string FileManager::UPLOAD_DIR ="uploads/"; // Accedemos a la variable que guarda donde se guardan las cosas en el server.

//guarda un archivo en ./uploads/
bool FileManager::saveFile(const string& filename, const string& content) {
  //Para evitar ataqiues como "../virus.txt"
  if (filename.find("..") != string::npos) {
    return false;
  }

  // Abre el archivo en modo binario y escribe el contenido.
  ofstream file(UPLOAD_DIR + filename, std::ios::binary);
  if (!file) return false; //error porque no hay archivo a abrir
  file << content; //De lo contario le inyectamos a ese nuevo File en la carpeta de carga todo el contenido del archivo original
  return true;
}

  // Lista los archivos en ./uploads/, es como un ls de linux.
vector<string> FileManager::listFiles() {
  vector<string> files; // Para almacenar los nombres de los archivos que estén almacenados.
  DIR* dir = opendir(UPLOAD_DIR.c_str()); //Abre la carpeta donde se almacenan los archivos.
  if (dir) { //Si realmente existe la carpeta de carga, entonces procede a guardar los nombre de los archivos guardados
    dirent* entry; //Declaración de puntero a una estructura dirent, repreenta una entrada(ya sea archivo o carpeta) dentro del directorio actual.
    while ((entry = readdir(dir)) != nullptr) //readdir(dir) devuelve la siguiente entrada del folder abierto por opendir, entonces el while va buscando entradas del folder hasta que ya no hayan más, va devolviendo un puntero.
    {
      if (entry->d_type == DT_REG) { // Si es un archivo (no unna carpeta)
        files.push_back(entry->d_name); //Agrega el nombre a la estructura, al vector... Esto es o que se mostrará al hacer /list
        //acá entry->d_type == DT_REG  verifica si la entrada a analizar es un archivo regular y no una carpeta ni un enlace.
        //el entry->d_type va a extraer el nombre del archivo y lo va a pushear al vector a devolver como lista de archivos almacenados.
      }
    }
    closedir(dir); //cierra la carpeta cuando ya no hayan más entradas.
  }
  files.push_back("La carpeta no existe");
  return files; //En caso de que no haya carpeta de carga, se retorna la lista vacia
}

  // Lee un archivo y devuelve su contenido como string
string FileManager::readFile(const string& filename) {
  ifstream file(UPLOAD_DIR + filename, ios::binary); //Se abre el archivo en modo binario, esto ayuda por ejemplo a analizar imágenes que no son puro texto.

  if (!file) return "El archivo no existe"; //Si no existe, retorna vacio.
  // De lo contario lee todo el contenido
  return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  //Estos son dos iteraciones que recorren el archivo en cuestión desde el inicio hasta el final, pero no es que se hacen dos iteraciones
  //si no que son marcadores de inicio, al final: std::string(iterador_inicio, iterador_final).
}