#include <stdlib.h>
#include "main.h"
#include "metod_07.h"

static int n;
static double a;
static double b;

static double *x = NULL;
static double *f_x = NULL;
static double *c = NULL;

void Coeff_07(int n, double *x, double *f_x, double *c);
double Value_07(double t, int n, double *c, double *x, double *f_x);

int Init_07(int n_, double a_, double b_)
{
	n = n_;
	a = a_;
	b = b_;

	x = (double*)malloc(n * sizeof(double));
	f_x = (double*)malloc(n * sizeof(double));
	c = (double*)malloc((n - 1) * sizeof(double));

	if (!(x && f_x && c)) return 0;

	return 1;
}

void Finalize_07(void)
{
	if (x) free(x); x = NULL;
	if (f_x) free(f_x); f_x = NULL;
	if (c) free(c); c = NULL;
}

void Input_07(void)
{
	int i;
	double h;

	h = (b - a)/(n - 1);
	for (i = 0; i < n; i++)
	{
		x[i] = a + i * h;
		f_x[i] = f(x[i]);
	}
}

void Delta_07(int number, double delta)
{
	f_x[number] += delta;
}

void Calc_07(void)
{
	Coeff_07(n, x, f_x, c);
}

double f_07(double t)
{
	return Value_07(t, n, c, x, f_x);
}
