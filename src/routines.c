#include <stdio.h>
#include <stdlib.h>
#include "mm.h"

bool lsolve(mat_mar* L, real* b){
	dim i, j;
	
	for(i=0;i<L->m;i++){
		b[i] /= L->dat[L->J[i]];
		for(j = L->J[i]+1; j < L->J[i+1]; j++)
			b[L->I[j]] -= L->dat[j] * b[i];
	}
	return 1;
}

real* CCSvectoArr(mat_mar* b){
	real* x;
	dim i;
	
	x = (real*)calloc(b->m,sizeof(real));
	for(i=0;i<b->nz;i++)
		x[b->I[i]] = b->dat[i];

	return x;
}

/*
int lsolve (int n, int* Lp, int* Li, double* Lx, double *x){
int p, j;
if (!Lp || !Li || !x) return (0) ;  //check inputs 
for (j = 0 ; j < n ; j++)
{
x [j] /= Lx [Lp [j]] ;
for (p = Lp [j]+1 ; p < Lp [j+1] ; p++)
{
x [Li [p]] -= Lx [p] * x [j] ;
}
}
return (1) ;
}
*/
