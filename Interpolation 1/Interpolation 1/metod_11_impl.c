#include <math.h>
#include "main.h"
#include "metod_11.h"

void Coeff_11(int n, double *x, double *f_x, double *c, double *d)
{
	int i, j;
	double tmp1, tmp2;

	for (i = 1; i < n - 1; i++)
		d[i] = f_first(x[i]);

	d[0] = f_first(x[0]);
	d[n - 1] = f_first(x[n - 1]);

	j = 0;

	for (i = 0; i < n - 1; i++)
	{
		c[j + 0] = f_x[i];
		c[j + 1] = d[i];
		tmp2 = x[i + 1] - x[i];
		tmp1 = (f_x[i + 1] - f_x[i]) / tmp2;
		c[j + 2] = (3 * tmp1 - 2 * d[i] - d[i + 1]) / tmp2;
		tmp2 *= tmp2;
		c[j + 3] = (d[i] + d[i + 1] - 2 * tmp1) / tmp2;
		j += 4;
	}
}

double Value_11(double t, int n, double *c, double *x)
{
	int i;

	for (i = 0; i < n - 2; i++) if (t <= x[i + 1]) break;

	return c[4 * i] + c[4 * i + 1] * (t - x[i]) +
		c[4 * i + 2] * (t - x[i]) * (t - x[i]) +
		c[4 * i + 3] * (t - x[i]) * (t - x[i]) * (t - x[i]);
}
