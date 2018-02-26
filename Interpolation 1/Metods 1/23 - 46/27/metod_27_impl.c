#include <math.h>
#include "main.h"
#include "metod_27.h"

/* Some useful macros */
#define mItem(a, i, j) a[(i) * size + j]
#define vItem(a, i) a[i]

static int SolveSystem(int size, double *a, double *b, double *x)
{
	int i, j, k;
	double p;

	/* Gauss forward move */
	for (i = 0; i < size; i++)
	{
		if (fabs(p = mItem(a, i, i)) < 1e-100)
			return 0;
		
		for (k = i; k < size; k++)
			mItem(a, i, k) /= p;
		
		vItem(b, i) /= p;

		for (j = i + 1; j < size; j++)
		{
			p = mItem(a, j, i);
			for (k = i; k < size; k++)
				mItem(a, j, k) -= p * mItem(a, i, k);
			vItem(b, j) -= p * vItem(b, i);
		}
	}

	/* Gauss back move */
	for (i = size - 1; i >= 0; i--)
	{
		p = vItem(b, i);
		for (j = i + 1; j < size; j++)
			p -= vItem(x, j) * mItem(a, i, j);
		vItem(x, i) = p/mItem(a, i, i);
	}
	
	return 1;
}

void Coeff_27(int n, double *x, double *f_x, double *c, double *d, double *a1, double *b1)
{
	int i, j;
	double tmp1, tmp2, tmp3, tmp4;
	double f1, f2;

	for (i = 1; i < n - 1; i++)
	{
		tmp1 = (f_x[i + 1] - f_x[i]) / (x[i + 1] - x[i]);
		tmp2 = (f_x[i] - f_x[i - 1]) / (x[i] - x[i - 1]);
		c[i] = fabs(tmp1 - tmp2);
	}

	for (i = 2; i < n - 2; i++)
	{
		tmp1 = (f_x[i] - f_x[i - 1]) / (x[i] - x[i - 1]);
		tmp2 = (f_x[i + 1] - f_x[i]) / (x[i + 1] - x[i]);
		if (c[i - 1] * c[i - 1] + c[i + 1] * c[i + 1] < 1e-50)
		{
			d[i] = (x[i + 1] - x[i]) * tmp1 + (x[i] - x[i - 1]) * tmp2;
			d[i] /= x[i + 1] - x[i - 1];
		}
		else
		{
			d[i] = c[i + 1] * tmp1 + c[i - 1] * tmp2;
			d[i] /= c[i + 1] + c[i - 1];
		}
	}

	for (i = 0; i < 16; i++)
		a1[i] = 0.0;

	tmp1 = x[0] - (x[1] - x[0]);
	f1 = f(tmp1);

	a1[4 * 0 + 0] = 1.0;
	a1[4 * 0 + 1] = (tmp1 - x[1]);
	a1[4 * 0 + 2] = (tmp1 - x[1]) * (tmp1 - x[1]);
	a1[4 * 0 + 3] = (tmp1 - x[1]) * (tmp1 - x[1]) * (tmp1 - x[1]);

	a1[4 * 1 + 0] = 1.0;
	a1[4 * 1 + 1] = (x[0] - x[1]);
	a1[4 * 1 + 2] = (x[0] - x[1]) * (x[0] - x[1]);
	a1[4 * 1 + 3] = (x[0] - x[1]) * (x[0] - x[1]) * (x[0] - x[1]);

	a1[4 * 2 + 0] = 1.0;

	a1[4 * 3 + 1] = 1.0;

	tmp3 = (f_x[1] - f_x[0]) / (x[1] - x[0]);
	tmp4 = (f_x[0] - f1) / (x[0] - tmp1);
	c[0] = fabs(tmp3 - tmp4);

	tmp3 = (f_x[1] - f_x[0]) / (x[1] - x[0]);
	tmp4 = (f_x[2] - f_x[1]) / (x[2] - x[1]);
	if (c[0] * c[0] + c[2] * c[2] < 1e-50)
	{
		d[1] = (x[2] - x[1]) * tmp3 + (x[1] - x[0]) * tmp4;
		d[1] /= x[2] - x[0];
	}
	else
	{
		d[1] = c[2] * tmp3 + c[0] * tmp4;
		d[1] /= c[2] + c[0];
	}

	b1[0] = f1; b1[1] = f_x[0]; b1[2] = f_x[1]; b1[3] = d[1];

	SolveSystem(4, a1, b1, c);
/////////////////////////////////
	for (i = 0; i < 16; i++)
		a1[i] = 0.0;

	tmp2 = x[n - 1] + (x[n - 1] - x[n - 2]);
	f2 = f(tmp2);

	a1[4 * 0 + 0] = 1.0;

	a1[4 * 1 + 0] = 1.0;
	a1[4 * 1 + 1] = (x[n - 1] - x[n - 2]);
	a1[4 * 1 + 2] = (x[n - 1] - x[n - 2]) * (x[n - 1] - x[n - 2]);
	a1[4 * 1 + 3] = (x[n - 1] - x[n - 2]) * (x[n - 1] - x[n - 2]) * (x[n - 1] - x[n - 2]);

	a1[4 * 2 + 0] = 1.0;
	a1[4 * 2 + 1] = (tmp2 - x[n - 2]);
	a1[4 * 2 + 2] = (tmp2 - x[n - 2]) * (tmp2 - x[n - 2]);
	a1[4 * 2 + 3] = (tmp2 - x[n - 2]) * (tmp2 - x[n - 2]) * (tmp2 - x[n - 2]);

	a1[4 * 3 + 1] = 1.0;

	tmp3 = (f2 - f_x[n - 1]) / (tmp2 - x[n - 1]);
	tmp4 = (f_x[n - 1] - f_x[n - 2]) / (x[n - 1] - x[n - 2]);
	c[n - 1] = fabs(tmp3 - tmp4);

	tmp3 = (f_x[n - 2] - f_x[n - 3]) / (x[n - 2] - x[n - 3]);
	tmp4 = (f_x[n - 1] - f_x[n - 2]) / (x[n - 1] - x[n - 2]);
	if (c[n - 3] * c[n - 3] + c[n - 1] * c[n - 1] < 1e-50)
	{
		d[n - 2] = (x[n - 1] - x[n - 2]) * tmp3 + (x[n - 2] - x[n - 3]) * tmp4;
		d[n - 2] /= x[n - 1] - x[n - 3];
	}
	else
	{
		d[n - 2] = c[n - 1] * tmp3 + c[n - 3] * tmp4;
		d[n - 2] /= c[n - 1] + c[n - 3];
	}

	b1[0] = f_x[n - 2]; b1[1] = f_x[n - 1]; b1[2] = f2; b1[3] = d[n - 2];

	SolveSystem(4, a1, b1, c + 4 * (n - 2));

	j = 4;

	for (i = 1; i < n - 2; i ++)
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

double Value_27(double t, int n, double *c, double *x)
{
	int i;

	if (t <= x[1]) return c[0] + c[1] * (t - x[1]) + c[2] * (t - x[1]) * (t - x[1]) +
			c[3] * (t - x[1]) * (t - x[1]) * (t - x[1]);

	for (i = 1; i < n - 2; i++)
	{
		if (t <= x[i + 1]) return c[4 * i] + c[4 * i + 1] * (t - x[i]) +
					    c[4 * i + 2] * (t - x[i]) * (t - x[i]) +
					    c[4 * i + 3] * (t - x[i]) * (t - x[i]) * (t - x[i]);
	}

	return c[4 * (n - 2)] + c[4 * (n - 2) + 1] * (t - x[n - 2]) +
		c[4 * (n - 2) + 2] * (t - x[n - 2]) * (t - x[n - 2]) +
		c[4 * (n - 2) + 3] * (t - x[n - 2]) * (t - x[n - 2]) * (t - x[n - 2]);
}
