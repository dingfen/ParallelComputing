#include "util.h"
#include <stdio.h>

#ifndef N
#define N 20
#endif


#ifndef M
#define M 20
#endif

int _1() {
    int n = (M+2)*N;
    int A[n];
    int B[n];
    int C = 2;
    
    random_array(A, n);
    copy_array(B, A, n);
    omp_set_num_threads(8);

    clock_t start = clock();
    for(int i = 1; i <= M; i++) {
        #pragma omp parallel for
        for (int j = 1; j <= N; j++) {
            A[IN(i+1,j+1,N)] = A[IN(i,j,N)]+C;
        }
    }
    clock_t end = clock();
    printf("OpenMP costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    start = clock();
    for(int i = 1; i <= M; i++) {
        for (int j = 1; j <= N; j++) {
            B[IN(i+1,j+1,N)] = B[IN(i,j,N)]+C;
        }
    }
    end = clock();
    printf("C costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    return check_ans(A, B, n);
}

int _2() {
    int X[101];
    int X2[101];
    int Y[201];
    int Y2[201];
    int B[101];
    int B2[101];
    int *A, *C;
    int *A2;

    int n = 110*110;
    A = malloc(n*sizeof(int));
    A2 = malloc(n*sizeof(int));
    C = malloc(n*sizeof(int));
    random_array(A, n);
    random_array(C, n);
    random_array(Y, 201);
    copy_array(A2, A, n);
    copy_array(Y2, Y, 201);

    omp_set_num_threads(10);
    clock_t start = clock();
    
    for(int i = 1; i <= 100; i++) {
        for(int j = 1; j <= 100; j++) {
            B[j] = A[IN(i,N,110)];
            #pragma omp parallel for
            for(int k = 1; k <= 100; k++) {
                A[IN(j+1, k, 110)] = B[j] + C[IN(j, k, 110)];
            }
            Y[i+j] = A[IN(j+1, N, 110)];
        }
    }
    #pragma omp parallel for
    for(int i = 1; i <= 100; i++)
        X[i] = Y[i] + 10;

    clock_t end = clock();
    printf("OpenMP costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);


    start = clock();
    for(int i = 1; i <= 100; i++) {
        X2[i] = Y2[i] + 10;
        for(int j = 1; j <= 100; j++) {
            B2[j] = A2[IN(i,N,110)];
            for(int k = 1; k <= 100; k++) {
                A2[IN(j+1, k, 110)] = B2[j] + C[IN(j, k, 110)];
            }
            Y2[i+j] = A2[IN(j+1, N, 110)];
        }
    }
    end = clock();
    printf("C costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    free(A);
    free(A2);
    free(C);
    return check_ans(X+1, X2+1, 100);
}

int main()
{
    if (!_1())
        printf("Error! 2_1\n");
    else 
        printf("2_1 Done.\n");

    if (!_2())
        printf("Error! 2_2\n");
    else 
        printf("2_2 Done.\n");
}
