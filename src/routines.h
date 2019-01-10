#ifndef ROUT_H
#define ROUT_H

//-----------------//
// MATRIX ROUTINES //
//-----------------//

real* lsolve(mat_mar* L, mat_mar* b);
real* lsolve_GP(mat_mar* L, mat_mar* b, Graph* graph);

#endif
