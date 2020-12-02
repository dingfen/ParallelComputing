
#include "mpi.h" /*MPI head file*/

#include <stdio.h>

typedef struct { 
  double real,imag; 
} Complex; 
     
 /* the user-defined function  */ 
void myProd( Complex *in, Complex *inout, int *len, MPI_Datatype *dptr ) 
{ 
    int i; 
    Complex c; 
               
    for (i=0; i< *len; ++i) { 
      c.real = inout->real*in->real - inout->imag*in->imag; 
      c.imag = inout->real*in->imag + inout->imag*in->real; 
      *inout = c; 
      in++; inout++; 
} 
                                                                                         } 
                                                                                         

int main( int argc, char** argv )
{
   #define LEN 5
   int i, rank, size;
   
   Complex a[LEN], answer[LEN]; 
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
 MPI_Type_contiguous( 2, MPI_DOUBLE, &ctype ); 
 MPI_Type_commit( &ctype ); 

 for(i=0;i<LEN;i++)
   { 
     a[i].real = (double)(i+1);
     a[i].imag = (double)(i+2);
   }
 /* create the complex-product user-op  */ 
 MPI_Op_create( myProd,1, &myOp ); 

 MPI_Reduce( a, answer, LEN, ctype, myOp, 0, MPI_COMM_WORLD ); 
                                             

   /* At this point, the answer resides on process root   */ 
  if (rank == 0 ) { 
        /* read ranks out  */ 
     printf("answer[0]: real = %f, imag = %f\n",answer[0].real, answer[0].imag); 
  }

MPI_Barrier(MPI_COMM_WORLD);

MPI_Finalize(); /*quit from MPI world*/
return (0);
}
