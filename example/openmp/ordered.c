#include <stdio.h>
#include <omp.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    #pragma omp parallel num_threads(8)
    {
        #pragma omp for ordered
        for(int i=0; i<10; ++i){
            #pragma omp critical
                printf("%d ", i);
            #pragma omp ordered
            {
                #pragma omp critical
                    printf("-%d ", i);
            }
        }
    }
    printf("\n");
	return 0;
}
