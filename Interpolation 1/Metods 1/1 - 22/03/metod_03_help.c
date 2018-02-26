#include <stdlib.h>
#include "main.h"
#include "metod_03.h"

static int n;
static double a;
static double b;

static double *x = NULL;
static double *f_x = NULL;
static double *c = NULL;

void Coeff_03(int n, double *x, double *f_x, double *c);
double Value_03(double t, int n, double *c, double *x);

int Init_03(int n_, double a_, double b_)
{
	n = n_;
	a = a_;
	b = b_;

	x = (double*)malloc(n * sizeof(double));
	f_x = (double*)malloc(n * sizeof(double));
	c = (double*)malloc(2 * n * sizeof(double));

	if (!(x && f_x && c)) return 0;

	return 1;
}

void Finalize_03(void)
{
	if (x) free(x); x = NULL;
	if (f_x) free(f_x); f_x = NULL;
	if (c) free(c); c = NULL;
}

void Input_03(void)
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

void Delta_03(int number, double delta)
{
	f_x[number] += delta;
}

void Calc_03(void)
{
	Coeff_03(n, x, f_x, c);
}

double f_03(double t)
{
	return Value_03(t, n, c, x);
}
