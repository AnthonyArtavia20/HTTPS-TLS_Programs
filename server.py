import http.server
import ssl

server_address = ('localhost', 4433)
httpd = http.server.HTTPServer(server_address, http.server.SimpleHTTPRequestHandler)

# Configurar contexto SSL correctamente
context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
context.load_cert_chain(certfile="cert.pem", keyfile="key.pem")

httpd.socket = context.wrap_socket(httpd.socket, server_side=True)

print("Servidor HTTPS corriendo en https://localhost:4433")
httpd.serve_forever()
