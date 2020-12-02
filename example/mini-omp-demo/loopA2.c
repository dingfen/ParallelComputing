/*************************************************************************
DESCRIPTION:       
		Parallelizing an inner loop with dependences
		Backward dependency

		for (iter=0; iter<numiter; iter++) {
			for (i=0; i<size-1; i++) {
				V[i] = f( V[i], V[i+1] );
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

#define THREADS_NUM 4

/* PROTOYPES */

/* MAIN: PROCESS PARAMETERS */
void main() {


int V[VSIZE],U[VSIZE];

int border,size;
int LimitL, LimitR;	
int i,id;


omp_set_num_threads(THREADS_NUM);

size = VSIZE / THREADS_NUM ;

for (i=0; i<VSIZE; i++) {
	V[i] = U[i] = i;
	}

for (i=0; i<VSIZE; i++) {
	U[i] = U[i] + U[i+1];
	}
	
	
#pragma omp parallel default(none) shared(V,size) private(id,LimitL,LimitR,border,i)
	{
	
	id = omp_get_thread_num();

	
	LimitL = id*size;
	LimitR = (id+1)*size-1;
	
	if (id != THREADS_NUM) border = V[LimitR+1];

	
#pragma omp 	barrier

	
		for (i=LimitL; i<LimitR; i++) {
			V[i] = V[i] + V[i+1];
			}
	
		if (id != THREADS_NUM-1)
			V[LimitR] = V[LimitR] + border ;
		
	
#pragma omp 	barrier
	
	}		


for ( i = 0; i<VSIZE;i++) 
    if ( V[i] != U[i] ) printf("V[%d] error\n",i);

}

