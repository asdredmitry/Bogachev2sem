#include <math.h>
#include "main.h"
#include "metod_08.h"

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

static double CalcFirstRight(int N, double *x, double *y, double *f_y)
{
	int i;
	int p1;
	double res;
	double tmp;

	p1 = 0;
	for (i = 1; i < N; i++)
		if (y[i] >= x[1])
		{
			p1 = i - 1;
			break;
		}

	tmp = (f_y[p1 + 1] - f_y[p1])/(y[p1 + 1] - y[p1]);
	res = tmp * (x[1] - y[p1 + 1]) + f_y[p1 + 1];
	res *= -x[1] * x[1]/2.0;

	res -= f_y[0] * x[0] * (x[0]/2.0 - x[1]);

	for (i = 0; i < p1; i++)
	{
		tmp = (f_y[i + 1] - f_y[i])/(y[i + 1] - y[i]);
		tmp *= (y[i] * y[i] * (y[i]/3.0 - x[1]) - y[i + 1] * y[i + 1] * (y[i + 1]/3.0 - x[1]))/2.0;
		res += tmp;
	}

	tmp = (f_y[p1 + 1] - f_y[p1])/(y[p1 + 1] - y[p1]);
	tmp *= (2.0 * x[1] * x[1] * x[1]/3.0 + y[p1] * y[p1] * (y[p1]/3.0 - x[1]))/2.0;
	res += tmp;

	res /= (x[0] - x[1]);

	return res;
}


static double CalcRight(int N, int i, double *x, double *y, double *f_y)
{
	int j, k;
	int pi;
	int qi;
	double res1;
	double res2;
	double tmp;

	res1 = 0.0;
	res2 = 0.0;

	k = 0;
	for (j = 1; j < N; j++)
		if (y[j] > x[i - 1])
		{
			k = j - 1;
			break;
		}

	pi = 0;
	for (j = 0; j < N; j++)
		if (y[j] > x[i])
		{
			pi = j - 1;
			break;
		}

	qi = N - 2;
	for (j = pi; j < N; j++)
		if (y[j] >= x[i + 1])
		{
			qi = j - 1;
			break;
		}

	tmp = (f_y[pi + 1] - f_y[pi])/(y[pi + 1] - y[pi]);
	res1 = tmp * (x[i] - y[pi + 1]) + f_y[pi + 1];
	res1 *= x[i] * (x[i]/2.0 - x[i - 1]);

	tmp = (f_y[k + 1] - f_y[k])/(y[k + 1] - y[k]);
	res1 += x[i - 1] * x[i - 1] * (tmp * (x[i - 1] - y[k + 1]) + f_y[k + 1])/2.0;

	tmp = (f_y[k + 1] - f_y[k])/(y[k + 1] - y[k]);
	res1 -= tmp * (y[k + 1] * y[k + 1] * (y[k + 1]/3.0 - x[i - 1]) + 2.0 * x[i - 1] * x[i - 1] * x[i - 1]/3.0)/2.0;

	for (j = k + 1; j < pi; j++)
	{
		tmp = (f_y[j + 1] - f_y[j])/(y[j + 1] - y[j]);
		res1 += tmp * (y[j] * y[j] * (y[j]/3.0 - x[i - 1]) - y[j + 1] * y[j + 1] * (y[j + 1]/3.0 - x[i - 1]))/2.0;
	}

	tmp = (f_y[pi + 1] - f_y[pi])/(y[pi + 1] - y[pi]);
	res1 += tmp * (y[pi] * y[pi] * (y[pi]/3.0 - x[i - 1]) - x[i] * x[i] * (x[i]/3.0 - x[i - 1]))/2.0;
	res1 /= x[i] - x[i - 1];


	tmp = (f_y[qi + 1] - f_y[qi])/(y[qi + 1] - y[qi]);
	res2 = tmp * (x[i + 1] - y[qi + 1]) + f_y[qi + 1];
	res2 *= -x[i + 1] * x[i + 1]/2.0;

	tmp = (f_y[pi + 1] - f_y[pi])/(y[pi + 1] - y[pi]);
	tmp = tmp * (x[i] - y[pi + 1]) + f_y[pi + 1];
	res2 -= tmp * x[i] * (x[i]/2.0 - x[i + 1]);

	tmp = (f_y[pi + 1] - f_y[pi])/(y[pi + 1] - y[pi]);
	res2 += tmp * (x[i] * x[i] * (x[i]/3.0 - x[i + 1]) - y[pi + 1] * y[pi + 1] * (y[pi + 1]/3.0 - x[i + 1]))/2.0;

	for (j = pi + 1; j < qi; j++)
	{
		tmp = (f_y[j + 1] - f_y[j])/(y[j + 1] - y[j]);
		res2 += tmp * (y[j] * y[j] * (y[j]/3.0 - x[i + 1]) - y[j + 1] * y[j + 1] * (y[j + 1]/3.0 - x[i + 1]))/2.0;
	}

	tmp = (f_y[qi + 1] - f_y[qi])/(y[qi + 1] - y[qi]);
	res2 += tmp * (2.0 * x[i + 1] * x[i + 1] * x[i + 1]/3.0 + y[qi] * y[qi] * (y[qi]/3.0 - x[i + 1]))/2.0;

	res2 /= x[i] - x[i + 1];

	return res1 + res2;
}

