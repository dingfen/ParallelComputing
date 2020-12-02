#include "mpi.h"

int main( int argc, char* argv[] ){
    int i;
    int rank, nproc;
    int isend, irecv[32];
 
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &nproc );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

    isend = rank + 1;
    MPI_Gather( &isend, 1, MPI_INT, irecv, 1, MPI_INT, 0,
                           MPI_COMM_WORLD);
    if(rank == 0) {
        for(i=0; i<nproc; i++)
            printf("irecv = %d\n", irecv[i]);
        }
    MPI_Finalize();
}

