#include <math.h>
#include <mpi.h>

#include "sol.h"

void SolveSystem(int n, double *a, double *b, double *x, int my_rank, int p)
{
	int i, j, k;
	int rows;
	int first;
	double tmp;

	if (my_rank + 1 > n%p) rows = n/p;
	else rows = n/p + 1;

	for (i = 0; i < n; i++)
	{
		if (my_rank == i%p)
		{
			tmp = 1.0/a[i/p * n + i];
			for (j = i; j < n; j++)
				a[i/p * n + j] *= tmp;
			b[i/p] *= tmp;

			if (i == n - 1) continue;

			for (j = i; j < n; j++)
				x[j] = a[i/p * n + j];
			x[n] = b[i/p];

			MPI_Bcast(x, n + 1, MPI_DOUBLE, i%p, MPI_COMM_WORLD);
			for (j = i/p + 1; j < rows; j++)
			{
				tmp = a[j * n + i];
				for (k = i; k < n; k++)
					a[j * n + k] -= tmp * a[i/p * n + k];
				b[j] -= tmp * b[i/p];
			}
		}
		else
		{
			if (i == n - 1) continue;

			if (my_rank > i%p) first = i/p;
			else first = i/p + 1;

			MPI_Bcast(x, n + 1, MPI_DOUBLE, i%p, MPI_COMM_WORLD);
			for (j = first; j < rows; j++)
			{
				tmp = a[j * n + i];
				for (k = i; k < n; k++)
					a[j * n + k] -= tmp * x[k];
				b[j] -= tmp * x[n];
			}
		}
	}

	for (i = n - 1; i >= 1; i--)
	{
		if (my_rank == i%p)
		{
			MPI_Bcast(&b[i/p], 1, MPI_DOUBLE, i%p, MPI_COMM_WORLD);
			for (j = i/p - 1; j >= 0 ; j--)
				b[j] -= b[i/p] * a[j * n + i];
		}
		else
		{
			MPI_Bcast(x, 1, MPI_DOUBLE, i%p, MPI_COMM_WORLD);

			if (my_rank < i%p) first = i/p;
			else if (i/p - 1 >= 0) first = i/p - 1;
			else continue;

			for (j = first; j >= 0; j--)
				b[j] -= x[0] * a[j * n + i];
		}
	}
}
