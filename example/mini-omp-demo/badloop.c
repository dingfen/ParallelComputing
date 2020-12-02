/*

DESCRIPTION:
		Parallelizing an inner loop with dependences
		Backward dependency

		 
			for (i=0; i<VSIZE-1; i++) {
				V[i] = ( V[i] + V[i+1] ) / 2;
			}
		 

		Method: Try direct parallelization with PARALLEL FOR
		Result: NON-CORRECT!!!

*/

#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

#define NUM_THREADS 4 

#define VSIZE  100

void main()
{  
   int V[ VSIZE+1 ],i,U[VSIZE+1];
   for (i=0; i<VSIZE+1; i++) {
	V[i]=  i ;
        U[i] = i ;
	}
   for(i=0;i<VSIZE;i++) U[i] = U[i] + U[i+1];

   printf("Before Parallel Loop\n");
/*   for( i = 0 ; i<10;i++) printf("V[%d]=%d ",i,V[i]); */
   printf("\n");   

   omp_set_num_threads(NUM_THREADS);

#pragma omp parallel for default(none) shared(V) private(i) schedule(static)
	for (i=0; i<VSIZE; i++) {
		V[i] = V[i] + V[i+1];
		}

   printf("After Parallel Loop\n");	
/*   for( i = 0 ; i<10;i++) printf("V[%d]=%d ",i,V[i]); */
   printf("\n");

   for( i=0;i<VSIZE;i++)
     if ( V[i] != U[i] ) printf("Found V[%d] error\n", i);

}
