
#include "mpi.h" /*MPI head file*/

#include <stdio.h>

int main( int argc, char** argv )
{
int i, rank, size;

double a[10],b[10];

double sum,c;

int senddata,recvdata;

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
  a[i] = b[i] = (double)(rank + 1);

sum = 0.0;

for(i=0;i<10;i++)
  sum += a[i] * b[i];

MPI_Reduce( &sum, &c, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 

if ( rank == 0 ) printf("Proc#0 got the final sum %lf\n", c);

MPI_Barrier(MPI_COMM_WORLD);

MPI_Finalize(); /*quit from MPI world*/
return (0);
}
