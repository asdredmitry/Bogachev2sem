#include "inv.h"

int InvMatrix(int n, double *a, double *b, double *x1, double *x2, int my_rank, int p)
{
  int i, j, k;
  int rows, err = 0;
  int first, pr_up, pr_down;
  double tmp, tmp1, tmp2;
  double q1, q2;
  MPI_Status status;

  if (my_rank + 1 > n%p) rows = n/p;
  else rows = n/p + 1;
  
  /*B = I*/
  for (i = 0; i < rows; i++)
        for (j = 0; j < n; j++)
              b[i * n + j] = (my_rank + i * p == j);
  
  /*QR decomposition*/
  for (k = 0; k < n; k++)
    {
        first = (k - my_rank + p - 1)/p;
        for (i = rows - 1; i > first; i--)
        {
              q1 = a[(i - 1) * n + k];
              q2 = a[i * n + k];
              if (fabs(q2) < 1E-100) continue;
              tmp = sqrt(q1 * q1 + q2 * q2);
              if (fabs(tmp) < 1E-100) continue;
              q1 = q1 / tmp; q2 = -q2/tmp;
              a[(i - 1) * n + k] = tmp;
              a[i * n + k] = 0.0;
              for (j = k + 1; j < n; j++)
                    {
                          tmp1 = a[(i - 1) * n + j];
                          tmp2 = a[i * n + j];
                          a[(i - 1) * n + j] = tmp1 * q1 - tmp2 * q2;
                          a[i * n + j] = tmp2 * q1 + tmp1 * q2;
                    }
 
              for (j = 0; j < n; j++)
                   {
                          tmp1 = b[(i - 1) * n + j];
                          tmp2 = b[i * n + j];
                          b[(i - 1) * n + j] = tmp1 * q1 - tmp2 * q2;
                          b[i * n + j] = tmp2 * q1 + tmp1 * q2;
 
                  }
        }



        for (i = 0; i < ((k <= n - p) ? (p - 1) : (n - k - 1)); i++)
        {
             pr_up = (k <= n - p) ? ((k + p - 2 - i)%p) : ((n - 2 - i)%p);
             pr_down = (k <= n - p) ? ((k + p - 1 - i)%p) : ((n - 1 - i)%p);
             
             if (my_rank == pr_up)
             {
                 for (j = k; j < n; j++) x1[j] = a[first * n + j];
                 for (j = 0; j < n; j++) x1[j + n] = b[first * n + j];
                 MPI_Sendrecv (x1 + k, 2 * n - k, MPI_DOUBLE, pr_down, 0 , x2 + k, 2 * n - k, MPI_DOUBLE, pr_down, 0, MPI_COMM_WORLD, &status);
                 q1 = a[first * n + k];
                 q2 = x2[k];
                 if (fabs(q2) < 1E-100) continue;
                 tmp = sqrt (q1 * q1 + q2 * q2);
                 if (fabs(tmp) < 1E-100) continue;
                 q1 = q1 / tmp; q2 = -q2/tmp;
                 a[first * n + k] = tmp;
                 for (j = k + 1; j < n; j++)
                    {
                          tmp1 = a[first * n + j];
                          tmp2 = x2[j];
                          a[first * n + j] = tmp1 * q1 - tmp2 * q2;
                    }
                 for (j = 0; j < n; j++)
                    {
                          tmp1 = b[first * n + j];
                          tmp2 = x2[j + n];
                          b[first * n + j] = tmp1 * q1 - tmp2 * q2;
                    }
             }           

             if (my_rank == pr_down)
             {
                 for (j = k; j < n; j++) x1[j] = a[first * n + j];
                 for (j = 0; j < n; j++) x1[j + n] = b[first * n + j];
                 MPI_Sendrecv (x1 + k, 2 * n - k, MPI_DOUBLE, pr_up, 0 , x2 + k, 2 * n - k, MPI_DOUBLE, pr_up, 0, MPI_COMM_WORLD, &status);
                 q1 = x2[k];
                 q2 = a[first * n + k];
                 if (fabs(q2) < 1E-100) continue;
                 tmp = sqrt(q1 * q1 + q2 * q2);
                 if (fabs(tmp) < 1E-100) continue;
                 q1 = q1 / tmp; q2 = -q2/tmp;
                 a[first * n + k] = 0.0;
                 for (j = k + 1; j < n; j++)
                    {
                          tmp1 = x2[j];
                          tmp2 = a[first * n + j];
                          a[first * n + j] = tmp2 * q1 + tmp1 * q2;
                    }
                 for (j = 0; j < n; j++)
                    {
                          tmp1 = x2[j + n];
                          tmp2 = b[first * n + j];
                          b[first * n + j] = tmp2 * q1 + tmp1 * q2;
                    }
             }        
             
        }
    }

//  printf("\nQR complete, %d\n", my_rank);  
  
  /*back Gauss RX = Q^{-1}*/ 
  for (i = 0; i < n; i++)
  {
      if (my_rank == i%p) 
            {
                if (fabs(a[i/p * n + i]) < 1e-100) err = -1;
                else 
                {
                        tmp = 1.0/a[i/p * n + i];
                        for (j = i; j < n; j++)
                              a[i/p * n + j] *= tmp;
                        for (j = 0; j < n; j++)
				              b[i/p * n + j] *= tmp;
				}    
            }    
  }
  
  for (i = n - 1; i > 0; i--)
  {  
      if (my_rank == i%p)
      for (j = 0; j < n; j++) x1[j] = b[i/p * n + j];  
      
      MPI_Bcast(x1, n, MPI_DOUBLE, i%p, MPI_COMM_WORLD);
      if (my_rank < i%p) first = i/p;
      else if (i/p - 1 >= 0) first = i/p - 1;
      else continue;

      for (j = first; j >= 0; j--)
        for (k = 0; k < n; k++)
          b[j * n + k] -= x1[k] * a[j * n + i];
  }    

return err;
}
