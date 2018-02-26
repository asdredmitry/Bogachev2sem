#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "main.h"
#include "xlib.h"

#include "metod_01.h"
#include "metod_07.h"

/*#define CONSOLE_INPUT*/		/* For console input */

static int n = 20;
static double a = -10;
static double b = +10;

static int m = 20;
static double c = -10;
static double d = +10;

static double max, min;

static double x_left, x_right;
static double y_up, y_down;

static int variant = 0;

static int num_steps = 80;
static double phi = 30.0 * 3.1075926/180.0;
static double psi = 10.0 * 3.1075926/180.0;

/* Function */
double f(double x, double y)
{
	return cos(sqrt(x * x + y * y));
}

/* Functions, that will be use for drawing */
static double f_method1(double x, double y)
{
	return f_01(x, y);
}

static double f_method2(double x, double y)
{
	return f_07(x, y);
}

static double f_pogreshnost_method1(double x, double y)
{
	return f(x, y) - f_method1(x, y);
}

static double f_pogreshnost_method2(double x, double y)
{
	return f(x, y) - f_method2(x, y);
}

int YMax[1000];
int YMin[1000];
#define NO_VALUE  7777

typedef struct _Point
{
	int x;
	int y;
} Point;

void drawLine(Point* p1, Point* p2)
{
	int dx = fabs(p2->x - p1->x);
	int dy = fabs(p2->y - p1->y);
	int sx = p2->x >= p1->x ? 1 : -1;
	int sy = p2->y >= p1->y ? 1 : -1;
	int d;
	int d1;
	int d2;
	int x;
	int y;
	int i;
	int m1;
	int m2;

	if (dy <= dx)
	{
		d = -dx;
		d1 = dy << 1;
		d2 = (dy - dx) << 1;

		for (x = p1->x, y = p1->y, i = 0; i <= dx; i++, x += sx)
		{
			if (YMin[x] == NO_VALUE)
			{
				xlibDrawPoint(x, y);
				YMin[x] = YMax[x] = y;
			}
			else if (y < YMin[x])
			{
				xlibDrawPoint(x, y);
				YMin[x] = y;
			}
			else if (y > YMax[x])
			{
				xlibDrawPoint(x, y);
				YMax[x] = y;
			}

			if (d > 0)
			{
				d += d2;
				y += sy;
			}
			else d += d1;
		}
	}
	else
	{
		d = -dy;
		d1 = dx << 1;
		d2 = (dx - dy) << 1;
		m1 = YMin[p1->x];
		m2 = YMax[p1->x];

		for (x = p1->x, y = p1->y, i = 0; i <= dy; i++, y += sy)
		{
			if (YMin[x]  == NO_VALUE)
			{
				xlibDrawPoint(x, y);
				YMin[x] = YMax[x] = y;
			}
			else if (y < m1)
			{
				xlibDrawPoint(x, y);
				if (y < YMin[x]) YMin[x] = y;
			}
			else if (y > m2)
			{
				xlibDrawPoint(x, y);
				if (y > YMax[x]) YMax[x] = y;
			}

			if (d > 0)
			{
				d += d2;
				x += sx;
				m1 = YMin[x];
				m2 = YMax[x];
			}
			else d += d1;
		}
	}
}

