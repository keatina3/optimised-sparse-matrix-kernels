#ifndef _UTILS_H_ 
#define _UTILS_H_

void decomp1d(dim n, dim p, dim myid, dim *s, dim *e);
dim* assignCols(dim* colCount, int myid, int nprocs, int root, levelSet* G, dim level, MPI_Comm communicator);
void BcastMatrix(mat_mar* L, int myid, int root, MPI_Comm communicator);
void levelSync(mat_mar* L, real* x, levelSet* G, dim, dim level, int myid, int root, MPI_Comm comm);

#endif
