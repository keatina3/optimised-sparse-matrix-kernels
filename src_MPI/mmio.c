#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mm.h"

bool read_mm_head(mat_mar* A){
	char buffer[MM_MAX_LINE_LENGTH];
	char banner[MM_MAX_TOKEN_LENGTH];
	char mtx[MM_MAX_TOKEN_LENGTH]; 
	char crd[MM_MAX_TOKEN_LENGTH];
	char data_type[MM_MAX_TOKEN_LENGTH];
	char storage_scheme[MM_MAX_TOKEN_LENGTH];
	char *ptr;

	if(fgets(buffer, MM_MAX_LINE_LENGTH, A->file) == NULL) 
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
	A->head[0]='M';

    /* second field describes whether this is a sparse matrix (in coordinate
            storgae) or a dense array */


	if(strcmp(crd, SPARSE) == 0)
		A->head[1]='C';
	else if (strcmp(crd, DENSE) == 0)
		A->head[1]='A';
	else
		return 0;
    

    /* third field */

	if(strcmp(data_type, REAL) == 0)
		A->head[2]='R';
	else if(strcmp(data_type, COMPLEX) == 0)
		A->head[2]='C';
	else if(strcmp(data_type, PATT) == 0)
		A->head[2]='P';
	else if(strcmp(data_type, INT) == 0)
		A->head[2]='I';
	else
		return 0;
    

    /* fourth field */

	if(strcmp(storage_scheme, GENERAL) == 0)
		A->head[3]='G';
	else if(strcmp(storage_scheme, SYMM) == 0)
		A->head[3]='S';
	else if(strcmp(storage_scheme, HERMIT) == 0)
		A->head[3]='H';
	else if(strcmp(storage_scheme, SKEW) == 0)
		A->head[3]='K';
	else
		return 0;
    
	return 1;
}

bool read_mm_size(mat_mar* A){
	char buffer[MM_MAX_LINE_LENGTH];
	
	if(!(*(A->head)))
		read_mm_head(A);

	do	{
		if(fgets(buffer,MM_MAX_LINE_LENGTH,A->file) == NULL) 
			return 0;
	}	while(buffer[0] == '%');
	if(A->head[1]=='C'){
		if(sscanf(buffer, "%lu %lu %lu", &A->m, &A->n, &A->nz) == 3){
			return 1;
		}
	} else{
		if(sscanf(buffer, "%lu %lu", &A->m, &A->n) == 2){
			A->nz = (A->m) * (A->n);
			return 1;
		}
	}
	return 0;
}

bool read_mm_data(mat_mar* A){
	if(is_real(A->head) && is_sparse(A->head))
		return read_CCS(A);
	else if(is_real(A->head) && is_dense(A->head))
		return read_arr(A);
	return 0;
}

bool read_CCS(mat_mar* A){
	dim i,tmp1,tmp2;
	dim count1 = 0, count2 = 0;

	A->dat = (real *) malloc((A->nz)*sizeof(real));
	A->I = (dim *) malloc((A->nz)*sizeof(dim));
	A->J = (dim *) malloc(((A->n) +1)*sizeof(dim));

	for (i=0; i<(A->nz); i++){
		fscanf(A->file, "%lu %lu %lf\n", &(A->I)[i], &tmp1, &(A->dat)[i]);
		(A->I)[i]--;
		if(tmp1 != tmp2){
			(A->J)[count2] = count1;
			count2++;
			tmp2 = tmp1;
		}
		count1++;
	}
	A->J[A->n] = A->nz;
	return 1;
}

bool read_arr(mat_mar* A){
	dim i,j;

	A->dat = (real *) malloc((A->m) * (A->n)  * sizeof(real));
	A->arr = (real **)malloc((A->m) * sizeof(real*));
	for(i=0;i<(A->m);i++){
		A->arr[i] = &(A->dat[i*(A->n)]); 
		for(j=0;j<(A->n);j++){
			if (!fscanf(A->file, "%lf", &A->arr[i][j])) 
        		return 0;		
		}
	}
	return 1;
}

mat_mar init_mat(char* file){
	mat_mar A;

	A.file = fopen(file,"r");
	
	if(A.file == NULL)
		printf("\nCould not open file %s\n\n",file);
	else{
		printf("\nFile opened succesfully:\n");
		printf("%s\n\n",file);
	}
	if(!read_mm_head(&A))
		printf("Error getting header!\n");
	if(!read_mm_size(&A))
		printf("Error getting size!\n");
	if(!read_mm_data(&A))
		printf("Error reading data!\n");
	
	printf("Matrix type: %c%c%c%c\n",A.head[0],A.head[1],A.head[2],A.head[3]);
	printf("Dimensions: %lu, %lu\n",A.m,A.n);
	printf("Non-zero elements: %lu\n\n\n",A.nz);
	
	fclose(A.file);
	return A;
}

bool writeMM(mat_mar* A, char* filename){
	if(is_sparse(A->head))
		writeCCS(A,filename);
	else
		printf("Writing dense format not complete\n");
	
	printf("Succesfull writing to file.\n");
	printf("Matrix type: %c%c%c%c\n",A->head[0],A->head[1],A->head[2],A->head[3]);
	printf("Dimensions: %lu, %lu\n",A->m,A->n);
	printf("Non-zero elements: %lu\n\n\n",A->nz);

	return 1;
}

