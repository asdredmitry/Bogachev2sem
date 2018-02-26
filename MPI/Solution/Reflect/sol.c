#include <math.h>
#include <mpi.h>

#include "sol.h"

void SolveSystem(int n, double *a, double *b, double *x1, double *x2, int my_rank, int p)
{
	int i, j, k;
	int rows;
	int first;
	double tmp;
	double q1, q2;
	double inv_norma;

	if (my_rank + 1 > n%p) rows = n/p;
	else rows = n/p + 1;

	for (i = 0; i < n; i++)
	{
		if (my_rank == i%p)
		{
			if (i == n - 1)
			{
				tmp = 1.0/a[i/p * n + i];
				a[i/p * n + i] = 1.0;
				b[i/p] *= tmp;
				continue;
			}

			for (q1 = 0.0, j = i/p + 1; j < rows; j++)
				q1 += a[j * n + i] * a[j * n + i];

			MPI_Reduce(&q1, &q2, 1, MPI_DOUBLE, MPI_SUM, i%p, MPI_COMM_WORLD);

			tmp = sqrt(q2 + a[i/p * n + i] * a[i/p * n + i]);
			a[i/p * n + i] -= tmp;
			inv_norma = 1.0/sqrt(q2 + a[i/p * n + i] * a[i/p * n + i]);
			MPI_Bcast(&inv_norma, 1, MPI_DOUBLE, i%p, MPI_COMM_WORLD);

			for (j = i/p; j < rows; j++)
				a[j * n + i] *= inv_norma;

			for (k = i + 1; k < n; k++)
			{
				for (q1 = 0.0, j = i/p; j < rows; j++)
					q1 += a[j * n + i] * a[j * n + k];
				x1[k] = q1;
			}

			MPI_Allreduce(x1, x2, n, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

			for (k = i + 1; k < n; k++)
			{
				x2[k] *= 2.0;
				for (j = i/p; j < rows; j++)
					a[j * n + k] -= x2[k] * a[j * n + i];
			}

			for (q1 = 0.0, j = i/p; j < rows; j++)
				q1 += a[j * n + i] * b[j];

			MPI_Allreduce(&q1, &q2, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
			q2 *= 2.0;

			for (j = i/p; j < rows; j++)
				b[j] -= q2 * a[j * n + i];

			a[i/p * n + i] = tmp;
//			for (j = i/p + 1; j < rows; j++)
//				a[j * n + i] = 0.0;

			tmp = 1.0/a[i/p * n + i];
			for (j = i; j < n; j++)
				a[i/p * n + j] *= tmp;
			b[i/p] *= tmp;
		}
		else
		{
			if (i == n - 1) continue;

			if (my_rank > i%p) first = i/p;
			else first = i/p + 1;

			for (q1 = 0.0, j = first; j < rows; j++)
				q1 += a[j * n + i] * a[j * n + i];

			MPI_Reduce(&q1, &q2, 1, MPI_DOUBLE, MPI_SUM, i%p, MPI_COMM_WORLD);

			MPI_Bcast(&inv_norma, 1, MPI_DOUBLE, i%p, MPI_COMM_WORLD);

			for (j = first; j < rows; j++)
				a[j * n + i] *= inv_norma;

			for (k = i + 1; k < n; k++)
			{
				for (q1 = 0.0, j = first; j < rows; j++)
					q1 += a[j * n + i] * a[j * n + k];
				x1[k] = q1;
			}

			MPI_Allreduce(x1, x2, n, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

			for (k = i + 1; k < n; k++)
			{
				x2[k] *= 2.0;
				for (j = first; j < rows; j++)
					a[j * n + k] -= x2[k] * a[j * n + i];
			}

			for (q1 = 0.0, j = first; j < rows; j++)
				q1 += a[j * n + i] * b[j];

			MPI_Allreduce(&q1, &q2, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
			q2 *= 2.0;

			for (j = first; j < rows; j++)
				b[j] -= q2 * a[j * n + i];

//			for (j = first; j < rows; j++)
//				a[j * n + i] = 0.0;
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
			MPI_Bcast(x2, 1, MPI_DOUBLE, i%p, MPI_COMM_WORLD);

			if (my_rank < i%p) first = i/p;
			else if (i/p - 1 >= 0) first = i/p - 1;
			else continue;

			for (j = first; j >= 0; j--)
				b[j] -= x2[0] * a[j * n + i];
		}
	}
}
