#include <stdio.h>
#include <math.h>

#include "main.h"
#include "xlib.h"

#include "metod_11.h"
#include "metod_41.h"

/*#define CONSOLE_INPUT*/		/* For console input */

static int n = 20;
static double a = -10;
static double b = +10;

static double max, min;

static double x_left, x_right;
static double y_up, y_down;

static int variant = 0;

/* Function */
double f(double x)
{
	return x * sin(x);
}

#define h 0.00000001
/* First derivative */
double f_first(double x)
{
	return 0.5 * (f(x + h) - f(x - h))/h;
}

/* Second derivative */
double f_second(double x)
{
	return 0.5 * (f_first(x + h) - f_first(x - h))/h;
}

/* Functions, that will be use for drawing */
static double f_method1(double x)
{
	return f_11(x);
}

static double f_method2(double x)
{
	return f_41(x);
}

static double f_pogreshnost_method1(double x)
{
	return f(x) - f_method1(x);
}

static double f_pogreshnost_method2(double x)
{
	return f(x) - f_method2(x);
}

void DrawFunction(int width, int height, double (*func)(double x))
{
	int i;
	double hx, hy;
	double x, y_start, y_end;

	hx = (x_right - x_left)/(width - 1);
	hy = height/(y_up - y_down);

	y_start = height - (func(x_left) - y_down) * hy;

	for (i = 1; i < width; i++)
	{
		x = x_left + i * hx;
		y_end = height - (func(x) - y_down) * hy;
		xlibDrawLine(i - 1, y_start, i, y_end);
		y_start = y_end;
	}
}

void CalcMaxAndMin(int width, double (*func)(double x), double* maxim, double* minim)
{
	int i;
	double x, y, hx;

	*maxim = *minim = func(x_left);
	hx = (x_right - x_left)/(width - 1);

	for (i = 1; i < width; i++)
	{
		x = x_left + i * hx;
		y = func(x);
		if (y > *maxim) *maxim = y;
		if (y < *minim) *minim = y;
	}
}

void DisplayHelp(void)
{
	/* Now you can print RUSSIAN text. Both in Windows and in Linux */
	int y = 20;
	xlibSetColor(1.0, 0.0, 0.0);
	xlibDrawString(50, y, "Acknowledgment to Yurii Silaev, whose program you are using now!");
	y += 40;
	xlibSetColor(1.0, 1.0, 1.0);
	xlibDrawString(50, y, "You can do following operations:");
	y += 20;
	xlibDrawString(50, y, "Press '1' for changing the method");
	y += 20;
	xlibDrawString(50, y, "Press '2'/'3' for zooming/unzoomig");
	y += 20;
	xlibDrawString(50, y, "Press '4'/'5' for increasing/decreasing n");
	y += 20;
	xlibDrawString(50, y, "Press '6'/'7' for adding/removing some number in some knot");
	y += 20;
	xlibDrawString(50, y, "Press arrows for moving to left/right");
	y += 20;
	xlibDrawString(50, y, "Press 'q'/'escape' to quit");
}

