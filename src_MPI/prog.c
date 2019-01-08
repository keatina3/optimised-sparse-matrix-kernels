#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "mm.h"
#include "DAG.h"

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
	int myid, nprocs;
	int root = 0;
	mat_mar A,L,b;
	real *x;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

	if(myid==root){
		A = init_mat(fileA);
		L = getLvals(&A);
		b = init_mat(fileb);
		Graph* DG = getReach(&L, &b);
	}
	
	// INSERT DATA DISPERSION HERE //

	// INSERT ROUTINES HERE //

	// INSERT DATA GATHERING HERE //

	if(myid==root){
		free_mat(&A);
		free_mat(&b);
		free_mat(&L);
	}
	
	MPI_Finalize();
	return 0;
}
