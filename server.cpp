#include <iostream>
#include <winsock2.h>
#include <thread>
#include <vector>
#include <direct.h>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>

const int MAX_BUFFER_SIZE = 1024;
const std::string server_Folder = "C:/Users/jabbour.dandan/backupsvr/";

void HandleClient(SOCKET clientSocket) {
    char buffer[MAX_BUFFER_SIZE];
    int bytesReceived;

    // Receive user ID from the client
    bytesReceived = recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "Error receiving user ID from client." << std::endl;
        closesocket(clientSocket);
        return;
    }
    buffer[bytesReceived] = '\0';

    // Extract user ID from the received data
    std::string userId(buffer);

    // Create a new folder with the user ID in the SERVER_FOLDER
    std::string fullPath = std::string(server_Folder) + userId;
    if (mkdir(fullPath.c_str()) == 0) {
        std::cout << "Folder created successfully." << std::endl;
    } else {
        std::cerr << "Failed to create folder." << std::endl;
    }

    // Send a response to the client
    const char* response = "Folder created"; // Implement your response protocol
    send(clientSocket, response, strlen(response), 0);

    // Clean up
    closesocket(clientSocket);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(12345); // Example port number

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Error while listening." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port 12345..." << std::endl;

    while (true) {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);

        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Failed to accept client connection." << std::endl;
            continue;
        }

        std::thread clientThread(HandleClient, clientSocket);
        clientThread.detach(); // Detach the thread (fire and forget)
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
