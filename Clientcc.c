#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

void receive_messages(SOCKET sock) {
    while (1) {
        char buffer[1024];
        int result = recv(sock, buffer, sizeof(buffer), 0);
        if (result == SOCKET_ERROR) {
            fprintf(stderr, "recv failed with error: %d\n", WSAGetLastError());
            break;
        }
        buffer[result] = '\0';
        printf(": %s\n", buffer);
    }
}

void send_messages(SOCKET sock, char* username) {
    char message[1024];
    while (1) {
        // Get message from user
        printf("Enter your message: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = '\0'; // Remove newline character
        if (strcmp(message, "quit") == 0) {
            break;
        }

        // Send message to server
        char send_buf[1124];
        sprintf(send_buf, "@%s:%s", username, message);
        int result = send(sock, send_buf, strlen(send_buf), 0);
        if (result == SOCKET_ERROR) {
            fprintf(stderr, "send failed with error: %d\n", WSAGetLastError());
            break;
        }
    }
}


int main() {
	
    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        fprintf(stderr, "WSAStartup failed with error: %d\n", result);
        return 1;
    }

    // Create a socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "socket failed with error: %d\n", WSAGetLastError());
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
        fprintf(stderr, "connect failed with error: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
	char message[1024];
    char username[1024];

    printf("Enter your name: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';
    // Start threads for receiving and sending messages
    _beginthread(receive_messages, 0, (void*)sock);
    _beginthread(send_messages, 0, (void*)sock,username);

    // Wait for threads to finish
    while (1) {}

    // Clean up
    closesocket(sock);
    WSACleanup();

    return 0;
}
