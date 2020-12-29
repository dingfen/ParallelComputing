#ifndef _MPI_SORT_H
#define _MPI_SORT_H

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define INDEX(i, j, n) (((i)*(n))+(j))

#define SURCLK(x) \
  do {                  \
  clock_t t1 = clock(); \
  x                     \
  clock_t t2 = clock(); \
  printf("Time: %lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);  \
  }while(0);


void random_array(int *a, int num) {
    for(int i = 0; i < num; i++) {
        srand(clock());
        a[i] = rand() % 100;
    }
}

void print_array(int *a, int num) {
    for(int i = 0; i < num; i++) {
        if (i % 10 == 0)
            printf("\n");
        printf("%d ", a[i]);
    }
}

int check_array(int *B, int *C, int num) {
    for(int i = 0; i < num; i++) {
        if (B[i] != C[i]) {
            printf("A[%d] = %d not %d!\n", i, B[i], C[i]);
            return 0;
        }
    }
    return 1;
}

int compare(const void *arg1, const void *arg2) {
    return *(int*)arg1 >= *(int*)arg2;
}

#endif // _MPI_SORT_H
