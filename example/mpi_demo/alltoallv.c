#include "mpi.h"
int main( int argc, char* argv[] ){
    int i;
    int rank, nproc;
    int isend[6] = {1,2,2,3,3,3}, irecv[9];
    int iscnt[3] = {1,2,3}, isdsp[3] = {0,1,3}, ircnt[3], irdsp[3];
 
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &nproc );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

    for(i=0; i<6; i++)
        isend[i] = isend[i] + nproc * rank;
    for(i=0; i<nproc; i++) {
        ircnt[i] = rank + 1;
        irdsp[i] = i * (rank + 1);
        }
    MPI_Alltoallv( isend, iscnt, isdsp, MPI_INT, 
                          irecv, ircnt, irdsp, MPI_INT, MPI_COMM_WORLD);
    for(i=0; i<iscnt[rank] * nproc; i++) printf("My rank %d  irecv = %d\n", rank, irecv[i]);
    MPI_Finalize();
}