static double CalcLastRight(int n, int N, double *x, double *y, double *f_y)
{
	int i;
	int qn;
	double res;
	double tmp;

	qn = N;
	for (i = N - 2; i >= 0; i--)
		if (y[i] <= x[n - 2])
		{
			qn = i;
			break;
		}

	res = x[n - 1] * (x[n - 1]/2.0 - x[n - 2]) * f_y[N - 1];
	tmp = (f_y[qn + 1] - f_y[qn])/(y[qn + 1] - y[qn]);
	tmp = tmp * (x[n - 2] - y[qn + 1]) + f_y[qn + 1];
	res += tmp * x[n - 2] * x[n - 2]/2.0;

	tmp = (f_y[qn + 1] - f_y[qn])/(y[qn + 1] - y[qn]);
	tmp *= (y[qn + 1] * y[qn + 1] * (x[n - 2] - y[qn + 1]/3.0) - 2.0 * x[n - 2] * x[n - 2] * x[n - 2]/3.0)/2.0;
	res += tmp;

	for (i = qn + 1; i < N - 1; i++)
	{
		tmp = (f_y[i + 1] - f_y[i])/(y[i + 1] - y[i]);
		tmp *= (y[i] * y[i] * (y[i]/3.0 - x[n - 2]) - y[i + 1] * y[i + 1] * (y[i + 1]/3.0 - x[n - 2]))/2.0;
		res += tmp;
	}

	res /= (x[n - 1] - x[n - 2]);

	return res;
}

void Coeff_08(int n, int N, double *x, double *y, double *f_y, double *c, double *a1, double *c1, double *d1, double *e1)
{
	int i;

	for (i = 1; i < n - 1; i++)
	{
		a1[i] = (x[i + 1] - x[i - 1])/3.0;
		c1[i] = (x[i + 1] - x[i])/6.0;
		d1[i - 1] = (x[i] - x[i - 1])/6.0;
		e1[i] = CalcRight(N, i, x, y, f_y);
	}

	c1[0] = (x[1] - x[0])/6.0;
	a1[0] = (x[1] - x[0])/3.0;
	d1[n - 2] = (x[n - 1] - x[n - 2])/6.0;
	a1[n - 1] = (x[n - 1] - x[n - 2])/3.0;

	e1[0] = CalcFirstRight(N, x, y, f_y);
	e1[n - 1] = CalcLastRight(n, N, x, y, f_y);

	SolveSystem(n, a1, c1, d1, e1, c);
}

double Value_08(double t, int n, double *c, double *x)
{
	int i;

	if (t < x[1]) return c[0] * (t - x[1])/(x[0] - x[1]) + c[1] * (t - x[0])/(x[1] - x[0]);

	for (i = 1; i < n - 2; i++)
		if (t <= x[i + 1]) return c[i] * (t - x[i + 1])/(x[i] - x[i + 1]) + c[i + 1] * (t - x[i])/(x[i + 1] - x[i]);

	return c[n - 2] * (t - x[n - 1])/(x[n - 2] - x[n - 1]) + c[n - 1] * (t - x[n - 2])/(x[n - 1] - x[n - 2]);
}
