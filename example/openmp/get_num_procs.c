#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[])
{
	printf("the num of procs = %d\n", omp_get_num_procs());
	printf("the num of threads = %d\n", omp_get_num_threads());
	#pragma omp parallel  
	{
	    // YOUR CODE HERE
	    printf("%d\n", omp_get_num_procs());
	    // END OF YOUR CODE
	}
	return 0;
}

