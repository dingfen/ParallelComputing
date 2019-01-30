#include <stdio.h>
#include <omp.h>

static omp_lock_t lock;

int main(int argc, char* argv[])
{
    int i;
	omp_init_lock(&lock); 
	#pragma omp parallel for   
	for (i = 0; i < 5; ++i)
	{
	    // YOUR CODE HERE
		omp_set_lock(&lock);
		// END OF YOUR CODE
		printf("%d+\n", omp_get_thread_num());
		printf("%d-\n", omp_get_thread_num());
		// YOUR CODE HERE
		omp_unset_lock(&lock); 
		// END OF YOUR CODE
	}
	omp_destroy_lock(&lock);
	return 0;
}

