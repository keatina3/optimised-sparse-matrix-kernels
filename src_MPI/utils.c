#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "mmio.h"
#include "DAG.h"
#include "utils.h"

// divides up N tasks between P processors. returns starting point s, end point e //
void decomp1d(dim n, dim p, dim myid, dim *s, dim *e){
    int d,r;
	if(p >  n){		// if nprocs > ntasks then assigned 1 task to ntasks processors
		if(myid < n){
			*s = myid;
			*e = myid;
		} else {
			*s = 2;
			*e = 1;
		}
	} else {
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
}

// assgining columns from level set to each proc //
dim* assignCols(dim* colCount, int myid, int nprocs, int root, levelSet* G, dim level, MPI_Comm communicator){
	dim s,e;		// start pt and end pt
	dim* colInd;		// array to hold the columns
	int i,j;
	MPI_Status status;
	MPI_Request request;
	dim setCard;		// cardinality of set
	if(myid == root){	// assinging cardinality
		setCard = G->level_ptr[level].numElems;
	}
	// broadcasting cardinality //
	MPI_Bcast(&setCard,1,MPI_DIM,root,communicator);
	// breaking up set into start and end points //
	decomp1d(setCard, nprocs, myid, &s, &e);
	*colCount = e-s+1;		// number of columns for proc myid
	if(*colCount>0)			// if assigned cols, malloc spaces for their indices
		colInd = (dim*)malloc((*colCount) * sizeof(dim));
	if(myid == root){
		node* tmp = G->level_ptr[level].tail;
		// assigning vertices from level set as columns into the array //
		for(i=0;i<*colCount;i++){
			colInd[i] = tmp->vertex;
			tmp = tmp->prev;
		}
		// sending column info to the other procs
		for(i=1;i<nprocs;i++){
			decomp1d(setCard, nprocs, i, &s, &e);	// getting each procs breakdown
			dim* sendArr;				// array to store col indices to be sent
			dim sendCount = e-s+1;			// num of col indices being sent
			if(sendCount > 0){			// if sending any cols
				sendArr = (dim*)malloc(sendCount*sizeof(dim));
				// assgining cols to the array to be sent
				for(j=0;j<sendCount;j++){
					sendArr[j] = tmp->vertex;
					tmp = tmp->prev;
				}
				// non blocking send of col inds to other procs //
				MPI_Isend(sendArr,sendCount,MPI_DIM,i,0,MPI_COMM_WORLD,&request);
				free(sendArr);		// free temp array
			} else {
				break;
			}
		}
	} else {	
		// if not root, recv cols //
		if(*colCount>0){
			MPI_Recv(colInd,*colCount,MPI_DIM,root,0,MPI_COMM_WORLD, &status);
		}
	}
	return colInd;
}

// broadcast matrix data //
void BcastMatrix(mat_mar* A, int myid, int root, MPI_Comm communicator){
	// bcasting header code //
	MPI_Bcast(&A->head, 4, MPI_CHAR, root, communicator);
	// dimension info //
	MPI_Bcast(&A->m, 1, MPI_DIM, root, communicator);
	MPI_Bcast(&A->n, 1, MPI_DIM, root, communicator);
	MPI_Bcast(&A->nz, 1, MPI_DIM, root, communicator);
	
	// allocating mem depending on sparse or dense if not root (as already allocated on root //
	if(myid!=root){
		A->dat = (real*)malloc(A->nz*sizeof(real));
		if(is_sparse(A->head)){
			A->I = (dim*)malloc(A->nz*sizeof(dim));
			A->J = (dim*)malloc(A->m*sizeof(dim));
		} else {
			A->arr = (real**)malloc(A->m*sizeof(real*));
		}
	}
	
	// bcasting data //
	MPI_Bcast(A->dat, A->nz, MPI_REAL_TYPE, root, communicator);
	
	// bcasting col ptr and row index //
	if(is_sparse(A->head)){
		MPI_Bcast(A->I, A->nz, MPI_DIM, root, communicator);
		MPI_Bcast(A->J, 1+A->m, MPI_DIM, root, communicator);
	} else	// bcasting 2d array
		MPI_Bcast(A->arr, A->m, MPI_REAL_TYPE, root, communicator);
}

// sync x at end of each level iteration //
void levelSync(mat_mar* L, real* x, levelSet* G, dim level, dim colCount, int myid, int root, MPI_Comm comm){
	node* tmp;
	dim i;
	MPI_Status stat;
	dim col;
	real localsum;
	
	// if root, iterate through each index in levelset and recv x val from other thread //
	if(myid==root){
		tmp = G->level_ptr[level].tail;
		for(i=0;i<colCount;i++)
			tmp = tmp->prev;
		while(tmp!=NULL){
			col = tmp->vertex;
			MPI_Recv(&x[col],1,MPI_REAL_TYPE,MPI_ANY_SOURCE,col,comm,&stat);
			for(i=L->J[col]+1;i<L->J[col+1];i++){
				MPI_Recv(&localsum,1,MPI_REAL_TYPE,MPI_ANY_SOURCE,L->I[i],comm,&stat);
				x[L->I[i]] += localsum; // manual reduced MPI_ADD for x vals the usually have -= in algorithm
			}
			tmp = tmp->prev;
		}
	}
	// broadcast x once synchronised //
	MPI_Bcast(x,L->m,MPI_REAL_TYPE,root,comm);
}
