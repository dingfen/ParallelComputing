
#include "mpi.h" /*MPI head file*/

#include <stdio.h>

int main( int argc, char** argv )
{
int i, rank, size;

double ain[30], aout[30];           
int  ind[30];               
struct {                   
  double val;
  int   rank;
} in[30], out[30];


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


  /* each process has an array of 30 double: ain[30] 
       */ 

srandom((unsigned int)(&in[0]));
  
     for (i=0; i<30; ++i) { 
          in[i].val = ain[i] = (double)(random()%1000); 
          in[i].rank = rank;
     } 
  MPI_Reduce( in, out, 30, MPI_DOUBLE_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD); 
   /* At this point, the answer resides on process root   */ 
  if (rank == 0 ) { 
        /* read ranks out  */ 
    for (i=0; i<30; ++i) { 
       aout[i] = out[i].val; 
       ind[i] = out[i].rank; 
       printf("aout[%d] = %f  ind[%d] = %d\n",i,aout[i],i,ind[i]);
    } 
  }

MPI_Barrier(MPI_COMM_WORLD);

MPI_Finalize(); /*quit from MPI world*/
return (0);
}
