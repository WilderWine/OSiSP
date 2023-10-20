#include <windows.h>
#include <wingdi.h>
#include <math.h>
#include <compressapi.h>
#include <fileapi.h>
#include <string>

#include "globals_defines.h"




void save_to_file()
{
	WaitForSingleObject(iterMutex, INFINITE);
	std::string s_iter = std::to_string(iter);
	ReleaseMutex(iterMutex);
	HANDLE wfile = CreateFileA(
		"cur_iter.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_COMPRESSED, NULL);
	DWORD res;
	WriteFile(wfile, s_iter.c_str(), s_iter.size(), &res, NULL);
	CloseHandle(wfile);
}

std::string load_from_file() {

	HANDLE rfile = CreateFileA(
		"cur_iter.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_COMPRESSED, NULL);
	DWORD deres;
	char fromfile[6];
	bool success = ReadFile(rfile, fromfile, sizeof(fromfile), &deres, NULL);
	CloseHandle(rfile);

	std::string res = std::to_string(0);

	if (success) {
		res = std::string(fromfile).substr(0, deres);
	}
	return res;
}

WNDCLASS createWindowClass(HBRUSH background_color, HCURSOR cursor_type, HINSTANCE hInstance, HICON Icon, LPCWSTR window_name, WNDPROC procedure) {

	// 	создание пустого класса
	WNDCLASS new_window = { 0 };

	//  наполняем пустой класс параметрами, требуемыми для конкретного окна
	new_window.hbrBackground = background_color;
	new_window.hCursor = cursor_type;
	new_window.hInstance = hInstance;
	new_window.hIcon = Icon;
	new_window.lpszClassName = window_name;
	new_window.lpfnWndProc = procedure;

	return new_window;
}

void CreateMenus(HWND wnd) {
	HMENU root = CreateMenu();
	AppendMenu(root, MF_STRING, MENU_ITER_PAUSE_RESUME, L"On/Off iter");
	AppendMenu(root, MF_STRING, MENU_FILE_PAUSE_RESUME, L"On/Off file");
	AppendMenu(root, MF_STRING, MENU_BOTH_PAUSE, L"Off both");
	AppendMenu(root, MF_STRING, MENU_BOTH_RESUME, L"On both");
	SetMenu(wnd, root);
}

void CreateWidgets(HWND wnd) {
	itertext = CreateWindowA("static", "0", WS_VISIBLE | WS_CHILD | ES_CENTER, 20, 20, 100, 50, wnd, NULL, NULL, NULL);
	filetext = CreateWindowA("static", "0", WS_VISIBLE | WS_CHILD | BS_MULTILINE | WS_VSCROLL, 150, 20, 400, 800, wnd, NULL, NULL, NULL);
	SendMessageA(itertext, WM_SETFONT, WPARAM(chelovecheskiy_shrift), TRUE);
	SendMessageA(filetext, WM_SETFONT, WPARAM(chelovecheskiy_shrift), TRUE);
}


DWORD WINAPI ThreadIterate(LPVOID lpParameter) {
	while (cont = true) {
		while (iter <= 10000) {
			SetWindowTextA(itertext, std::to_string(iter).c_str());
			WaitForSingleObject(iterMutex, INFINITE);
			++iter;
			ReleaseMutex(iterMutex);			
		}
		iter = 0;
	}
	ExitThread(0);
	
}

DWORD WINAPI ThreadFileWork(LPVOID lpParameter)
{
	while (cont == true) {
		std::string chariter = load_from_file();
		int ss = chariter.size();
		save_to_file();
		std::string bigger_text;

		int cur_size = GetWindowTextA(filetext, filedata, size);
		if (ss + cur_size > size - 2) {
			filedata = new char[size];
			bigger_text = chariter;
		}
		else {
			bigger_text = std::string(filedata).substr(0, cur_size) + " " + chariter;
		}

		SetWindowTextA(filetext, bigger_text.c_str());
		Sleep(10);
	}

	ExitThread(0);
	
}

LRESULT CALLBACK createNewProcedure(HWND window, UINT message, WPARAM wp, LPARAM lp) {
	switch (message) {
	case WM_CREATE:
	{
		iterthread = CreateThread(NULL, 0, ThreadIterate, NULL, 0, NULL );
		filethread = CreateThread(NULL, 0, ThreadFileWork, NULL, 1, NULL);
		CreateWidgets(window);
		SetTimer(window, 1, 1, NULL);
		CreateMenus(window);
		break;
	}
	case WM_DESTROY:
	{	
		//TerminateThread(iterthread, 0);
		//TerminateThread(filethread, 0);
		cont = false;
		PostQuitMessage(0);
		break;
	}
	case WM_COMMAND:
		switch (wp)
		{
		case MENU_ITER_PAUSE_RESUME:
		{
			
			if (iter_suspended) {
				ResumeThread(iterthread);
				iter_suspended = false;
			}
			else {
				SuspendThread(iterthread);
				iter_suspended = true;
			}
			
			break;
		}
		case MENU_FILE_PAUSE_RESUME:
		{
			if (file_suspended) {
				ResumeThread(filethread);
				file_suspended = false;
			}
			else {
				SuspendThread(filethread);
				file_suspended = true;
			}
			break;
		}
		case MENU_BOTH_PAUSE:
		{
			if (!iter_suspended) SuspendThread(iterthread);
			if (!file_suspended) SuspendThread(filethread);
			iter_suspended = file_suspended = true;
			break;
		}
		case MENU_BOTH_RESUME:
		{
			ResumeThread(iterthread);
			ResumeThread(filethread);
			iter_suspended = file_suspended = false;
			break;
		}

		}

		break;

	default: return DefWindowProc(window, message, wp, lp);
	}
}



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	chelovecheskiy_shrift = CreateFontA(20, 7, 0, 0, FW_EXTRALIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_ROMAN, "Font1");

	WNDCLASS main_window = createWindowClass(CreateSolidBrush(RGB(200, 200, 200)), LoadCursor(NULL, IDC_CROSS), hInst, LoadIcon(NULL, IDI_ASTERISK), L"MainWindow", createNewProcedure);

	if (!RegisterClassW(&main_window)) {
		return -1;
	}

	MSG main_window_message = { 0 };

	HWND mainwnd = CreateWindow(L"MainWindow", L"THREADING", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 200, 200, 600, 500, NULL, NULL, NULL, NULL);


	while (GetMessage(&main_window_message, NULL, NULL, NULL)) {
		TranslateMessage(&main_window_message);
		DispatchMessage(&main_window_message);

	}

	return 0;
}