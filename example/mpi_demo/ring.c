
#include "mpi.h" /*MPI head file*/

#include <stdio.h>

int main( int argc, char** argv )
{
int i, rank, size, tag=1;

int to, from;

int senddata,recvdata;
MPI_Status status; 
MPI_Init(&argc, &argv); /*initializing */
MPI_Comm_rank(MPI_COMM_WORLD, &rank); /*Process#*/
MPI_Comm_size(MPI_COMM_WORLD, &size); /*Total processes#*/

// cyclic shift send-recv with step 1 ~ Size-1

for(i=1;i<size;i++) {

  to = (rank+i)%size;

  from = (size+rank-i)%size;

MPI_Barrier(MPI_COMM_WORLD);
  
if (rank==0) printf("\n*****Shift Step#%d*****\n\n",i);

//int MPI_Sendrecv(
// void *sendbuf, int sendcount, MPI_Datatype sendtype, int dest, int sendtag, 
// void *recvbuf, int recvcount, MPI_Datatype recvtype, int source, MPI_Datatype recvtag, 
// MPI_Comm comm, MPI_Status *status) 
 
senddata=rank; 

MPI_Sendrecv(&senddata, 1, MPI_INT, to, i, 
             &recvdata, 1, MPI_INT, from, i,
             MPI_COMM_WORLD, &status);

printf("data# %d :  Proc#%d ---> Proc#%d\n", recvdata, from, rank);

MPI_Barrier(MPI_COMM_WORLD);

}

MPI_Finalize(); /*quit from MPI world*/
return (0);
}
