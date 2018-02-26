#include <math.h>
#include "main.h"
#include "metod_03.h"

void Coeff_03(int n, double *x, double *f_x, double *c)
{
	int i, j;

	j = 0;

	for (i = 0; i < n; i++)
	{
		c[j] = f_x[i];
		c[j + 1] = c[j];
		j += 2;
	}

	j = n - 1;
	for (i = 2 * n - 1; i > 0; i--)
		if ((i + 1) % 2 == 0) c[i] = f_first(x[j--]);
		else c[i] = (c[i] - c[i - 1]) / (x[i/2] - x[i/2 - 1]);

	for (i = 2; i < 2 * n; i++)
		for (j = 2 * n - 1; j > i - 1; j--)
			c[j] = (c[j] - c[j - 1]) / (x[j/2] - x[(j - i)/2]);
}

double Value_03(double t, int n, double *c, double *x)
{
	int i;
	double fx;

	fx = c[2 * n - 1];

	for (i = 2 * n - 2; i >= 0; i--)
	{
		fx *= (t - x[i/2]);
		fx += c[i];
	}

	return fx;
}
