#include <omp.h> 
int alpha[10], beta[10], i;
#pragma omp threadprivate(alpha) 
int main ()  
{
	/* First parallel region */
	#pragma omp parallel private(i,beta)   
	for (i=0; i < 10; i++)     
		alpha[i] = beta[i] = i;
	/* Second parallel region */
	#pragma omp parallel   
		printf("alpha[3]= %d and beta[3]=%d\n",alpha[3],beta[3]);
} 
