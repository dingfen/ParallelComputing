#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int num_procs;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int i;
    int vec;
    int *mat = (int *)malloc(sizeof(int)*num_procs);
    int *ret = (int *)malloc(sizeof(int)*num_procs);
    int *ans = (int *)malloc(sizeof(int)*num_procs);
    srand(time(0));
    for(i = 0;i < num_procs;i++)
    {
        mat[i] = rand() % 10;
    }
    vec = rand() % 10;

    for(i = 0;i < num_procs;i++)
    {
        printf("%d: %d\n", rank, mat[i]);
        printf("%d: vec = %d\n", rank, vec);
    }
    for(i = 0;i < num_procs;i++)
    {
        ans[i] = vec * mat[i];
    }
    
    MPI_Reduce(ans, ret, num_procs, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank == 0)
    {
        for(i = 0;i < num_procs;i++)
            printf("%d, ", ret[i]);
    }

    MPI_Finalize();
    return 0;
}
