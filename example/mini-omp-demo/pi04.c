#include <omp.h> 
static long num_steps = 100000;
double step; 
#define NUM_THREADS 8 
void main () 
{	  int i; 	  
	  double x, pi, sum = 0.0; 
	  step = 1.0/(double) num_steps; 
	  omp_set_num_threads(NUM_THREADS); 
	  #pragma omp parallel for reduction(+:sum) private(x) 
	  for (i=0;i<num_steps; i++){ 
		  x = (i+0.5)*step; 
		  sum = sum + 4.0/(1.0+x*x); 
	  } 
	  pi = step * sum; 
	  printf("Pi = %lf\n",pi);
}