bool writeCCS(mat_mar* A, char* filename){
	A->file = fopen(filename,"w");
	if(A->file == NULL)
		printf("\nCould not open file %s\n\n",filename);
	else{
		printf("\nFile opened succesfully:\n");
		printf("%s\n\n",filename);
	}

	fprintf(A->file, "%s ", MatrixMarketBanner);
    fprintf(A->file, "%s\n", mm_typecode_to_str(A->head));
	
	fprintf(A->file, "%lu %lu %lu\n",A->m, A->n, A->nz);    /* print values */
   	
	dim i;
	if (is_pattern(A->head)){
    	for (i=0; i<A->nz; i++)
	    	fprintf(A->file, "%lu %lu\n", 1+A->I[i], A->J[i]);
	} else if (is_real(A->head)){
     	for (i=0; i<A->nz; i++){
    	   	fprintf(A->file, "%lu %lu %20.16g\n", 1+A->I[i], 1, A->dat[i]);
		}
    } else if (is_complex(A->head)){
        for (i=0; i<A->nz; i++)
       		fprintf(A->file, "%lu %lu %20.16g %20.16g\n", 1+A->I[i], A->J[i], A->dat[2*i],A->dat[2*i+1]);
	}

	return 1;
}

bool CCSvectoArr(mat_mar* b, real* x){
	dim i;
	for(i=0;i<b->nz;i++)
		x[b->I[i]] = b->dat[i];
	return 1;
}

mat_mar ArrtoCCS(real* x, dim* nzInd, dim m, dim nz){
	mat_mar A;
	
	A.nz = nz;
	A.m = m;
	A.n = 1;
	
	A.head[0]='M';
	A.head[1]='C';
	A.head[2]='R';
	A.head[3]='G';
	
	A.dat = (real*)malloc(A.nz*sizeof(real));
	A.I = (dim*)malloc(A.nz*sizeof(dim));
	A.J = (dim*)malloc(((A.n)+1)*sizeof(dim));
	
	dim i = 0;
	for(i=0;i<nz;i++){
		A.dat[i] = x[nzInd[i]];
		A.I[i] = nzInd[i];
	}
	A.J[0] = 0;
	A.J[1] = nz;

	return A;
}

mat_mar getLvals(mat_mar* A){
	dim count = 0;
	dim i,j;
	mat_mar L;

	for(i=0;i<A->n;i++)
		for(j = (A->J)[i]; j < (A->J)[i+1]; j++)
			if( A->I[j] >= i)
				count++;
	
	for(i=0;i<4;i++)
		L.head[i] = A->head[i];
	L.m = A->m;
	L.n = A->n;
	L.nz = count;
	L.dat = (real*)malloc(count*sizeof(real));
	L.I = (dim*)malloc(count*sizeof(dim));
	L.J = (dim*)malloc(((L.m)+1)*sizeof(dim));
	
	count = 0;
	for(i=0;i<L.n;i++){
		L.J[i] = count;
		for(j = (A->J)[i]; j < (A->J)[i+1]; j++){
			if( A->I[j] >= i){
				L.dat[count] = A->dat[j];
				L.I[count] = A->I[j];
				count++;
			}
		}
	}
	L.J[L.n] = L.nz;

	printf("\nRemoved upper triangular elements from A.\nNow left with matrix L\n\n");
	printf("Matrix type: %c%c%c%c\n",L.head[0],L.head[1],L.head[2],L.head[3]);
	printf("Dimensions: %lu, %lu\n",L.m,L.n);
	printf("Non-zero elements: %lu\n\n\n",L.nz);
	
	return L;
}

void free_mat(mat_mar* A){
	free(A->dat);
	if(is_sparse(A->head)){
		free(A->I);
		free(A->J);
	}
	else
		free(A->arr);
}

char *mm_strdup(const char *s){
	int len = strlen(s);
	char *s2 = (char *) malloc((len+1)*sizeof(char));
	return strcpy(s2, s);
}

char  *mm_typecode_to_str(char* header){
	char buffer[MM_MAX_LINE_LENGTH];
	char *types[4];
	char *mm_strdup(const char *);


    /* check for MTX type */
	if (is_matrix(header))
		types[0] = MATRIX;

    /* check for CRD or ARR matrix */
	if (is_sparse(header))
    	types[1] = SPARSE;
	else if (is_dense(header))
		types[1] = DENSE;
	else
    	return NULL;

    /* check for element data type */
	if (is_real(header))
    	types[2] = REAL;
	else if (is_complex(header))
        types[2] = COMPLEX;
    else if (is_pattern(header))
        types[2] = PATT;
    else if (is_integer(header))
    	types[2] = INT;
	else
    	return NULL;


    /* check for symmetry type */
	if (is_general(header))
		types[3] = GENERAL;
	else if (is_symmetric(header))
		types[3] = SYMM;
	else if (is_hermitian(header))
		types[3] = HERMIT;
    else if (is_skew(header))
		types[3] = SKEW;
	else
		return NULL;

	sprintf(buffer,"%s %s %s %s", types[0], types[1], types[2], types[3]);
	return mm_strdup(buffer);
}
