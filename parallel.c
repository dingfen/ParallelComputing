#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[])
{
	// YOUR CODE HERE
    #pragma omp parallel num_threads(6)
	// END OF YOUR CODE
	{
		printf("Thread: %d\n", omp_get_thread_num());
	}
	return 0;
}
