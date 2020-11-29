#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[])
{
	printf("%d\n", omp_get_thread_num());
	#pragma omp parallel  
	{	    
	    // YOUR CODE HERE
	    printf("%d\n", omp_get_thread_num());
	    // END OF YOUR CODE
	}
	return 0;
}

