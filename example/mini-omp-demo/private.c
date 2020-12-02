#include <omp.h>

int alpha[10],beta[10],i;
#pragma omp threadprivate(alpha)

main()
{
/* first parallel region */
#pragma omp parallel private(i,beta)
{   int id ;
    id = omp_get_thread_num();
    
    for(i=0;i<10;i++)
        alpha[i] = beta[i] = id * i;
}
/* second parallel region */
#pragma omp parallel
    printf("I am thread %d :alpha[3] = %d and beta[3] = %d\n", omp_get_thread_num(),alpha[3],beta[3]);
    
}
