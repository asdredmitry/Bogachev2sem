#include <math.h>
#include "main.h"
#include "metod_05.h"

/* Some useful macros */
#define mItem(a, i, j) a[(i) * size + j]

static int InvMatrix(int size, double *a, double *x)
{
	int i, j, k;
	double p, q;

	/* Generating R, Q */
	for (i = 0; i< size; i++)
		for (j = 0; j < size; j++)
			mItem(x, i, j) = (i == j);

	for (i = 0; i < size; i++)
	{
		for (p = 0, j = i + 1; j < size; j++)
			p += mItem(a, j, i) * mItem(a, j, i);

		q = sqrt(p + mItem(a, i, i) * mItem(a, i, i));

		if (q < 1e-100)
			return 0;

		mItem(a, i, i) -= q;

		p = sqrt(mItem(a, i, i) * mItem(a, i, i) + p);

		if (p < 1e-100)
		{
			mItem(a, i, i) += q;
			continue;
		}

		for (j = i; j < size; j++)
			mItem(a, j, i) /= p;

		for (k = i + 1; k < size; k++)
		{
			for (p = 0, j = i; j < size; j++)
				p += mItem(a, j, i) * mItem(a, j, k);

			p *= 2;

			for (j = i; j < size; j++)
				mItem(a, j, k) -= p * mItem(a, j, i);
		}

		for (k = 0; k < size; k++)
		{
			for (p = 0, j = i; j < size; j++)
				p += mItem(a, j, i) * mItem(x, k, j);

			p *= 2;

			for (j = i; j < size; j++)
				mItem(x, k, j) -= p * mItem(a, j, i);
		}

		mItem(a, i, i) = q;
	}

	/* Generating Q^{-1} = Q^{t} */
	for (i = 0; i < size; i++)
		for (j = i + 1; j < size; j++)
		{
			p = mItem(x, i, j);
			mItem(x, i, j) = mItem(x, j, i);
			mItem(x, j, i) = p;
		}

	/* Solving RX = Q^{-1} */
	for (k = 0; k < size; k++)
		for (i = size - 1; i >= 0; i--)
		{
			p = mItem(x, i, k);
			for (j = i + 1; j < size; j++)
				p -= mItem(x, j, k) * mItem(a, i, j);
			mItem(x, i, k) = p/mItem(a, i, i);
		}

	return 1;
}

void Coeff_05(int n, double a, double b, double *x, double *f_x, double *alpha, double *a1, double *b1, double *c1)
{
	int i, j, k;
	double tmp, tmp2;

	for (i = 0; i < n; i++)
		for (j = i; j < n; j++)
		{
			for (tmp = 0.0, k = 0; k < n; k++)
			{
				tmp2 = (2.0 * x[k] - (b + a))/(b - a);
				tmp += cos(i * acos(tmp2)) * cos(j * acos(tmp2));
			}
			a1[i * n + j] = a1[j * n + i] = tmp;
		}

	InvMatrix(n, a1, b1);

	for (i = 0; i < n; i++)
	{
		for (tmp = 0.0, j = 0; j < n; j++)
		{
			tmp2 = (2.0 * x[j] - (b + a))/(b - a);
			tmp += cos(i * acos(tmp2)) * f_x[j];
		}
		c1[i] = tmp;
	}

	for (i = 0; i < n; i++)
	{
		for (tmp = 0.0, j = 0; j < n; j++)
			tmp += b1[i * n + j] * c1[j];
		alpha[i] = tmp;
	}
}

double Value_05(double x, double a, double b, int n, double *alpha)
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
