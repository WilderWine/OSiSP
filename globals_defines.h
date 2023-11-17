HINSTANCE someinstance;

HWND button1;
HWND button2;
HWND button3;

LPCWSTR torrent_standart_path = L"C:\\Users\\Lenovo\\AppData\\Roaming\\uTorrent Web\\utweb.exe,0";
LPCWSTR py_standart_path = L"\"C:\\Windows\\py.exe\",1";
LPCWSTR text_standart_path = L"%SystemRoot%\\System32\\imageres.dll,-102";

LPCWSTR icon1_path = L"c:\\texticon\\texticon.ico";
LPCWSTR icon2_path = L"c:\\texticon\\texticon2.ico";
LPCWSTR icon_xpython_path = L"c:\\texticon\\xpython.ico";
LPCWSTR icon_caterpillar_path = L"c:\\texticon\\caterpillar.ico";

LPCWSTR text_key_path = L"txtfile";
LPCWSTR torrent_key_path = L"Torrent File";
LPCWSTR py_key_path = L"Python.File";

LPWSTR key_path = (LPWSTR)text_key_path;
LPWSTR chosen_icon = (LPWSTR)text_standart_path;
WORD file_type = 1;

HFONT chelovecheskiy_shrift;
HKEY txt_key;
HANDLE event_handle;


CRITICAL_SECTION exit_section = { 0 };
HANDLE file_mutex = NULL;
HANDLE clear_event = NULL;
HANDLE reg_success_event = NULL;
HANDLE reg_failure_event = NULL;
HANDLE winlog_success_event = NULL;
HANDLE winlog_failure_event = NULL;

HANDLE thread1 = NULL;
HANDLE thread2 = NULL;
HANDLE thread3 = NULL;
HANDLE thread4 = NULL;
HANDLE thread5 = NULL;
HANDLE thread6 = NULL;

LPCWSTR log_reg_success = L"Registry field modified - icon changed\n";
LPCWSTR log_reg_failure = L"Failure while modifying registry field\n";
LPCWSTR log_winlog_success = L"Winlog INFO event registered\n";
LPCWSTR log_winlog_failure = L"Winlog ERROR event registered\n";
LPCWSTR logfile_path = L"logfile.txt";

HANDLE log_file = NULL;
HWND msg_line = NULL;


#define TXT_CHOSEN 1
#define PY_CHOSEN 2
#define TORRENT_CHOSEN 3

#define ICON1_CHOSEN 4
#define ICON2_CHOSEN 5
#define ICON_PY_CHOSEN 6
#define ICON_CATERPILLAR_CHOSEN 7
#define NORMAL_ICON_CHOSEN 8

#define LOAD_ICON_CLICKED 9

HANDLE nasty = NULL;