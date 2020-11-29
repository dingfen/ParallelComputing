#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    /* code */
    MPI_Init(&argc, &argv);
    int m = 233;
    int n = 341;
    printf("%d\n", m*n);
    MPI_Finalize();
}
