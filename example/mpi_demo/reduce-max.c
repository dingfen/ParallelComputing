
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


//int MPI_Reduce(
    // void* sendbuf, 
    // void* recvbuf, 
    // int count    ,
    // MPI_Datatype datatype, 
    // MPI_Op op, 
    // int root, 
    // MPI_Comm comm
    // )


sum = 0.0;

srandom((unsigned int)(&sum)%100000);

for(i=0;i<10;i++)
  sum += (double)(random()%100);
  
MPI_Barrier(MPI_COMM_WORLD);

printf("Prco#%d has sum : %.1f\n",rank, sum);

MPI_Barrier(MPI_COMM_WORLD);  

MPI_Reduce( &sum, &c, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); 

if ( rank == 0 ) printf("Proc#0 got the final MAX of sum %.1f\n", c);

MPI_Barrier(MPI_COMM_WORLD);

MPI_Finalize(); /*quit from MPI world*/
return (0);
}
