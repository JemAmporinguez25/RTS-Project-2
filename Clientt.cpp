#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

void receive_messages(SOCKET sock) {
    while (true) {
        char buffer[1024];
        int result = recv(sock, buffer, sizeof(buffer), 0);
        if (result == SOCKET_ERROR) {
            std::cerr << "recv failed with error: " << WSAGetLastError() << std::endl;
            break;
        }
        buffer[result] = '\0';
        std::cout << "Received message: " << buffer << std::endl;
    }
}

void send_messages(SOCKET sock) {
    std::string message;
    while (true) {
        // Get message from user
        std::cout << "Enter your message: ";
        std::getline(std::cin, message);
        if (message == "quit") {
            break;
        }

        // Send message to server
        int result = send(sock, message.c_str(), message.length(), 0);
        if (result == SOCKET_ERROR) {
            std::cerr << "send failed with error: " << WSAGetLastError() << std::endl;
            break;
        }
    }
}

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed with error: " << result << std::endl;
        return 1;
    }

    // Create a socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Connect to the server
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    result = connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR) {
        std::cerr << "connect failed with error: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Start threads for receiving and sending messages
    std::thread receive_thread(receive_messages, sock);
    std::thread send_thread(send_messages, sock);

    // Wait for threads to finish
    receive_thread.join();
    send_thread.join();

    // Clean up
    closesocket(sock);
    WSACleanup();

    return 0;
}
