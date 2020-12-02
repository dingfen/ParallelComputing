#include <omp.h>
main()
{
	int x;
	x = 0;
	#pragma omp parallel shared(x) 
	    {  
	    #pragma omp critical   
		   	x = x + 1;  
	}  /* end of parallel section */
		
	printf("out of the parallel region : X = %d\n",x);		
} 


