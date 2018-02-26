#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include"mpi.h"
#include"mspin.h"
#include"matrmul.h"
#include"createmat.h"
// Functions for spin algorithm of making matrix triangular.
// Functions for making the prototype of spin matrix T_ij, multiplying it by any given
// matrix or vector
int creatematrixspin(double* v1,double* v2,int n,int k,mspin* outmat){
  double kor=sqrt(v1[k]*v1[k]+v2[k]*v2[k]);
  double cosf=v1[k]/kor,sinf=v2[k]/kor;
  n=n;
  outmat->cosf=cosf;
  outmat->sinf=-sinf;
  outmat->k=k;
  outmat->kor=kor;
  return 0;
}
int matrixspinmultmatrix(mspin *s,int n,double* v1,double* v2){
  int ii;
  double tmp;
  for(ii=0;ii<s->k;ii++){
    tmp=v1[ii];
    v1[ii]=v1[ii]*s->cosf-v2[ii]*s->sinf;
    v2[ii]=tmp*s->sinf+v2[ii]*s->cosf;
  }
  v1[s->k]=s->kor;
  v2[s->k]=0.;
  for(ii=s->k+1;ii<n;ii++){
    tmp=v1[ii];
    v1[ii]=v1[ii]*s->cosf-v2[ii]*s->sinf;
    v2[ii]=tmp*s->sinf+v2[ii]*s->cosf;
  }
  return 0;
}      
int matrixspinmultvector(mspin *s,double* v1,double* v2){
  double tmp=v1[0];
  v1[0]=v1[0]*s->cosf-v2[0]*s->sinf;
  v2[0]=tmp*s->sinf+v2[0]*s->cosf;
  return 0;
}
void rotate(double* v1,double* v2,double* w1,double* w2,int n,int k,mspin* outmat){
//  int l;
  creatematrixspin(v1,v2,n,k,outmat);
  matrixspinmultmatrix(outmat,n,v1,v2);
  matrixspinmultvector(outmat,w1,w2);
}
void BackGauz(double* mat,double* vec,double* v2,int n,int my_rank,int p,int max_rows){
	int i,j,frst;
	for(i=0;i<max_rows;i++){
		vec[i]/=mat[i*n+i*p+my_rank];
		for(j=n-1;j>=i*p+my_rank;j--)
			mat[i*n+j]/=mat[i*n+i*p+my_rank];
	}		
//	print_data(mat,vec,v2,n,my_rank,p,(8>n)?n:8,(8>n)?n:8);//For deBUGgin'
	for(i=n-1;i>=1;i--){
		if(my_rank<=i%p){
			if(i/p<0) {
//				printf("Step %d,Process:%d is waiting everybody(i/p>=max_rows)\n",i,my_rank);
				MPI_Bcast(v2+n,1,MPI_DOUBLE,i%p,MPI_COMM_WORLD);
				MPI_Barrier(MPI_COMM_WORLD);
				continue;
			}	
			frst=i/p;
		}	
		else if(i/p-1>=0)
			frst=i/p-1;	
		else  {
//			printf("Step %d,Process:%d is waiting everybody(i/p+1>=max_rows)\n",i,my_rank);
			MPI_Bcast(v2+n,1,MPI_DOUBLE,i%p,MPI_COMM_WORLD);
			MPI_Barrier(MPI_COMM_WORLD);
			continue;
		}	
		if(my_rank==i%p){
//			printf("Step %d,Process:%d sends data to everybody\n",i,my_rank);
			MPI_Bcast(vec+i/p,1,MPI_DOUBLE,my_rank,MPI_COMM_WORLD);
//			printf("Step %d,Process:%d sent data to everybody\n",i,my_rank);
			for(j=i/p-1;j>=0;j--)
				vec[j]-=vec[i/p]*mat[j*n+i];
		}
		else{
//			printf("Step %d,Process:%d receives data from %d\n",i,my_rank,i%p);
			MPI_Bcast(v2+n,1,MPI_DOUBLE,i%p,MPI_COMM_WORLD);
//			printf("Step %d,Process:%d received data from %d\n",i,my_rank,i%p);
			for(j=frst;j>=0;j--)
				vec[j]-=v2[n]*mat[j*n+i];
		}
//	printf("Step %d,Process:%d is waiting everybody\n",i,my_rank);
	MPI_Barrier(MPI_COMM_WORLD);
	}
}	
/*BackGauz will be replaced with its mpi version

void BackGauz(double* TheMatrix,double* vector,int n){
  int l,m;
  for(l=n-1;l>0;l--){
    for(m=l-1;m>=0;m--){
      vector[m]-=(TheMatrix[m*n+l]/TheMatrix[l*n+l])*vector[l];
    }
  }
  for(l=0;l<n;l++) vector[l]/=TheMatrix[l*n+l];
}
*/
void Solve_it(double* mat,double* vec,double* v2, int n, mspin* ms, int my_rank, int p,int max_rows){
	int i,j,frst,flag;
	MPI_Status status;
	for(i=0;i<n-1;i++){
		if(my_rank>=i%p){
			if(i/p>=max_rows) {
//				printf("Step %d,Process:%d is waiting everybody(i/p>=max_rows)\n",i,my_rank);
				MPI_Barrier(MPI_COMM_WORLD);
				continue;
			}	
			frst=i/p;
		}	
		else if(i/p+1<max_rows)
			frst=i/p+1;	
		else  {
//			printf("Step %d,Process:%d is waiting everybody(i/p+1>=max_rows)\n",i,my_rank);
			MPI_Barrier(MPI_COMM_WORLD);
			continue;
		}	
		for(j=frst+1;j<max_rows;j++)
			rotate(mat+frst*n,mat+j*n,vec+frst,vec+j,n,i,ms);
		if(my_rank==i%p){
			if(i>=n-p+1){
//				printf("Step %d > n-p+1 flag = %d\n",i,(n-i)%p);
				flag=(n-i)%p;
				flag=(my_rank+flag)%p;
			}	
			else flag=my_rank;	
			for(j=(my_rank+1)%p;j!=flag;j=(j+1)%p){
			//	MPI_Bcast(&j,1,MPI_INT,i%p,MPI_COMM_WORLD);
//				printf("Step %d,Process:%d Receives data from %d\n",i,i%p,j);
	    			MPI_Recv(v2,n+1,MPI_DOUBLE,j,0,MPI_COMM_WORLD,&status);
				rotate(mat+frst*n,v2,vec+frst,v2+n,n,i,ms);
//				printf("Step %d,Process:%d sends data back to %d\n",i,i%p,j);
				MPI_Send(v2,n+1,MPI_DOUBLE,j,0,MPI_COMM_WORLD);
			}		
				
		}
		else{
			for(j=0;j<n;j++)
				v2[j]=mat[frst*n+j];
			v2[n]=vec[frst];
//			do{
//				MPI_Bcast(&flag,1,MPI_INT,i%p,MPI_COMM_WORLD);
//			}while(flag!=my_rank);
//			printf("Step %d,Process:%d sends data to %d\n",i,my_rank,i%p);
			MPI_Send(v2,n+1,MPI_DOUBLE,i%p,0,MPI_COMM_WORLD);
//			printf("Step %d,Process:%d receives data back from %d\n",i,my_rank,i%p);
			MPI_Recv(v2,n+1,MPI_DOUBLE,i%p,0,MPI_COMM_WORLD,&status);
			for(j=0;j<n;j++){
				mat[frst*n+j]=v2[j];
			//	v2[j]=0.;
			}	
			vec[frst]=v2[n];
			//v2[n]=0.;
//			while(flag!=(i+p-1)%p){
//				MPI_Bcast(&flag,1,MPI_INT,i%p,MPI_COMM_WORLD);
//			}
		}
//		printf("Step %d,Process:%d is waiting everybody\n",i,my_rank);
		MPI_Barrier(MPI_COMM_WORLD);
	
	//	MPI_Barrier(MPI_COMM_WORLD);
	}

	BackGauz(mat,vec,v2,n,my_rank,p,max_rows);
//	print_data(mat,vec,v2,n,my_rank,p,(8>n)?n:8,(8>n)?n:8);//For deBUGgin'
	MPI_Barrier(MPI_COMM_WORLD);	
}
