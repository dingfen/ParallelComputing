#include<stdio.h>
#include<omp.h>

#define NUM_THREAD 4

int main()
{
	int i;
	double pi=0.0;
	
	long num_steps = 100000;
	double step = 1.0/num_steps;
	
	for(i=0;i<num_steps;i++)
	{
		double x = (i+0.5)*step;
		pi += 4.0/(1.0+x*x);
	}
	printf("%.12lf\n",pi*step);

	// 第一种方法 计算pi 使用并行域 parallel
	omp_set_num_threads(NUM_THREAD);
	double x;
	double sum[NUM_THREAD];
	pi=0.0;
#pragma omp parallel private(x,i) 
	{
		int id = omp_get_thread_num();
		for(i=id,sum[id]=0.0;i<num_steps;i=i+NUM_THREAD)
		{
			x = (i+0.5)*step;
			sum[id] += 4.0/(1.0+x*x);
		}
	}
	for(i=0;i<NUM_THREAD;i++)
		pi+=step*sum[i];
	printf("%.12lf\n",pi);

	// 第二种方法 计算pi 使用 for reduction(+:pi)
	pi = 0.0;
#pragma omp parallel for reduction(+:pi) private(x)
	for(i=0;i<num_steps;i++)
	{
		x = (i+0.5)*step;
		pi += 4.0/(1.0+x*x);
	}

	printf("%.12lf\n",pi*step);


	// 第三种方法 并行域中加上 #pragma omp for
#pragma omp parallel private(x,i)
	{   
    int id = omp_get_thread_num();       
    sum[id] = 0; 
    #pragma omp for
    for (i=0;i< num_steps; i++)
	{
    	x = (i+0.5)*step; 
        sum[id] += 4.0/(1.0+x*x); 
    } 
    }
	for(i=0, pi=0.0;i<NUM_THREAD;i++)
		pi += sum[i] * step; 
	printf("%.12lf\n",pi);

	// 第四种方法 使用critical 
	double s;
	pi = 0.0;
#pragma omp parallel private (x,i,s) 
    {   
        int id = omp_get_thread_num(); 
        for (i=id,s=0.0;i< num_steps;i=i+NUM_THREAD){ 
            x = (i+0.5)*step; 
            s += 4.0/(1.0+x*x);
        } 
        //critical指定某一区域的代码，每次只能同时被一个线程执行。
        #pragma omp critical
            pi += s*step; 

     }
    printf("%.12lf\n",pi);

}