void render(int width, int height)
{
	double hx, hy;
	double r1, r2;
	char s[256];

	xlibSetColor(0.0, 0.0, 0.0);
	xlibFillRectangle(0, 0, width, height);

	switch (variant)
	{
	case 0:
		DisplayHelp();
		return;
	case 1:
		sprintf(s, "Method 11");
		break;
	case 2:
		sprintf(s, "Method 41");
		break;
	case 3:
		sprintf(s, "Both methods");
		break;
	case 4:
		sprintf(s, "Pogreshnost");
		break;
	}

	xlibSetColor(1.0, 1.0, 1.0);
	xlibDrawString(50, 20, "%s, a = %.2f, b = %.2f, n = %d", s, a, b, n);
	xlibDrawString(50, 40, "Press 'q'/'escape' to quit");

	xlibDrawString(50, 60, "x_left = %.2f  x_right = %.2f", x_left, x_right);

	if (variant != 4)
	{
		CalcMaxAndMin(width, f, &max, &min);
		y_up = max + 1.0;
		y_down = min - 1.0;
		xlibDrawString(50, 80, "max of function = %.2f  min of function = %.2f", max, min);
	}
	else
	{
		CalcMaxAndMin(width, f_pogreshnost_method1, &r1, &r2);
		y_up = r1 + 1.0;
		y_down = r2 - 1.0;
		if (fabs(r1) < fabs(r2)) r1 = r2;
		r1 = fabs(r1);
		xlibDrawString(50, 80, "pogreshnost of first method = %e", r1);

		CalcMaxAndMin(width, f_pogreshnost_method2, &r1, &r2);
		if (r1 + 1.0 > y_up) y_up = r1 + 1.0;
		if (r2 - 1.0 < y_down) y_down = r2 - 1.0;
		if (fabs(r1) < fabs(r2)) r1 = r2;
		r1 = fabs(r1);
		xlibDrawString(50, 100, "pogreshnost of second method = %e", r1);
	}

	hx = (x_right - x_left)/(width - 1);
	hy = height/(y_up - y_down);

	xlibDrawLine(0, y_up * hy, width, y_up * hy);
	xlibDrawLine(1 - x_left/hx, 0, 1 - x_left/hx, height);

	switch (variant)
	{
	case 1:
		xlibSetColor(0.0, 0.0, 1.0);
		DrawFunction(width, height, f_method1);

		xlibSetColor(1.0, 0.0, 0.0);
		DrawFunction(width, height, f);
		break;
	case 2:
		xlibSetColor(0.0, 1.0, 0.0);
		DrawFunction(width, height, f_method2);

		xlibSetColor(1.0, 0.0, 0.0);
		DrawFunction(width, height, f);
		break;
	case 3:
		xlibSetColor(0.0, 0.0, 1.0);
		DrawFunction(width, height, f_method1);

		xlibSetColor(0.0, 1.0, 0.0);
		DrawFunction(width, height, f_method2);

		xlibSetColor(1.0, 0.0, 0.0);
		DrawFunction(width, height, f);
		break;
	case 4:
		xlibSetColor(0.0, 0.0, 1.0);
		DrawFunction(width, height, f_pogreshnost_method1);

		xlibSetColor(0.0, 1.0, 0.0);
		DrawFunction(width, height, f_pogreshnost_method2);
		break;
	}
}

void keyboard(int key)
{
	double tmp;
	int number;
	double delta;

	switch (key)
	{
	case 'q':
	case 'Q':
	case KEY_ESC:
		xlibPostExit();
		return;
	case '1':
		if (++variant > 4) variant = 0;
		if (!variant) break;
		if (variant != 2)
		{
			Input_11();
			Calc_11();
		}
		if (variant != 1)
		{
			Input_41();
			Calc_41();
		}
		break;
	case '2':
	case '3':
		tmp = (x_right - x_left);
		if (key == '2') tmp /= 4.0;
		else tmp /= -2.0;
		x_left += tmp;
		x_right -= tmp;
		break;
	case '4':
	case '5':
		Finalize();

		if (key == '4') n *= 2;
		else if (n > 5) n /= 2;

		if (!Init_11(n, a, b) || !Init_41(n, a, b))
		{
			printf("Not enough memory.\n");
			Finalize();
			xlibPostExit();
			return;
		}

		if (variant != 2)
		{
			Input_11();
			Calc_11();
		}
		if (variant != 1)
		{
			Input_41();
			Calc_41();
		}
		break;
	case '6':
	case '7':
		number = n/3;
		if (key == '6') delta = +3.0;
		else delta = -3.0;
		if (variant != 2)
		{
			Delta_11(number, delta);
			Calc_11();
		}
		if (variant != 1)
		{
			Delta_41(number, delta);
			Calc_41();
		}
		break;
	case KEY_LEFT:
		x_left -= 1.0;
		x_right -= 1.0;
		break;
	case KEY_RIGHT:
		x_left += 1.0;
		x_right += 1.0;
		break;
	default:
		return;
	}

	xlibPostRedisplay();
}

int Init(void)
{
#ifdef CONSOLE_INPUT
	printf("Please, enter a and b: ");
	scanf("%lg %lg", &a, &b);
	printf("Now, please, enter n: ");
	scanf("%d", &n);
#endif

	if (b <= a || n < 3)
	{
		printf("Some input error.\n");
		return 0;
	}

	x_left = a;
	x_right = b;

	if (!Init_11(n, a, b) || !Init_41(n, a, b))
	{
		printf("Not enough memory.\n");
		Finalize();
		return 0;
	}

	return 1;
}

void Finalize(void)
{
	Finalize_11();
	Finalize_41();
}

int main(void)
{
	int width = 750;
	int height = 550;

	xlibInitPosition(0, 0);
	xlibInitWindowSize(width, height);

	xlibRenderFunc(render);
	xlibKeyboardFunc(keyboard);

	if (!Init()) return -1;

	xlibMainLoop("Interpolation");

	Finalize();

	return 0;
}
