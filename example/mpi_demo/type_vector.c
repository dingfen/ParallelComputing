
#include "mpi.h" /*MPI head file*/

#include <stdio.h>
#include <stdlib.h>

int main( int argc, char** argv )
{

int myrank,size;

double A[100];

MPI_Datatype EvenElements;

void *TempBuffer;

int i, Position, BufferSize;

MPI_Status status; 

MPI_Init(&argc, &argv); /*initializing */
MPI_Comm_rank(MPI_COMM_WORLD, &myrank); /*Process#*/
MPI_Comm_size(MPI_COMM_WORLD, &size); /*Total processes#*/


MPI_Type_vector(50, 1, 2, MPI_DOUBLE, &EvenElements);
MPI_Type_commit(&EvenElements);

if (myrank == 0) 
{  /* Sender Code */

    for(i=0;i<100;i++) A[i] = (double)i*(double)i;
 
    MPI_Send(A, 1, EvenElements, 1, 0, MPI_COMM_WORLD);  


} 
else  /* RECEIVER CODE */ 
{ 
      for(i=0;i<100;i++) A[i] = 0.0;

      MPI_Recv(A, 1, EvenElements,0, 0, MPI_COMM_WORLD,&status);

      for(i=0;i<10;i++)printf("Proc#1: A[%d] = %3.2f\n",i,A[i]);  
}
MPI_Finalize(); /*quit from MPI world*/
return (0);
}
