#include "client.h";
#include <string>

void log_error(std::string message) {
    SetConsoleTextAttribute(console_handle, wred);
    printf(("\n"+message).c_str());
    SetConsoleTextAttribute(console_handle, wwhite);
}

void print_server_message(char* message, int res) {
    SetConsoleTextAttribute(console_handle, wyellow);
    printf("\rServer:\t");
    SetConsoleTextAttribute(console_handle, wgray);
    printf(" %.*s",res,message);
    SetConsoleTextAttribute(console_handle, wgreen);
    printf("Me: ");
    SetConsoleTextAttribute(console_handle, wgray);
}



void get_message(SOCKET ConnectSocket) {
    char recvbuf[DEFAULT_BUFLEN];
    int response_length;

    while (true) {
        response_length = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);
        if (response_length > 0) {
            print_server_message(recvbuf,response_length);
        }
        else if (response_length == 0) {
            closesocket(ConnectSocket);
            SetConsoleTextAttribute(console_handle, wblue);
            printf("Server is off...\n");
            SetConsoleTextAttribute(console_handle, wwhite);
            break;
        }
        else {
            closesocket(ConnectSocket);
            log_error("Error while receiving a message: " + std::to_string(WSAGetLastError()));
            break;
        }
    };
}

int main() {
    
    int response_length;
    struct addrinfo* result = NULL, *ptr = NULL, hints;
    console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    response_length = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (response_length != 0) {
        log_error("Error occured while WSA starting: " + std::to_string(response_length));
        //closesocket(_socket);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    response_length = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
    if (response_length != 0) {
        log_error("Error occured while getting addres: " + std::to_string(response_length));
        //closesocket(_socket);
        WSACleanup();
        return 1;
    }

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        _socket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (_socket == INVALID_SOCKET) {
            log_error("Invalid Socket: " + std::to_string(WSAGetLastError()));
            //closesocket(_socket);
            WSACleanup();
            return 1;
        }

        response_length = connect(_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (response_length == SOCKET_ERROR) {
            closesocket(_socket);
            _socket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (_socket == INVALID_SOCKET) {
        WSACleanup();
        //closesocket(_socket);
        log_error("Unable to connect to server!\n");
        return 1;
    }

    printf("Connected to the server. Start typing messages...\n");


    std::thread receiveThread(get_message, _socket);


    char sendbuf[DEFAULT_BUFLEN];
    while(true) {
        SetConsoleTextAttribute(console_handle, wgreen);
        printf("Me: ");
        SetConsoleTextAttribute(console_handle, wgray);
        fgets(sendbuf, DEFAULT_BUFLEN, stdin);
        SetConsoleTextAttribute(console_handle, wwhite);

        response_length = send(_socket, sendbuf, strlen(sendbuf), 0);
        if (response_length == SOCKET_ERROR) {
            log_error("send failed with error: %d\n" + std::to_string(WSAGetLastError()));
            WSACleanup();
            //closesocket(_socket);
            break;
        }

        if (strcmp(sendbuf, "bye\n") == 0) {
            SetConsoleTextAttribute(console_handle, wblue);
            printf("Chat is over...\n");
            SetConsoleTextAttribute(console_handle, wwhite);
            //closesocket(_socket);
            break;
        }

    };


    receiveThread.join();

    closesocket(_socket);
    WSACleanup();

    return 0;
}