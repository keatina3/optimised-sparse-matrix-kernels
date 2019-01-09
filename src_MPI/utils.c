#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "mm.h"
#include "DAG.h"
#include "utils.h"

void decomp1d(dim n, dim p, dim myid, dim *s, dim *e){
    int d,r;
	if(p > n){
		if(myid < n){
			*s = myid;
			*e = myid;
		} else {
			*s = -1;
			*e = -2;
		}
		return;
	}
    d = n/p;
    r = n%p;
    if(myid < r){
    	*s = myid*(d+1);
    	*e = *s + d;
    } else {
    	*s = r*(d+1) + (myid-r)*d;
    	*e = *s + (d-1);
    }
}

dim* assignCols(dim* colCount, int myid, int nprocs, int root, levelSet* G, dim level, MPI_Comm communicator){
	dim s,e;
	dim* colInd;
	int i,j;
	MPI_Status status;
	dim setCard;

	if(myid == root){
		setCard = G->level_ptr[level].numElems;
	}
	MPI_Bcast(&setCard,1,MPI_UNSIGNED_LONG,root,communicator);
	decomp1d(setCard, nprocs, myid, &s, &e);
	*colCount = e-s+1;
	colInd = (dim*)malloc((*colCount) * sizeof(dim));
	if(myid == root){
		node* tmp = G->level_ptr[level].tail;
		for(i=0;i<*colCount;i++){
			colInd[i] = tmp->vertex;
			tmp = tmp->prev;
		}
		for(i=1;i<nprocs;i++){
			decomp1d(setCard, nprocs, i, &s, &e);
			dim* sendArr;
			dim sendCount = e-s+1;
			if(sendCount > 0){
				sendArr = (dim*)malloc(sendCount*sizeof(dim));
				for(j=0;j<sendCount;j++){
					sendArr[j] = tmp->vertex;
					tmp = tmp->prev;
				}
				MPI_Send(sendArr,sendCount,MPI_UNSIGNED_LONG,i,0,MPI_COMM_WORLD);
				free(sendArr);
			} else {
				break;
			}
		}
	} else {
		if(*colCount>0){
			MPI_Recv(colInd,*colCount,MPI_UNSIGNED_LONG,root,0,MPI_COMM_WORLD, &status);
		}
	}
	return colInd;
}

void BcastMatrix(mat_mar* A, int root, int myid, MPI_Comm communicator){
	MPI_Bcast(&A->head, 4, MPI_CHAR, root, communicator);
	MPI_Bcast(&A->m, 1, MPI_UNSIGNED_LONG, root, communicator);
	MPI_Bcast(&A->n, 1, MPI_UNSIGNED_LONG, root, communicator);
	MPI_Bcast(&A->nz, 1, MPI_UNSIGNED_LONG, root, communicator);
	if(myid!=root){
		A->dat = (real*)malloc(A->nz*sizeof(real));
		if(is_sparse(A->head)){
			A->I = (dim*)malloc(A->nz*sizeof(dim));
			A->J = (dim*)malloc(A->m*sizeof(dim));
		} else {
			A->arr = (real**)malloc(A->m*sizeof(real*));
		}
	}
	MPI_Bcast(A->dat, A->nz, MPI_DOUBLE, root, communicator);
	if(is_sparse(A->head)){
		MPI_Bcast(A->I, A->nz, MPI_UNSIGNED_LONG, root, communicator);
		MPI_Bcast(A->J, 1+A->m, MPI_UNSIGNED_LONG, root, communicator);
	} else
		MPI_Bcast(A->arr, A->m, MPI_DOUBLE, root, communicator);
}
