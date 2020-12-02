
#include "mpi.h" /*MPI head file*/

#include <stdio.h>

     
 /* the user-defined function  */ 
#define LEN 5

void myProd( double *in, double *inout, int *len, MPI_Datatype *dptr ) 
{ 
    int i,j; 
    for (i=0; i< *len; ++i)        
      for(j=0;j<LEN*LEN;j++) {
          *inout = (*inout)* (*in); 
          in++; inout++; 
      }
}

int main( int argc, char** argv )
{
   int i, j, rank, size;
   
   double a[LEN][LEN], answer[LEN][LEN]; 
   MPI_Op myOp; 
   MPI_Datatype ctype; 
                                                                                                  

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


//  srandom(((unsigned int)(&in.val))*(rank+10));

 /* explain to MPI how type Complex is defined   */ 
 MPI_Type_contiguous( LEN*LEN, MPI_DOUBLE, &ctype ); 
 MPI_Type_commit( &ctype ); 

 for(i=0;i<LEN;i++)
   for(j=0;j<LEN;j++)
     a[i][j] = (double)(i+j+1);

 /* create the sum of matrix user-op  */ 
 
 MPI_Op_create( myProd,1, &myOp ); 

 MPI_Reduce( a, answer, 1, ctype, myOp, 0, MPI_COMM_WORLD ); 
                                             

   /* At this point, the answer resides on process root   */ 
  if (rank == 0 ) { 
        /* read ranks out  */ 
    for(i=0;i<LEN;i++){
      for(j=0;j<LEN;j++)
        printf("%10.1f ",answer[i][j]);
      printf("\n");
    }

  }

MPI_Barrier(MPI_COMM_WORLD);

MPI_Finalize(); /*quit from MPI world*/
return (0);
}
