#include <stdio.h>
#include "mm.h"
#include "utils.h"

void decomp1d(dim n, dim p, dim myid, dim *s, dim *e){
    int d,r;
    d = n/p;
    r = n%p;
    if(myid < r){
    	*s = myid*(d+1);
    	*e = *s + d;
    } else {
    	*s = r*(d+1) + (myid-r)*d;
    	*e = *s + (d-1);
    }
}
