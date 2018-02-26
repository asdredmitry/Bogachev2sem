#include <stdlib.h>
#include "main.h"
#include "metod_07.h"

static int n;
static double a;
static double b;

static int m;
static double c;
static double d;

static double *xarr = NULL;
static double *yarr = NULL;
static double *f_ij = NULL;
static double *coef = NULL;

void Coeff_07(int n, int m, double *f_ij, double *coef);
double Value_07(double x, double y, int n, int m, double *coef, double *xarr, double *yarr);

int Init_07(int n_, double a_, double b_, int m_, double c_, double d_)
{
	n = n_;
	a = a_;
	b = b_;

	m = m_;
	c = c_;
	d = d_;

	xarr = (double*)malloc(n * sizeof(double));
	yarr = (double*)malloc(m * sizeof(double));
	f_ij = (double*)malloc(n * m * sizeof(double));
	coef = (double*)malloc(n * m * sizeof(double));

	if (!(xarr && yarr && f_ij && coef)) return 0;


	return 1;
}

void Finalize_07(void)
{
	if (xarr) free(xarr); xarr = NULL;
	if (yarr) free(yarr); yarr = NULL;
	if (f_ij) free(f_ij); f_ij = NULL;
	if (coef) free(coef); coef = NULL;
}

void Input_07(void)
{
	int i, j;
	double hx, hy;

	hx = (b - a)/(n - 1);
	hy = (d - c)/(m - 1);

	for (i = 0; i < n; i++)
	{
		xarr[i] = a + i * hx;
		for (j = 0; j < m; j++)
		{
			yarr[j] = c + j * hy;
			f_ij[i * m + j] = f(xarr[i], yarr[j]);
		}
	}
}

void Delta_07(int number1, int number2, double delta)
{
	f_ij[number1 * m + number2] += delta;
}

void Calc_07(void)
{
	Coeff_07(n, m, f_ij, coef);
}

double f_07(double x, double y)
{
	return Value_07(x, y, n, m, coef, xarr, yarr);
}
