#include "mpi.h"

int main( int argc, char* argv[] )
{
    int rank;
    int ibuf;
 
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

    if(rank == 0) ibuf = 12345;
    else ibuf = 0; // set ibuf Zero for non-root processes
    MPI_Bcast(&ibuf, 1, MPI_INT, 0, MPI_COMM_WORLD);
    //if (rank !=0 ) 
    printf("my rank = %d  ibuf = %d\n", rank,ibuf);

    MPI_Finalize();
}
