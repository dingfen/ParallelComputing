#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    int id_procs, num_procs;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_procs);

    srand(clock());
    int data = rand() % 100;
    int recvdata;
    MPI_Status status;

    printf("data = %d\n", data);
    // Butterfly sum
    // each loop is a layer
    for(int i = 2; i <= num_procs; i <<= 1) {
        int tag = i >> 1;
        int dest = id_procs ^ tag;
        MPI_Send(&data, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
        MPI_Recv(&recvdata, 1, MPI_INT, dest, tag, MPI_COMM_WORLD, &status);
        data += recvdata;
    }

    printf("Sum is = %d\n", data);

    MPI_Finalize();
    return 0;
}
