#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[])
{
	#pragma omp parallel 
	{
	    int i, j;
		// YOUR CODE HERE
        #pragma omp for
		// END OF YOUR CODE
		for (i = 0; i < 5; i++)
			printf("i = %d\n", i);
		// YOUR CODE HERE
        #pragma omp for
		// END OF YOUR CODE
		for (j = 0; j < 5; j++)
			printf("j = %d\n", j);
	}
	return 0;
}

