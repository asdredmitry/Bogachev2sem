#include <stdlib.h>
#include "main.h"
#include "metod_02.h"

static int n;
static int k;
static double a;
static double b;

static double *x = NULL;
static double *f_x = NULL;
static double *c = NULL;

void Coeff_02(int n, double *x, double *f_x, double *c, int k);
double Value_02(double t, int n, double *c, double *x, int k);

int Init_02(int n_, double a_, double b_)
{
	n = n_;
	a = a_;
	b = b_;
	k = 2;

	x = (double*)malloc(n * sizeof(double));
	f_x = (double*)malloc(n * sizeof(double));
	c = (double*)malloc((n - k) * (k + 1) * sizeof(double));

	if (!(x && f_x && c)) return 0;

	return 1;
}

void Finalize_02(void)
{
	if (x) free(x); x = NULL;
	if (f_x) free(f_x); f_x = NULL;
	if (c) free(c); c = NULL;
}

void Input_02(void)
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

void Delta_02(int number, double delta)
{
	f_x[number] += delta;
}

void Calc_02(void)
{
	Coeff_02(n, x, f_x, c, k);
}

double f_02(double t)
{
	return Value_02(t, n, c, x, k);
}
