
#include "mpi.h" /*MPI head file*/

#include <stdio.h>


int main( int argc, char** argv )
{
   #define LEN 5
   int i, rank, size, true;
   
   double a[LEN], b[LEN]; 
                                                                                                  

   MPI_Status status; 

   MPI_Init(&argc, &argv); /*initializing */
   MPI_Comm_rank(MPI_COMM_WORLD, &rank); /*Process#*/
   MPI_Comm_size(MPI_COMM_WORLD, &size); /*Total processes#*/

//int MPI_Scan(
//    void* sendbuf, 
//    void* recvbuf, 
//    int count, 
//    MPI_Datatype datatype, 
//    MPI_Op op, 
//    MPI_Comm comm ) 



//  srandom(((unsigned int)(&in.val))*(rank+10));

 for(i=0;i<LEN;i++)
     a[i]  = (double)(i+1);
  MPI_Barrier(MPI_COMM_WORLD);
    printf("Before Scan Proc# %d : ", rank);
    for(i=0;i<LEN;i++) printf("%5.1f",a[i]);
    printf("\n");
  MPI_Barrier(MPI_COMM_WORLD);


 MPI_Scan( a, b, LEN, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD ); 
                                             

  MPI_Barrier(MPI_COMM_WORLD);
     printf("After  Scan Proc# %d : ", rank);   
     for(i=0;i<LEN;i++) printf("%5.1f",b[i]);
     printf("\n"); 
  MPI_Barrier(MPI_COMM_WORLD);

MPI_Finalize(); /*quit from MPI world*/
return (0);
}
