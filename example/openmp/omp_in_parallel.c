#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[])
{
	printf("max threads = %d\n", omp_get_max_threads());
	printf("%d\n", omp_in_parallel());
	omp_set_num_threads(4);
	#pragma omp parallel  
	{
	    // YOUR CODE HERE
	    printf("%d\n", omp_in_parallel());
	    // END OF YOUR CODE
	}
	return 0;
}

