#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[])
{
	
	int i;
	long sum = 10;
	// YOUR CODE HERE
	#pragma omp parallel for reduction(+: sum)
	// END OF YOUR CODE
	for (i = 0; i < 10; i++)
	{
		sum += i;
		printf("%ld\n", sum);
	}
	printf("sum = %ld\n", sum);
	return 0;
}

