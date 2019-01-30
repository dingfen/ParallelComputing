#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[])
{
    int i;
    // YOUR CODE HERE
	omp_set_dynamic(1);
	printf("%d\n", omp_get_dynamic());
	// END OF YOUR CODE
	#pragma omp parallel for
	for (i = 0; i < 16; i++)
	{
		printf("%d\n", omp_get_thread_num());
	}
	return 0;
}

