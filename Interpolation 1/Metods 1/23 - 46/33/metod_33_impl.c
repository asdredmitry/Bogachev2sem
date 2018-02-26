#include <math.h>
#include "main.h"
#include "metod_33.h"

void Coeff_33(int n, double *x, double *f_x, double *c, double *d)
{
	int i, j;
	double tmp1, tmp2;
	double f1, f2, f_1, f_2;

	for (i = 1; i < n - 1; i++)
	{
		tmp1 = (f_x[i] - f_x[i - 1]) / (x[i] - x[i - 1]);
		tmp2 = (f_x[i + 1] - f_x[i]) / (x[i + 1] - x[i]);
		if (tmp1 * tmp2 > 0) d[i] = ((tmp1 > 0) ? 1 : -1) * ((fabs(tmp1) > fabs(tmp2)) ? fabs(tmp2) : fabs(tmp1));
		else d[i] = 0.0;
	}

	tmp1 = x[0] - (x[1] - x[0]);
	f1 = f(tmp1);
	tmp2 = x[n - 1] + (x[n - 1] - x[n - 2]);
	f2 = f(tmp2);

	f_1 = (f_x[0] - f1) / (x[0] - tmp1);
	f_2 = (f_x[1] - f_x[0]) / (x[1] - x[0]);
	if (f_1 * f_2 > 0) d[0] = ((f_1 > 0) ? 1 : -1) * ((fabs(f_1) > fabs(f_2)) ? fabs(f_2) : fabs(f_1));
	else d[0] = 0.0;

	f_1 = (f_x[n - 1] - f_x[n - 2]) / (x[n - 1] - x[n - 2]);
	f_2 = (f2 - f_x[n - 1]) / (tmp2 - x[n - 1]);
	if (f_1 * f_2 > 0) d[n - 1] = ((f_1 > 0) ? 1 : -1) * ((fabs(f_1) > fabs(f_2)) ? fabs(f_2) : fabs(f_1));
	else d[n - 1] = 0.0;

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

double Value_33(double t, int n, double *c, double *x)
{
	int i;

	for (i = 0; i < n - 2; i++) if (t <= x[i + 1]) break;

	return c[4 * i] + c[4 * i + 1] * (t - x[i]) +
		c[4 * i + 2] * (t - x[i]) * (t - x[i]) +
		c[4 * i + 3] * (t - x[i]) * (t - x[i]) * (t - x[i]);
}
