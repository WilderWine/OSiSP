#pragma once

#define M_SIN_CHECKED 1
#define M_COS_CHECKED 2
#define M_TAN_CHECKED 3
#define M_CTG_CHECKED 4

#define M_SQRT_CHECKED 5
#define M_EX_CHECKED 6
#define M_X2_CHECKED 7
#define M_1DELX_CHECKED 8

#define M_HELP_CHECKED 9

#define B0_CLICKED 10
#define B1_CLICKED 11
#define B2_CLICKED 12
#define B3_CLICKED 13

#define B4_CLICKED 14
#define B5_CLICKED 15
#define B6_CLICKED 16
#define B7_CLICKED 17

#define B8_CLICKED 18
#define B9_CLICKED 19
#define BMUL_CLICKED 20
#define BDIV_CLICKED 21

#define BPLUS_CLICKED 22
#define BMIN_CLICKED 23
#define BPOINT_CLICKED 24
#define BEQ_CLICKED 25

#define BC_CLICKED 26
#define BCE_CLICKED 27

HWND number_line;
HFONT human_font;
HBRUSH brush;

char sign = '+';
double first = 0, second = 0;
char buff[20];

bool must_be_cleared = false;
bool sign_changed = false;

LPCWSTR help_message = L"Welcome to a brand new calculator application on Win32 API! \
\n------------------------------------------------------------ \n\n\
This calculator can solve simple problems, some elementary or trigonometrical functions\n\
* To calculate a trigonometric function, choose 'Trigonometrics' option of menu bar.\n'\
  Here you'l be able to chose:\n\
  - sin(x)\n\
  - cos(x)\n\
  - tg(x)\n\
  - ctg(x),\n\
  number on the screen will be considered as x (in radians)\n\n\
* There're some elementary functions ('Functions' menu bar):\n\
  - sqrt(x)\n\
  - x^2\n\
  - e^x\n\
  - 1/x,\n\
  where x is the number on the screen\n\n\
NOTE:\n\
Though this calculator is not very vulnerable, keep in mind that you should be careful :)\n\
Don't use it if you need accurancy of more than 20 characters!!!\
";