## HTTPS Server in C++ to upload, list and download files.

## 📥 Requisitos
- **Compilador** con soporte C++17  
- **OpenSSL** (`libssl-dev`)  
- **Pthreads**  
- **nlohmann/json** (`libnlohmann-json-dev`)  
- Cabeceras de [cpp-httplib](https://github.com/yhirose/cpp-httplib) (solo `httplib.h`)  

## Características

- **Upload**: POST `/upload` (multipart/form-data)  
- **List**: GET `/list` → devuelve un JSON con los nombres de los archivos  
- **Download**: GET `/download/{filename}` → descarga el archivo como attachment  

Todo esto sobre HTTPS, usando la librería [cpp-httplib](https://github.com/yhirose/cpp-httplib) con soporte OpenSSL.

## ¿Cómo usar?

### Consideraciones importantes antes de utilizar:
> **Nota**: asegúrate de crear la carpeta `uploads/` antes de iniciar el servidor en caso de que no esté presente:
> ```bash
> mkdir -p uploads
> ```

##### Compilación del programa en caso de modificación en el código:
1. Abre la carpeta en VSCode.  
2. Pulsa `Ctrl+Shift+B` para ejecutar la tarea **C/C++: g++ build project**. 
Esto generará el ejecutable "server"

##### Generar certificado (opcional)
En caso de requerir generar otro certificado y otra llave "cert.pem / key.pem" se puedes autogenerar otro par:
> ```bash
> openssl req -x509 -newkey rsa:4096 -nodes -keyout key.pem -out cert.pem -days 365 -subj "/C=CR/ST=SanJose/L=SanJose/O=MiOrg/OU=IT/CN=localhost"
> ```

##### Tener en cuenta que:
> IMPORTANTE: jamás se tiene que subir los cert.pem y key.pem a un repositorio.
En este ejemplo están incluidos solo con fines académicos.
Añade siempre al .gitignore cualquier certificado, clave privada o binario generado.


### Uso:
## 💻 Instalación
### 1. Clonar el repositorio
```bash
git clone https://github.com/AnthonyArtavia20/HTTPS-TLS_Programs.git
cd HTTPS-TLS_Programs
```
### 2.  Instalar dependencias
```bash
sudo apt install build-essential libssl-dev libnlohmann-json-dev
```

#### 3. Inicia el servidor (por defecto en el puerto 8443):
Se realiza en una terminal propia para el server.
> ```bash
> -/server
> ```
Verás este mensaje por consola: Servidor HTTPS iniciado en https://0.0.0.0:8443

#### Prueba los endpoints con curl(operaciones):
Estas se realizan  en una terminal aparte de la del server, dentro de la carpeta del proyecto, en la raíz.
- **Subir archivos(/Upload):**

> ```bash
> curl -k -X POST -F "file=@./ruta/archivo.txt" https://localhost:8443/upload
> ```

- **Listar los archivos(/list):**

> ```bash
> curl -k https://localhost:8443/list
> ```

- **Descargar un archivo subido a la carpeta de carga(/download):**

> ```bash
> curl -k https://localhost:8443/download/archivo.txt -o archivo.txt
> ```
