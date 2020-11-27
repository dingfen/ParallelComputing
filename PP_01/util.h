#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#include <stdio.h>

#define IN(i, j, LINE) ((i)*(LINE)+(j)) 

void random_array(int *a, int num) {
    srand(time(NULL));
    for(int i = 0; i < num; i++) {
        a[i] = rand() % 100 - 50;
    }
}

void random_array_f(float *a, int num) {
    srand(time(NULL));
    for(int i = 0; i < num; i++) {
        a[i] = (float)rand() / (RAND_MAX);
    }
}

void copy_array(int *dst, int *src, int num) {
    for(int i = 0; i < num; i++) {
        dst[i] = src[i];
    }
}

void copy_array_f(float *dst, float *src, int num) {
    for(int i = 0; i < num; i++) {
        dst[i] = src[i];
    }
}

int check_ans(int *a, int *b, int num) {
    for(int i = 0; i < num; i++) {
        if (a[i] != b[i])
            return 0;
    }
    return 1;
}

int check_ans_f(float *a, float *b, int num) {
    for(int i = 0; i < num; i++) {
        if (fabs(a[i]-b[i]) > 1e-2)
            return 0;
    }
    return 1;
}

void print(int *D, int num) {
    for(int i = 0; i < num; i++)
        printf("%d ", D[i]);
    printf("\n");
}

int min(int a, int b) {
    return a > b ? b : a;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int cmp(const void *a , const void *b) {
  return *(int *)a - *(int *)b;  //升序排序
}
//return *(int *)b - *(int *)a; //降序排序