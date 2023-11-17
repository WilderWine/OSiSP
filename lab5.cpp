#include <windows.h>
#include <string>
#include "globals_defines.h"

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

void WriteToFile(LPCWSTR msg, LPCWSTR path)
{
	WaitForSingleObject(file_mutex, INFINITE);
	DWORD res = 0;
	WriteFile(log_file, msg, wcslen(msg)*2, &res, NULL);	
	ReleaseMutex(file_mutex);
}

void WriteToEventLogInfo(LPCWSTR message) {

	event_handle = RegisterEventSource(NULL, L"IconChangeEvent");
	if (event_handle) {
		ReportEvent(event_handle, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, &message, NULL);
		DeregisterEventSource(event_handle);
		SetEvent(winlog_success_event);
	}
	
}

void WriteToEventLogError(LPCWSTR message) {

	event_handle = RegisterEventSource(NULL, L"IconChangeEventError");
	if (event_handle) {
		ReportEvent(event_handle, EVENTLOG_ERROR_TYPE, 0, 0, NULL, 1, 0, &message, NULL);
		DeregisterEventSource(event_handle);
		SetEvent(winlog_failure_event);
	}
	
}
void ChangeFileIcon(LPWSTR key_path, LPWSTR chosen_icon) {
	txt_key = NULL;
	if (RegOpenKey(HKEY_CLASSES_ROOT, key_path, &txt_key) == ERROR_SUCCESS) {
		if (RegSetValue(txt_key, L"DefaultIcon", REG_SZ, chosen_icon, 50) == 0) {
			WriteToEventLogInfo(L"Extention icons changed successfully");
			RegCloseKey(txt_key);
			SetWindowTextA(msg_line, "Congratulations! File icons were changed successfully!");
			SetEvent(reg_success_event);
		}
		else
		{
			WriteToEventLogError(L"Icon Change Error - trouble changing value");
			SetWindowTextA(msg_line, "Some troubles accured while trying to change the icon!");
			SetEvent(reg_failure_event);
		}
	}
	else
	{
		WriteToEventLogError(L"Icon Change Error - trouble while opening registry key");
		SetWindowTextA(msg_line, "Some troubles accured while trying to change the icon!");
		SetEvent(reg_failure_event);
	}
	SetEvent(clear_event);
}

void AddRadioButtons(HWND window) {
	HINSTANCE second = NULL;

	// расширения

	SendMessageA(
		CreateWindowA("button", "*.txt", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 10, 10, 100, 30, window, (HMENU)TXT_CHOSEN, someinstance, NULL),
		WM_SETFONT, WPARAM(chelovecheskiy_shrift), TRUE);
	SendMessageA(
		CreateWindowA("button", "*.py", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 10, 50, 100, 30, window, (HMENU)PY_CHOSEN, someinstance, NULL),
		WM_SETFONT, WPARAM(chelovecheskiy_shrift), TRUE);
	SendMessageA(
		CreateWindowA("button", "*.torrent", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 10, 90, 100, 30, window, (HMENU)TORRENT_CHOSEN, someinstance, NULL),
		

	// иконки
		
		WM_SETFONT, WPARAM(chelovecheskiy_shrift), TRUE);
	SendMessageA(
		CreateWindowA("button", "text1", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 150, 10, 100, 30, window, (HMENU)ICON1_CHOSEN, someinstance, NULL),
		WM_SETFONT, WPARAM(chelovecheskiy_shrift), TRUE);
	SendMessageA(
		CreateWindowA("button", "text2", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 150, 50, 100, 30, window, (HMENU)ICON2_CHOSEN, someinstance, NULL),
		WM_SETFONT, WPARAM(chelovecheskiy_shrift), TRUE);
	SendMessageA(
		CreateWindowA("button", "xpython", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 150, 90, 100, 30, window, (HMENU)ICON_PY_CHOSEN, someinstance, NULL),
		WM_SETFONT, WPARAM(chelovecheskiy_shrift), TRUE);
	SendMessageA(
		CreateWindowA("button", "caterpillar", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 150, 130, 100, 30, window, (HMENU)ICON_CATERPILLAR_CHOSEN, someinstance, NULL),
		WM_SETFONT, WPARAM(chelovecheskiy_shrift), TRUE);
	SendMessageA(
		CreateWindowA("button", "normal", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 150, 170, 100, 30, window, (HMENU)NORMAL_ICON_CHOSEN, someinstance, NULL),
		WM_SETFONT, WPARAM(chelovecheskiy_shrift), TRUE);
	
	
	// кнопка
	
	SendMessageA(
		CreateWindowA("button", "change icon", WS_CHILD | WS_VISIBLE, 70, 210, 120, 30, window, (HMENU)LOAD_ICON_CLICKED, someinstance, NULL),
		WM_SETFONT, WPARAM(chelovecheskiy_shrift), TRUE);


	// сообщение

	msg_line = CreateWindowA("static", "", WS_VISIBLE | WS_CHILD | ES_LEFT, 10, 400, 450, 30, window, NULL, NULL, NULL);

}



