#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[])
{
	// YOUR CODE HERE
	#pragma omp parallel sections num_threads(4)
	// END OF YOUR CODE
	{
		#pragma omp section 
		printf("Section 1 ThreadId = %d\n", omp_get_thread_num());
		#pragma omp section
		printf("Section 2 ThreadId = %d\n", omp_get_thread_num());
		#pragma omp section
		printf("Section 3 ThreadId = %d\n", omp_get_thread_num());
		#pragma omp section
		printf("Section 4 ThreadId = %d\n", omp_get_thread_num());
	}
	return 0;
}

