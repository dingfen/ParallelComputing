
#include "mpi.h" /*MPI head file*/

#include <stdio.h>

int main( int argc, char** argv )
{
int myrank,size;

double f;
int position, i;
int a[2]; 
char buff[1000]; 
int j;

MPI_Status status; 
MPI_Init(&argc, &argv); /*initializing */
MPI_Comm_rank(MPI_COMM_WORLD, &myrank); /*Process#*/
MPI_Comm_size(MPI_COMM_WORLD, &size); /*Total processes#*/

if (myrank == 0) 
{ 
      /* SENDER CODE */ 
      position = 0;
      i = 100; j = 200; f = 1.0;
      MPI_Pack(&i, 1, MPI_INT, buff, 1000, &position, MPI_COMM_WORLD); 
      printf("pos = %d\n",position);
      MPI_Pack(&j, 1, MPI_INT, buff, 1000, &position, MPI_COMM_WORLD);
      printf("pos = %d\n",position);
      MPI_Pack(&f, 1, MPI_DOUBLE, buff, 1000, &position, MPI_COMM_WORLD); 
      printf("pos = %d\n",position);
      MPI_Send( buff, position, MPI_PACKED, 1, 0, MPI_COMM_WORLD); 
} 
else  /* RECEIVER CODE */ 
if(myrank == 1){ 
      //MPI_Recv( a, 2, MPI_INT, 0, 0, MPI_COMM_WORLD,&status);
printf(" before Proc#1 received 2 int(s) from Proc#0 : %d %d\n",a[0],a[1]);

      MPI_Recv(buff, 1000,MPI_PACKED,0,0,MPI_COMM_WORLD,&status);
      position = 0;
      MPI_Unpack(buff,1000,&position,&a[0],1,MPI_INT,MPI_COMM_WORLD);
      MPI_Unpack(buff,1000,&position,&a[1],1,MPI_INT,MPI_COMM_WORLD);
 printf("Proc#1 received 2 int(s) from Proc#0 : %d %d\n",a[0],a[1]);  
}
MPI_Finalize(); /*quit from MPI world*/
return (0);
}
