#include <stdlib.h>
#include "main.h"
#include "metod_35.h"

static int n;
static double a;
static double b;

static double *x = NULL;
static double *f_x = NULL;
static double *c = NULL;
static double *d = NULL;
static double *a1 = NULL;
static double *c1 = NULL;
static double *d1 = NULL;

void Coeff_35(int n, double *x, double *f_x, double *c, double *d, double *a1, double *c1, double *d1);
double Value_35(double t, int n, double *c, double *x);

int Init_35(int n_, double a_, double b_)
{
	n = n_;
	a = a_;
	b = b_;

	x = (double*)malloc(n * sizeof(double));
	f_x = (double*)malloc(n * sizeof(double));
	c = (double*)malloc(4 * (n - 1) * sizeof(double));
	d = (double*)malloc(n * sizeof(double));
	a1 = (double*)malloc(n * sizeof(double));
	c1 = (double*)malloc((n - 1) * sizeof(double));
	d1 = (double*)malloc((n - 1) * sizeof(double));

	if (!(x && f_x && c && d && a1 && c1 && d1)) return 0;

	return 1;
}

void Finalize_35(void)
{
	if (x) free(x); x = NULL;
	if (f_x) free(f_x); f_x = NULL;
	if (c) free(c); c = NULL;
	if (d) free(d); d = NULL;
	if (a1) free(a1); a1 = NULL;
	if (c1) free(c1); c1 = NULL;
	if (d1) free(d1); d1 = NULL;
}

void Input_35(void)
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

void Delta_35(int number, double delta)
{
	f_x[number] += delta;
}

void Calc_35(void)
{
	Coeff_35(n, x, f_x, c, d, a1, c1, d1);
}

double f_35(double t)
{
	return Value_35(t, n, c, x);
}
