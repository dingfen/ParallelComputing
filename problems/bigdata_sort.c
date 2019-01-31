#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#define length 1000000

void swap(int *data, int i, int j) {
int temp = data[i];
data[i] = data[j];
data[j] = temp;
}

int partition(int *data, int start, int end) {
    if (start >= end) return 0;
    int pivotValue = data[start];
    int low = start;
    int high = end - 1;
    while (low < high) {
        while (data[low] <= pivotValue && low < end) low++;
        while (data[high] > pivotValue && high > start) high--;
        if (low < high) swap(data, low, high);
    }
    swap(data, start, high);
    return high;
}

void quicksort(int *data, int start, int end) {
    // why not end-start < 1
    if (end-start+1 < 2) return;

    int pivot = partition(data, start, end);
    quicksort(data, start, pivot);
    quicksort(data, pivot+1, end);
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);

    // 随机生成data 数组
    srand(time(0));
    int *data = (int*)malloc(sizeof(int)*length);
    int i;
    for (i=0; i<length/size; i++)
        data[i] = rand();
    
    MPI_Status status;
    if (rank == 0) {
        // 进程0 接收所有的数据
        for (i=1; i<size; i++)
            MPI_Recv(data+i*length/size, length/size, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }
    else {
        MPI_Send(data,  length/size,  MPI_INT,  0,  0,  MPI_COMM_WORLD);
    }

    struct timeval start, end;
    gettimeofday(&start, 0);
    int s;
    int localDataSize =  length;
    int pivot;

    // 数据分割
    for (s=size; s > 1; s /= 2) {
        if (rank % s == 0) {
            pivot = partition(data, 0, localDataSize);
            MPI_Send(data+pivot,  localDataSize  -  pivot,MPI_INT, rank + s/2, 0, MPI_COMM_WORLD);
            localDataSize = pivot;
        }
        else if (rank % s == s/2) {
            MPI_Recv(data,  length,  MPI_INT,  rank  -  s/2,
                    MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status,  MPI_INT,
                    &localDataSize);
        }
    }

    quicksort(data, 0, localDataSize);
    gettimeofday(&end, 0);
    if (rank == 0)
    {
        float  time  =  (end.tv_sec  -  start.tv_sec)  +
            0.000001*(end.tv_usec - start.tv_usec);
        // for(int i = 0;i < length;i++)
        //    printf("%d, ", data[i]);
        printf("Time: %f s\n", time);
    }
    MPI_Finalize();
    return 0;
}