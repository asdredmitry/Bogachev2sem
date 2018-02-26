#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<sys/types.h>
#include"mpi.h"
#include"matrmul.h"
#include"createmat.h"
#include"mspin.h"
#define  BUF_LEN 256
#define N_MAX 8
/*typedef struct _ARGS{
  mspin *matrixspin;
  double* TheMatrix;
  double* vector;
  int n;
  int p;
  int my_rank;
}ARGS;  */
int main(int argc,char* argv[]){
    int my_rank;
    int p;
    int n=500,mi,mj; //               PA3MEPHOCTb 3ADAETC9I TyT
    mspin* matrixspin;
    int /*frst,lst,rows,*/max_rows;
    double t;
    double* mat;
    double* vec;
    double* v2;//,*verr,*vinit;
//    double err=0.,nev=0.,tmp;
    clock_t cl;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    //max_rows=n/p+n%p;
    mj=(n>N_MAX)?N_MAX:n;
    //mi=(mj+1>n)?mj+1:n;
    max_rows=0;
    for(mi=my_rank;mi<n;mi+=p) max_rows++;
    mat=(double*)malloc(n*max_rows*sizeof(double));
    vec=(double*)malloc(max_rows*sizeof(double));
    v2=(double*)malloc((n+1)*sizeof(double));
    //    verr=(double*)malloc(max_rows*sizeof(double));
//    vinit=(double*)malloc(max_rows*sizeof(double));
    matrixspin=(mspin*)malloc(sizeof(mspin));
    if(!(mat&&vec/*&&v2&&verr&&vinit&&matrixspin*/)){
      printf("Not enough memory!\n");
      fflush(stdout);
      MPI_Abort(MPI_COMM_WORLD,1);
    }
    mi=(n>N_MAX)?N_MAX:n;
//    mj=(n>N_MAX)?N_MAX:n;
    init_data(mat,vec,n,my_rank,p,max_rows);
//    MPI_Barrier(MPI_COMM_WORLD);
    print_data(mat,vec,v2,n,my_rank,p,mi,mj);
    
    //The main thing 
    MPI_Barrier(MPI_COMM_WORLD);
    cl=clock();
    Solve_it(mat,vec,v2,n,matrixspin,my_rank,p,max_rows);
    MPI_Barrier(MPI_COMM_WORLD);
    t=(double)(clock()-cl)/CLOCKS_PER_SEC;
    print_data(mat,vec,v2,n,my_rank,p,mi,mj);    
    if(!my_rank) printf("Time:%lg\n",t);
//    free(mat);
//    free(vec);
//    free(v2);
//    free(verr);
//    free(vinit);
//    free(matrixspin);
    MPI_Finalize();
    return 0;
}
