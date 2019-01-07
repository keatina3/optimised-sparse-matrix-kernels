#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mm.h"
#include "DAG.h"
#include "routines.h"

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
	
	mat_mar A;
	mat_mar b;
	mat_mar L;
	real *x,*y;
	dim i,j;

	A = init_mat(fileA);
	b = init_mat(fileb);
	L = getLvals(&A);
	//printf("%lu, %lf\n",A.I[10],A.dat[10]);
	//printf("%lu\n",A.J[A.m-1]);

	Graph* DG = createGraph(L.n);
	
	x = lsolve(&L, &b);
	printf("x[0] = %lf\n",x[L.m-1]);
	//for(i=0;i<L.m;i++)
	//	printf("x[i] = %lf\n",x[i]);

	y = lsolve_GP(&L, &b, DG);		
	printf("y[0] = %lf\n",y[L.m-1]);	
	//for(i=0;i<L.m;i++)
	//	printf("y[i] = %lf\n",y[i]);
	
	real SSE = 0;
	for(i=0;i<L.m;i++)
		SSE += (y[i]-x[i])*(y[i]-x[i]); 
	printf("SSE = %lf\n",SSE);
	
	/*	
    //displayGraph(DG);
	dim count = 0;
	node* tmp = DG->reach.tail;
	while(tmp!=NULL){
		printf("%u\n",1+tmp->vertex);
		count++;
		tmp = tmp->prev;
	}
	*/

	freeGraph(DG);
	free_mat(&A);
	free_mat(&b);
	free_mat(&L);
	free(x);

	return 0;
}
