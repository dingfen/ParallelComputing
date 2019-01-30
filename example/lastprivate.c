#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[])
{
	int t = 20, i;
	// YOUR CODE HERE
	#pragma omp parallel for firstprivate(t), lastprivate(t)
	// END OF YOUR CODE
	for (i = 0; i < 5; i++)
	{
		t += i;
		printf("t = %d\n", t);
	}
	printf("outside t = %d\n", t);
	return 0;
}

