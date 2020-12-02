
#include "mpi.h" /*MPI head file*/

#include <stdio.h>

int main( int argc, char** argv )
{
int myrank,size;

int i;

struct Partstruct 
   { 
      int    class;  /* particle class */ 
      double d[6];   /* particle coordinates */ 
      char   b[7];   /* some additional information */ 
   }Par,p[100]; 
               


MPI_Status status; 
MPI_Init(&argc, &argv); /*initializing */
MPI_Comm_rank(MPI_COMM_WORLD, &myrank); /*Process#*/
MPI_Comm_size(MPI_COMM_WORLD, &size); /*Total processes#*/

if (myrank == 0) 
{ 
      /* SENDER CODE */ 
      
      Par.class = 100;
      for(i=0;i<6;i++) Par.d[i] = (double)i;
      for(i=0;i<7;i++) Par.b[i] = 'A' + i;

      printf("Par = %d, P = %d\n",sizeof(Par), sizeof(p));            

      MPI_Send(&Par, sizeof(Par), MPI_BYTE, 1,0, MPI_COMM_WORLD); 
} 
else  /* RECEIVER CODE */ 
{ 
      MPI_Recv(&Par, sizeof(Par), MPI_BYTE,0,0,MPI_COMM_WORLD,&status);
    
      printf("Proc#1 received structs from Proc#0 : \n");

      printf("class = %d\n", Par.class);

      printf("array d is : ");
      for(i=0;i<6;i++) printf("%3.1lf ",Par.d[i]);

      printf("\narray b is : ");
      for(i=0;i<7;i++) printf("%c ",Par.b[i]);        

      printf("\n");
}
MPI_Finalize(); /*quit from MPI world*/
return (0);
}
