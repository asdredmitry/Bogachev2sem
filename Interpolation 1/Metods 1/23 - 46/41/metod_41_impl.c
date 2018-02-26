#include <math.h>
#include "main.h"
#include "metod_41.h"

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

void Coeff_41(int n, double a, double b, double *x, double *f_x, double *c, double *v, double *ksi, double *a1, double *c1, double *d1)
{
	int i, j;
	double tmp1;

	for (i = 1; i < n; i++)
		ksi[i] = 0.5 * (x[i - 1] + x[i]);

	ksi[0] = a - (ksi[2] - ksi[1]);
	ksi[n] = b + (ksi[n - 1] - ksi[n - 2]);

	for (i = 1; i < n; i++)
	{
		a1[i] = 1.0/(ksi[i] - x[i - 1]) + 1.0/(ksi[i] - ksi[i - 1]) + 1.0/(x[i] - ksi[i]) + 1.0/(ksi[i + 1] - ksi[i]);
		c1[i] = 1.0/(ksi[i + 1] - x[i]) - 1.0/(ksi[i + 1] - ksi[i]);
		d1[i - 1] = 1.0/(x[i - 1] - ksi[i - 1]) - 1.0/(ksi[i] - ksi[i - 1]);
		c[i] = f_x[i - 1] * (1.0/(x[i - 1] - ksi[i - 1]) + 1.0/(ksi[i] - x[i - 1])) + 
			f_x[i] * (1.0/(x[i] - ksi[i]) + 1.0/(ksi[i + 1] - x[i]));
	}

	a1[0] = 1.0/(ksi[1] - ksi[0]) - 1.0/(x[0] - ksi[0]);
	c1[0] = 1.0/(ksi[1] - x[0]) - 1.0/(ksi[1] - ksi[0]);
	a1[n] = 1.0/(ksi[n] - x[n - 1]) - 1.0/(ksi[n] - ksi[n - 1]);
	d1[n - 1] = 1.0/(ksi[n] - ksi[n - 1]) - 1.0/(x[n - 1] - ksi[n - 1]);

	c[0] = f_first(x[0]) - f_x[0] * (1.0/(x[0] - ksi[0]) - 1.0/(ksi[1] - x[0]));
	c[n] = f_first(x[n - 1]) - f_x[n - 1] * (1.0/(x[n - 1] - ksi[n - 1]) - 1.0/(ksi[n] - x[n - 1]));

	SolveSystem(n + 1, a1, c1, d1, c, v);

	j = 0;

	for (i = 0; i < n; i ++)
	{
		c[j + 0] = v[i];
		tmp1 = ((v[i + 1] - f_x[i]) / (ksi[i + 1] - x[i]) - (f_x[i] - v[i]) / (x[i] - ksi[i])) / (ksi[i + 1] - ksi[i]);
		c[j + 1] = (f_x[i] - v[i]) / (x[i] - ksi[i]) - (x[i] - ksi[i]) * tmp1;
		c[j + 2] = tmp1;
		j += 3;
	}
}

double Value_41(double t, int n, double *c, double *ksi)
{
	int i;

	for (i = 0; i < n - 1; i++) if (t <= ksi[i + 1]) break;

	return c[3 * i] + c[3 * i + 1] * (t - ksi[i]) + c[3 * i + 2] * (t - ksi[i]) * (t - ksi[i]);
}
