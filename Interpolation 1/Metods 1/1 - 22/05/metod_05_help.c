#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "metod_05.h"

static int n;
static double a;
static double b;

static double *x = NULL;
static double *f_x = NULL;
static double *c = NULL;
static double *a1 = NULL;
static double *b1 = NULL;
static double *c1 = NULL;

void Coeff_05(int n, double a, double b, double *x, double *f_x, double *c, double *a1, double *b1, double *c1);
double Value_05(double t, double a, double b, int n, double *c);

int Init_05(int n_, double a_, double b_)
{
	n = n_;
	a = a_;
	b = b_;

	x = (double*)malloc(n * sizeof(double));
	f_x = (double*)malloc(n * sizeof(double));
	c = (double*)malloc(n * sizeof(double));
	a1 = (double*)malloc(n * n * sizeof(double));
	b1 = (double*)malloc(n * n * sizeof(double));
	c1 = (double*)malloc(n * sizeof(double));

	if (!(x && f_x && c && a1 && b1 && c1)) return 0;

	return 1;
}

void Finalize_05(void)
{
	if (x) free(x); x = NULL;
	if (f_x) free(f_x); f_x = NULL;
	if (c) free(c); c = NULL;
	if (a1) free(a1); a1 = NULL;
	if (b1) free(b1); b1 = NULL;
	if (c1) free(c1); c1 = NULL;
}

void Input_05(void)
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

void Delta_05(int number, double delta)
{
	f_x[number] += delta;
}

void Calc_05(void)
{
	Coeff_05(n, a, b, x, f_x, c, a1, b1, c1);
}

double f_05(double t)
{
	return Value_05(t, a, b, n, c);
}
