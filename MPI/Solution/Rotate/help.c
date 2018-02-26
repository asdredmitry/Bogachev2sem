#include "help.h"

#define MAX_OUTPUT_SIZE 14

double f(int i, int j)
{
//    return i > j ? j : i;
      return 1.0/(i + j + 1.0);
//    return fabs(i - j);  
//    return ((i > j) ? i : j) - i;
//    return i > j ? j : i;  
//    return (i >= j) ? 0 : 5;
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

double residue(int n, double *a, double *b, double *x, int my_rank, int p)
{   
    int k, l, i, rows;
    double res1 = 0.0, res2 = 0.0, tmp = 0.0;
    
    if (my_rank + 1 > n%p) rows = n/p;
    else rows = n/p + 1;
    
    for (k = 0; k < n; k++)
    {
        if (my_rank == k%p) for (i = 0; i < n; i++) x[i] = a[k/p * n + i];
        MPI_Bcast(x, n, MPI_DOUBLE, k%p, MPI_COMM_WORLD);
        for (l = 0; l < n; l++)
        {
            for (i = 0; i < rows; i++)
               tmp += x[my_rank + i * p] * b[i * n + l];   
            MPI_Reduce(&tmp, &res1, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);  //res1 = AB[k,l]
            if (my_rank == 0)
            { 
                  if (k == l) res2 += (res1 - 1.0) * (res1 - 1.0);
                  else res1 += res1 * res1;  
                  res1 = 0.0;
            }                
            tmp = 0.0;   
        }    
    }
    return sqrt(res2);
}
