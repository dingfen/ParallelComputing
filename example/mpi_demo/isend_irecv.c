#include "mpi.h"
int main( int argc, char* argv[] ){
    int rank, nproc;
    int isbuf, irbuf, count;
    MPI_Request request;
    MPI_Status status;
    int TAG = 100;
 
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &nproc );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

    if(rank == 0) {
         isbuf = 9;
         MPI_Isend( &isbuf, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD, &request );
    } else if(rank == 1) {
        MPI_Irecv( &irbuf, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        printf( "irbuf = %d source = %d tag = %d count = %d\n", 
                   irbuf, status.MPI_SOURCE, status.MPI_TAG, count);
        }
    MPI_Finalize();
}
