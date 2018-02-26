#include <math.h>
#include "main.h"
#include "metod_01.h"

void Coeff_01(int n, double *x, double *f_x, double *c)
{
	int i, j;

	for (i = 0; i < n; i++)
		c[i] = f_x[i];

	for (i = 1; i < n; i++)
		for (j = n - 1; j > i - 1; j--)
			c[j] = (c[j] - c[j - 1]) / (x[j] - x[j - i]);
}

double Value_01(double t, int n, double *c, double *x)
{
	int i;
	double fx;

	fx = c[n - 1];

	for (i = n - 2; i >= 0; i--)
	{
		fx *= (t - x[i]);
		fx += c[i];
	}

	return fx;
}
