#include "server.h"
#include <string>


void log_error(std::string message) {
    SetConsoleTextAttribute(console_handle, wred);
    printf(("\n" + message).c_str());
    SetConsoleTextAttribute(console_handle, wwhite);
}


void print_client_message(char* message, int res) {
    SetConsoleTextAttribute(console_handle, wyellow);
    printf("\rClient:\t");
    SetConsoleTextAttribute(console_handle, wgray);
    printf(" %.*s", res, message);
    SetConsoleTextAttribute(console_handle, wgreen);
    printf("Me: ");
    SetConsoleTextAttribute(console_handle, wgray);
}


void get_message(SOCKET ClientSocket) {
    char recvbuf[DEFAULT_BUFLEN];
    int response_length;

    while (true) {
        response_length = recv(ClientSocket, recvbuf, DEFAULT_BUFLEN, 0);
        if (response_length > 0) {
            print_client_message(recvbuf, response_length);
        }
        else if (response_length == 0) {
            closesocket(ClientSocket);
            SetConsoleTextAttribute(console_handle, wblue);
            printf("Client is off...\n");
            SetConsoleTextAttribute(console_handle, wwhite);
            break;
        }
        else {
            closesocket(ClientSocket);
            log_error("Error while receiving a message: " + std::to_string(WSAGetLastError()));
            break;
        }
    };
}


int main() {

    console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    int response_length;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    response_length = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (response_length != 0) {
        log_error("Error occured while WSA starting: " + std::to_string(response_length));
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    response_length = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (response_length != 0) {
        WSACleanup();
        log_error("Error occured while getting addres: " + std::to_string(response_length));
        return 1;
    }

    _socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (_socket == INVALID_SOCKET) {
        log_error("Invalid Socket: " + std::to_string(WSAGetLastError()));
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    response_length = bind(_socket, result->ai_addr, (int)result->ai_addrlen);
    if (response_length == SOCKET_ERROR) {
        log_error("Error while binding with the client: " + std::to_string(WSAGetLastError()));
        freeaddrinfo(result);
        closesocket(_socket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    response_length = listen(_socket, SOMAXCONN);
    if (response_length == SOCKET_ERROR) {
        log_error("Error while listening to client: " + std::to_string(WSAGetLastError()));
        closesocket(_socket);
        WSACleanup();
        return 1;
    }

    printf("Binding connection...\n");

    _client_sock = accept(_socket, NULL, NULL);
    if (_client_sock == INVALID_SOCKET) {
        log_error("Error accepting message: " + std::to_string(WSAGetLastError()));
        //closesocket(_socket);
        WSACleanup();
        return 1;
    }

    closesocket(_socket);

    std::thread receiveThread(get_message, _client_sock);

    char sendbuf[DEFAULT_BUFLEN];
    while (true) {
        SetConsoleTextAttribute(console_handle, wgreen);
        printf("Me: ");
        SetConsoleTextAttribute(console_handle, wgray);
        fgets(sendbuf, DEFAULT_BUFLEN, stdin);
        SetConsoleTextAttribute(console_handle, wwhite);

        response_length = send(_client_sock, sendbuf, strlen(sendbuf), 0);
        if (response_length == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            break;
        }

        if (strcmp(sendbuf, "bye\n") == 0) {
            SetConsoleTextAttribute(console_handle, wblue);
            printf("Chat is over...\n");
            SetConsoleTextAttribute(console_handle, wwhite);
            //closesocket(_client_sock);
            break;
        }

    };

    receiveThread.join();

    closesocket(_client_sock);
    WSACleanup();

    return 0;
}