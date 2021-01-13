#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>


#define INDEX(i, j, N) (((i)*(N))+(j))

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

void comp(int *A, int *B, int *C, int num) {
    for(int i = 0; i < num; i++) {
        for(int j = 0; j < num; j++) {
            for(int k = 0; k < num; k++)
                C[INDEX(i, j, num)] += A[INDEX(i, k, num)] * B[INDEX(k, j, num)];
        }
    }
}


int check(int *C, int *nC, int num) {
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

int main(int argc, char *argv[])
{
    int id_procs, num_procs;
    int blksize, sqrt_procs;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_procs);

    sqrt_procs = sqrt(num_procs);
    if (sqrt_procs * sqrt_procs != num_procs) {
        fprintf(stderr, "The Num of Proc must be Perfect square!\n");
        return 1;
    }
    if (argc != 2) {
        fprintf(stderr, "Please add a Parameter about the block size!\n");
        return 1;
    }
    blksize = atoi(argv[1]);
    
    // produce random data
    int *A, *B, *C, *ans;
    int *A_in, *B_in;
    int *sA, *sB, *sC;
    int N = blksize*sqrt_procs;

    if (id_procs == 0) {
        sA = (int*)malloc(N*N*sizeof(int));
        sB = (int*)malloc(N*N*sizeof(int));
        sC = (int*)malloc(N*N*sizeof(int));

        memset(sC, 0, N*N*sizeof(int));
        random_mat(A, blksize);
        random_mat(B, blksize);
        comp(sA, sB, sC, N);
    }
    A = (int*)malloc(blksize*blksize*sizeof(int));
    B = (int*)malloc(blksize*blksize*sizeof(int));
    C = (int*)malloc(blksize*blksize*sizeof(int));
    ans = (int*)malloc(blksize*blksize*sizeof(int));
    A_in = (int*)malloc(blksize*blksize*sizeof(int));
    B_in = (int*)malloc(blksize*blksize*sizeof(int));

    memset(C, 0, blksize*blksize*sizeof(int));

    MPI_Datatype SubMat, Mat;
    MPI_Status status;
    MPI_Request request;
    MPI_Type_vector(blksize, blksize, N, MPI_INT, &SubMat);
    MPI_Type_commit(&SubMat);

    MPI_Type_vector(blksize, blksize, blksize, MPI_INT, &Mat);
    MPI_Type_commit(&Mat);

    if (id_procs == 0) {
        for(int i = 0; i < sqrt_procs; i++) {
            int lineoff = blksize * N * i;
            for(int j = 0; j < sqrt_procs; j++) {
                if (i == 0 && j == 0) {
                    // 分发矩阵 A
                    MPI_Isend(sA, 1, SubMat, 0, 0, MPI_COMM_WORLD, &request);
                    MPI_Irecv(A, 1, Mat, 0, 0, MPI_COMM_WORLD, &request);
                    MPI_Wait(&request, &status);
                    // 分发矩阵 B
                    MPI_Isend(sB, 1, SubMat, 0, 1, MPI_COMM_WORLD, &request);
                    MPI_Irecv(B, 1, Mat, 0, 1, MPI_COMM_WORLD, &request);
                    MPI_Wait(&request, &status);
                    continue;
                }
                int offset = j * blksize + lineoff;
                MPI_Send(sA+offset, 1, SubMat, i*sqrt_procs+j, 0, MPI_COMM_WORLD);
                MPI_Send(sB+offset, 1, SubMat, i*sqrt_procs+j, 1, MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Recv(A, 1, Mat, 0, 0, MPI_COMM_WORLD, &status);
        // print_mat(A, blksize, id_procs);
        MPI_Recv(B, 1, Mat, 0, 1, MPI_COMM_WORLD, &status);
        // print_mat(B, blksize, id_procs);
    }

    MPI_Comm row_comm, col_comm;
    int rank, size;
    int color;
    int key;

    printf("++++++++++++++++++++++\n");
    for(int k = 0; k < sqrt_procs; k++) {
        // Comm Group by row
        key = id_procs % sqrt_procs;
        color = id_procs / sqrt_procs;
        MPI_Comm_split(MPI_COMM_WORLD, color, key, &row_comm);
        MPI_Comm_rank(row_comm, &rank);
        MPI_Comm_size(row_comm, &size);

        if (rank == k) {
            memcpy(A_in, A, blksize*blksize*sizeof(int));
        }
        // broadcast Ai,j
        MPI_Bcast(A_in, 1, Mat, k, row_comm);

        // compute
        comp(A_in, B, C, blksize);
        MPI_Comm_free(&row_comm);

        // Comm Group by B
        key = id_procs / sqrt_procs;
        color = id_procs % sqrt_procs;
        MPI_Comm_split(MPI_COMM_WORLD, color, key, &col_comm);
        MPI_Comm_rank(col_comm, &rank);
        MPI_Comm_size(col_comm, &size);

        for(int i = 0; i < size; i++) {
            int dest = (rank-1 + size)%size;
            MPI_Send(B, 1, Mat, dest, 0, col_comm);
            MPI_Recv(B_in, 1, Mat, (rank+1)%size, 0, col_comm, &status);
        }
        memcpy(B, B_in, blksize*blksize*sizeof(int));
        MPI_Comm_free(&col_comm);
    }


    // 分发结果 自行比较
    if (id_procs == 0) {
        for(int i = 0; i < sqrt_procs; i++) {
            int lineoff = blksize * N * i;
            for(int j = 0; j < sqrt_procs; j++) {
                if (i == 0 && j == 0) {
                    // 分发矩阵 A
                    MPI_Isend(sC, 1, SubMat, 0, 0, MPI_COMM_WORLD, &request);
                    MPI_Irecv(ans, 1, Mat, 0, 0, MPI_COMM_WORLD, &request);
                    MPI_Wait(&request, &status);
                    continue;
                }
                int offset = j * blksize + lineoff;
                MPI_Send(sC+offset, 1, SubMat, i*sqrt_procs+j, 100, MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Recv(ans, 1, Mat, 0, 100, MPI_COMM_WORLD, &status);
    }

    if (check(C, ans, blksize)) {
        printf("Proc#%d Done.\n", id_procs);
    }

    MPI_Finalize();

    free(A);
    free(B);
    free(C);
    free(ans);
    free(A_in);
    free(B_in);

    if (id_procs == 0) {
        free(sA);
        free(sB);
        free(sC);
    }
    return 0;
}
