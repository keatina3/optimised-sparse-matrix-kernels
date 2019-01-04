#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mm.h"

bool read_mm_head(mat_mar Mat){
	char buffer[MM_MAX_LINE_LENGTH];
	char banner[MM_MAX_TOKEN_LENGTH];
	char mtx[MM_MAX_TOKEN_LENGTH]; 
	char crd[MM_MAX_TOKEN_LENGTH];
	char data_type[MM_MAX_TOKEN_LENGTH];
	char storage_scheme[MM_MAX_TOKEN_LENGTH];
	char *ptr;

	if(fgets(buffer, MM_MAX_LINE_LENGTH, Mat.file) == NULL) 
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
	Mat.head[0]='M';

    /* second field describes whether this is a sparse matrix (in coordinate
            storgae) or a dense array */


	if(strcmp(crd, SPARSE) == 0)
		Mat.head[1]='C';
	else if (strcmp(crd, DENSE) == 0)
		Mat.head[1]='A';
	else
		return 0;
    

    /* third field */

	if(strcmp(data_type, REAL) == 0)
		Mat.head[2]='R';
	else if(strcmp(data_type, COMPLEX) == 0)
		Mat.head[2]='C';
	else if(strcmp(data_type, PATT) == 0)
		Mat.head[2]='P';
	else if(strcmp(data_type, INT) == 0)
		Mat.head[2]='I';
	else
		return 0;
    

    /* fourth field */

	if(strcmp(storage_scheme, GENERAL) == 0)
		Mat.head[3]='G';
	else if(strcmp(storage_scheme, SYMM) == 0)
		Mat.head[3]='S';
	else if(strcmp(storage_scheme, HERMIT) == 0)
		Mat.head[3]='H';
	else if(strcmp(storage_scheme, SKEW) == 0)
		Mat.head[3]='K';
	else
		return 0;
        
	return 0;
}

bool read_mm_size(mat_mar Mat){
	char buffer[MM_MAX_LINE_LENGTH];
	
	if(!Mat.head)
		read_mm_head(Mat);

	do	{
		if(fgets(buffer,MM_MAX_LINE_LENGTH,Mat.file) == NULL) 
			return 0;
	}	while(buffer[0] == '%');

	if(Mat.head[1]=='C')
		if(sscanf(buffer, "%lu %lu %lu", &Mat.m, &Mat.n, &Mat.nz) == 3)
			return 0;
	else
		if(sscanf(buffer, "%lu %lu", &Mat.m, &Mat.n) == 2)
			return 0;
}

bool read_mm_data(mat_mar Mat){
	if(is_real(Mat.head) && is_sparse(Mat.head))
		read_CCS(Mat);
	else
		return 0;
		//read_array_format
	return 0;
}

bool read_CCS(mat_mar Mat){
	dim i, tmp1,tmp2, count1 = 0, count2 = 0;

	Mat.dat = (real *) malloc(Mat.nz*sizeof(real));
	Mat.I = (dim *) malloc(Mat.nz*sizeof(dim));
	Mat.J = (dim *) malloc(((Mat.n)+1)*sizeof(dim));

	for (i=0; i<Mat.nz; i++){
		fscanf(Mat.file, "%lu %lu %lf\n", &Mat.I[i], &tmp1, &Mat.dat[i]);
		Mat.I[i]--;
		count1++;
		if(tmp1 != tmp2){
			Mat.J[count2] = count1;
			count2++;
		}
	}
	Mat.J[Mat.n] = Mat.nz;
	return 0;
}

mat_mar init_mat(char* file){
	mat_mar Mat;

	Mat.file = fopen(file,"r");
	
	if(Mat.file == NULL)
		printf("Could not open file %s",file);

	if(!read_mm_head(Mat))
		printf("Error");
	if(!read_mm_size(Mat))
		printf("Error");
	if(!read_mm_data(Mat))
		printf("Error");
	
	return Mat;
}

void free_mat(mat_mar Mat){
	free(Mat.dat);
	free(Mat.I);
	free(Mat.J);
	free(Mat.arr);
}
