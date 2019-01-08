#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "mm.h"
#include "DAG.h"
#include "routines.h"

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
	
	mat_mar A, b, L;
	real *x,*y;
	dim i;
	clock_t start, end;
	double time_taken;

	A = init_mat(fileA);
	b = init_mat(fileb);
	L = getLvals(&A);

	start = clock();
	x = lsolve(&L, &b);
	end = clock();
	time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Time taken: %lf\n", time_taken);
	
	start = clock();
	Graph* DG = getReach(&L, &b);
	//for(i=0;i<b.nz;i++)
	//	DFS(&L,DG,b.I[i]);
	y = lsolve_GP(&L, &b, DG);
	end = clock();
	time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Time taken: %lf\n", time_taken);
	
	printf("y[n] = %lf, x[n] = %lf\n",y[L.m-1],x[L.m-1]);	
	for(i=0;i<L.m;i++)
		if(abs(y[L.m-1]) > 0.0 || abs(x[L.m-1]) > 0.0)
			printf("y[n] = %lf, x[n] = %lf\n",y[L.m-1],x[L.m-1]);	
	
	real SSE = 0;
	for(i=0;i<L.m;i++)
		SSE += (y[i]-x[i])*(y[i]-x[i]); 
	printf("SSE = %lf\n",SSE);
	
	
	dim count = 0;
	node* tmp = DG->reach.tail;
	while(tmp!=NULL){
		count++;
		tmp = tmp->prev;
	}
	printf("Count = %lu\n",count);	

	freeGraph(DG);
	free_mat(&A);
	free_mat(&b);
	free_mat(&L);
	free(x);

	return 0;
}
