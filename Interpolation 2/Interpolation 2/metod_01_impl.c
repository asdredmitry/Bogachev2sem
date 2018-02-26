#include <math.h>
#include "main.h"
#include "metod_01.h"

static void Interp_X(int i, int n, int m, double *coef, double *yarr)
{
	int k, j;
n=n;
	for (k = 1; k < m; k++)
		for (j = m - 1; j > k - 1; j--)
			coef[i * m + j] = (coef[i * m + j] - coef[i * m + j - 1])/(yarr[j] - yarr[j - k]);
}

static void Interp_Y(int j, int n, int m, double *coef, double *xarr)
{
	int i, k;

	for (i = 1; i < n; i++)
		for (k = n - 1; k > i - 1; k--)
			coef[k * m + j] = (coef[k * m + j] - coef[(k - 1) * m + j])/(xarr[k] - xarr[k - i]);
}

void Coeff_01(int n, int m, double *xarr, double *yarr, double *f_ij, double *coef)
{
	int i, j;

	for (i = 0; i < n * m; i++)
		coef[i] = f_ij[i];

	for (i = 0; i < n; i++)
		Interp_X(i, n, m, coef, yarr);

	for (j = 0; j < m; j++)
		Interp_Y(j, n, m, coef, xarr);
}

double Value_01(double x, double y, int n, int m, double *coef, double *xarr, double *yarr)
{
	int i, j;
	double f_xy;
	double tmp;

	f_xy = 0.0;
	for (i = n - 1; i >= 0; i--)
	{
		f_xy *= (x - xarr[i]);
		tmp = coef[i * m + m - 1];
		for (j = m - 2; j >= 0; j--)
		{
			tmp *= (y - yarr[j]);
			tmp += coef[i * m + j];
		}
		f_xy += tmp;
	}

	return f_xy;
}
