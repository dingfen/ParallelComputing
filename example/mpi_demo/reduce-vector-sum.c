
#include "mpi.h" /*MPI head file*/

#include <stdio.h>

int main( int argc, char** argv )
{
int i, rank, size;

double a[10];

double sum[10];


MPI_Status status; 

MPI_Init(&argc, &argv); /*initializing */
MPI_Comm_rank(MPI_COMM_WORLD, &rank); /*Process#*/
MPI_Comm_size(MPI_COMM_WORLD, &size); /*Total processes#*/

//A routine that computes the dot product of two vectors that are distributed 
//across a group of processes and returns the answer at node zero. 

//int MPI_Reduce(
    // void* sendbuf, 
    // void* recvbuf, 
    // int count    ,
    // MPI_Datatype datatype, 
    // MPI_Op op, 
    // int root, 
    // MPI_Comm comm
    // )


for(i=0;i<10;i++)
  a[i] = (double)(rank*10 + i);

MPI_Barrier(MPI_COMM_WORLD);

printf("Proc#%d :",rank);
for(i=0;i<10;i++) printf("%5.1f ", a[i]);
printf("\n");

MPI_Barrier(MPI_COMM_WORLD);

for(i=0;i<10;i++) 
  sum[i] = 0.0;

MPI_Reduce( a, sum, 10, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 

if ( rank == 0 ) {
  printf("\nProc#0 :");
  for(i=0;i<10;i++) printf("%5.1f ", sum[i]);
  printf("\n");
}


MPI_Finalize(); /*quit from MPI world*/
return (0);
}
