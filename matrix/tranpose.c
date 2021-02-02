#include "matrix.h"
#include "math.h"

int main(int argc, char *argv[])
{
    int *sa;
    int *sb;
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
        fprintf(stderr, "Please add a Parameter about the matrix size!\n");
        return 1;
    }
    blksize = atoi(argv[1]);
    int n = blksize * sqrt_procs;
    int *a = (int*)malloc(blksize*blksize*sizeof(int));
    int *b = (int*)malloc(blksize*blksize*sizeof(int));

    if (id_procs == 0) {
        sb = (int *)malloc(n*n*sizeof(int));
        sa = (int *)malloc(n*n*sizeof(int));
        random_mat(sb, n);
        memcpy(sa, sb, n*n*sizeof(int));
        matrix_transpose(sb, n);
    }

    // Proc#0 send the transpose submat
    // Put A_ij to #Proc_ji
    MPI_Datatype SubMat, Mat;
    MPI_Status status;
    MPI_Request request;
    MPI_Type_vector(blksize, blksize, n, MPI_INT, &SubMat);
    MPI_Type_commit(&SubMat);
    MPI_Type_vector(blksize, blksize, blksize, MPI_INT, &Mat);
    MPI_Type_commit(&Mat);

    if (id_procs == 0) {
        for(int i = 0; i < sqrt_procs; i++) {
            int lineoff = blksize * n * i;
            for(int j = 0; j < sqrt_procs; j++) {
                if (i == 0 && j == 0) {
                    // 分发矩阵 A
                    MPI_Isend(sa, 1, SubMat, 0, 0, MPI_COMM_WORLD, &request);
                    MPI_Irecv(a, 1, Mat, 0, 0, MPI_COMM_WORLD, &request);
                    MPI_Wait(&request, &status);
                    continue;
                }
                int offset = j * blksize + lineoff;
                MPI_Send(sa+offset, 1, SubMat, j*sqrt_procs+i, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Recv(a, 1, Mat, 0, 0, MPI_COMM_WORLD, &status);
    }
    
    matrix_transpose(a, blksize);

    if (id_procs == 0) {
        for(int i = 0; i < sqrt_procs; i++) {
            int lineoff = blksize * n * i;
            for(int j = 0; j < sqrt_procs; j++) {
                if (i == 0 && j == 0) {
                    // 分发矩阵 A
                    MPI_Isend(sb, 1, SubMat, 0, 0, MPI_COMM_WORLD, &request);
                    MPI_Irecv(b, 1, Mat, 0, 0, MPI_COMM_WORLD, &request);
                    MPI_Wait(&request, &status);
                    continue;
                }
                int offset = j * blksize + lineoff;
                MPI_Send(sb+offset, 1, SubMat, i*sqrt_procs+j, 100, MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Recv(b, 1, Mat, 0, 100, MPI_COMM_WORLD, &status);
    }

    if(check_mat(a,b, blksize)) {
        printf("Proc#%d Done.\n", id_procs);
    }
    

    if (id_procs == 0) {
        free(sa);
        free(sb);
    }
    free(a);
    free(b);
    MPI_Finalize();
    return 0;
}
