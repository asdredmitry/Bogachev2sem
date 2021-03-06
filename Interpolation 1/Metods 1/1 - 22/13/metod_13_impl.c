#include <math.h>
#include "main.h"
#include "metod_13.h"

void Coeff_13(int n, double *x, double *f_x, double *c, double *d)
{
	int i, j;
	double tmp1, tmp2;

	for (i = 1; i < n - 1; i++)
		d[i] = f_first(x[i]);

	tmp1 = (f_x[1] - f_x[0]) / (x[1] - x[0]);
	tmp2 = (f_x[n - 1] - f_x[n - 2]) / (x[n - 1] - x[n - 2]);

	d[0] = 0.5 * (3 * tmp1 - d[1]);
	d[n - 1] = 0.5 * (3 * tmp2 - d[n - 2]);

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

double Value_13(double t, int n, double *c, double *x)
{
	int i;

	for (i = 0; i < n - 2; i++) if (t <= x[i + 1]) break;

	return c[4 * i] + c[4 * i + 1] * (t - x[i]) +
		c[4 * i + 2] * (t - x[i]) * (t - x[i]) +
		c[4 * i + 3] * (t - x[i]) * (t - x[i]) * (t - x[i]);
}
