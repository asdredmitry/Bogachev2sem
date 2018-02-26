#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#include "help.h"

#define MAX_OUTPUT_SIZE 5

double f(int i, int j)
{
	return 1.0/(i + j + 1.0);
}

double *AllocVector(int size)
{
	return (double*)malloc(size * sizeof(double));
}

void InputMatrix(int n, double *a, int my_rank, int p)
{
	int i, j;
	int rows;

	if (my_rank + 1 > n%p) rows = n/p;
	else rows = n/p + 1;

	for (i = 0; i < rows; i++)
		for (j = 0; j < n; j++)
			a[i * n + j] = f(my_rank + p * i, j);
}

void OutputMatrix(int n, double *a, double *x, int my_rank, int p)
{
	int i, j, m;
	MPI_Status status;

	m = (n < MAX_OUTPUT_SIZE) ? n : MAX_OUTPUT_SIZE;

	for (i = 0; i < m; i++)
	{
		if (my_rank == 0)
		{
			if (my_rank == i%p)
			{
				printf("| ");
				for (j = 0; j < m; j++)
					printf("%10.3g ", a[i/p * n + j]);
				printf("|\n");
			}
			else
			{
				MPI_Recv(x, m, MPI_DOUBLE, i%p, 0, MPI_COMM_WORLD, &status);
				printf("| ");
				for (j = 0; j < m; j++)
					printf("%10.3g ", x[j]);
				printf("|\n");
			}
		}
		else if (my_rank == i%p)
		{
			for (j = 0; j < m; j++)
				x[j] = a[i/p * n + j];
			MPI_Send(x, m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		}
	}
}
