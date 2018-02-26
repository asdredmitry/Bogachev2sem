#ifndef __INV_H__
#define __INV_H__

#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int InvMatrix(int n, double *a, double *b, double *x1, double *x2, int my_rank, int p);

#endif
