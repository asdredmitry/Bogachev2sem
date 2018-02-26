#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "metod_06.h"

static int n;
static double a;
static double b;

static double *x = NULL;
static double *f_x = NULL;
static double *c = NULL;

void Coeff_06(int n, double *f_x, double *c);
double Value_06(double t, double a, double b, int n, double *c);

int Init_06(int n_, double a_, double b_)
{
	n = n_;
	a = a_;
	b = b_;

	x = (double*)malloc(n * sizeof(double));
	f_x = (double*)malloc(n * sizeof(double));
	c = (double*)malloc(n * sizeof(double));

	if (!(x && f_x && c)) return 0;

	return 1;
}

void Finalize_06(void)
{
	if (x) free(x); x = NULL;
	if (f_x) free(f_x); f_x = NULL;
	if (c) free(c); c = NULL;
}

void Input_06(void)
{
	int i;
	double tmp1, tmp2;

	tmp1 = (a + b)/2.0;
	tmp2 = (b - a)/2.0;

	for (i = 0; i < n; i++)
	{
		x[i] = tmp1 + tmp2 * cos(3.1415926 * (2 * i + 1) * 0.5/n);
		f_x[i] = f(x[i]);
	}
}

void Delta_06(int number, double delta)
{
	f_x[number] += delta;
}

void Calc_06(void)
{
	Coeff_06(n, f_x, c);
}

double f_06(double t)
{
	return Value_06(t, a, b, n, c);
}
