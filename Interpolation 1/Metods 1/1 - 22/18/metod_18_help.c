#include <stdlib.h>
#include "main.h"
#include "metod_18.h"

static int n;
static double a;
static double b;

static double *x = NULL;
static double *f_x = NULL;
static double *c = NULL;
static double *d = NULL;

void Coeff_18(int n, double *x, double *f_x, double *c, double *d);
double Value_18(double t, int n, double *c, double *x);

int Init_18(int n_, double a_, double b_)
{
	n = n_;
	a = a_;
	b = b_;

	x = (double*)malloc(n * sizeof(double));
	f_x = (double*)malloc(n * sizeof(double));
	c = (double*)malloc(4 * (n - 1) * sizeof(double));
	d = (double*)malloc(n * sizeof(double));

	if (!(x && f_x && c && d)) return 0;

	return 1;
}

void Finalize_18(void)
{
	if (x) free(x); x = NULL;
	if (f_x) free(f_x); f_x = NULL;
	if (c) free(c); c = NULL;
	if (d) free(d); d = NULL;
}

void Input_18(void)
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

void Delta_18(int number, double delta)
{
	f_x[number] += delta;
}

void Calc_18(void)
{
	Coeff_18(n, x, f_x, c, d);
}

double f_18(double t)
{
	return Value_18(t, n, c, x);
}
