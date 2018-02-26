#include <math.h>
#include "main.h"
#include "metod_07.h"

void Coeff_07(int n, int m, double *f_ij, double *coef)
{
	int i;

	for (i = 0; i < n * m; i++)
		coef[i] = f_ij[i];
}

double Value_07(double x, double y, int n, int m, double *coef, double *xarr, double *yarr)
{
	int i, j;
	double f_xy;

	for (i = 0; i < n - 2; i++) if (x <= xarr[i + 1]) break;

	for (j = 0; j < m - 2; j++) if (y <= yarr[j + 1]) break;

	f_xy = coef[i * m + j] * (x - xarr[i + 1]) * (y - yarr[j + 1])/((xarr[i] - xarr[i + 1]) * (yarr[j] - yarr[j + 1]))
	    + coef[i * m + j + 1] * (x - xarr[i + 1]) * (y - yarr[j])/((xarr[i] - xarr[i + 1]) * (yarr[j + 1] - yarr[j]))
	    + coef[(i + 1) * m + j] * (x - xarr[i]) * (y - yarr[j + 1])/((xarr[i + 1] - xarr[i]) * (yarr[j] - yarr[j + 1]))
	    + coef[(i + 1) * m + j + 1] * (x - xarr[i]) * (y - yarr[j])/((xarr[i + 1] - xarr[i]) * (yarr[j + 1] - yarr[j]));

	return f_xy;
}
