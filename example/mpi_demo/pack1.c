
#include "mpi.h" /*MPI head file*/

#include <stdio.h>
#include <stdlib.h>

int main( int argc, char** argv )
{

int myrank,size;

double A[50][50];

void *TempBuffer;

int i, Position, BufferSize;

MPI_Status status; 

MPI_Init(&argc, &argv); /*initializing */
MPI_Comm_rank(MPI_COMM_WORLD, &myrank); /*Process#*/
MPI_Comm_size(MPI_COMM_WORLD, &size); /*Total processes#*/

if (myrank == 0) 
{ 
      for(i=0;i<50;i++) A[i][i] = (double)i;
      
      //Apply memeory space for 50 double data  
      MPI_Pack_size(50, MPI_DOUBLE, MPI_COMM_WORLD, &BufferSize);
      TempBuffer = malloc(BufferSize);
      
      Position = 0;
      for (i=0;i<50;i++)
	MPI_Pack(&A[i][i], 1, MPI_DOUBLE, 
	         TempBuffer, BufferSize, &Position,MPI_COMM_WORLD);
      printf("buffersize is %d\n",BufferSize);
      MPI_Send(TempBuffer, Position, MPI_PACKED, 1, 0, MPI_COMM_WORLD);       
} 
else  /* RECEIVER CODE */ 
{ 
      MPI_Pack_size(50, MPI_DOUBLE, MPI_COMM_WORLD, &BufferSize);
      TempBuffer = malloc(BufferSize);
      MPI_Recv(TempBuffer, BufferSize, MPI_PACKED,0, 0, MPI_COMM_WORLD,&status);
      Position = 0;
      for(i=0;i<50;i++)
         MPI_Unpack(TempBuffer,BufferSize,&Position,
                    &A[i][i], 1, MPI_DOUBLE, MPI_COMM_WORLD);

      for(i=0;i<5;i++)printf("Proc#1 received %3.1lf from Proc#0\n",A[i][i]);  
}
MPI_Finalize(); /*quit from MPI world*/
return (0);
}
