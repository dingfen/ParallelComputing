
#include "mpi.h" /*MPI head file*/

#include <stdio.h>

int main( int argc, char** argv )
{
int i, rank, size;

double a[10],b[10];

double sum,c;


MPI_Status status; 

MPI_Init(&argc, &argv); /*initializing */
MPI_Comm_rank(MPI_COMM_WORLD, &rank); /*Process#*/
MPI_Comm_size(MPI_COMM_WORLD, &size); /*Total processes#*/


//int MPI_Allreduce(
    // void* sendbuf, 
    // void* recvbuf, 
    // int count    ,
    // MPI_Datatype datatype, 
    // MPI_Op op, 
    // MPI_Comm comm
    // )


for(i=0;i<10;i++)
  a[i] = b[i] = (double)(rank*10 + i);

sum = 0.0;

for(i=0;i<10;i++)
  sum += a[i] * b[i];

MPI_Barrier(MPI_COMM_WORLD);
printf("Proc# %d got sub-sum : %.1f\n",rank,sum);
MPI_Barrier(MPI_COMM_WORLD);

MPI_Allreduce( &sum, &c, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD); 

MPI_Barrier(MPI_COMM_WORLD);

printf("Proc#%d got the final sum %.1f\n", rank,c);

MPI_Barrier(MPI_COMM_WORLD);

MPI_Finalize(); /*quit from MPI world*/
return (0);
}
