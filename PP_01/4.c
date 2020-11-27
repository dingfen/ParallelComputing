#include "util.h"

int _1() {
    int A[128];
    int B[128];

    random_array(A, 128);
    copy_array(B, A, 128);

    omp_set_num_threads(4);
    clock_t start = clock();
    // diagonal parallel 
    for(int i = 4; i <= 20; i++) {
        #pragma omp parallel for
        for(int j = max(2, i-10); j <= min(i/2, 10); j++) {
            A[IN(j, i-j, 11)] = (A[IN(j, i-j-1, 11)] + A[IN(j-1, i-j, 11)])*0.5;
        }
    }
    clock_t end = clock();
    printf("OpenMP costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    start = clock();
    for(int i = 2; i <= 10; i++) {
        for(int j = i; j <= 10; j++) {
            B[IN(i, j, 11)] = (B[IN(i, j-1, 11)] + B[IN(i-1, j, 11)])*0.5;
        }
    }
    end = clock();
    printf("C costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    return check_ans(A, B, 128);
}

int _2() {
    int A[20];
    int A2[20];
    int B[20];

    random_array(A, 20);
    random_array(B, 20);
    copy_array(A2, A, 20);

    omp_set_num_threads(3);
    clock_t start = clock();
    for(int k = 1; k <= 16; k+=3) {
        #pragma omp parallel for
        for(int i = k; i <= min(16, k+2); i++) {
            A2[i+3] = A2[i] + B[i];
        }
    }
    clock_t end = clock();
    printf("OpenMP costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    start = clock();
    for(int i = 1; i <= 16; i++) {
        A[i+3] = A[i] + B[i];
    }
    end = clock();
    printf("C costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    return check_ans(A, A2, 20);
}


int main() {
    if (!_1())
        printf("Error! 4_1\n");
    else 
        printf("4_1 Done.\n");

    if (!_2())
        printf("Error! 4_2\n");
    else 
        printf("4_2 Done.\n");

    printf("4_3 is the same as 4_2\n");
}