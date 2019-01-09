#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "mm.h"
#include "DAG.h"
#include "utils.h"

int main(int argc, char* argv[]){
	//char* fileA = "../torso1/torso1.mtx";
	//char* fileb = "../torso1/b_for_torso1.mtx";
	//char* fileA = "../TSOPF_RS_b678_c2/TSOPF_RS_b678_c2.mtx";
	//char* fileb = "../TSOPF_RS_b678_c2/b_for_TSOPF_RS_b678_c2_b.mtx";
	//char* fileA = "../af_0_k101/af_0_k101.mtx";
	//char* fileb = "../af_0_k101/b_sparse_af_0_k101.mtx";
	//char* fileb = "../af_0_k101/b_dense_af_0_k101.mtx";
	char* fileA = "../test_L.mtx";
	char* fileb = "../test_b.mtx";
	int myid, nprocs, root = 0;
	mat_mar A,L,b;
	real *x;
	dim *colInd;
	dim colCount;
	levelSet* G;
	Graph* DG;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

	if(myid==root){
		A = init_mat(fileA);
		L = getLvals(&A);
		b = init_mat(fileb);
		DG = getReach(&L, &b);
		node* tmp = DG->reach.tail;
		G = assignLevelSet(DG);
		dim i;
		/*
		for(i=0;i<G->numLevels;i++){
			node* tmp = G->level_ptr[i].tail;
			while(tmp!=NULL){
				printf("Level = %lu, Node = %lu, Level Cardinality = %lu\n",i+1,1+tmp->vertex,G->level_ptr[i].numElems);
				tmp = tmp->prev;
			}
		}
		*/
	}
	MPI_Barrier(MPI_COMM_WORLD);

	// INSERT DATA DISPERSION HERE //
	
	BcastMatrix(&L, 0, myid, MPI_COMM_WORLD);
	BcastMatrix(&b, 0, myid, MPI_COMM_WORLD);
	x = (real*)malloc(b.m*sizeof(real));

	// INSERT ROUTINES HERE //
	
	assignCols(colInd, &colCount, myid, nprocs, root, G, 0,MPI_COMM_WORLD);
	//printf("myid = %d, colCount = %lu\n", myid, colCount);
	//dim i;
	//for(i=0;i<G->numLevels;i++){

	//}

	// INSERT DATA GATHERING HERE //



	free_mat(&L);
	free_mat(&b);
	if(myid==root){
		free_mat(&A);
		freeGraph(DG);
		freeLevelSet(G);
	}

	MPI_Finalize();
	return 0;
}
