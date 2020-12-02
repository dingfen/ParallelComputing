#include <omp.h>
main()
{
	int x;
	x = 0;
	#pragma omp parallel shared(x) 
	    {  
	      #pragma omp single    
	      { 
	        int id = omp_get_thread_num();
	        printf("I am thread #%d\n",id);
	       	x = x + 1;
	      }
	}  /* end of parallel section */
		
	printf("out of the parallel region : X = %d\n",x);		
} 


