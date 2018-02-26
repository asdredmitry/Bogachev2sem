#include"mspin.h"
#ifndef MATRMUL_H
#define MATRMUL_H
int creatematrixspin(double* v1,double* v2,int n,int k,mspin* outmat);
int matrixspinmultmatrix(mspin *s,int n,double* v1,double* v2);
int matrixspinmultvector(mspin *s,double* v1,double* v2);
void rotate(double* v1,double* v2,double* w1,double* w2,int n,int k,mspin* outmat);
void Solve_it(double* mat,double* vec,double* v2, int n, mspin* ms, int my_rank, int p,int max_rows);
#endif
