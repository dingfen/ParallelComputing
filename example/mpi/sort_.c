#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
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


int main(int argc, char const *argv[])
{
    srand(time(0));
    int *data = (int*)malloc(sizeof(int)*length);
    int i;
    for (i=0; i<length ; i++)
        data[i] = rand();

    struct timeval start, end;
    gettimeofday(&start, 0);
    quicksort(data, 0, length);
    gettimeofday(&end, 0);
    float  time  =  (end.tv_sec  -  start.tv_sec)  +
        0.000001*(end.tv_usec - start.tv_usec);
    // for(int i = 0;i < length;i++)
    //    printf("%d, ", data[i]);
    printf("Time: %f s\n", time);
}
