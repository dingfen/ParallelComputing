
#include "mpi.h" /*MPI head file*/

#include <stdio.h>

int main( int argc, char** argv )
{

#define LEN 1000

int i, rank, size;
int j;
double ain[LEN];

struct {                   
  double val;
  int   index;
} in, out;


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


  srandom(((unsigned int)(&in.val))*(rank+10));

  for(i=0;i<LEN;++i)
    ain[i] = (double)(random()%1000+random()%1000);

// find local minloc
     in.val = ain[0];
     in.index = 0; 

     for (i=1;i<LEN; ++i) { 
          if( in.val > ain[i] ) {
              in.val = ain[i];
              in.index = i;
          } 
     } 
  in.index = rank * LEN + in.index;

  MPI_Reduce( &in, &out, 1, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD); 
   /* At this point, the answer resides on process root   */ 
  if (rank == 0 ) { 
       int minrank, minindex;
        /* read ranks out  */ 
       minrank = out.index / LEN;
       minindex = out.index % LEN;
       printf("Proc#%d has the Minimum of ain[%d] = %f  \n",
                               minrank,minindex,out.val);
  }

MPI_Barrier(MPI_COMM_WORLD);

MPI_Finalize(); /*quit from MPI world*/
return (0);
}
