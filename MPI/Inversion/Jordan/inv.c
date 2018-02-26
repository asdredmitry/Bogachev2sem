#include <math.h>
#include <mpi.h>

#include "inv.h"

void InvMatrix(int n, double *a, double *b, double *x, int my_rank, int p)
{
	int i, j, k;
	int rows;
	int first;
	double tmp;

	if (my_rank + 1 > n%p) rows = n/p;
	else rows = n/p + 1;

	for (i = 0; i < rows; i++)
		for (j = 0; j < n; j++)
			b[i * n + j] = (my_rank + p * i == j);

	for (i = 0; i < n; i++)
	{
		if (my_rank == i%p)
		{
			tmp = 1.0/a[i/p * n + i];
			for (j = i; j < n; j++)
				a[i/p * n + j] *= tmp;
			for (j = 0; j < n; j++)
				b[i/p * n + j] *= tmp;

			for (j = i; j < n; j++)
				x[j] = a[i/p * n + j];
			for (j = 0; j < n; j++)
				x[j + n] = b[i/p * n + j];

			MPI_Bcast(x, 2 * n, MPI_DOUBLE, i%p, MPI_COMM_WORLD);
			for (j = i/p + 1; j < rows; j++)
			{
				tmp = a[j * n + i];
				for (k = i; k < n; k++)
					a[j * n + k] -= tmp * a[i/p * n + k];
				for (k = 0; k < n; k++)
					b[j * n + k] -= tmp * b[i/p * n + k];
			}
			for (j = 0; j < i/p; j++)
			{
				tmp = a[j * n + i];
				for (k = i; k < n; k++)
					a[j * n + k] -= tmp * a[i/p * n + k];
				for (k = 0; k < n; k++)
					b[j * n + k] -= tmp * b[i/p * n + k];
			}
		}
		else
		{
			if (my_rank > i%p) first = i/p;
			else first = i/p + 1;

			MPI_Bcast(x, 2 * n, MPI_DOUBLE, i%p, MPI_COMM_WORLD);
			for (j = 0; j < rows; j++)
			{
				tmp = a[j * n + i];
				for (k = i; k < n; k++)
					a[j * n + k] -= tmp * x[k];
				for (k = 0; k < n; k++)
					b[j * n + k] -= tmp * x[k + n];
			}
		}
	}
}
