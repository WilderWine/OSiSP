#include <windows.h>
#include <string>

//#include "calculator_funcs.cpp"

#include "globals_defines.h"




double eq(char sign, double first, double second) {
	switch (sign) {
	case '+':
		return first + second;
		break;
	case '-':
		return first - second;
		break;
	case '/':
		if (second != 0) return first / second;
		break;
	case '*':
		return first * second;
		break;
	default:
		return 0;
	}
}


void func(std::string f) {
	GetWindowTextA(number_line, buff, 20);
	std::string number = buff;
	if (number == "error" || number == "-") return;
	if (number[number.size() - 1] == '.') number = number + '0';
	double d_number = atof(number.c_str());

	if (f == "sin") {
		d_number = sin(d_number);
	}
	else if (f == "cos") {
		d_number = cos(d_number);
	}
	else if (f == "tan") {
		d_number = tan(d_number);
	}
	else if (f == "ctg") {
		d_number = 1 / tan(d_number);
	}
	else if (f == "sqrt") {

		d_number = sqrt(d_number);
	}
	else if (f == "exp") {
		d_number = exp(d_number);
	}
	else if (f == "x2") {
		d_number = d_number * d_number;
	}
	else if (f == "1/x") {
		d_number = 1 / d_number;
	}

	number = std::to_string(d_number);
	if (number.size() > 20) number = number.substr(0, 20);
	bool is_numeric = true;
	for (int i = 0; i < number.size(); i++) {
		char d = number[i];
		if ((d < '0' || d > '9') && d != '.' && d != '-') {
			is_numeric = false;
			break;
		}
	}
	if (!is_numeric) {
		first = second = 0;
		sign = '+';
		SetWindowTextA(number_line, "error");
		return;
	}

	SetWindowTextA(number_line, number.c_str());
}

void add_character(char digit) {

	if (must_be_cleared) {
		first = second = 0;
		sign = '+';
		SetWindowTextA(number_line, "0");
		must_be_cleared = false;
	}
	sign_changed = false;

	GetWindowTextA(number_line, buff, 20);
	std::string number = buff;
	std::string output = number;

	if (digit == '0') {
		if (number == "error") output = "0";
		else if (number.size() == 1 && number != "0") output = number + "0";
		else if (number == "0") return;
		else if (number.size() < 20) output = number + "0";
	}
	else if (digit == '.') {
		if (number == "-") output = "-0.";
		else if (number == "error") output = "0.";
		else if (number.size() < 19) {
			bool contain = false;
			for (int i = 0; i < number.size(); i++) {
				char a = number[i];
				if (a == 46) {
					contain = true;
					break;
				}
			}
			if (!contain) {
				output = number + '.';
			}
		}
	}
	else {
		if (number == "error") output = digit;
		else if (number.size() == 1 && number == "0") output = digit;
		else if (number.size() < 20) output = number + digit;
	}

	SetWindowTextA(number_line, output.c_str());
}

void remove_character() {
	GetWindowTextA(number_line, buff, 20);
	std::string number = buff;
	std::string output;

	if (number.size() == 1 || number == "error") output = "0";
	else {
		number.pop_back();
		output = number;
	}

	SetWindowTextA(number_line, output.c_str());
}



//  функция, позволяющая создать новое окно (объект класса WNDCLASS) с определенным цветом фона, формой курсора и т.д.


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

// create menu foe app

void CreateMenus(HWND wnd) {
	HMENU root = CreateMenu();
	HMENU elem_funcs = CreateMenu();
	HMENU trigonometrics = CreateMenu();

	AppendMenu(root, MF_POPUP, (UINT_PTR)trigonometrics, L"Trigonomentrics");
	AppendMenu(trigonometrics, MF_STRING, M_SIN_CHECKED, L"sin (x)");
	AppendMenu(trigonometrics, MF_STRING, M_COS_CHECKED, L"cos (x)");
	AppendMenu(trigonometrics, MF_STRING, M_TAN_CHECKED, L"tg (x)");
	AppendMenu(trigonometrics, MF_STRING, M_CTG_CHECKED, L"ctg (x)");

	AppendMenu(root, MF_POPUP, (UINT_PTR)elem_funcs, L"Functions");
	AppendMenu(elem_funcs, MF_STRING, M_SQRT_CHECKED, L"sqrt (x)");
	AppendMenu(elem_funcs, MF_STRING, M_EX_CHECKED, L"e^x");
	AppendMenu(elem_funcs, MF_STRING, M_X2_CHECKED, L"x^2");
	AppendMenu(elem_funcs, MF_STRING, M_1DELX_CHECKED, L"1 / x");

	AppendMenu(root, MF_STRING, M_HELP_CHECKED, L"Help");

	SetMenu(wnd, root);
}

