#ifndef ROUT_H
#define ROUT_H

//-----------------//
// MATRIX ROUTINES //
//-----------------//

real* lsolve(mat_mar* L, mat_mar* b);
real* lsolve_Par(mat_mar* L, mat_mar* b, levelSet* G, int myid, int nprocs, int root, MPI_Comm comm);

#endif
