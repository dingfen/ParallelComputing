#include "matrix.h"
#include <math.h>


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
        random_mat(sA, N);
        random_mat(sB, N);
        matrix_multi(sA, sB, sC, N);
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
    int rank_A, size_A;
    int color_A;
    int key_A;

    int rank_B, size_B;
    int color_B;
    int key_B;

    // Comm Group by row
    key_A = id_procs % sqrt_procs;
    color_A = id_procs / sqrt_procs;
    MPI_Comm_split(MPI_COMM_WORLD, color_A, key_A, &row_comm);
    MPI_Comm_rank(row_comm, &rank_A);
    MPI_Comm_size(row_comm, &size_A);

    // Comm Group by B
    key_B = id_procs / sqrt_procs;
    color_B = id_procs % sqrt_procs;
    MPI_Comm_split(MPI_COMM_WORLD, color_B, key_B, &col_comm);
    MPI_Comm_rank(col_comm, &rank_B);
    MPI_Comm_size(col_comm, &size_B);

    for(int k = 0; k < sqrt_procs; k++) {
        if (rank_A == (color_A+k)%size_A) {
            memcpy(A_in, A, blksize*blksize*sizeof(int));
        }
        // broadcast Ai,j
        MPI_Bcast(A_in, 1, Mat, (color_A+k)%size_A, row_comm);

        // compute
        matrix_multi(A_in, B, C, blksize);

        int dest = (rank_B-1 + size_B)%size_B;
        MPI_Send(B, 1, Mat, dest, 0, col_comm);
        MPI_Recv(B_in, 1, Mat, (rank_B+1)%size_B, 0, col_comm, &status);
        memcpy(B, B_in, blksize*blksize*sizeof(int));
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

    // print_mat(ans, blksize, id_procs);

    if (check_mat(C, ans, blksize)) {
        printf("Proc#%d Done.\n", id_procs);
    }

    // print_mat(C, blksize, id_procs);

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
    MPI_Finalize();
    return 0;
}