void AddWidgets(HWND wnd) {
	number_line = CreateWindowA("static", "0", WS_VISIBLE | WS_CHILD | ES_RIGHT , 50, 60, 455, 70, wnd, NULL, NULL, NULL);
	SendMessageA(number_line, WM_SETFONT, (WPARAM)human_font, TRUE);

	SendMessageA(CreateWindowA("button", "C", WS_VISIBLE | WS_CHILD, 50 + 345, 140, 50, 50, wnd, (HMENU)BC_CLICKED, NULL, NULL), WM_SETFONT, (WPARAM)human_font, TRUE);
	SendMessageA(CreateWindowA("button", "CE", WS_VISIBLE | WS_CHILD, 50 + 345 +60, 140, 50, 50, wnd, (HMENU)BCE_CLICKED, NULL, NULL), WM_SETFONT, (WPARAM)human_font, TRUE);

	SendMessageA(CreateWindowA("button", "7", WS_VISIBLE | WS_CHILD, 50, 200, 110, 60, wnd, (HMENU)B7_CLICKED, NULL, NULL), WM_SETFONT, (WPARAM)human_font, TRUE);
	SendMessageA(CreateWindowA("button", "8", WS_VISIBLE | WS_CHILD, 50+115, 200, 110, 60, wnd, (HMENU)B8_CLICKED, NULL, NULL), WM_SETFONT, (WPARAM)human_font, TRUE);
	SendMessageA(CreateWindowA("button", "9", WS_VISIBLE | WS_CHILD, 50+230, 200, 110, 60, wnd, (HMENU)B9_CLICKED, NULL, NULL), WM_SETFONT, (WPARAM)human_font, TRUE);
	SendMessageA(CreateWindowA("button", "*", WS_VISIBLE | WS_CHILD, 50+345, 200, 110, 60, wnd, (HMENU)BMUL_CLICKED, NULL, NULL), WM_SETFONT, (WPARAM)human_font, TRUE);

	SendMessageA(CreateWindowA("button", "4", WS_VISIBLE | WS_CHILD, 50, 200+75, 110, 60, wnd, (HMENU)B4_CLICKED, NULL, NULL), WM_SETFONT, (WPARAM)human_font, TRUE);
	SendMessageA(CreateWindowA("button", "5", WS_VISIBLE | WS_CHILD, 50+115, 200+75, 110, 60, wnd, (HMENU)B5_CLICKED, NULL, NULL), WM_SETFONT, (WPARAM)human_font, TRUE);
	SendMessageA(CreateWindowA("button", "6", WS_VISIBLE | WS_CHILD, 50+230, 200+75, 110, 60, wnd, (HMENU)B6_CLICKED, NULL, NULL), WM_SETFONT, (WPARAM)human_font, TRUE);
	SendMessageA(CreateWindowA("button", "/", WS_VISIBLE | WS_CHILD, 50+345, 200+75, 110, 60, wnd, (HMENU)BDIV_CLICKED, NULL, NULL), WM_SETFONT, (WPARAM)human_font, TRUE);

	SendMessageA(CreateWindowA("button", "3", WS_VISIBLE | WS_CHILD, 50, 200+150, 110, 60, wnd, (HMENU)B3_CLICKED, NULL, NULL), WM_SETFONT, (WPARAM)human_font, TRUE);
	SendMessageA(CreateWindowA("button", "2", WS_VISIBLE | WS_CHILD, 50+115, 200+150, 110, 60, wnd, (HMENU)B2_CLICKED, NULL, NULL), WM_SETFONT, (WPARAM)human_font, TRUE);
	SendMessageA(CreateWindowA("button", "1", WS_VISIBLE | WS_CHILD, 50+230, 200+150, 110, 60, wnd, (HMENU)B1_CLICKED, NULL, NULL), WM_SETFONT, (WPARAM)human_font, TRUE);
	SendMessageA(CreateWindowA("button", "-", WS_VISIBLE | WS_CHILD, 50 + 345, 200 + 150, 110, 60, wnd, (HMENU)BMIN_CLICKED, NULL, NULL), WM_SETFONT, (WPARAM)human_font, TRUE);

	SendMessageA(CreateWindowA("button", ".", WS_VISIBLE | WS_CHILD, 50, 200+225, 110, 60, wnd, (HMENU)BPOINT_CLICKED, NULL, NULL), WM_SETFONT, (WPARAM)human_font, TRUE);
	SendMessageA(CreateWindowA("button", "0", WS_VISIBLE | WS_CHILD, 50+115, 200+225, 110, 60, wnd, (HMENU)B0_CLICKED, NULL, NULL), WM_SETFONT, (WPARAM)human_font, TRUE);
	SendMessageA(CreateWindowA("button", "=", WS_VISIBLE | WS_CHILD, 50+230, 200+225, 110, 60, wnd, (HMENU)BEQ_CLICKED, NULL, NULL), WM_SETFONT, (WPARAM)human_font, TRUE);
	SendMessageA(CreateWindowA("button", "+", WS_VISIBLE | WS_CHILD, 50 + 345, 200+225, 110, 60, wnd, (HMENU)BPLUS_CLICKED, NULL, NULL), WM_SETFONT, (WPARAM)human_font, TRUE);

}


