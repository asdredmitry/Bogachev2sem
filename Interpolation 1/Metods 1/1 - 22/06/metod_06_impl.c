#include <math.h>
#include "main.h"
#include "metod_06.h"

void Coeff_06(int n, double *f_x, double *alpha)
{
	int i, j;
	double z_j, a1, b1, c1;

	for (i = 0; i < n; i++)
		alpha[i] = 0.0;

	for (j = 0; j < n; j++)
	{
		z_j = 2.0 * cos(3.1415926 * (2 * j + 1) * 0.5 / n );
		a1 = f_x[j];
		b1 = 0.5 * z_j * a1;
		alpha[0] += a1;
		alpha[1] += b1;
		
		for (i = 2; i < n; i++)
		{
			c1 = z_j * b1 - a1;
			alpha[i] += c1;
			a1 = b1;
			b1 = c1;
		}
	}

	alpha[0] /= n;
	a1 = 2.0 / n;
	for (i = 1; i < n; i++)
		alpha[i] *= a1;
}

double Value_06(double x, double a, double b, int n, double *alpha)
{
	double z, fx;
	double a1, b1, c1;
	int i;

	z = 2.0 * (2.0 * x - (b + a)) / (b - a);
	a1 = 1.0;
	b1 = z * 0.5;
	fx = alpha[0] * a1 + alpha[1] * b1;
	
	for (i = 2; i < n; i++)
	{
		c1 = z * b1 - a1;
		fx += alpha[i] * c1;
		a1 = b1;
		b1 = c1;
	}

	return fx;
}
