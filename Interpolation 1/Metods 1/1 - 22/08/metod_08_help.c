#include <stdlib.h>
#include "main.h"
#include "metod_08.h"

static int n;
static int N;
static double a;
static double b;

static double *x = NULL;
static double *y = NULL;
static double *f_y = NULL;
static double *c = NULL;
static double *a1 = NULL;
static double *c1 = NULL;
static double *d1 = NULL;
static double *e1 = NULL;

void Coeff_08(int n, int N, double *x, double *y, double *f_y, double *c, double *a1, double *c1, double *d1, double *e1);
double Value_08(double t, int n, double *c, double *x);

int Init_08(int n_, double a_, double b_)
{
	n = n_;
	a = a_;
	b = b_;

	N = n + n;

	x = (double*)malloc(n * sizeof(double));
	y = (double*)malloc(N * sizeof(double));
	f_y = (double*)malloc(N * sizeof(double));
	c = (double*)malloc(n * sizeof(double));
	a1 = (double*)malloc(n * sizeof(double));
	c1 = (double*)malloc((n - 1) * sizeof(double));
	d1 = (double*)malloc((n - 1) * sizeof(double));
	e1 = (double*)malloc(n * sizeof(double));

	if (!(x && y && f_y && c && a1 && c1 && d1 && e1)) return 0;

	return 1;
}

void Finalize_08(void)
{
	if (x) free(x); x = NULL;
	if (y) free(y); y = NULL;
	if (f_y) free(f_y); f_y = NULL;
	if (c) free(c); c = NULL;
	if (a1) free(a1); a1 = NULL;
	if (c1) free(c1); c1 = NULL;
	if (d1) free(d1); d1 = NULL;
	if (e1) free(e1); e1 = NULL;
}

void Input_08(void)
{
	int i;
	double h;

	h = (b - a)/(N - 1);
	for (i = 0; i < N; i++)
	{
		y[i] = a + i * h;
		f_y[i] = f(y[i]);
	}

	h = (b - a)/(n - 1);
	for (i = 0; i < n; i++) x[i] = a + i * h;
}

void Delta_08(int number, double delta)
{
	f_y[number] += delta;
}

void Calc_08(void)
{
	Coeff_08(n, N, x, y, f_y, c, a1, c1, d1, e1);
}

double f_08(double t)
{
	return Value_08(t, n, c, x);
}
