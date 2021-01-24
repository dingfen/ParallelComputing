#include "matrix.h"
#include "math.h"

int main(int argc, char *argv[])
{
    int *a;
    int *b;
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

    if (id_procs == 0) {
        a = (int *)malloc(4*4*sizeof(int));
        random_mat(a, 4);
        print_mat(a, 4, 0);
        matrix_transpose(a, 4);
        printf("********************\n");
        print_mat(a, 4, 0);
    }

    if (id_procs == 0) {
        free(a);
        free(b);
    }

    MPI_Finalize();
    return 0;
}
