#include <omp.h>
static long num_steps = 100000;
double step; 
#define NUM_THREADS 2 
void main () 
{	  
	  int i; 	  
	  double x, pi, sum[NUM_THREADS]; 
	  step = 1.0/(double) num_steps; 
	  omp_set_num_threads(NUM_THREADS); 
#pragma omp parallel private(i)
{	
		double x;
		int id;
	  	id = omp_get_thread_num();       
		sum[id] = 0; 
		#pragma omp for
	 	for (i=0;i< num_steps; i++){
		 	 x = (i+0.5)*step; 
		  	sum[id] += 4.0/(1.0+x*x); 
	  	} 
	  }
for(i=0, pi=0.0;i<NUM_THREADS;i++)pi += sum[i] * step; 
printf("Pi = %lf\n",pi);
} 
