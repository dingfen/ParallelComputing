#include "mpi.h"

int main( int argc, char* argv[] )
{
    int i;
    int rank, nproc;
    int isend[30], irecv;
 
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &nproc );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

    if(rank == 0) {
        for(i=0; i<nproc; i++)   isend[i] = i+1;
         }
    MPI_Scatter( isend, 1, MPI_INT, &irecv, 1, MPI_INT, 0,MPI_COMM_WORLD);
    printf("My rank = %d  irecv = %d\n", rank, irecv);

    MPI_Finalize();
}

