#include "util.h"
#include <stdio.h>

int _1() {
    int A[256];
    int B[256];
    random_array(A, 256);
    copy_array(B, A, 256);
    omp_set_num_threads(9);

    clock_t start = clock();
    for(int i = 2; i <= 10; i++) {
        #pragma omp parallel for
        for (int j = 2; j <= 10; j++) {
            A[IN(i,j,16)] = 0.5*(A[IN(i-1,j-1,16)]+A[IN(i+1,j+1,16)]);
        }
    }
    clock_t end = clock();
    printf("OpenMP costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    start = clock();
    for(int i = 2; i <= 10; i++) {
        for (int j = 2; j <= 10; j++) {
            B[IN(i,j,16)] = 0.5*(B[IN(i-1,j-1,16)]+B[IN(i+1,j+1,16)]);
        }
    }
    end = clock();
    printf("C costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    return check_ans(A, B, 256);
}

int _2() {
    int A[50];
    int B[22];
    int C[50];
    random_array(A, 50);
    random_array(B, 50);
    copy_array(C, A, 50);
    omp_set_num_threads(8);

    clock_t start = clock();
    for(int k = 2; k <= 20; k+=2) {
        #pragma omp parallel for
        for(int i = k; i <= min(20,k+1); i++) {
            A[2*i+2] = A[2*i-2]+B[i];
        }
    }
    clock_t end = clock();
    printf("OpenMP costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    start = clock();
    for(int i = 2; i <= 20; i++) {
        C[2*i+2] = C[2*i-2]+B[i];
    }
    end = clock();
    printf("C costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    return check_ans(A, C, 50);

}

int _3() {
    int A[24];
    int B[24];
    int C[24];

    int B2[24];
    int C2[24];

    random_array(A, 24);
    random_array(B, 24);
    random_array(C, 24);
    copy_array(B2, B, 24);
    copy_array(C2, C, 24);

    omp_set_num_threads(8);

    int D[24];
    int share = 1;
    D[0] = 1;
    clock_t start = clock();
    // find the parallel block border
    #pragma omp parallel for shared(share)
    for(int i = 2; i <= 20; i++) {
        if (A[i] < 0 && A[i+1] > 0)
            D[share++] = i;
    }
    qsort(D, share, sizeof(int), cmp);
    D[share++] = 20;

    // Parallel in the Block, Serial the Block
    for(int j = 0; j < share-1; j++)
        #pragma omp parallel for
        for(int i = D[j]+1; i <= D[j+1]; i++) {
            if (A[i] > 0)
                B[i] = C[i-1]+1;
            else
                C[i] = B[i]-1;
        }
    clock_t end = clock();
    printf("OpenMP costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    start = clock();
    for(int i = 2; i <= 20; i++) {
        if (A[i] > 0)
            B2[i] = C2[i-1]+1;
        else 
            C2[i] = B2[i]-1;
    }
    end = clock();
    printf("C costs : %Lf\n", (long double)(end-start)/CLOCKS_PER_SEC);

    int ans = check_ans(B, B2, 24);
    ans &= check_ans(C, C2, 24);
    return ans;
}

int main() {
    if (!_1())
        printf("Error! 1_1\n");
    else 
        printf("1_1 Done.\n");

    if (!_2())
        printf("Error! 1_2\n");
    else 
        printf("1_2 Done.\n");
    
    if (!_3())
        printf("Error! 1_3\n");
    else 
        printf("1_3 Done.\n");
}