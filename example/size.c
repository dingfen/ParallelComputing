#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[])
{
	int i;
	// YOUR CODE HERE
	#pragma omp parallel for schedule(static, 5)
	// END OF YOUR CODE
	for (i = 0; i < 10; i++)
	{
		printf("i = %d, thread %d\n", i, omp_get_thread_num());
	}
	return 0;
}

