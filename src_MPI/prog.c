#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include "mm.h"
#include "DAG.h"
#include "routines.h"
#include "utils.h"

#define err 1E-15

int compare (const void * a, const void * b) {return ( *(int*)a - *(int*)b );}

int main(int argc, char* argv[]){
	//char* fileA = "../torso1/torso1.mtx";
	//char* fileb = "../torso1/b_for_torso1.mtx";
	//char* fileA = "../TSOPF_RS_b678_c2/TSOPF_RS_b678_c2.mtx";
	//char* fileb = "../TSOPF_RS_b678_c2/b_for_TSOPF_RS_b678_c2_b.mtx";
	char* fileA = "../af_0_k101/af_0_k101.mtx";
	char* fileb = "../af_0_k101/b_sparse_af_0_k101.mtx";
	//char* fileb = "../af_0_k101/b_dense_af_0_k101.mtx";
	//char* fileA = "../test_L.mtx";
	//char* fileb = "../test_b.mtx";
	int myid, nprocs, root = 0;
	mat_mar A,L,b;
	real *x,*y;
	real SSE;
	levelSet* G;
	Graph* DG;
	dim i;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	
	if(myid==root){
		A = init_mat(fileA);
		L = getLvals(&A);
		b = init_mat(fileb);
		DG = getReach(&L, &b);
		G = assignLevelSet(DG);
	}
	
	// INSERT DATA DISPERSION HERE //
	
	BcastMatrix(&L, myid, root, MPI_COMM_WORLD);
	BcastMatrix(&b, myid, root, MPI_COMM_WORLD);

	// INSERT ROUTINES HERE //
	
	x = lsolve_Par(&L, &b, G, myid, nprocs, root, MPI_COMM_WORLD);
	
	SSE = 0;
	if(myid==root){
		/*
		y = lsolve(&L, &b);
		for(i=0;i<b.m;i++){
			SSE += (x[i]-y[i])*(x[i]-y[i]); 
			if(fabs(x[i]-y[i]) > err)
				printf("x[i] = %0.16lf, y[i] = %0.16lf,col = %ld\n",x[i],y[i],i);
		}
		printf("SSE = %0.16lf\n",SSE);
		*/
		dim* nzInd = (dim*)malloc(DG->reach.numElems*sizeof(dim));
		node* tmp = DG->reach.tail;
		i=0;
		while(tmp!=NULL){
				nzInd[i] = tmp->vertex;
				tmp = tmp->prev;
				i++;
		}
		qsort (nzInd, DG->reach.numElems, sizeof(dim), compare);
		mat_mar xMM = ArrtoCCS(x,nzInd,b.m,DG->reach.numElems);
		free(nzInd);
		writeMM(&xMM,"x.mtx");

		free_mat(&A);
		free_mat(&xMM);
		freeGraph(DG);
		freeLevelSet(G);
	}
	free_mat(&L);
	free_mat(&b);

	MPI_Finalize();
	return 0;
}
