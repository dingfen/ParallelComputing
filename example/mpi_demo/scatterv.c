#include "mpi.h"

int main( int argc, char* argv[] ){
    int i;
    int rank, nproc;
    int iscnt[3] = {1,2,3}, irdisp[3] = {0,1,3};
    int isend[6] = {1,2,2,3,3,3}, ircnt,irecv[3];
 
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &nproc );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

   irecv[0] = -1; irecv[1] = -1 ; irecv[2] = -1;
   
    ircnt = rank + 1;
    MPI_Scatterv( isend, iscnt, irdisp, MPI_INT, irecv, ircnt, 
                             MPI_INT, 0, MPI_COMM_WORLD);
    printf("My rank = %d  irecv[0] = %d irecv[1] = %d irecv[2] = %d \n", 
               rank,      irecv[0],     irecv[1],     irecv[2]);

    MPI_Finalize();
}
