#include <math.h>
#include "main.h"
#include "metod_02.h"

void Coeff_02(int n, double *x, double *f_x, double *c, int k)
{
	int i, j;

	for (i = 0; i < n - k; i++)
		c[i * (k + 1)] = f_x[i];

	for (i = 0; i < k + 1; i++)
		c[(n - k - 1) * (k + 1) + i] = f_x[i + n - k - 1];

	for (i = 1; i < k + 1; i++)
		for (j = k; j > i - 1; j--)
			c[(n - k - 1) * (k + 1) + j] = 
			(c[(n - k - 1) * (k + 1) + j] - c[(n - k - 1) * (k + 1) + j - 1]) / (x[n - k + j - 1] - x[n - k - 1 + j - i]);

	for (i = 1; i < k + 1; i++)
		for (j = 0; j < n - k - 1; j++)
			c[j * (k + 1) + i] = (c[(j + 1) * (k + 1) + i - 1] - c[j * (k + 1) + i - 1]) / (x[j + i] - x[j]);
}

double Value_02(double t, int n, double *c, double *x, int k)
{
	int i, j;
	double fx;

	for (i = 0; i < n - 2; i++)
		if (t <= x[i + 1]) break;

	if (i > n - k - 1) i = n - k - 1;

	fx = c[i * (k + 1) + k];

	for (j = k - 1; j >= 0; j--)
	{
		fx *= (t - x[i + j]);
		fx += c[i * (k + 1) + j];
	}

	return fx;
}
