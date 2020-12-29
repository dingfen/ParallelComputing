#include "sort.h"

/**
 * @input  输入数组 a
 * @output 输出数组 b
 */ 
int main(int argc, char *argv[])
{
    int num_procs, id_procs;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_procs);

    int *a, *b, *b2;
    a = (int*)malloc(num_procs*sizeof(int));
    if (id_procs == 0) {
        b2 = (int*)malloc(num_procs*sizeof(int));
        b  = (int*)malloc(num_procs*sizeof(int));
        random_array(b2, num_procs);
        memcpy(a, b2, num_procs*sizeof(int));
        SURCLK(qsort(b2, num_procs, sizeof(int), compare);)
    }

    
    MPI_Bcast(a, num_procs, MPI_INT, 0, MPI_COMM_WORLD);
    int k = 0;
    for(int j = 0; j < num_procs; j++) {
        if (a[id_procs] > a[j] || (a[id_procs] == a[j] && id_procs > j))
            k++;
    }


    if (id_procs == 0) {
        int recv;
        for(int i = 1; i < num_procs; i++) {
            MPI_Recv(&recv, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
            b[recv] = a[i];
        }
        b[k] = a[0];
    } else {
        MPI_Send(&k, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    if (id_procs == 0) {
        if (check_array(b, b2, num_procs))
            printf("Done.\n");
        else 
            printf("Error Occured!\n");
    }

    free(a);
    if (id_procs == 0) {
        free(b2);
        free(b);
    }
    MPI_Finalize();
    return 0;
}
