
#include "mpi.h" /*MPI head file*/

#include <stdio.h>

#define RELA 

int main( int argc, char** argv )
{
int myrank,size;

int i;

struct Partstruct 
   { 
      int class; 
      double d[6]; 
      char b[7]; 
   }; 
                
 struct Partstruct particle; 
                 
              
 MPI_Datatype Particletype; 
 MPI_Datatype type[3] = {MPI_INT, MPI_DOUBLE, MPI_CHAR}; 
 int          block[3] = {1, 6, 7}; 
 MPI_Aint     disp[3]; 
 int base;    // using relative displacement                  

MPI_Status status; 
MPI_Init(&argc, &argv); /*initializing */
MPI_Comm_rank(MPI_COMM_WORLD, &myrank); /*Process#*/
MPI_Comm_size(MPI_COMM_WORLD, &size); /*Total processes#*/


 /* Particletype : using absolute addresses */ 
 
  MPI_Address( &particle, disp);
  MPI_Address( &particle.d, disp+1);   
  MPI_Address( &particle.b, disp+2);   



#ifdef RELA
//  the following codes use relative displacement
  base = disp[0]; 
  for (i=0; i <3; i++) disp[i] -= base; 
  printf(" Using realtive displacement\n");
#else
 printf(" Using absolute address\n");
#endif


  MPI_Type_struct( 3, block, disp, type, &Particletype); 

  MPI_Type_commit( &Particletype);


if (myrank == 0) 
{ 
      /* SENDER CODE */ 
      
      particle.class = 100;
      for(i=0;i<6;i++) particle.d[i] = (double)i;
      for(i=0;i<7;i++) particle.b[i] = 'A' + i;

#ifdef RELA

// Using relative displacement
    MPI_Send( &particle,  1, Particletype, 1, 0, MPI_COMM_WORLD); 

#else

// Using absolute address
    MPI_Send( MPI_BOTTOM, 1, Particletype, 1, 0, MPI_COMM_WORLD); 

#endif
} 
else  /* RECEIVER CODE */ 
{ 
#ifdef RELA
// Using relative displacement

    MPI_Recv(&particle , 1, Particletype, 0, 0, MPI_COMM_WORLD, &status);

#else
// Using absolute address 

    MPI_Recv(MPI_BOTTOM, 1, Particletype, 0, 0, MPI_COMM_WORLD,&status);

#endif    
      printf("Proc#1 received structs Particle from Proc#0 : \n");

      printf("class = %d\n", particle.class);

      printf("array d is : ");
      for(i=0;i<6;i++) printf("%3.1lf ",particle.d[i]);

      printf("\narray b is : ");
      for(i=0;i<7;i++) printf("%c ", particle.b[i]);        

      printf("\n");
}
MPI_Finalize(); /*quit from MPI world*/
return (0);
}
