#pragma once

#include <windows.h>
#include <vector>
#include <iostream>
#include <sstream>


volatile int iter = 0;
int size = 1000;
char* filedata = new char[size];
HWND itertext;
HWND filetext;
HANDLE iterthread;
HANDLE filethread;
HANDLE iterMutex;
HFONT chelovecheskiy_shrift;

#define MENU_ITER_PAUSE_RESUME 1
#define MENU_FILE_PAUSE_RESUME 2
#define MENU_BOTH_PAUSE 3
#define MENU_BOTH_RESUME 4

bool iter_suspended = false;
bool file_suspended = false;
bool cont = true;

