#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mm.h"

bool read_CCS(mat_mar mat){

}

bool read_mm_head(mat_mar mat){
{
	char buffer[MM_MAX_LINE_LENGTH];
	char banner[MM_MAX_TOKEN_LENGTH];
	char mtx[MM_MAX_TOKEN_LENGTH]; 
	char crd[MM_MAX_TOKEN_LENGTH];
	char data_type[MM_MAX_TOKEN_LENGTH];
	char storage_scheme[MM_MAX_TOKEN_LENGTH];
	char *ptr;

    if (fgets(buffer, MM_MAX_LINE_LENGTH, mat.file) == NULL) 
        return 0;

    if (sscanf(buffer, "%s %s %s %s %s", banner, mtx, crd, data_type, storage_scheme) != 5)
        return 0

    for (p=mtx; *p!='\0'; *p=tolower(*p),p++);  /* convert to lower case */
    for (p=crd; *p!='\0'; *p=tolower(*p),p++);  
    for (p=data_type; *p!='\0'; *p=tolower(*p),p++);
    for (p=storage_scheme; *p!='\0'; *p=tolower(*p),p++);

    /* check for banner */
    if (strncmp(banner, MatrixMarketBanner, strlen(MatrixMarketBanner)) != 0)
        return 0;

    /* first field should be "mtx" */
    if (strcmp(mtx, MATRIX) != 0)
        return  0;
	*mat.header[0]='M';

    /* second field describes whether this is a sparse matrix (in coordinate
            storgae) or a dense array */


    if (strcmp(crd, SPARSE) == 0)
        *mat.header[1]='C';
    else if (strcmp(crd, MM_DENSE_STR) == 0)
		*mat.header[1]='A'
    else
        return 0;
    

    /* third field */

    if (strcmp(data_type, REAL) == 0)
        *mat.header[2]='R';
    else if (strcmp(data_type, COMPLEX) == 0)
        *mat.header[2]='C';
    else if (strcmp(data_type, PATTERN) == 0)
        *mat.header[2]='P';
    else if (strcmp(data_type, INTEGER) == 0)
        *mat.header[2]='I';
    else
        return 0;
    

    /* fourth field */

    if (strcmp(storage_scheme, GENERAL) == 0)
    	*mat.header[3]='G';
    else if (strcmp(storage_scheme, SYMM) == 0)
        *mat.header[3]='S';
    else if (strcmp(storage_scheme, HERM) == 0)
        *mat.header[3]='H';
    else if (strcmp(storage_scheme, SKEW) == 0)
        *mat.header[3]='K'
    else
        return 0;
        

    return 0;
 
}

bool read_mm_size(mat_mar mat){
	char buffer[MM_MAX_LINE_LENGTH];
	
	if(!mat.header)
		read_mm_head(mat);

	do {
        if (fgets(buffer,MM_MAX_LINE_LENGTH,mat.f) == NULL) 
            return 0;
    }	while (buffer[0] == '%');

	if(mat.header[1]=='C')
		if (sscanf(buffer, "%lu %lu %lu", mat.m, mat.n, mat.nz) == 3)
        	return 0;
	else
		if (sscanf(buffer, "%lu %lu %lu", mat.m, mat.n) == 2)
        	return 0;
}

bool read_mm_data(mat_mar mat){

}

mat_mar init_mat(char* file){

}
