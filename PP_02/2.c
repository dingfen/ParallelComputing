#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void print(int id, int id_procs, int num_procs, int *recvbuf) {
    if (id_procs == id) {
        printf("recv buf %d : ", id_procs);
        for(int i = 0; i < num_procs; i++) {
            printf("%d, ", recvbuf[i]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    int id_procs, num_procs;
    MPI_Status status;
    MPI_Request req;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_procs);

    int sendbuf[num_procs];
    int recvbuf[num_procs];
    for(int i = 0; i < num_procs; i++)
        sendbuf[i] = 10*(i+1)+id_procs;

    for(int i = 0; i < num_procs; i++) {
        if (i != id_procs) {
            MPI_Send(sendbuf+i, 1, MPI_INT, i, id_procs, MPI_COMM_WORLD);
            MPI_Recv(recvbuf+i, 1, MPI_INT, i, i, MPI_COMM_WORLD, &status);
        }
        else {
            recvbuf[i] = sendbuf[i];
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    for(int i = 0; i < num_procs; i++)
        print(i, id_procs, num_procs, recvbuf);

    MPI_Finalize();
    return 0;
}
