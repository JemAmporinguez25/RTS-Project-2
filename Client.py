import socket
import threading

class Client:
    def __init__(self):
        self.host = "127.0.0.1"
        self.port = 8080
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.name = input("Enter your name: ")
        self.sock.connect((self.host, self.port))
        print("[+] Connected to server")

        self.receive_thread = threading.Thread(target=self.receive)
        self.receive_thread.start()

        self.send_thread = threading.Thread(target=self.send)
        self.send_thread.start()

    def receive(self):
        while True:
            try:
                data = self.sock.recv(2048).decode()
                print(data)
            except socket.error:
                print("[-] Disconnected from server")
                break

    def send(self):
        while True:
            message = input()
            if message == "bye":
                self.sock.send(bytes("@" + self.name + ":" + message, "utf-8"))
                self.sock.close()
                break
            else:
                self.sock.send(bytes("@" + self.name + ":" + message, "utf-8"))

if __name__ == "__main__":
    client = Client()