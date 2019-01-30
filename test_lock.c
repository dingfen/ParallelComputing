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
		if (omp_test_lock(&lock))
		// END OF YOUR CODE
		{
			printf("%d+\n", omp_get_thread_num());
			printf("%d-\n", omp_get_thread_num());
			omp_unset_lock(&lock);
		}
		else
		{
			printf("fail to get lock\n");
		}
	}
	omp_destroy_lock(&lock);
	return 0;
}

