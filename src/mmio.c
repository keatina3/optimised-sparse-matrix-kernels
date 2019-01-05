#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mm.h"

bool read_mm_head(mat_mar A){
	char buffer[MM_MAX_LINE_LENGTH];
	char banner[MM_MAX_TOKEN_LENGTH];
	char mtx[MM_MAX_TOKEN_LENGTH]; 
	char crd[MM_MAX_TOKEN_LENGTH];
	char data_type[MM_MAX_TOKEN_LENGTH];
	char storage_scheme[MM_MAX_TOKEN_LENGTH];
	char *ptr;

	if(fgets(buffer, MM_MAX_LINE_LENGTH, A.file) == NULL) 
		return 0;

	if(sscanf(buffer, "%s %s %s %s %s", banner, mtx, crd, data_type, storage_scheme) != 5)
		return 0;

	for(ptr=mtx; *ptr!='\0'; *ptr=tolower(*ptr),ptr++);  /* convert to lower case */
	for(ptr=crd; *ptr!='\0'; *ptr=tolower(*ptr),ptr++);  
	for(ptr=data_type; *ptr!='\0'; *ptr=tolower(*ptr),ptr++);
	for(ptr=storage_scheme; *ptr!='\0'; *ptr=tolower(*ptr),ptr++);

    /* check for banner */
	if(strncmp(banner, MatrixMarketBanner, strlen(MatrixMarketBanner)) != 0)
		return 0;

    /* first field should be "mtx" */
	if(strcmp(mtx, MATRIX) != 0)
		return  0;
	A.head[0]='M';

    /* second field describes whether this is a sparse matrix (in coordinate
            storgae) or a dense array */


	if(strcmp(crd, SPARSE) == 0)
		A.head[1]='C';
	else if (strcmp(crd, DENSE) == 0)
		A.head[1]='A';
	else
		return 0;
    

    /* third field */

	if(strcmp(data_type, REAL) == 0)
		A.head[2]='R';
	else if(strcmp(data_type, COMPLEX) == 0)
		A.head[2]='C';
	else if(strcmp(data_type, PATT) == 0)
		A.head[2]='P';
	else if(strcmp(data_type, INT) == 0)
		A.head[2]='I';
	else
		return 0;
    

    /* fourth field */

	if(strcmp(storage_scheme, GENERAL) == 0)
		A.head[3]='G';
	else if(strcmp(storage_scheme, SYMM) == 0)
		A.head[3]='S';
	else if(strcmp(storage_scheme, HERMIT) == 0)
		A.head[3]='H';
	else if(strcmp(storage_scheme, SKEW) == 0)
		A.head[3]='K';
	else
		return 0;
        
	return 0;
}

bool read_mm_size(mat_mar A){
	char buffer[MM_MAX_LINE_LENGTH];
	
	if(!A.head)
		read_mm_head(A);

	do	{
		if(fgets(buffer,MM_MAX_LINE_LENGTH,A.file) == NULL) 
			return 0;
	}	while(buffer[0] == '%');

	if(A.head[1]=='C'){
		if(sscanf(buffer, "%lu %lu %lu", &A.m, &A.n, &A.nz) == 3){
			return 0;
		}
	} else{
		if(sscanf(buffer, "%lu %lu", &A.m, &A.n) == 2){
			return 0;
		}
	}
	return 1;
}

bool read_mm_data(mat_mar A){
	if(is_real(A.head) && is_sparse(A.head))
		read_CCS(A);
	else if(is_real(A.head) && is_dense(A.head))
		read_arr(A);
	return 0;
}

bool read_CCS(mat_mar A){
	dim i,tmp1,tmp2;
	dim count1 = 0, count2 = 0;

	A.dat = (real *) malloc(A.nz*sizeof(real));
	A.I = (dim *) malloc(A.nz*sizeof(dim));
	A.J = (dim *) malloc(((A.n)+1)*sizeof(dim));

	for (i=0; i<A.nz; i++){
		fscanf(A.file, "%lu %lu %lf\n", &A.I[i], &tmp1, &A.dat[i]);
		A.I[i]--;
		if(tmp1 != tmp2){
			A.J[count2] = count1;
			count2++;
			tmp2 = tmp1;
		}
		count1++;
	}
	A.J[A.n] = A.nz;
	return 0;
}

bool read_arr(mat_mar A){
	dim i,j;

	A.dat = (real *) malloc(A.m * A.n  * sizeof(real));
	A.arr = (real **)malloc(A.m * sizeof(real*));
	for(i=0;i<A.m;i++){
		A.arr[i] = &A.dat[i*A.n]; 
		for(j=0;j<A.n;j++){
			if (!fscanf(A.file, "%lf", &A.arr[i][j])) 
        		break;		
		}
	}

	return 0;
}

mat_mar init_mat(char* file){
	mat_mar A;

	A.file = fopen(file,"r");
	
	if(A.file == NULL)
		printf("Could not open file %s",file);

	if(!read_mm_head(A))
		printf("Error");
	if(!read_mm_size(A))
		printf("Error");
	if(!read_mm_data(A))
		printf("Error");
	
	return A;
}

void free_mat(mat_mar A){
	free(A.dat);
	free(A.I);
	free(A.J);
	free(A.arr);
}