LRESULT CALLBACK createNewProcedure(HWND window, UINT message, WPARAM wp, LPARAM lp) {
	switch (message) {
	case WM_CREATE:
	{
		//ChangeTxtIcon();
		
		AddRadioButtons(window);
		CheckRadioButton(window, 1, 1, 1);
		CheckRadioButton(window, 8, 8, 8);
		break;
	}
	case WM_DESTROY:
	{
		LeaveCriticalSection(&exit_section);
		Sleep(100);
		DeleteCriticalSection(&exit_section);
		CloseHandle(clear_event);
		CloseHandle(reg_success_event);
		CloseHandle(reg_failure_event);
		CloseHandle(winlog_success_event);
		CloseHandle(winlog_failure_event);
		CloseHandle(file_mutex);
		CloseHandle(log_file);
		PostQuitMessage(0);
		break;
	}
	case WM_COMMAND:
		// Проверяем, не щелкнули ли на первой группе радиокнопок.
		if (1 <= LOWORD(wp) && LOWORD(wp) <= 3)
		{
			// Отмечаем кпопку из первой группы.
			CheckRadioButton(window, 1, 3, LOWORD(wp));
			
		}
		// Проверяем, не щелкнули ли на второй группе радиокнопок.
		if (4 <= LOWORD(wp) && LOWORD(wp) <= 8)
		{
			// Отмечаем кпопку из второй группы.
			CheckRadioButton(window, 4, 8, LOWORD(wp));
			
		}

		switch (wp)
		{
		case 1:
			file_type = 1;
			key_path = (LPWSTR)text_key_path;
			CheckRadioButton(window, 4, 8, 8);
			chosen_icon = (LPWSTR)text_standart_path;
			break;
		case 2:
			file_type = 2;
			key_path = (LPWSTR)py_key_path;
			CheckRadioButton(window, 4, 8, 8);
			chosen_icon = (LPWSTR)py_standart_path;
			break;
		case 3:
			file_type = 3;
			key_path = (LPWSTR)torrent_key_path;
			CheckRadioButton(window, 4, 8, 8);
			chosen_icon = (LPWSTR)torrent_standart_path;
			break;
		case 4:
			chosen_icon = (LPWSTR)icon1_path;
			break;
		case 5:
			chosen_icon = (LPWSTR)icon2_path;
			break;
		case 6:
			chosen_icon = (LPWSTR)icon_xpython_path;
			break;
		case 7:
			chosen_icon = (LPWSTR)icon_caterpillar_path;
			break;
		case 8:
			switch (file_type)
			{
			case 1:
				chosen_icon = (LPWSTR)text_standart_path;
				break;
			case 2:
				chosen_icon = (LPWSTR)py_standart_path;
				break;
			case 3:
				chosen_icon = (LPWSTR)torrent_standart_path;
				break;
			default:
				break;
			}

			break;


		case 9:
			ChangeFileIcon(key_path, chosen_icon);
			break;
		default:
			break;
		}

		break;

	default: return DefWindowProc(window, message, wp, lp);
	}
}

