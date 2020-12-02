/*************************************************************************
DESCRIPTION:       
		Parallelizing an inner loop with dependences
		Backward dependency

		for (iter=0; iter<numiter; iter++) {
			for (i=0; i<size-1; i++) {
				V[i] = V[i]+V[i+1];
			}
		}

		Method: Eliminate dependences by duplicating data
		Optimization: None, duplicate the full data

COMMENTS:          

*/

#include<stdio.h>
#include<stdlib.h>
#include<omp.h>


#define VSIZE 40000

/* PROTOYPES */

/* MAIN: PROCESS PARAMETERS */
int main(int argc, char *argv[]) {


int V[VSIZE],oldV[VSIZE],U[VSIZE];



int i;


for (i=0; i<VSIZE; i++) {
	V[i] = U[i] = i;
	}

for (i=0; i<VSIZE; i++) {
	U[i] = U[i] + U[i+1];
	}


#pragma omp parallel for default(none) shared(V,oldV) private(i) schedule(static)
	for (i=0; i<VSIZE; i++) {
		oldV[i] = V[i] ;
		}

	
#pragma omp parallel for default(none) shared(V,oldV) private(i) schedule(static)
	for (i=0; i<VSIZE-1; i++) {
		V[i] = V[i]+oldV[i+1];
		}


for ( i = 0; i<VSIZE;i++) 
    if ( V[i] != U[i] ) printf("V[%d] error\n",i);

}

