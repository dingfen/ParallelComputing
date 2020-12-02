#include "mpi.h"

int main( int argc, char* argv[] ){
    int i;
    int rank, nproc;
    int isend[32], irecv[32];
 
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &nproc );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

   printf("Before : My rank = %d  <",rank );
    for(i=0; i<nproc; i++)
        printf(" %d ", isend[i] = i + nproc * rank);
        
    printf( " >\n");
    
    MPI_Alltoall(isend, 1, MPI_INT, irecv, 1, MPI_INT, 
                       MPI_COMM_WORLD);
    printf("After  : My rank = %d  <", rank);                        
    for(i=0; i<nproc; i++)
        printf(" %d ", irecv[i]);
    printf(" >\n");

    MPI_Finalize();
    
}
