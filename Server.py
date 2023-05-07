import socket
import threading
from Crypto.Cipher import Blowfish
import base64
class Server:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.clients = []
        key = input("Enter the key to decrypt: ")
        self.key=key

    def start(self):
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen()

        print(f"Server started on {self.host}:{self.port}")

        # Thread for handling client connections
        threading.Thread(target=self.client_connections).start()

        # Thread for waiting for server input
        ##threading.Thread(target=self.server_input).start()

    def client_connections(self):
        
        while True:
            client_socket, client_address = self.server_socket.accept()
            print(f"New client connected: {client_address}")
            self.clients.append(client_socket)

            # Thread for receiving messages from client
            threading.Thread(target=self.receive_message, args=(client_socket,)).start()
    
    def receive_message(self, client_socket):
        while True:
            
            cipher = Blowfish.new(self.key.encode('utf-8'), Blowfish.MODE_ECB)
            
            messagesclient= client_socket.recv(1024).decode()
            name,message=messagesclient.split(":",1)

            if message.startswith("decrypt "):
                # Decrypt the message, but not the "decrypt" command itself
                command, data = message.split(" ", 1)
                encrypted_data_bytes = base64.b64decode(data)
                decrypted_data_bytes = cipher.decrypt(encrypted_data_bytes)
                decrypted_data = decrypted_data_bytes.decode('utf-8')
                message = decrypted_data
                
            print(f"Received message: {name}:{message}")
            for client in self.clients:
                if client != client_socket:
                    client.sendall(f"[Received message]: {name}:{message} ".encode())
    def remove_client(self, client_socket):
        if client_socket in self.clients:
            self.clients.remove(client_socket)
            print(f"Client disconnected: {name}")

    def server_input(self):
        while True:
            message = input("Server: ")
            # Broadcast the message to all connected clients
            for client in self.clients:
                client.sendall(f"[Server]: {message}\n".encode())

if __name__ == '__main__':
    server = Server('localhost', 8080)
    server.start()
