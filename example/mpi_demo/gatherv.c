#include "mpi.h"

int main( int argc, char* argv[] ){
    int i;
    int rank, nproc;
    int isend[3],iscnt,irecv[6];
    int ircnt[3] = {1,2,3}, idisp[3] = {0,1,3};
 
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &nproc );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

   if ( rank == 0 ) printf("This program must run with 3 processes: np = 3 \n");
   
    for(i=0; i<3; i++)
        isend[i] = rank + 1;
        iscnt = rank + 1;
       MPI_Gatherv( isend, iscnt, MPI_INT, irecv, ircnt, idisp, 
                               MPI_INT, 0, MPI_COMM_WORLD);
       if(rank == 0) {
             for(i=0; i<6; i++)  printf("irecv = %d\n", irecv[i]);
       }
    MPI_Finalize();
}
