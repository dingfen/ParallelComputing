
#include "mpi.h" /*MPI head file*/

#include <stdio.h>
#include <stdlib.h>

int main( int argc, char** argv )
{

int myrank,size;

double A[20][20];

MPI_Datatype EvenLine;

void *TempBuffer;

int i, j;

MPI_Status status; 

MPI_Init(&argc, &argv); /*initializing */
MPI_Comm_rank(MPI_COMM_WORLD, &myrank); /*Process#*/
MPI_Comm_size(MPI_COMM_WORLD, &size); /*Total processes#*/


MPI_Type_vector(10, 20, 40, MPI_DOUBLE, &EvenLine);
MPI_Type_commit(&EvenLine);

if (myrank == 0) 
{  /* Sender Code */
  /*  All Even# Lines */
    for(i=0;i<20;i++) 
      for(j=0;j<20;j++)
         A[i][j] = (double)i;
 
    MPI_Send(A, 1, EvenLine, 1, 0, MPI_COMM_WORLD);  


} 
else  /* RECEIVER CODE */ 
{ 
      for(i=0;i<20;i++)
         for(j=0;j<20;j++)
           A[i][j] = 0.0;

      MPI_Recv(A, 1, EvenLine,0, 0, MPI_COMM_WORLD,&status);

      for(i=0;i<20;i++)
       {
        for(j=0;j<10;j++)
          printf("%3.1lf ",A[i][j]);  
        printf("\n");  
       }
}
MPI_Finalize(); /*quit from MPI world*/
return (0);
}

