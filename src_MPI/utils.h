#ifndef _UTILS_H_ 
#define _UTILS_H_

void decomp1d(dim n, dim p, dim myid, dim *s, dim *e);
void assignCols(dim* colInd, dim* colCount, int myid, int nprocs, int root, levelSet* G, dim level, MPI_Comm communicator);
void BcastMatrix(mat_mar* L, int root, int myid, MPI_Comm communicator);

#endif
