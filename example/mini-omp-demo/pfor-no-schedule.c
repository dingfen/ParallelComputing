#include <omp.h>
#define N       100000
int main () 
{
	int i, chunk,id;
	float a[N], b[N], c[N];
	/* Some initializations */
	for (i=0; i < N; i++)  
		a[i] = b[i] = i * 1.0;

	#pragma omp parallel for shared(a,b,c) private(i,chunk,id) 
		for (i=0; i < N; i++)    
			{

			 c[i] = a[i] + b[i];
			
			 chunk = N / omp_get_num_threads(); 
			 id = omp_get_thread_num();
			 
	                 if ( (i%chunk)==0 ) printf("Iteration #%d in  thread #%d\n",i, id);

			}
        return 0;
} 
