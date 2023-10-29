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


void WriteToEventLogInfo(LPCWSTR message) {

	event_handle = RegisterEventSource(NULL, L"IconChangeEvent");
	if (event_handle) {
		ReportEvent(event_handle, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, &message, NULL);
		DeregisterEventSource(event_handle);
	}

}

void WriteToEventLogError(LPCWSTR message) {

	event_handle = RegisterEventSource(NULL, L"IconChangeEventError");
	if (event_handle) {
		ReportEvent(event_handle, EVENTLOG_ERROR_TYPE, 0, 0, NULL, 1, 0, &message, NULL);
		DeregisterEventSource(event_handle);
	}

}
void ChangeFileIcon(LPWSTR key_path, LPWSTR chosen_icon) {
	txt_key = NULL;
	if (RegOpenKey(HKEY_CLASSES_ROOT, key_path, &txt_key) == ERROR_SUCCESS) {
		if (RegSetValue(txt_key, L"DefaultIcon", REG_SZ, chosen_icon, 50) == 0) {
			WriteToEventLogInfo(L"Extention icons changed successfully");
			RegCloseKey(txt_key);
		}
		else WriteToEventLogError(L"Icon Change Error - trouble changing value");
	}
	else WriteToEventLogError(L"Icon Change Error - trouble while opening registry key");
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



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	someinstance = hInst;
	chelovecheskiy_shrift = CreateFontA(21, 8, 0, 0, FW_EXTRALIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_ROMAN, "Font1");

	WNDCLASS main_window = createWindowClass(CreateSolidBrush(RGB(200, 200, 200)), LoadCursor(NULL, IDC_CROSS), hInst, LoadIcon(NULL, IDI_ASTERISK), L"MainWindow", createNewProcedure);

	if (!RegisterClassW(&main_window)) {
		return -1;
	}

	MSG main_window_message = { 0 };

	HWND mainwnd = CreateWindow(L"MainWindow", L"Icon Change", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 200, 200, 600, 500, NULL, NULL, NULL, NULL);


	while (GetMessage(&main_window_message, NULL, NULL, NULL)) {
		TranslateMessage(&main_window_message);
		DispatchMessage(&main_window_message);

	}

	return 0;
}