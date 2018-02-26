#include <math.h>
#include "main.h"
#include "metod_09.h"

void Coeff_09(int n, int N, double *x, double *f_x, double *c)
{
	int i, j;
	double tmp;
	double first_t11, first_t21, first_t31;
	double first_t12, first_t22, first_t32;
	double second_t11, second_t21, second_t31;
	double second_t12, second_t22, second_t32;
	double a1, b1;

	for (i = 0; i < n; i++)
		c[i] = 0.0;

	for (j = 0; j < N + 1; j++)
	{
		if (j != 0)
		{
			first_t11 = 1.0;
			first_t21 = 2.0 * x[j];
			first_t12 = 1.0;
			first_t22 = 2.0 * x[j - 1];

			a1 = -acos(x[j]) + acos(x[j - 1]);
			b1 = -sqrt(1.0 - x[j] * x[j]) + sqrt(1.0 - x[j - 1] * x[j - 1]);

			c[0] += f_x[j] * (b1 - a1 * x[j - 1])/(x[j] - x[j - 1]);

			a1 = -sqrt(1.0 - x[j] * x[j]) + sqrt(1.0 - x[j - 1] * x[j - 1]);
			b1 = -0.5 * (acos(x[j]) + 0.5 * sqrt(1.0 - x[j] * x[j]) * first_t21) +
				0.5 * (acos(x[j - 1]) + 0.5 * sqrt(1.0 - x[j - 1] * x[j - 1]) * first_t22);

			c[1] += f_x[j] * (b1 - a1 * x[j - 1])/(x[j] - x[j - 1]);
		}

		if (j != N)
		{
			second_t11 = 1.0;
			second_t21 = 2.0 * x[j + 1];
			second_t12 = 1.0;
			second_t22 = 2.0 * x[j];

			a1 = -acos(x[j + 1]) + acos(x[j]);
			b1 = -sqrt(1.0 - x[j + 1] * x[j + 1]) + sqrt(1.0 - x[j] * x[j]);

			c[0] += f_x[j] * (a1 * x[j + 1] - b1)/(x[j + 1] - x[j]);

			a1 = -sqrt(1.0 - x[j + 1] * x[j + 1]) + sqrt(1.0 - x[j] * x[j]);
			b1 = -0.5 * (acos(x[j + 1]) + 0.5 * sqrt(1.0 - x[j + 1] * x[j + 1]) * second_t21) +
				0.5 * (acos(x[j]) + 0.5 * sqrt(1.0 - x[j] * x[j]) * second_t22);

			c[1] += f_x[j] * (a1 * x[j + 1] - b1)/(x[j + 1] - x[j]);
		}

		for (i = 2; i < n; i++)
		{
			if (j != 0)
			{
				first_t31 = 2.0 * x[j] * first_t21 - first_t11;
				first_t32 = 2.0 * x[j - 1] * first_t22 - first_t12;

				a1 = -sqrt(1.0 - x[j] * x[j]) * first_t21;
				a1 += sqrt(1.0 - x[j - 1] * x[j - 1]) * first_t22;
				a1 /= i;

				b1 = -sqrt(1.0 - x[j] * x[j]) * (first_t11/(i - 1) + first_t31/(i + 1));
				b1 += sqrt(1.0 - x[j - 1] * x[j - 1]) * (first_t12/(i - 1) + first_t32/(i + 1));
				b1 *= 0.5;

				c[i] += f_x[j] * (b1 - a1 * x[j - 1])/(x[j] - x[j - 1]);

				first_t11 = first_t21;
				first_t21 = first_t31;
				first_t12 = first_t22;
				first_t22 = first_t32;
			}

			if (j != N)
			{
				second_t31 = 2.0 * x[j + 1] * second_t21 - second_t11;
				second_t32 = 2.0 * x[j] * second_t22 - second_t12;

				a1 = -sqrt(1.0 - x[j + 1] * x[j + 1]) * second_t21;
				a1 += sqrt(1.0 - x[j] * x[j]) * second_t22;
				a1 /= i;

				b1 = -sqrt(1.0 - x[j + 1] * x[j + 1]) * (second_t11/(i - 1) + second_t31/(i + 1));
				b1 += sqrt(1.0 - x[j] * x[j]) * (second_t12/(i - 1) + second_t32/(i + 1));
				b1 *= 0.5;

				c[i] += f_x[j] * (a1 * x[j + 1] - b1)/(x[j + 1] - x[j]);

				second_t11 = second_t21;
				second_t21 = second_t31;
				second_t12 = second_t22;
				second_t22 = second_t32;
			}
		}
	}

	c[0] /= 3.1415926;
	tmp = 2.0/3.1415926;
	for (i = 1; i < n; i++)
		c[i] *= tmp;
}

double Value_09(double x, double a, double b, int n, double *c)
{
	double z, fx;
	double a1, b1, c1;
	int i;

	z = 2.0 * (2.0 * x - (b + a))/(b - a);
	a1 = 1.0;
	b1 = z * 0.5;
	fx = c[0] * a1 + c[1] * b1;
	
	for (i = 2; i < n; i++)
	{
		c1 = z * b1 - a1;
		fx += c[i] * c1;
		a1 = b1;
		b1 = c1;
	}

	return fx;
}
