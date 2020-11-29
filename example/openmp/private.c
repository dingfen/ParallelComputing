#include <stdio.h>
#include <omp.h>


int main(int argc, char* argv[])
{
	int i = 20;
	int x = 2;
	int y = 2;
	// YOUR CODE HERE
	#pragma omp parallel for private(i,y)
	// END OF YOUR CODE
	for (i = 0; i < 10; i++)
	{
		printf("i = %d\n", i);
		x = i;
		y = i;
		printf("x = %d\n", x);
		printf("y = %d\n", y);
	}
	printf("outside i = %d\n", i);
	printf("outside x = %d\n", x);
	printf("outside y = %d\n", y);
	return 0;
}


