#ifndef __HELP_H__
#define __HELP_H__

double f(int i, int j);

double *AllocVector(int size);

void InputMatrix(int n, double *a, int my_rank, int p);

void OutputMatrix(int n, double *a, double *x, int my_rank, int p);

#endif
