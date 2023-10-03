#include <windows.h>
#include <windowsx.h>
#include <wingdi.h>
#include <math.h>

#include "resource.h"
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

void CreateMenus(HWND wnd) {
	HMENU root = CreateMenu();

	AppendMenu(root, MF_STRING, M_SPEED_UP, L"Speed Up");
	AppendMenu(root, MF_STRING, M_SPEED_DOWN, L"Speed Down");
	AppendMenu(root, MF_STRING, M_CLEANING, L"On/Off Cleaning");
	AppendMenu(root, MF_STRING, M_RAD_UP, L"Bigger Ball");
	AppendMenu(root, MF_STRING, M_RAD_DOWN, L"Smaller Ball");
	AppendMenu(root, MF_STRING, M_GRAPHIC, L"Graphic Mode");

	SetMenu(wnd, root);
}


LRESULT CALLBACK createNewProcedure(HWND window, UINT message, WPARAM wp, LPARAM lp) {
	switch (message) {
	case WM_CREATE:
	{

		SetTimer(window, 1, 6, NULL);
		CreateMenus(window);

		break;
	}
	case WM_DESTROY:
	{
		// выход из программы по закрытию окна
		PostQuitMessage(0);
		break;
	}
	case WM_PAINT:
	{
		GetClientRect(window, &clientRect);

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(window, &ps);
		if (need_cleaning) FillRect(hdc, &clientRect, WHITE_BRUSH);



		if (cos_a > 0 && x_0 > clientRect.right - 2 * rad || cos_a < 0 && x_0 < 1) cos_a *= -1;
		if (sin_a < 0 && y_0 < 1 || sin_a > 0 && y_0 > clientRect.bottom - 2 * rad) sin_a *= -1;

		x_0 = x_0 + v * cos_a;
		y_0 = y_0 + v * sin_a;

		if (need_colour) {

			HPEN newPen = CreatePen(PS_ENDCAP_SQUARE, 3, RGB(rand() % 255, rand() % 255, rand() % 255));
			HBRUSH newBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));

			HGDIOBJ oldPen = SelectObject(hdc, newPen);
			HGDIOBJ oldBrush = SelectObject(hdc, newBrush);


			Ellipse(hdc, x_0, y_0, x_0 + 2 * 54, y_0 + 2 * rad);


			SelectObject(hdc, oldPen);
			SelectObject(hdc, oldBrush);

			DeleteObject(newPen);
			DeleteObject(newBrush);
		}
		else Ellipse(hdc, x_0, y_0, x_0 + 2 * rad, y_0 + 2 * rad);

		EndPaint(window, &ps);

		break;
	}
	case WM_TIMER:
	{
		RedrawWindow(window, NULL, NULL, RDW_INVALIDATE);
		break;
	}
	case WM_LBUTTONDOWN:
	{

		double x_1 = GET_X_LPARAM(lp), y_1 = GET_Y_LPARAM(lp);
		double dx = x_1 - x_0, dy = y_1 - y_0;
		sin_a = dy / sqrt(dy * dy + dx * dx);
		cos_a = dx / sqrt(dx * dx + dy * dy);
		break;
	}
	case WM_COMMAND:
		switch (wp)
		{
		case M_SPEED_UP:
			if (v < 15) v += 1;
			break;
		case M_SPEED_DOWN:
			if (v >= 2) v -= 1;
			break;
		case M_CLEANING:
			need_cleaning = !need_cleaning;
			break;

		case M_RAD_UP:
			if (rad < 90.0) rad += 5.0;
			break;
		case M_RAD_DOWN:
			if (rad > 15.0) rad -= 5.0;
			break;
		case M_GRAPHIC:
			need_colour = !need_colour;
		}

		break;

	default: return DefWindowProc(window, message, wp, lp);
	}
}



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {


	WNDCLASS main_window = createWindowClass(CreateSolidBrush(RGB(255, 255, 255)), LoadCursor(NULL, IDC_CROSS), hInst, LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)), L"MainWindow", createNewProcedure);

	if (!RegisterClassW(&main_window)) {
		return -1;
	}
	

	MSG main_window_message = { 0 };

	CreateWindow(L"MainWindow", L"DISCOBALL", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 200, 200, 600, 500, NULL, NULL, NULL, NULL);

	while (GetMessage(&main_window_message, NULL, NULL, NULL)) {
		TranslateMessage(&main_window_message);
		DispatchMessage(&main_window_message);

	}

	return 0;
}
