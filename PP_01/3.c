#include "util.h"

int _1() {
    int *A;
    int *B;
    int n = 310*110;
    A = malloc(n*sizeof(int));
    B = malloc(n*sizeof(int));
    random_array(A, n);
    copy_array(B, A, n);
    omp_set_num_threads(10);

    clock_t start = clock();
    for(int i = 1; i <= 100; i++) {
        for (int j = 1; j <= 50; j++) {
            A[IN(3*i+2, 2*j-1, 110)] = A[IN(5*j,i+3,110)]+2;
        }
    }
    clock_t end = clock();
    printf("OpenMP costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    start = clock();
    for(int i = 1; i <= 100; i++) {
        for (int j = 1; j <= 50; j++) {
            B[IN(3*i+2, 2*j-1, 110)] = B[IN(5*j,i+3,110)]+2;
        }
    }
    end = clock();
    printf("C costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    int ans = check_ans(A, B, n);
    free(A);
    free(B);
    return ans;
}

int _2() {
    int x;
    int y = rand() % 100 -50;
    int z = rand() % 100 -50;
    int z2 = z;
    int B[101];
    int A[101];
    int A2[101];
    int C[102];
    int C2[102];
    int *D;
    int *D2;

    int n = 101*51;
    D  = malloc(n*sizeof(int));
    D2 = malloc(n*sizeof(int));
    random_array(A, 101);
    random_array(B, 101);
    random_array(C, 102);
    random_array(D, n);
    copy_array(A2, A, 101);
    copy_array(C2, C, 102);
    copy_array(D2, D, n);

    omp_set_num_threads(10);

    clock_t start = clock();
    x = y * 2;
    for(int i = 1; i <= 100; i++) {
        C[i] = B[i] + x;
        A[i] = C[i-1] + z;
        C[i+1] = A[i] * B[i];
        for(int j = 1; j <= 50; j++) {
            D[IN(i, j, 51)] = D[IN(i, j-1, 51)] + x;
        }
    }
    z = y + 4;
    clock_t end = clock();
    printf("OpenMP costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    start = clock();
    x = y * 2;
    for(int i = 1; i <= 100; i++) {
        C2[i] = B[i] + x;
        A2[i] = C2[i-1] + z2;
        C2[i+1] = A2[i] * B[i];
        for(int j = 1; j <= 50; j++) {
            D2[IN(i, j, 51)] = D2[IN(i, j-1, 51)] + x;
        }
    }
    z2 = y + 4;
    end = clock();
    printf("C costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);


    int ans = check_ans(A2, A, 101);
    ans &= check_ans(C2, C, 102);
    ans &= check_ans(D2, D, n);
    free(D);
    free(D2);
    return ans;
}

int main() {
    if (!_1())
        printf("Error! 3_1\n");
    else 
        printf("3_1 Done.\n");

    if (!_2())
        printf("Error! 3_2\n");
    else 
        printf("3_2 Done.\n");
}