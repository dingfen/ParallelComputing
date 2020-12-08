#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifndef N
#define N 50
#endif

#define INDEX(i, j) (((i)*N)+(j))


void random_array(double *a, int num) {
    for(int i = 0; i < num; i++) {
        srand(time(NULL));
        a[i] = rand() % 100;
    }
}

void comp(double *A, double *B, int num) {
    for(int i = 1; i < N-1; i++) {
        for(int j = 1; j < N-1; j++) {
            B[INDEX(i, j)] = (A[INDEX(i-1, j)]+A[INDEX(i, j+1)]+A[INDEX(i+1, j)]+A[INDEX(i, j-1)]) / 4.0;
        }
    }
}

int check(double *B, double *C) {
    for(int i = 1; i < N-1; i++) {
        for(int j = 1; j < N-1; j++) {
            if (fabs(B[INDEX(i, j)]-C[INDEX(i, j)]) >= 1e-2) {
                printf("B[%d,%d] = %lf not %lf!\n", i, j, B[INDEX(i, j)], C[INDEX(i, j)]);
                return 0;
            }
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    double *A, *B, *B2;
    A = (double*)malloc(N*N*sizeof(double));
    B = (double*)malloc(N*N*sizeof(double));
    B2= (double*)malloc(N*N*sizeof(double));

    int id_procs, num_procs, num_1;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_procs);

    num_1 = num_procs -1;
    // Proc#N-1 randomize the data
    if (id_procs == num_1) {
        random_array(A, N*N);
        comp(A, B2, N*N);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // P0 broadcast 3 lines of A to each Proc
    int ctn = 0;
    for(int i = 0; i < N-2; i++) {
        if (id_procs == num_1) {
            int dest = i % num_1;
            int tag = i / num_1;
            MPI_Send(A+i*N, N*3, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
        }
    }

    for(int i = 0; i < (N-2)/num_1; i++) {
        if (id_procs != num_1) {
            MPI_Recv(A+ctn*3*N, 3*N, MPI_DOUBLE, num_1, ctn, MPI_COMM_WORLD, &status);
            ctn++;
        }
    }
    if (id_procs < (N-2) % num_1) {
        MPI_Recv(A+ctn*3*N, 3*N, MPI_DOUBLE, num_1, ctn, MPI_COMM_WORLD, &status);
        ctn++;
    }

    // compute
    if (id_procs != num_1) {
        for(int i = 1; i <= ctn; i++) {
            for(int j = 1; j < N-1; j++) {
                B[INDEX(i, j)] = (A[INDEX(i-1, j)]+A[INDEX(i, j+1)]+A[INDEX(i+1, j)]+A[INDEX(i, j-1)]) / 4.0;
            }
        }
    }

    // Gather
    for(int i = 0; i < N-2; i++) {
        if (id_procs == num_1) {
            int src = i % num_1;
            MPI_Recv(B+(i+1)*N+1, N-2, MPI_DOUBLE, src, i/num_1+N, MPI_COMM_WORLD, &status);
        }
        else {
            for(int j = 0; j < ctn; j++)
                MPI_Send(B+(j+1)*N+1, N-2, MPI_DOUBLE, num_1, j+N, MPI_COMM_WORLD);
        }
    }


    if (id_procs == num_1) {
        if(check(B, B2)) {
            printf("Done.\n");
        } else {
            printf("Error Occured!\n");
        }
    }
    free(A);
    free(B);
    free(B2);
    MPI_Finalize();
    return 0;
}
