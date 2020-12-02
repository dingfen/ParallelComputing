#include "mpi.h"

int main( int argc, char* argv[] ){
    int rank, nproc;
    int isbuf, irbuf;
    int TAG = 100;
   MPI_Status 	status;
 
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &nproc );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    if(rank == 0) {
         isbuf = 9;
         MPI_Send( &isbuf, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);
    } else if(rank == 1) {
        MPI_Recv( &irbuf, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);
        printf( "%d\n", irbuf );
      }
    MPI_Finalize();
    
}