//  функция, позволяющая создать процедуру

LRESULT CALLBACK createNewProcedure(HWND window, UINT message, WPARAM wp, LPARAM lp) {
	switch (message) {
	case WM_CREATE:
		CreateMenus(window);
		AddWidgets(window);
		break;
	case WM_DESTROY:

		// выход из программы по закрытию окна
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		switch (wp)
		{
		case M_SIN_CHECKED:
			func("sin");
			break;
		case M_COS_CHECKED:
			func("cos");
			break;
		case M_TAN_CHECKED:
			func("tan");
			break;
		case M_CTG_CHECKED:
			func("ctg");
			break;

		case M_SQRT_CHECKED:
			func("sqrt");
			break;
		case M_EX_CHECKED:
			func("exp");
			break;
		case M_X2_CHECKED:
			func("x2");
			break;
		case M_1DELX_CHECKED:
			func("1/x");
			break;

		case M_HELP_CHECKED:
		
			MessageBox(window, help_message, L"Help", NULL);
			break;

		case B0_CLICKED:
			add_character('0');
			break;
		case B1_CLICKED:
			add_character('1');
			break;
		case B2_CLICKED:
			add_character('2');
			break;
		case B3_CLICKED:
			add_character('3');
			break;
		case B4_CLICKED:
			add_character('4');
			break;
		case B5_CLICKED:
			add_character('5');
			break;
		case B6_CLICKED:
			add_character('6');
			break;
		case B7_CLICKED:
			add_character('7');
			break;
		case B8_CLICKED:
			add_character('8');
			break;
		case B9_CLICKED:
			add_character('9');
			break;
		case BPOINT_CLICKED:
			add_character('.');
			break;
		case BEQ_CLICKED:
		{
			GetWindowTextA(number_line, buff, 20);
			std::string num = buff;
			if (num == "error" || num == "-") {
				first = second = 0;
				sign = '+';
				SetWindowTextA(number_line, "0");
			}
			else if (num[num.size() - 1] == '.') {
				num = num + '0';
			}

			second = atof(num.c_str());
			first = eq(sign, first, second);
			second = 0;
			num = std::to_string(first);
			if (num.size() > 20) num = num.substr(0, 20);

			SetWindowTextA(number_line, num.c_str());

			must_be_cleared = true;

			break;
		}
		case BMUL_CLICKED:
			if (!sign_changed) {
				
					GetWindowTextA(number_line, buff, 20);
					std::string number = buff;
					if (number == "error" || number == "-") {
						SetWindowTextA(number_line, "0");
						break;
					}
					else if (number[number.size() - 1] == '.') number = number + '0';
					if (!must_be_cleared) {
						first = eq(sign, first, atof(number.c_str()));
					}
				
					else {
						first = atof(number.c_str());
						must_be_cleared = false;
					}
				SetWindowTextA(number_line, "0");
				
				sign_changed = true;
			}
			sign = '*';
			
			break;
		case BDIV_CLICKED:
			if (!sign_changed) {
				
					GetWindowTextA(number_line, buff, 20);
					std::string number = buff;
					if (number == "error" || number == "-") {
						SetWindowTextA(number_line, "0");
						//return;
					}
					else if (number == "0") {
						SetWindowTextA(number_line, "error");
						//return;
					}
					else if (number[number.size() - 1] == '.') number = number + '0';

					if(!must_be_cleared)
					first = eq(sign, first, atof(number.c_str()));
					else {
						first = atof(number.c_str());
						must_be_cleared = false;
					}
				

				SetWindowTextA(number_line, "0");
				
				sign_changed = true;
			}
			sign = '/';

			break;
		case BPLUS_CLICKED:
			if (!sign_changed) {
				
					GetWindowTextA(number_line, buff, 20);
					std::string number = buff;
					if (number == "error" || number == "-") {
						SetWindowTextA(number_line, "0");
						//return;
					}
					else if (number[number.size() - 1] == '.') number = number + '0';
					if (!must_be_cleared) {
						first = eq(sign, first, atof(number.c_str()));
					}
					else {
						first = atof(number.c_str());
						must_be_cleared = false;
					}

				
				
				SetWindowTextA(number_line, "0");
				sign_changed = true;
			}
			sign = '+';

			break;
		case BMIN_CLICKED:
			if (!sign_changed) {
				
					GetWindowTextA(number_line, buff, 20);
					std::string number = buff;
					if (number == "error" || number == "-") {
						SetWindowTextA(number_line, "0");
						//return;
					}
					else if (number[number.size() - 1] == '.') number = number + '0';
					if (!must_be_cleared) {
					first = eq(sign, first, atof(number.c_str()));
				}
				
				else {
					first = atof(number.c_str());
					must_be_cleared = false;
				}
				SetWindowTextA(number_line, "0");
				
				sign_changed = true;
			}
			sign = '-';
			break;
		case BC_CLICKED:
			remove_character();
			break;
		case BCE_CLICKED:
			first = second = 0;
			sign = '+';
			SetWindowTextA(number_line, "0");
			break;
		default:
			break;
		}
		break;
	default: return DefWindowProc(window, message, wp, lp);
	}
}



//  главная функция

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	//  создаем основное окно (класс), используя описанную выше функцию

	human_font = CreateFontA(30, 10, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,FF_ROMAN, "Font1");
	brush = CreateSolidBrush(RGB(12, 13, 235));
	WNDCLASS main_window = createWindowClass(brush, LoadCursor(NULL, IDC_CROSS), hInst, LoadIcon(NULL, IDI_WINLOGO), L"MainWindow", createNewProcedure);

	// регистрируем класс главного окна (нужно, ибо создаем через имя, заданное при инициализации переменной типа WNDCLASS)
	if(!RegisterClassW(&main_window)){
		return -1;
	}

	MSG main_window_message = { 0 };


	// создаем само основное окно с меню, 3мя главнами кнопками, возможностью перемещать/менять размер
	CreateWindow(L"MainWindow", L"Kind of headline", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 200, 200, 555, 600, NULL, NULL, NULL, NULL);

	while (GetMessage(&main_window_message, NULL, NULL, NULL)) {
		TranslateMessage(&main_window_message);
		DispatchMessage(&main_window_message);

	}

	return 0;
}
