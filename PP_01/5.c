#include "util.h"

int _1() {
    float A[101];
    float B[101];
    float C[101];
    float D[101];
    float A2[101];
    float B2[101];
    float C2[101];
    float D2[101];

    random_array_f(A, 101);
    random_array_f(B, 101);
    random_array_f(C, 101);
    random_array_f(D, 101);
    copy_array_f(A2, A, 101);
    copy_array_f(B2, B, 101);
    copy_array_f(C2, C, 101);
    copy_array_f(D2, D, 101);

    omp_set_num_threads(10);

    clock_t start = clock();
    for(int i = 1; i <= 100; i++) {
        B[i] = C[i-1] * 2;
        C[i] = 1 / B[i];
    }
    #pragma omp parallel for
    for(int i = 1; i <= 100; i++) {
        A[i] = A[i] + B[i-1];
        D[i] = C[i] * C[i];
    }
    clock_t end = clock();
    printf("OpenMP costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    start = clock();
    for(int i = 1; i <= 100; i++) {
        A2[i] = A2[i] + B2[i-1];
        B2[i] = C2[i-1] * 2;
        C2[i] = 1 / B2[i];
        D2[i] = C2[i] * C2[i];
    }
    end = clock();
    printf("C costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    return check_ans_f(D, D2, 101);
}

int _2() {
    int *A;
    int *A2;
    int *B;
    int *C;
    int *D;
    int *D2;

    int n = 1000;
    A = malloc(n*sizeof(int));
    A2 = malloc(n*sizeof(int));
    B = malloc(n*sizeof(int));
    C = malloc(n*sizeof(int));
    D = malloc(n*sizeof(int));
    D2 = malloc(n*sizeof(int));
    random_array(A, n);
    random_array(B, n);
    random_array(C, n);
    copy_array(A2, A, n);

    omp_set_num_threads(5);

    clock_t start = clock();
    #pragma omp parallel for
    for(int i = 1; i <= 500; i++) {
        A[i] = B[i] + C[i];
        D[i] = (A[i] + A[1000-i]) / 2;
    }
    #pragma omp parallel for
    for(int i = 501; i <= 999; i++) {
        A[i] = B[i] + C[i];
        D[i] = (A[i] + A[1000-i]) / 2;
    }
    clock_t end = clock();
    printf("OpenMP costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    start = clock();
    for(int i = 1; i <= 999; i++) {
        A2[i] = B[i] + C[i];
        D2[i] = (A2[i] + A2[1000-i]) / 2;
    }
    end = clock();
    printf("C costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    int ans = check_ans(D, D2, n);
    free(A);
    free(A2);
    free(B);
    free(C);
    free(D);
    free(D2);
    return ans;
}

int _3() {
    int *A;
    int *A2;
    int *C;
    int *D;
    int *D2;

    int n = 601 * 201;
    A = malloc(n*sizeof(int));
    A2 = malloc(n*sizeof(int));
    C = malloc(101*101*sizeof(int));
    D = malloc(101*101*sizeof(int));
    D2 = malloc(101*101*sizeof(int));

    random_array(C, 101*101);
    random_array(A, n);
    copy_array(A2, A, n);

    omp_set_num_threads(5);
    clock_t start = clock();
    #pragma omp parallel for
    for(int i = 1; i <= 100; i++) {
        for(int j = 1; j <= 100; j++) {
            A[IN(3*i+2*j+99, 2*j, 201)] = C[IN(i, j, 101)] * 2;
            D[IN(i, j, 101)] = A[IN(i-j+99, i+j, 201)];
        }
    }
    clock_t end = clock();
    printf("OpenMP costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    start = clock();
    for(int i = 1; i <= 100; i++) {
        for(int j = 1; j <= 100; j++) {
            A2[IN(3*i+2*j+99, 2*j, 201)] = C[IN(i, j, 101)] * 2;
            D2[IN(i, j, 101)] = A2[IN(i-j+99, i+j, 201)];
        }
    }
    end = clock();
    printf("C costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    int ans = check_ans(D2, D, 101*101);
    free(A);
    free(A2);
    free(C);
    free(D);
    free(D2);
    return ans;
}

int main() {
    if (!_1())
        printf("Error! 5_1\n");
    else 
        printf("5_1 Done.\n");

    if (!_2())
        printf("Error! 5_2\n");
    else 
        printf("5_2 Done.\n");
    
    if (!_3())
        printf("Error! 5_3\n");
    else 
        printf("5_3 Done.\n");
}