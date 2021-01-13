#include "matrix.h"
#include "math.h"

int main(int argc, char *argv[])
{
    int id_procs, num_procs;
    int blksize, sqrt_procs;
    MPI_Status status;
    MPI_Request request;

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

    // Proc#0 产生随机矩阵 A B
    int *A, *B, *C, *ans;
    int *A_in, *B_in;
    int *sA, *sB, *sC;
    int N = blksize*sqrt_procs;

    if (id_procs == 0) {
        // 串行计算结果
        sA = (int*)malloc(N*N*sizeof(int));
        sB = (int*)malloc(N*N*sizeof(int));
        sC = (int*)malloc(N*N*sizeof(int));
    
        memset(sC, 0, N*N*sizeof(int));
        random_mat(sA, N);
        // print_mat(sA, N, 0);
        random_mat(sB, N);
        // print_mat(sB, N, 0);
        matrix_multi(sA, sB, sC, N);
    }
    A = (int*)malloc(blksize*blksize*sizeof(int));
    B = (int*)malloc(blksize*blksize*sizeof(int));
    C = (int*)malloc(blksize*blksize*sizeof(int));
    ans = (int*)malloc(blksize*blksize*sizeof(int));
    A_in = (int*)malloc(blksize*blksize*sizeof(int));
    B_in = (int*)malloc(blksize*blksize*sizeof(int));
    
    random_mat(A, blksize);
    random_mat(B, blksize);
    memset(C, 0, blksize*blksize*sizeof(int));

    MPI_Datatype SubMat;
    MPI_Type_vector(blksize, blksize, N, MPI_INT, &SubMat);
    MPI_Type_commit(&SubMat);

    MPI_Datatype Mat;
    MPI_Type_vector(blksize, blksize, blksize, MPI_INT, &Mat);
    MPI_Type_commit(&Mat);

    // Proc#0 将相应的数据发送到各个 Proc 中
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

    // 算法正式开始
    // 初始对齐 Aij 向左移i格 Bij 向上移j格
    MPI_Comm row_comm;
    int rank_A, size_A;
    int color_A;
    int key_A;
    key_A = id_procs % sqrt_procs;
    color_A = id_procs / sqrt_procs;

    MPI_Comm_split(MPI_COMM_WORLD, color_A, key_A, &row_comm);
    MPI_Comm_rank(row_comm, &rank_A);
    MPI_Comm_size(row_comm, &size_A);

    if (color_A > 0) {
        MPI_Send(A, 1, Mat, (rank_A-color_A+size_A)%size_A, 2, row_comm);
        MPI_Recv(A_in, 1, Mat, (rank_A+color_A)%size_A, 2, row_comm, &status);
        memcpy(A, A_in, blksize*blksize*sizeof(int));
    }

    MPI_Comm col_comm;
    int rank_B, size_B;
    int color_B;
    int key_B;
    key_B = id_procs / sqrt_procs;
    color_B = id_procs % sqrt_procs;

    MPI_Comm_split(MPI_COMM_WORLD, color_B, key_B, &col_comm);
    MPI_Comm_rank(col_comm, &rank_B);
    MPI_Comm_size(col_comm, &size_B);

    if (color_B > 0) {
        MPI_Send(B, 1, Mat, (rank_B-color_B+size_B)%size_B, 2, col_comm);
        MPI_Recv(B_in, 1, Mat, (rank_B+color_B)%size_B, 2, col_comm, &status);
        memcpy(B, B_in, blksize*blksize*sizeof(int));
    }

    // 重复执行 sqrt(num_procs) 次
    for(int i = 0; i < sqrt_procs; i++) {
        matrix_multi(A, B, C, blksize);

        MPI_Send(A, 1, Mat, (rank_A-1+size_A)%size_A, i+3, row_comm);
        MPI_Recv(A_in, 1, Mat, (rank_A+1)%size_A, i+3, row_comm, &status);
        memcpy(A, A_in, blksize*blksize*sizeof(int));

        MPI_Send(B, 1, Mat, (rank_B-1+size_B)%size_B, i+3, col_comm);
        MPI_Recv(B_in, 1, Mat, (rank_B+1)%size_B, i+3, col_comm, &status);
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
