#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[])
{
	int t = 20, i;
	// YOUR CODE HERE
    #pragma omp parallel for shared(t)
	// END OF YOUR CODE
	for (i = 0; i < 10; i++)
	{
		if (i % 2 == 0)
			t++;
		printf("i = %d, t = %d\n", i, t);
	}
	return 0;
}

