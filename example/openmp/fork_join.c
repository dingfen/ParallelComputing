#include <stdio.h>
#include <time.h>

void foo()
{
	int cnt = 0;
	clock_t t1 = clock();
	int i;
	for (i = 0; i < 1e8; i++) {
		cnt++;
	}
	clock_t t2 = clock();
	printf("Time = %d\n", t2 - t1);
}

int main(int argc, char* argv[])
{
	clock_t t1 = clock();
	int i;
	// YOUR CODE HERE
#pragma omp parallel for
	// END OF YOUR CODE
	for (i = 0; i < 2; i++) {
		foo();
	}
	clock_t t2 = clock();
	printf("Total time = %d\n", t2 - t1);
	return 0;
}

