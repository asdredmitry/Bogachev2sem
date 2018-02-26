#include <stdlib.h>
#include "main.h"
#include "metod_41.h"

static int n;
static double a;
static double b;

static double *x = NULL;
static double *f_x = NULL;
static double *c = NULL;
static double *v = NULL;
static double *ksi = NULL;
static double *a1 = NULL;
static double *c1 = NULL;
static double *d1 = NULL;

void Coeff_41(int n, double a, double b, double *x, double *f_x, double *c, double *v, double *ksi, double *a1, double *c1, double *d1);
double Value_41(double t, int n, double *c, double *ksi);

int Init_41(int n_, double a_, double b_)
{
	n = n_;
	a = a_;
	b = b_;

	x = (double*)malloc(n * sizeof(double));
	f_x = (double*)malloc(n * sizeof(double));
	c = (double*)malloc(3 * n * sizeof(double));
	v = (double*)malloc((n + 1) * sizeof(double));
	ksi = (double*)malloc((n + 1) * sizeof(double));
	a1 = (double*)malloc((n + 1) * sizeof(double));
	c1 = (double*)malloc(n * sizeof(double));
	d1 = (double*)malloc(n * sizeof(double));

	if (!(x && f_x && c && v && ksi && a1 && c1 && d1)) return 0;

	return 1;
}

void Finalize_41(void)
{
	if (x) free(x); x = NULL;
	if (f_x) free(f_x); f_x = NULL;
	if (c) free(c); c = NULL;
	if (v) free(v); v = NULL;
	if (ksi) free(ksi); ksi = NULL;
	if (a1) free(a1); a1 = NULL;
	if (c1) free(c1); c1 = NULL;
	if (d1) free(d1); d1 = NULL;
}

void Input_41(void)
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

void Delta_41(int number, double delta)
{
	f_x[number] += delta;
}

void Calc_41(void)
{
	Coeff_41(n, a, b, x, f_x, c, v, ksi, a1, c1, d1);
}

double f_41(double t)
{
	return Value_41(t, n, c, ksi);
}
