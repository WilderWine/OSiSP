#include <windows.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <windowsx.h>
#include <wingdi.h>
#include <math.h>
#include <compressapi.h>
#include <fileapi.h>
#include <string>
#include "resource.h"
#include "globals_defines.h"




void compresss_and_save_to_file()
{
	// compress 

	std::string to_compress_data = "";
	to_compress_data += "v:\t" + std::to_string(v) + "\n";
	to_compress_data += "x0:\t" + std::to_string(x_0) + "\n";
	to_compress_data += "y0:\t" + std::to_string(y_0) + "\n";
	to_compress_data += "sin(a):\t" + std::to_string(sin_a) + "\n";
	to_compress_data += "cos(a):\t" + std::to_string(cos_a) + "\n";
	to_compress_data += "radius:\t" + std::to_string(rad) + "\n";
	to_compress_data += "color:\t";
	to_compress_data+=(need_colour == true) ? "1\n" : "0\n";
	to_compress_data += "this is just spam \n\n to make\n compression reasonable\ndkfjnbgisledfvbiernkv jarlg kberjh grdjbg jdfb gjhdbgj rjjerbg  bshg  sdb bjd jbv cv jd df dh iudfoudsf uosdf df hfs ";
	
	PCOMPRESSOR_HANDLE a = new COMPRESSOR_HANDLE();
	char compressed_data[600];
	PSIZE_T compressed_data_size = new SIZE_T();
	CreateCompressor(COMPRESS_ALGORITHM_MSZIP, NULL, a);
	Compress(*a, to_compress_data.c_str(), to_compress_data.size(), compressed_data, sizeof(compressed_data), compressed_data_size);
	CloseCompressor(*a);

	// save compressed 

	HANDLE wfile = CreateFileA(
		"f.___", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_COMPRESSED, NULL);
	DWORD res;
	WriteFile(wfile, compressed_data, *compressed_data_size, &res, NULL);
	CloseHandle(wfile);


}

void load_from_file_and_decompress() {

	// load data

	HANDLE rfile = CreateFileA(
		"f.___", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_COMPRESSED, NULL);
	DWORD deres;
	char fromfile[600];
	bool success = ReadFile(rfile, fromfile, sizeof(fromfile), &deres, NULL);
	CloseHandle(rfile);

	if (!success) return;

	// decompress

	PDECOMPRESSOR_HANDLE b = new DECOMPRESSOR_HANDLE();
	char decompressed_data[600];
	PSIZE_T decompressed_size = new SIZE_T();
	CreateDecompressor(COMPRESS_ALGORITHM_MSZIP, NULL, b);
	Decompress(*b, fromfile, deres, decompressed_data, sizeof(decompressed_data), decompressed_size);
	CloseDecompressor(*b);

	// initialize variables with loaded data
	
	std::string initialize_data(decompressed_data);
	std::istringstream iss(initialize_data);
	std::string temp;
	std::vector<std::string> strings;
	while (std::getline(iss, temp, '\n')) {
		strings.push_back(temp);
	}

	for (int i = 0; i < strings.size(); i++) {
		if (strings[i].starts_with("v:\t")) {
			v = std::stod(strings[i].substr(3));
		}
		else if (strings[i].starts_with("x0:\t")) {
			x_0 = std::stod(strings[i].substr(4));
		}
		else if (strings[i].starts_with("y0:\t")) {
			y_0 = std::stod(strings[i].substr(4));
		}
		else if (strings[i].starts_with("sin(a):\t")) {
			sin_a = std::stod(strings[i].substr(8));
		}
		else if (strings[i].starts_with("cos(a):\t")) {
			cos_a = std::stod(strings[i].substr(8));
		}
		else if (strings[i].starts_with("radius:\t")) {
			rad = std::stod(strings[i].substr(8));
		}
		else if (strings[i].starts_with("color:\t")) {
			need_colour = std::stoi(strings[i].substr(7)) == 1?true:false;
		}
	}
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
		load_from_file_and_decompress();
		SetTimer(window, 1, 6, NULL);
		CreateMenus(window);
		break;
	}
	case WM_DESTROY:
	{
		compresss_and_save_to_file();
		// выход из программы по закрытию окна
		PostQuitMessage(0);
		break;
	}
	case WM_PAINT:
	{
		GetClientRect(window, &clientRect);

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(window, &ps);
		SetTextColor(hdc, RGB(45, 87, 122));
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


			Ellipse(hdc, x_0, y_0, x_0 + 2.0 * rad, y_0 + 2.0 * rad);


			SelectObject(hdc, oldPen);
			SelectObject(hdc, oldBrush);

			DeleteObject(newPen);
			DeleteObject(newBrush);
		}
		else Ellipse(hdc, x_0, y_0, x_0 + 2.0 * rad, y_0 + 2.0 * rad);

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
		{
			

			int b = 6;
			if (v < 15) v += 1;
			break;
		}
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

	HWND mainwnd = CreateWindow(L"MainWindow", L"DISCOBALL", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 200, 200, 600, 500, NULL, NULL, NULL, NULL);

	//SetTextColor()
	//HFONT font = CreateFontA(18, 9, 3, 30, FW_EXTRALIGHT, TRUE, TRUE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_SWISS, NULL);
	//SendMessageA(mainwnd, DS_SETFONT, (WPARAM)font, TRUE);

	

	while (GetMessage(&main_window_message, NULL, NULL, NULL)) {
		TranslateMessage(&main_window_message);
		DispatchMessage(&main_window_message);

	}

	return 0;
}
