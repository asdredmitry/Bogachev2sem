#include <stdlib.h>
#include "main.h"
#include "metod_09.h"

static int n;
static int N;
static double a;
static double b;

static double *x = NULL;
static double *f_x = NULL;
static double *c = NULL;

void Coeff_09(int n, int N, double *x, double *f_x, double *c);
double Value_09(double t, double a, double b, int n, double *c, double *x);

int Init_09(int n_, double a_, double b_)
{
	n = n_;
	a = a_;
	b = b_;

	N = n + n;

	x = (double*)malloc((N + 1) * sizeof(double));
	f_x = (double*)malloc((N + 1) * sizeof(double));
	c = (double*)malloc(n * sizeof(double));

	if (!(x && f_x && c)) return 0;

	return 1;
}

void Finalize_09(void)
{
	if (x) free(x); x = NULL;
	if (f_x) free(f_x); f_x = NULL;
	if (c) free(c); c = NULL;
}

void Input_09(void)
{
	int i;
	double h;

	h = (b - a)/N;
	for (i = 0; i < N + 1; i++)
	{
		x[i] = a + i * h;
		f_x[i] = f(x[i]);
		x[i] = (2.0 * x[i] - (b + a))/(b - a);
	}
}

void Delta_09(int number, double delta)
{
	f_x[number] += delta;
}

void Calc_09(void)
{
	Coeff_09(n, N, x, f_x, c);
}

double f_09(double t)
{
	return Value_09(t, a, b, n, c, x);
}
