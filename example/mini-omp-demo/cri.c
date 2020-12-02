#include <omp.h>
main()
{
	int x;
        int i,id; 
	x = 0;
	#pragma omp parallel shared(x) private(i,id) 
	    { 
//		#pragma omp critical
		{	          
	    	  id = omp_get_thread_num();
    	          printf("before thread %d : X = %d\n",id,x);
 		  for(i=0;i<3000000;i++) x = x + 1;
	          printf("after  thread %d : X = %d\n",id,x);

		}
	}  /* end of parallel section */
		
	printf("out of the parallel region : X = %d\n",x);		
} 