void DrawFunction(int width, int height, double (*func)(double x, double y))
{
	Point* curLine = (Point*)malloc(num_steps * sizeof(Point));
	Point* nextLine = (Point*)malloc(num_steps * sizeof(Point));
	double sphi = sin(phi);
	double cphi = cos(phi);
	double spsi = sin(psi);
	double cpsi = cos(psi);
	double e1[3] = {cphi, sphi, 0.0};
	double e2[3] = {sphi * spsi, -spsi * cphi, cpsi};
	double x, y, z;
	double hx = (x_right - x_left)/(num_steps - 1);
	double hy = (y_up - y_down)/(num_steps - 1);
	double xMin = (e1[0] >= 0.0 ? x_left : x_right) * e1[0] + (e1[1] >= 0.0 ? y_down : y_up) * e1[1];
	double xMax = (e1[0] >= 0.0 ? x_right : x_left) * e1[0] + (e1[1] >= 0.0 ? y_up : y_down) * e1[1];
	double yMin = (e2[0] >= 0.0 ? x_left : x_right) * e2[0] + (e2[1] >= 0.0 ? y_down : y_up) * e2[1];
	double yMax = (e2[0] >= 0.0 ? x_right : x_left) * e2[0] + (e2[1] >= 0.0 ? y_up : y_down) * e2[1];
	double ax;
	double bx;
	double ay;
	double by;
	int i, j;

	width = width; height = height;

	if (e2[2] >= 0.0)
	{
		yMin += min * e2[2];
		yMax += max * e2[2];
	}
	else
	{
		yMin += max * e2[2];
		yMax += min * e2[2];
	}

	ax = -750 * xMin/(xMax - xMin);
	bx = 750/(xMax - xMin);
	ay = -550 * yMin/(yMax - yMin);
	by = 550/(yMax - yMin);

	for (i = 0; i < (int)(sizeof(YMax)/sizeof(int)); i++)
		YMin[i] = YMax[i] = NO_VALUE;

	for (i = 0; i < num_steps; i++)
	{
		x = x_left + i * hx;
		y = y_down + (num_steps - 1) * hy;
		z = func(x, y);
		curLine[i].x = (int)(ax + bx * (x * e1[0] + y * e1[1]));
		curLine[i].y = height - (int)(ay + by * (x * e2[0] + y * e2[1] + z * e2[2]));
	}
	for (i = num_steps - 1; i > -1; i--)
	{
		for (j = 0; j < num_steps - 1; j++) drawLine(&curLine[j], &curLine[j + 1]);
		if (i > 0)
			for (j = 0; j < num_steps; j++)
			{
				x = x_left + j * hx;
				y = y_down + (i - 1) * hy;
				z = func(x, y);
				nextLine[j].x = (int)(ax + bx * (x * e1[0] + y * e1[1]));
				nextLine[j].y = height - (int)(ay + by * (x * e2[0] + y * e2[1] + z * e2[2]));
				drawLine(&curLine[j], &nextLine[j]);
				curLine[j] = nextLine[j];
			}
	}

	free(curLine);
	free(nextLine);
}

void CalcMaxAndMin(int width, int height, double (*func)(double x, double y), double* maxim, double* minim)
{
	int i, j;
	double x, y, z;
	double hx, hy;

	hx = (x_right - x_left)/num_steps;//(width - 1);
	hy = (y_up - y_down)/num_steps;//(height - 1);

	*maxim = *minim = func(x_left, y_down);
	for (i = 0; i < num_steps; i++)
		for (j = 0; j < num_steps; j++)
		{
			x = x_left + i * hx;
			y = y_down + j * hy;
			z = func(x, y);
			if (z > *maxim) *maxim = z;
			if (z < *minim) *minim = z;
		}
width=width; height=height;
}

/*
void DrawFunction(int width, int height, double (*func)(double x, double y))
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
*/
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
	xlibDrawString(50, y, "Press '8'/'9' for rotating around Z-axis");
	y += 20;
	xlibDrawString(50, y, "Press '-'/'=' for increasing/decreasing num of steps used for drawing a function");
	y += 20;
	xlibDrawString(50, y, "Press arrows for moving to left/right/up/down");
	y += 20;
	xlibDrawString(50, y, "Press 'q'/'escape' to quit");
}

void render(int width, int height)
{
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
		sprintf(s, "Function");
		break;
	case 2:
		sprintf(s, "Method 01");
		break;
	case 3:
		sprintf(s, "Method 07");
		break;
	case 4:
		sprintf(s, "Pogreshnost 01");
		break;
	case 5:
		sprintf(s, "Pogreshnost 07");
		break;
	}

	xlibSetColor(1.0, 1.0, 1.0);
	xlibDrawString(50, 20, "%s, a = %.2f, b = %.2f, n = %d, c = %.2f, d = %.2f, m = %d", s, a, b, n, c, d, m);
	xlibDrawString(50, 40, "Press 'q'/'escape' to quit");
	xlibDrawString(50, 60, "x_left = %.2f  x_right = %.2f, y_up = %.2f, y_down = %.2f", x_left, x_right, y_up, y_down);

	if (variant != 4 && variant != 5)
	{
		CalcMaxAndMin(width, height, f, &max, &min);
		xlibDrawString(50, 80, "max of function = %.2f  min of function = %.2f", max, min);
	}
	else if (variant == 4)
	{
		CalcMaxAndMin(width, height, f_pogreshnost_method1, &r1, &r2);
		if (fabs(r1) < fabs(r2)) r1 = r2;
		r1 = fabs(r1);
		xlibDrawString(50, 80, "pogreshnost = %e", r1);
	}
	else if (variant == 5)
	{
		CalcMaxAndMin(width, height, f_pogreshnost_method2, &r1, &r2);
		if (fabs(r1) < fabs(r2)) r1 = r2;
		r1 = fabs(r1);
		xlibDrawString(50, 80, "pogreshnost = %e", r1);
	}

	switch (variant)
	{
	case 1:
		xlibSetColor(1.0, 0.0, 0.0);
		DrawFunction(width, height, f);
		break;
	case 2:
		xlibSetColor(0.0, 0.0, 1.0);
		DrawFunction(width, height, f_method1);
		break;
	case 3:
		xlibSetColor(0.0, 1.0, 0.0);
		DrawFunction(width, height, f_method2);
		break;
	case 4:
		xlibSetColor(0.0, 0.0, 1.0);
		DrawFunction(width, height, f_pogreshnost_method1);
		break;
	case 5:
		xlibSetColor(0.0, 1.0, 0.0);
		DrawFunction(width, height, f_pogreshnost_method2);
		break;
	}
}

