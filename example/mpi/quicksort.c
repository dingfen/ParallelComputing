#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>

int cmp(const void *a,const void *b)
{
	return *(int*)a-*(int*)b;
}


void swap(int *a,int *b)
{
	int tmp=*a;
	*a=*b;
	*b=tmp;
}


int partition(int *buf,int n)
{
	if(n == 0) return 0;
	--n;
	int pri = buf[n];       // privot buf[n-1]
	int i = -1;
	int j = 0;
	while(j < n)
	{
		if(buf[j] >= pri) 
        {
            ++j;
            continue;
        }
		++i;
		swap(&buf[i],&buf[j]);
		++j;
	}
	++i;
	swap(&buf[i],&buf[n]);
	return i;
}


int main(int argc,char **argv)
{
	MPI_Init(&argc,&argv);
	int rank,size;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	int n = 10;
	int i;
	
    int *buf = (int*)malloc(n*sizeof(int));
	if(buf == NULL)
	{
		printf("%d malloc failed\n");
		exit(1);
	}
	
    int *ofss=NULL;
	int *counts=NULL;
	int *retbuf=NULL;
	
    if(rank == 0)
	{
        // 打印未排序的数组
		int *var = (int*)malloc(n*sizeof(int));
		srand((int)time(0));
		for(i = 0;i < n; ++i)
		{
			buf[i] = (int)rand() % n;
			var[i] = buf[i];
			printf("%d\t",buf[i]);
		}
		printf("\n");

		qsort(var,n,sizeof(int),cmp);
		free(var);

		ofss = (int*)malloc(size*sizeof(int));
		counts = (int*)malloc(size*sizeof(int));
		retbuf = (int*)malloc(n*sizeof(int));
		
        if(ofss==NULL || counts==NULL || retbuf==NULL)
		{
			printf("malloc failed\n");
			exit(2);
		}
	}

	int mod=1;
	int sz=1;
	int block=n;
	int pos=0;
	MPI_Status st;

	while(1)
	{
		if(rank >= sz)
		{
			sz <<= 1;
			if(sz > size) sz = size;
			mod <<= 1;
			continue;
		}
		if(rank != 0 && rank-(mod>>1)>=0)
		{
			MPI_Recv(&pos, 1, MPI_INT, rank-(mod>>1), 1, MPI_COMM_WORLD, &st);
			MPI_Recv(&block, 1, MPI_INT, rank-(mod>>1), 2, MPI_COMM_WORLD, &st);
			MPI_Recv(buf+pos, block, MPI_INT, rank-(mod>>1), 3, MPI_COMM_WORLD, &st);
		}
		if(rank+mod >= size)
		{
			qsort(buf+pos,block,sizeof(int),cmp);
			break;
		}

		int p = partition(buf+pos,block)+pos;
		int tb=block-p+pos;
		
        MPI_Send(&p ,1, MPI_INT, rank+mod,1,MPI_COMM_WORLD);
		MPI_Send(&tb, 1, MPI_INT, rank+mod,2,MPI_COMM_WORLD);
		MPI_Send(buf+p, tb, MPI_INT, rank+mod,3,MPI_COMM_WORLD);
		
        block=p-pos;
		sz<<=1;
		if(sz>size) sz=size;
		mod<<=1;
	}
	MPI_Gather(&block, 1, MPI_INT, counts, 1, MPI_INT,0,MPI_COMM_WORLD);
	MPI_Gather(&pos, 1, MPI_INT, ofss,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Gatherv(buf+pos,block,MPI_INT,retbuf,counts,ofss,MPI_INT,0,MPI_COMM_WORLD);
	
    if(rank==0)
	{
		for(i=0;i<n;++i)
			printf("%d\t",retbuf[i]);
		printf("\n");
		free(counts);
		free(ofss);
		free(retbuf);
	}
	free(buf);
	MPI_Finalize();
	return 0;
}