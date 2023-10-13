#pragma once
#pragma comment ( lib , "Cabinet.lib" )

#define M_SPEED_UP 2
#define M_SPEED_DOWN 3
#define M_CLEANING 4
#define M_RAD_UP 5
#define M_RAD_DOWN 6
#define M_GRAPHIC 7

double v = 2.5;
double x_0 = 100.0, y_0 = 200.0;
double sin_a = 0.0, cos_a = 1.0;
double rad = 30.0;

BOOL need_cleaning = true;
BOOL need_colour = true;

RECT clientRect;

