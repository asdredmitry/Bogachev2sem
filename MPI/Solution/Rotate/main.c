#include "help.h"
#include "inv.h"

int main(int argc, char **argv)
{
  int n;
  double *a;
  double *b;
  double *x1; double *x2;
  double t, res;
  int rows;
  int my_rank, p;
  int err1, err2;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if (argc == 2) n = atoi(argv[1]);
  else
  {
    if (my_rank == 0) printf("Not correct usage.\n");
    MPI_Finalize();
    return 1;
  }

  if (my_rank + 1 > n%p) rows = n/p;
  else rows = n/p + 1;

  a = AllocVector(rows * n);
  b = AllocVector(rows * n);
  x1 = AllocVector(2 * n);
  x2 = AllocVector(2 * n);
  err1 = 0;
  err2 = 0;
  if (!(a && b && x1)) err1 = 1;

  MPI_Allreduce(&err1, &err2, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  if (err2)
  {
    if (my_rank == 0) printf("Not enough memory.\n");
    if (a) free(a);
    if (b) free(b);
    if (x1) free(x1);
    if (x2) free(x2);
    MPI_Finalize();
    return 1;
  }
  
  err1 = 0;
  err2 = 0;
  
  InputMatrix(n, a, my_rank, p);

  if (my_rank == 0) printf("Matrix A:\n\n");
  OutputMatrix(n, a, x2, my_rank, p);

  MPI_Barrier(MPI_COMM_WORLD);
  t = MPI_Wtime();
  
  if (InvMatrix(n, a, b, x1, x2, my_rank, p) == -1) err1 = 1;
  
  MPI_Barrier(MPI_COMM_WORLD);
  t = MPI_Wtime() - t;

  MPI_Allreduce(&err1, &err2, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  if (err2)
  {
    if (my_rank == 0) printf("\n\nSorry, irreversible matrix.\n");
    if (a) free(a);
    if (b) free(b);
    if (x1) free(x1);
    if (x2) free(x2);
    MPI_Finalize();
    return 1;
  }

  if (my_rank == 0) printf("\nMatrix A^{-1}:\n\n");
  OutputMatrix(n, b, x2, my_rank, p);

  if (my_rank == 0) printf("\n\nSolution time = %e\n", t);
  
  MPI_Barrier(MPI_COMM_WORLD);
  t = MPI_Wtime();
  
  InputMatrix(n, a, my_rank, p);
  res = residue(n, a, b, x1, my_rank, p);
  
  MPI_Barrier(MPI_COMM_WORLD);
  t = MPI_Wtime() - t;
  if (my_rank == 0) printf("\n\n|A * A^{-1} - I| = %e\ntime = %e\n", res, t);

  free(a);
  free(b);
  free(x1);
  free(x2);
  MPI_Finalize();

  return 0;
}
