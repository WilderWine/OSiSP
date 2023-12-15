#pragma once
#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_BUFLEN 1000
#define DEFAULT_PORT "27015"

HANDLE console_handle;
WSADATA wsaData;
SOCKET _socket;
WORD wgray = 8;
WORD wred = 12;
WORD wgreen = 10;
WORD wyellow = 14;
WORD wwhite = 15;
WORD wblue = 9;