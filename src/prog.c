#include <stdio.h>
#include <stdlib.h>
#include "mm.h"

int main(int argc, char* argv[]){
	//char* fileA = "../torso1/torso1.mtx";
	//char* fileb = "../b_for_torso1.mtx";
	char* fileA = "../TSOPF_RS_b678_c2/TSOPF_RS_b678_c2.mtx";
	char* fileb = "../b_for_TSOPF_RS_b678_c2_b.mtx";
	mat_mar A;
	mat_mar b;
	mat_mar L;
	real* x;
	int i;

	A = init_mat(fileA);
	b = init_mat(fileb);
	L = getLvals(&A);
	//printf("%lu, %lf\n",A.I[10],A.dat[10]);
	//printf("%lu\n",A.J[A.m-1]);
	
	x = (real*)calloc(b.m,sizeof(real));
	CCSvectoArr(&b,x);

	lsolve(&L, x);
	for(i=0;i<b.m;i++)
		printf("%lf\n",x[i]);
	free_mat(&A);
	free_mat(&b);
	free_mat(&L);
	free(x);

	return 0;
}
