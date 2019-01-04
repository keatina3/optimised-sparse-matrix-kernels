#ifndef MM_H
#define MM_H

#define MM_MAX_LINE_LENGTH 1025
#define MatrixMarketBanner "%%MatrixMarket"
#define MM_MAX_TOKEN_LENGTH 64

typedef unsigned long dim;
typedef double real;
typedef char header[4];
typedef int bool;
typedef struct
{
	FILE* file;
	header head;
	dim m;
	dim n;
	unsigned long nz;
	real **arr;
	real* dat;
	dim* I;
	dim* J;
}
mat_mar;




bool read_CCS(mat_mar mat);
bool read_mm_head(mat_mar mat);
bool read_mm_size(mat_mar mat);
bool read_mm_data(mat_mar mat);
mat_mar init_mat(char* file);

double* lsolve(mat_mar mat);




#define is_matrix(header)		((header)[0]=='M')

#define is_sparse(header)		((header)[1]=='C')
#define is_coordinate(header)	((header)[1]=='C')
#define is_dense(header)		((header)[1]=='A')
#define is_array(header)		((header)[1]=='A')

#define is_complex(header)		((header)[2]=='C')
#define is_real(header)			((header)[2]=='R')
#define is_pattern(header)		((header)[2]=='P')
#define is_integer(header) 		((header)[2]=='I')

#define is_symmetric(header)	((header)[3]=='S')
#define is_general(header)		((header)[3]=='G')
#define is_skew(header)			((header)[3]=='K')
#define is_hermitian(header)	((header)[3]=='H')




#define MATRIX	"matrix"

#define DENSE	"array"
#define SPARSE	"coordinate" 

#define COMPLEX	"complex"
#define REAL	"real"
#define INT		"integer"

#define GENERAL	"general"
#define SYMM	"symmetric"
#define HERMIT	"hermitian"
#define SKEW	"skew-symmetric"
#define PATT	"pattern"

#endif
