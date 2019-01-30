#include <stdio.h>
#include <omp.h>

int g = 0;
#pragma omp threadprivate(g) 
int main(int argc, char* argv[])
{
	int i;
	#pragma omp parallel for   
	for (i = 0; i < 4; i++)
	{
		g = omp_get_thread_num();
		printf("thread %d, g = %d\n", omp_get_thread_num(), g);
	}
	printf("global g: %d\n", g);
	// YOUR CODE HERE
	#pragma omp parallel for copyin(g)
	// END OF YOUR CODE
	for (i = 0; i < 4; i++)
		printf("thread %d, g = %d\n", omp_get_thread_num(), g);
	return 0;
}

