#include <stdio.h>
#include <stdlib.h>
#include "mm.h"

int main(int argc, char* argv[]){
	char* address = "/home/alex/OneDrive/College/PhDs/Coding_task/torso1/torso1.mtx";
	mat_mar A;

	A = init_mat(address);
	printf("%c%c%c%c\n",A.head[0],A.head[1],A.head[2],A.head[3]);
	printf("Dimensions of A: %lu,%lu,%lu\n",A.m,A.n,A.nz);	
	return 0;
}
