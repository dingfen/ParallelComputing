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

void comp(double *A, double *B, int a, int b) {
    for(int i = 1; i <= a; i++) {
        for(int j = 1; j <= b; j++) {
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


int main(int argc, char *argv[])
{
    double *A, *B, *B2;
    
    int id_procs, num_procs;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_procs);

    MPI_Datatype SubMat;
    int rows = sqrt(num_procs);
    int cols = num_procs / rows;
    int a = (N-2 + rows-1) / rows;
    int b = (N-2 + cols-1) / cols;
    int alloc_num = (a+1)*(b+1)*num_procs;
    A = (double*)malloc(alloc_num*sizeof(double));
    B = (double*)malloc(alloc_num*sizeof(double));
    B2= (double*)malloc(alloc_num*sizeof(double));

    // Proc#0 randomize the data
    if (id_procs == 0) {
        random_array(A, N*N);
        comp(A, B2, N-2, N-2);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Proc#0 broadcast (a+2)x(b+2) mat
    MPI_Type_vector(a+2, b+2, N, MPI_DOUBLE, &SubMat);
    MPI_Type_commit(&SubMat);

    if (id_procs == 0) {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                if (i == 0 && j == 0)
                    continue;
                MPI_Send(A+i*a*N+b*j, 1, SubMat, j+cols*i, 0, MPI_COMM_WORLD);
            }
        }
    }
    else {
        MPI_Recv(A, 1, SubMat, 0, 0, MPI_COMM_WORLD, &status);
    }

    // compute
    comp(A, B, a, b);

    // Gather result
    MPI_Datatype SubMat_B;
    MPI_Type_vector(a, b, N, MPI_DOUBLE, &SubMat_B);
    MPI_Type_commit(&SubMat_B);
    if (id_procs == 0) {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                if (i == 0 && j == 0)
                    continue;
                MPI_Recv(&B[INDEX(a*i+1, b*j+1)], 1, SubMat_B, i*cols+j, 1, MPI_COMM_WORLD, &status);
            }
        }
    } else {
        int x = id_procs / cols;
        int y = id_procs % cols;
        MPI_Send(&B[INDEX(1, 1)], 1, SubMat_B, 0, 1, MPI_COMM_WORLD);
    }

    if (id_procs == 0)
        if (check(B, B2)) {
            printf("Done.No Error\n");
        } else {
            printf("Error!\n");
        }

    free(A);
    free(B);
    free(B2);
    MPI_Finalize();
    return 0;
    return 0;
}
