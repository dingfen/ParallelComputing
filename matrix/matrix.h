#ifndef _MPI_SORT_H
#define _MPI_SORT_H

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#define INDEX(i, j, n)     (((i)*(n))+(j))

#define SURCLK(x) \
  do {                  \
  clock_t t1 = clock(); \
  x                     \
  clock_t t2 = clock(); \
  printf("Time: %lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);  \
  }while(0);


void random_mat(int *a, int num) {
    for(int i = 0; i < num; i++) {
        srand(clock());
        for(int j = 0; j < num; j++) {
            a[INDEX(i, j, num)] = rand() % 100;
        }
    }
}

void print_mat(int *a, int num, int id) {
    for(int i = 0; i < num; i++) {
        for(int j = 0; j < num; j++) {
            printf("|%d :  %d ", id, a[INDEX(i, j, num)]);
        }
        printf("\n");
    }
}

int check_mat(int *C, int *nC, int num) {
    for(int i = 0; i < num; i++) {
        for(int j = 0; j < num; j++) {
            if (C[INDEX(i, j, num)] != nC[INDEX(i, j, num)]) {
                printf("C[%d,%d] should be %d ,not %d\n", 
                    i,j,C[INDEX(i,j,num)],nC[INDEX(i,j,num)]);           
                return 0;
            }
        }
    }
    return 1;
}

int compare(const void *arg1, const void *arg2) {
    return *(int*)arg1 >= *(int*)arg2;
}

void matrix_multi(int *A, int *B, int *C, int num) {
    for(int i = 0; i < num; i++) {
        for(int j = 0; j < num; j++) {
            for(int k = 0; k < num; k++)
                C[INDEX(i, j, num)] += A[INDEX(i, k, num)] * B[INDEX(k, j, num)];
        }
    }
}


#endif // _MPI_SORT_H
