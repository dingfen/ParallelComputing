#include <omp.h>
static long num_steps = 100000;
double step;
#define NUM_THREADS 8 
void main () 
{	  
	int i,id; 	  
	double x, sum, pi=0.0; 
	step = 1.0/(double) num_steps; 
	omp_set_num_threads(NUM_THREADS); 
	#pragma omp parallel private (x,i,sum) 
	{	
		id = omp_get_thread_num(); 
	  	for (i=id,sum=0.0;i< num_steps;i=i+NUM_THREADS){ 
		  	x = (i+0.5)*step; 
		  	sum += 4.0/(1.0+x*x); 
	  	} 
		#pragma omp critical
	  		pi += sum*step; 

//		#pragma omp barrier
			  		
//	  	#pragma omp master
//  	          printf("Pi = %lf\n",pi);
  	 }
	printf("Pi = %lf\n",pi);

}
