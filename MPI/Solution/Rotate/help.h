#ifndef __HELP_H__
#define __HELP_H__

#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>



double f(int i, int j);

double *AllocVector(int size);

void InputMatrix(int n, double *a, int my_rank, int p);

void OutputMatrix(int n, double *a, double *x, int my_rank, int p);

double residue(int n, double *a, double *b, double *x, int my_rank, int p);

#endif
