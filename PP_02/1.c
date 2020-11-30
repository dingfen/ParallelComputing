#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int id_procs, num_procs;
    int msg = 10;
    int tag = 5;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_procs);


    int color = id_procs % 3;
    int key = id_procs / 3;
    MPI_Comm split_comm_world;
    MPI_Status status;
    int rank;
    int size;
    int msgin;

    // MPI split COMM_WORLD into 3 groups 
    MPI_Comm_split(MPI_COMM_WORLD, color, key, &split_comm_world);
    MPI_Comm_rank(split_comm_world, &rank);
    MPI_Comm_size(split_comm_world, &size);

    // message to 0 proc of Each Comm
    if (id_procs == 0) {
        msgin = msg;
        for (int i = 1; i < 3; i++)
            MPI_Send(&msg, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
    } else if (id_procs > 0 && id_procs < 3) {
        MPI_Recv(&msgin, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    // Broadcast within the group
    MPI_Bcast(&msgin, 1, MPI_INT, 0, split_comm_world);

    MPI_Barrier(split_comm_world);
    printf("MPI Comm rank %d, original id %d, size %d. the new msg is %d\n", rank, id_procs, size, msgin);
    MPI_Finalize();
    return 0;
}