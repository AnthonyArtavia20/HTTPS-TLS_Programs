# 🔐 TLS Client in C++ to Connect to a Local HTTPS Server

Este programa implementa un **cliente en C++** que establece una conexión **TLS segura** con un servidor HTTPS local, utilizando la biblioteca **OpenSSL**.  
Forma parte de un ejercicio práctico para demostrar cómo se realiza una conexión con `handshake TLS`, verificación de certificados y envío de solicitudes HTTP seguras.

---

## 📥 Requisitos

- **Compilador C++17** (`g++`)
- **OpenSSL** (`libssl-dev`)
- Servidor local Python HTTPS (incluido)
- Certificados TLS autofirmados (`cert.pem`, `key.pem`)

---

## ⚙️ Características

- Establece conexión TCP/IP con el servidor local (`localhost:4433`)
- Realiza el **handshake TLS** usando OpenSSL
- Verifica el **certificado del servidor**
- Envía una solicitud HTTP GET tras establecer conexión segura
- Imprime la respuesta del servidor (HTML)

---

## ❗ Consideraciones importantes antes de usar

### 1. Asegúrate de tener los archivos `cert.pem` y `key.pem`
Se incluyen con fines académicos. En la vida real, nunca deben subirse al repositorio.

### 2. Puedes generar tus propios certificados (opcional)

```bash
openssl req -x509 -nodes -newkey rsa:2048 -keyout key.pem -out cert.pem -days 365 -subj "/C=CR/ST=SanJose/L=SanJose/O=MiOrg/OU=Dev/CN=localhost"
```

---

## 📦 ¿Cómo usar?

### 1. Clona el repositorio y cambia a esta rama:

```bash
git clone https://github.com/AnthonyArtavia20/HTTPS-TLS_Programs.git
cd HTTPS-TLS_Programs
git checkout tls-implementation
```

---

### 2. Instala las dependencias

```bash
sudo apt update
sudo apt install build-essential libssl-dev
```

---

### 3. Compila el programa

```bash
g++ -o tls_client tls_client.cpp -lssl -lcrypto
```

---

### 4. Ejecuta el servidor HTTPS local (en otra terminal)

```bash
python3 server.py
```

Verás algo como:

```
Servidor HTTPS corriendo en https://localhost:4433
```

Este servidor está basado en `http.server` de Python y utiliza TLS con los certificados proporcionados.

---

### 5. Ejecuta el cliente TLS

```bash
./tls_client
```

El cliente realizará:

- Inicialización de OpenSSL
- Conexión al servidor local `localhost:4433`
- Handshake TLS
- Verificación del certificado del servidor
- Envío de una solicitud HTTP GET `/`
- Impresión de la respuesta del servidor (contenido HTML)

---

## 🛠️ Estructura de archivos

| Archivo         | Descripción                                                |
|-----------------|------------------------------------------------------------|
| `tls_client.cpp`| Cliente TLS en C++ con OpenSSL                             |
| `server.py`     | Servidor HTTPS local con certificado autofirmado           |
| `cert.pem`      | Certificado autofirmado del servidor (solo con fines educativos) |
| `key.pem`       | Clave privada correspondiente al certificado               |

---

## 🚨 Importante

- **Nunca** subas tus certificados (`cert.pem`, `key.pem`) reales a un repositorio público.
- Añade estos archivos al `.gitignore` si generas nuevos:
  ```bash
  echo "cert.pem" >> .gitignore
  echo "key.pem" >> .gitignore
  ```

---

## 🧪 ¿Qué se puede observar?

- El proceso completo de **establecimiento de una conexión TLS** usando C++ puro.
- Cómo se realiza una **verificación del certificado**.
- Cómo OpenSSL maneja el handshake, el cifrado, y la comunicación segura.
- La respuesta HTTP obtenida de un servidor en `https://localhost:4433`.
