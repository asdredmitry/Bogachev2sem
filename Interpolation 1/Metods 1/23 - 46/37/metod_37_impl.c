#include <math.h>
#include "main.h"
#include "metod_37.h"

static void SolveSystem(int n, double *a, double *c, double *d, double *b, double *x)
{
	int i;

	c[0] /= a[0];
	for (i = 1; i < n - 1; i++)
	{
		a[i] -= d[i - 1] * c[i - 1];
		c[i] /= a[i];
	}
	a[n - 1] -= d[n - 2] * c[n - 2];

	x[0] = b[0] / a[0];
	for (i = 1; i < n; i++)
		x[i] = (b[i] - d[i - 1] * x[i - 1]) / a[i];

	for (i = n - 2; i >= 0; i--)
		x[i] -= c[i] * x[i + 1];
}

void Coeff_37(int n, double *x, double *f_x, double *c, double *d, double *a1, double *c1, double *d1)
{
	int i, j;
	double tmp1, tmp2;

	for (i = 1; i < n - 1; i++)
	{
		tmp1 = (f_x[i] - f_x[i - 1]) / (x[i] - x[i - 1]);
		tmp2 = (f_x[i + 1] - f_x[i]) / (x[i + 1] - x[i]);
		c[i] = 3 * tmp1 * (x[i + 1] - x[i]) + 3 * tmp2 * (x[i] - x[i - 1]);
		a1[i] = 2.0 * (x[i + 1] - x[i - 1]);
		c1[i] = x[i] - x[i - 1];
		d1[i - 1] = x[i + 1] - x[i];
	}

	a1[0] = 2.0;
	c1[0] = 1.0;

	a1[n - 1] = 2.0;
	d1[n - 2] = 1.0;

	tmp1 = (f_x[1] - f_x[0]) / (x[1] - x[0]);
	tmp2 = (f_x[n - 1] - f_x[n - 2]) / (x[n - 1] - x[n - 2]);

	c[0] = 3.0 * tmp1;
	c[n - 1] = 3.0 * tmp2;

	SolveSystem(n, a1, c1, d1, c, d);

	j = 0;

	for (i = 0; i < n - 1; i ++)
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

double Value_37(double t, int n, double *c, double *x)
{
	int i;

	for (i = 0; i < n - 2; i++) if (t <= x[i + 1]) break;

	return c[4 * i] + c[4 * i + 1] * (t - x[i]) +
		c[4 * i + 2] * (t - x[i]) * (t - x[i]) +
		c[4 * i + 3] * (t - x[i]) * (t - x[i]) * (t - x[i]);
}
