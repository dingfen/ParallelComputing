
// Using the following command to compile your-c-file.c :
// pgCC -mp -o pi04 your-c-file.c

/*************************************************************************
		Parallelizing an inner loop with dependences
		Forward dependency (pipeline)

		for (iter=0; iter<numiter; iter++) {
			for (i=0; i<size-1; i++) {
				V[i] = f( V[i], V[i-1] );
			}
		}

		Method: Program a threads pipeline
**************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

#include <sys/time.h>

double omp_time() {
  static int sec = -1;  
  struct timeval tv;  
  gettimeofday(&tv, (void *)0);  
  if (sec < 0) sec = tv.tv_sec;  
  return (tv.tv_sec - sec) + 1.0e-6*tv.tv_usec;
}


void loop(int, int, int);



int main(int argc, char *argv[]) {

int nthreads, size, numiter;


double start,finish;

nthreads = omp_get_max_threads();

size = atoi(argv[1]);
numiter = atoi(argv[2]);

start = omp_time();
loop(nthreads, size, numiter);
finish = omp_time();

printf(" %d Threads of %d iterations with %d elements = %f (sec)\n",
                      nthreads, numiter, size, finish-start);

return 0;
}


#define f(x,y)	((x+y)/2.0)

/*
*
* PARALLEL LOOP
*
*/

void loop(int nthreads, int size, int numiter) {
/* VARIABLES */
int i,iter;

int thread;
int limitL, limitR;

int *leftLimit;
int *rightLimit;



/* DECLARE VECTOR AND ANCILLARY DATA STRUCTURES */
double *V=NULL;
double border;

int blockSize = size/nthreads;

V = (double *)malloc(size*sizeof(double));

leftLimit  = (int*)malloc(nthreads*sizeof(int));
rightLimit = (int*)malloc(nthreads*sizeof(int));


for(i = 0; i< nthreads; i++){
   leftLimit [i] = i * blockSize;
   rightLimit[i] = (i+1)*blockSize - 1; 
}

if( size % nthreads ){
   rightLimit[nthreads-1] = size - 1;
}
   

// Initialization of array V

for (i=0; i<size; i++) {
	V[i]= 0.0 + i;
	}


// Parallel Region 

#pragma omp parallel default(none) shared(V,size,nthreads,numiter,leftLimit,rightLimit) private(iter,thread,limitL,limitR,border,i)
{

	thread = omp_get_thread_num();

	limitL = leftLimit [thread];
	limitR = rightLimit[thread];

	for(iter=0; iter<(numiter+nthreads-1); iter++) {

		if (thread != 0) border = V[limitL-1];

#pragma omp 	barrier

		/*  COMPUTE LOCAL UPDATES */
		/*  (ONLY ACTIVE THREADS - CHECK PIPELINE STAGE) */

		if ( thread<=iter && thread>(iter-numiter) ) {

			/* COMPUTE FIRST ELEMENT (EXCEPT THREAD 0) */

			if (thread != 0)
				V[limitL] = f( V[limitL], border );

			/* COMPUTE THE REST OF ELEMENTS */

			for (i=limitL+1; i<=limitR; i++) {
				V[i] = f( V[i], V[i-1] );
				}
			}
		
		/* SYNCHRONIZE BEFORE COPYING UPDATED BORDER ELEMENT  */
#pragma omp 	barrier

	} // end-of-for

} // end-of-parallel-region

} // end-of-main
