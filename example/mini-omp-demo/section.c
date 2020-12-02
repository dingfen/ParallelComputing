#include <omp.h>
#define N     1000
int main (){
	int i,id;
	float a[N], b[N], c[N];
	/* Some initializations */
	for (i=0; i < N; i++)  
		a[i] = b[i] = i * 1.0;
	#pragma omp parallel shared(a,b,c) private(i,id) 
 	    { 
 	    #pragma omp sections nowait    
		{    
		#pragma omp section    
		id = omp_get_thread_num();
		printf("working in Thread %d\n",id);
		for (i=0; i < N/2; i++)      
			c[i] = a[i] + b[i];    
		#pragma omp section    
		id = omp_get_thread_num();
		printf("working in Thread %d\n",id);
		for (i=N/2; i < N; i++)      
			c[i] = a[i] + b[i];    
		}  /* end of sections */ 
 	    } /* end of parallel section */
} 
