#include <omp.h>
#define N       100000
#define CHUNKSIZE   1000
int main () {
	int i, chunk;
	float a[N], b[N], c[N];
	/* Some initializations */
	for (i=0; i < N; i++)  
		a[i] = b[i] = i * 1.0;
	chunk = CHUNKSIZE;
	#pragma omp parallel for \
		shared(a,b,c,chunk) private(i) \
		schedule(static,chunk)
		for (i=0; i < N; i++)    
			{
			 int id; 

			 c[i] = a[i] + b[i];

			 id = omp_get_thread_num();
			 if (  (i % chunk) == 0 )   printf("Iteration #%d in thread #%d\n",i, id);

			}
} 
