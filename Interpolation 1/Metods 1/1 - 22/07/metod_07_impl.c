#include <math.h>
#include "main.h"
#include "metod_07.h"

void Coeff_07(int n, double *x, double *f_x, double *c)
{
	int i;

	for (i = 0; i < n - 1; i++)
		c[i] = (f_x[i + 1] - f_x[i]) / (x[i + 1] - x[i]);
}

double Value_07(double t, int n, double *c, double *x, double *f_x)
{
	int i;

	for (i = 0; i < n - 2; i++)
	{
		if (t <= x[i + 1]) return f_x[i] + (t - x[i]) * c[i];
	}

	return f_x[n - 2] + (t - x[n - 2]) * c[n - 2];
}
