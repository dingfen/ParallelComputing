#include <stdio.h>
#include <omp.h>

int g = 0;
#pragma omp threadprivate(g)

int main(int argc, char* argv[])
{
	int t = 20, i;
	// YOUR CODE HERE
	#pragma omp parallel num_threads(4)
	// END OF YOUR CODE
	{
		g = omp_get_thread_num();
	}
	#pragma omp parallel num_threads(8)
	{
		printf("thread id: %d g: %d\n", omp_get_thread_num(), g);
	}
	return 0;
}

