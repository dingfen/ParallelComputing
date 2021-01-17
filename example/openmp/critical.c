#include <omp.h>
#include <stdio.h>


int main()
{
	int x;
	x = 0;
    omp_set_num_threads(10);
	#pragma omp parallel shared(x)   
	    {  
	    #pragma omp critical   
		   	x = x + 1;  
		}  /* end of parallel section */
    
    printf("x = %d\n", x);
}