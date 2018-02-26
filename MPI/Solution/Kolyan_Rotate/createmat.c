#include<stdio.h>
#include"mpi.h"
#include"createmat.h"
double f(int i,int j){
	return (j>i)?j+1:i+1;
}	
void init_data(double* mat,double* vec,int n,int my_rank,int p,int max_rows){
	int i,j;
    	for(i=0;i<max_rows;i++){
		vec[i]=0.;
		for(j=0;j<n;j++)
			mat[i*n+j]=f(i*p+my_rank,j);
		for(j=0;j<n;j+=2)
			vec[i]+=mat[i*n+j];
	}
}

void print_data(double* mat,double* vec,double* v2,int n,int my_rank,int p,int mi,int mj){
	int i,j;
	MPI_Status stat;
	if(!my_rank){
		printf("/*---------------------------------*/\n");
		for(i=0;i<mi;i++){
			if(i%p==0){
				for(j=0;j<mj;j++)
					printf("[%3.4lf]",mat[(i/p)*n+j]);
				printf("|[%3.4lf]\n",vec[i]);
			}
			else{
				MPI_Recv(v2,mj+1,MPI_DOUBLE,i%p,0,MPI_COMM_WORLD,&stat);
				for(j=0;j<mj;j++)
					printf("[%3.4lf]",v2[j]);
				printf("|[%3.4lf]\n",v2[mj]);
			}
		}
		printf("/*---------------------------------*/\n");	
	}
	else{
		for(i=0;i*p+my_rank<mi;i++){
			for(j=0;j<mj;j++)
				v2[j]=mat[i*n+j];
			v2[mj]=vec[i];
			MPI_Send(v2,mj+1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
		}
	}
				
			
/*	printf("Printing memory of process #%d:\n",my_rank);
	printf("Row |Matrix");
	mi=((n%p+n/p>N_MAX)?N_MAX:n%p+n/p);
	mj=(n>N_MAX)?N_MAX:n;
	for(i=0;i<mj-1;i++)
		printf("          ");
	printf("    |Vector\n");
	for(i=0;i<mi;i++){
		if(i*p+my_rank>=10)
			printf("[%d]",i*p+my_rank);			
		else
			printf("[ %d]",i*p+my_rank);
		for(j=0;j<mj;j++)
			printf("[%3.4lf]",mat[i*n+j]);
		printf("|[%3.4lf]\n",vec[i]);
	}*/
}			
