#include "mpi.h"
int main( int argc, char* argv[] )
{
    int i;
    int rank, nproc;
    int isend, irecv[32];
 
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &nproc );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

    isend = rank + 1;
    MPI_Allgather(&isend, 1, MPI_INT, irecv, 1, MPI_INT,
                              MPI_COMM_WORLD);
    for(i=0; i<nproc; i++)
         printf("My rank =  %d  irecv = %d\n", rank, irecv[i]);
    MPI_Finalize();
}