DWORD WINAPI logSucsessRegistry(LPVOID lpParameter) 
{
	while (true) {

		WaitForSingleObject(reg_success_event, INFINITE);
		WriteToFile(log_reg_success, logfile_path);
	}
	
}
DWORD WINAPI logFailureRegistry(LPVOID lpParameter)
{
	while (true) {

		WaitForSingleObject(reg_failure_event, INFINITE);
		WriteToFile(log_reg_failure, logfile_path);

	}
		
}
DWORD WINAPI logSucsessWinlog(LPVOID lpParameter)
{
	while (true) {

		WaitForSingleObject(winlog_success_event, INFINITE);
		WriteToFile(log_winlog_success, logfile_path);
	}
	
}
DWORD WINAPI logFailureWinlog(LPVOID lpParameter)
{
	while (true) {

		WaitForSingleObject(winlog_failure_event, INFINITE);
		WriteToFile(log_winlog_failure, logfile_path);
	}
	
}

DWORD WINAPI ClearMsg(LPVOID lpParameter)
{
	while (true) {

		WaitForSingleObject(clear_event, INFINITE);
		Sleep(2000);
		SetWindowTextA(msg_line, "");
	}
}

DWORD WINAPI TimerSmth(LPVOID lpParameter)
{
	DWORD secs = 0;
	while (true) {

		Sleep(1);
		secs++;

		// завершилась ли программа
		if (TryEnterCriticalSection(&exit_section)) {
			WCHAR wsecs[10];
			swprintf_s(wsecs, 10, L"%d", secs);
			WriteToFile(wsecs, logfile_path);
			break;
		}
	}
	LeaveCriticalSection(&exit_section);
	ExitThread(0);
}



VOID AddASinchronizationObjects(VOID) {
	InitializeCriticalSection(&exit_section);
	EnterCriticalSection(&exit_section);
	file_mutex = CreateMutex(NULL, FALSE, L"file_mutex");

	clear_event = CreateEvent(NULL, FALSE, FALSE, L"clear_event");
	reg_success_event = CreateEvent(NULL, FALSE, FALSE, L"reg_success_event");
	reg_failure_event = CreateEvent(NULL, FALSE, FALSE, L"reg_failure_event");
	winlog_success_event = CreateEvent(NULL, FALSE, FALSE, L"winlog_success_event");
	winlog_failure_event = CreateEvent(NULL, FALSE, FALSE, L"winlog_failure_event");

	log_file = CreateFile(logfile_path, FILE_GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	thread1 = CreateThread(NULL, 0, logSucsessRegistry, NULL, 0, NULL);
	thread2 = CreateThread(NULL, 0, logFailureRegistry, NULL,0, NULL);
	thread3 = CreateThread(NULL, 0, logSucsessWinlog, NULL, 0, NULL);
	thread4 = CreateThread(NULL, 0, logFailureWinlog, NULL,0, NULL);
	thread5 = CreateThread(NULL, 0, ClearMsg, NULL, 0, NULL);
	thread6 = CreateThread(NULL, 0, TimerSmth, NULL, 0, NULL);
	
}



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	AddASinchronizationObjects();

	someinstance = hInst;
	chelovecheskiy_shrift = CreateFontA(21, 8, 0, 0, FW_EXTRALIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_ROMAN, "Font1");

	WNDCLASS main_window = createWindowClass(CreateSolidBrush(RGB(240, 240, 240)), LoadCursor(NULL, IDC_CROSS), hInst, LoadIcon(NULL, IDI_ASTERISK), L"MainWindow", createNewProcedure);

	

	if (!RegisterClassW(&main_window)) {
		return -1;
	}

	MSG main_window_message = { 0 };

	HWND mainwnd = CreateWindow(L"MainWindow", L"Icon Change", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 200, 200, 600, 500, NULL, NULL, NULL, NULL);
	
	/*INT c[3] = { COLOR_3DFACE, COLOR_WINDOW, COLOR_WINDOWTEXT };
	COLORREF col[3] = { RGB(22,247,56),RGB(224,24,56),RGB(78,34,177) };
	BOOL j = SetSysColors(3, c, col);*/
	
	/*
	INT c[3] = { COLOR_3DFACE, COLOR_WINDOW, COLOR_WINDOWTEXT };
	COLORREF col[3] = { RGB(22,247,56),RGB(255,255,255),RGB(0,0,0) };
	BOOL j = SetSysColors(3, c, col);*/

	while (GetMessage(&main_window_message, NULL, NULL, NULL)) {
		TranslateMessage(&main_window_message);
		DispatchMessage(&main_window_message);

	}

	return 0;
}