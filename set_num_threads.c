#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[])
{
    // YOUR CODE HERE
    omp_set_num_threads(10);
    // END OF YOUR CODE
	#pragma omp parallel  
	{
		printf("%d of %d threads\n", omp_get_thread_num(), omp_get_num_threads());
	}
	return 0;
}

