#ifndef MM_H
#define MM_H

// ----------------------------------------------------------------------------------//
// INPUT/OUTPUT/MANIPULATION FUNCTIONALITY & DATA STRUCTURES FOR MATRIX MARKET FILES //
// --------------------------------------------------------------------------------- //


#define MM_MAX_LINE_LENGTH 1025
#define MatrixMarketBanner "%%MatrixMarket"
#define MM_MAX_TOKEN_LENGTH 64


// DATA STRUCTURES //

typedef unsigned long dim;		// represents dimensionality of matrices. Can be changed to Int or long long depending.
typedef double real;			// represents type of data in matrix. Can be changed to float etc.
typedef int bool;
typedef char header;			// char[4] filetype for matrix header

// matrix market data structure //
typedef struct
{
	FILE	*file;
	header	head[4];			// contains header code
	dim		m;					// row count
	dim		n;					// col count
	dim		nz;					// #nonzeros
	real	**arr;				// 2d array ptr for dense format
	real	*dat;				// 1d array for data
	dim		*I;					// Ai or row ptr for CCS format
	dim		*J;					// Ap or col ptr for CCS format
}
mat_mar;


// FUNCTIONS //

bool read_mm_head(mat_mar* A);
bool read_mm_size(mat_mar* A);
bool read_mm_data(mat_mar* A);
bool read_CCS(mat_mar* A);
bool read_arr(mat_mar* A);
mat_mar init_mat(char* file);
bool writeMM(mat_mar* A, char* filename);
bool writeCCS(mat_mar* A, char* filename);
bool CCSvectoArr(mat_mar* b, real* x);
mat_mar ArrtoCCS(real* x, dim* nzInd, dim m, dim nz);
mat_mar getLvals(mat_mar* A);
void free_mat(mat_mar* A);
char *mm_strdup(const char* s);
char *mm_typecode_to_str(char* header);


// BOOLEAN TESTS RELATING TO MATRIX HEADER //

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


// STRING DEFINITIONS FOR MATRIX HEADER CODE //

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
