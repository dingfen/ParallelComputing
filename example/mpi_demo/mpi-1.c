
#include "mpi.h" /*MPI head file*/
int main( int argc, char** argv )
{
int rank, size, tag=1;
int senddata,recvdata;
MPI_Status status; 
MPI_Init(&argc, &argv); /*initializing */
MPI_Comm_rank(MPI_COMM_WORLD, &rank); /*Process#*/
MPI_Comm_size(MPI_COMM_WORLD, &size); /*Total processes#*/


if (rank==0){
senddata=9999; 
MPI_Send( &senddata, 1, MPI_INT, 1, tag, MPI_COMM_WORLD); /*sending data to node#1*/
}
if (rank==1) 
MPI_Recv(&recvdata, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status); 
/*receiving from node#0*/
MPI_Finalize(); /*quit from MPI world*/
return (0);
}