void keyboard(int key)
{
	double tmp;
	int number1;
	int number2;
	double delta;

	switch (key)
	{
	case 'q':
	case 'Q':
	case KEY_ESC:
		xlibPostExit();
		return;
	case '1':
		if (++variant > 5) variant = 0;
		if (!variant) break;
		if (variant == 2 || variant == 4)
		{
			Input_01();
			Calc_01();
		}
		if (variant == 3 || variant == 5)
		{
			Input_07();
			Calc_07();
		}
		break;
	case '2':
	case '3':
		tmp = (x_right - x_left);
		if (key == '2') tmp /= 4.0;
		else tmp /= -2.0;
		x_left += tmp;
		x_right -= tmp;
		tmp = (y_up - y_down);
		if (key == '2') tmp /= 4.0;
		else tmp /= -2.0;
		y_down += tmp;
		y_up -= tmp;
		break;
	case '4':
	case '5':
		Finalize();

		if (key == '4')
		{
			n *= 2;
			m *= 2;
		}
		else if (n > 5 && m > 5)
		{
			n /= 2;
			m /= 2;
		}

		if (!Init_01(n, a, b, m, c, d) || !Init_07(n, a, b, m, c, d))
		{
			printf("Not enough memory.\n");
			Finalize();
			xlibPostExit();
			return;
		}

		if (variant == 2 || variant == 4)
		{
			Input_01();
			Calc_01();
		}
		if (variant == 3 || variant == 5)
		{
			Input_07();
			Calc_07();
		}
		break;
	case '6':
	case '7':
		number1 = n/3;
		number2 = m/3;
		if (key == '6') delta = +3.0;
		else delta = -3.0;
		if (variant == 2 || variant == 4)
		{
			Delta_01(number1, number2, delta);
			Calc_01();
		}
		if (variant == 3 || variant == 5)
		{
			Delta_07(number1, number2, delta);
			Calc_07();
		}
		break;
	case '8':
		phi += (3.1415926/180.0)/4.0;
		break;
	case '9':
		phi -= (3.1415926/180.0)/4.0;
		break;
	case '-':
		num_steps *= 2;
		break;
	case '=':
		if (num_steps > 5) num_steps /= 2;
		break;
	case KEY_LEFT:
		x_left -= 1.0;
		x_right -= 1.0;
		break;
	case KEY_RIGHT:
		x_left += 1.0;
		x_right += 1.0;
		break;
	case KEY_DOWN:
		y_down -= 1.0;
		y_up -= 1.0;
		break;
	case KEY_UP:
		y_down += 1.0;
		y_up += 1.0;
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
	printf("Please, enter c and d: ");
	scanf("%lg %lg", &c, &d);
	printf("Now, please, enter m: ");
	scanf("%d", &m);
#endif

	if (b <= a || n < 3 || d <= c || m < 3)
	{
		printf("Some input error.\n");
		return 0;
	}

	x_left = a;
	x_right = b;
	y_down = c;
	y_up = d;

	if (!Init_01(n, a, b, m, c, d) || !Init_07(n, a, b, m, c, d))
	{
		printf("Not enough memory.\n");
		Finalize();
		return 0;
	}

	return 1;
}

void Finalize(void)
{
	Finalize_01();
	Finalize_07();
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
