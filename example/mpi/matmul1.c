#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

const int rows = 40;  //the rows of matrix
const int cols = 100;  //the cols of matrix

int main(int argc, char* argv[])
{
    int i, j, k, myid, numprocs, anstag;
    int A[rows][cols], B[cols], C[rows];
    int masterpro,buf[cols], ans,cnt;
    double starttime,endtime;
    double tmp,totaltime;

    MPI_Status status;
    masterpro = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    for(cnt = 0; cnt < 100000; cnt++){        
        if(numprocs < 2){
            printf("Error:Too few processes!\n");
            MPI_Abort(MPI_COMM_WORLD,99);
        }
        if(myid == masterpro){
            starttime = MPI_Wtime();
            for (i = 0; i < cols; i++)
            {
                B[i] = rand()%10;
                for (j = 0; j < rows; j++)
                {
                    A[j][i] = rand()%10;
                }
            }
            //bcast the B vector to all slave processor
            MPI_Bcast(B, cols, MPI_INT, masterpro, MPI_COMM_WORLD);
            //partition the A matrix to all slave processor
            for (i = 1; i < numprocs; i++)
            {
                for (k = i - 1; k < rows; k += numprocs - 1)
                {
                    for (j = 0; j < cols; j++)
                    {    
                        buf[j] = A[k][j];
                    }
                    MPI_Send(buf, cols, MPI_INT, i, k, MPI_COMM_WORLD);
                }
            }
        }
        else{
            //starttime = MPI_Wtime();
            MPI_Bcast(B, cols, MPI_INT, masterpro, MPI_COMM_WORLD);
            //every processor receive the part of A matrix,and make Mul operator with B vector
            for ( i = myid - 1; i < rows; i += numprocs - 1){
                MPI_Recv(buf, cols, MPI_INT, masterpro, i, MPI_COMM_WORLD, &status);
                ans = 0;
        
                for ( j = 0; j < cols; j++)
                {
                    ans += buf[j] * B[j];
                }
                //send back the result
                MPI_Send(&ans, 1, MPI_INT, masterpro, i, MPI_COMM_WORLD);
            }
            //endtime = MPI_Wtime();
            //tmp = endtime-starttime;
        } 
        if(myid == masterpro){    
            //receive the result from all slave processor
            for ( i = 0; i < rows; i++)
            {
                MPI_Recv(&ans, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            //sender = status.MPI_SOURCE;
                anstag = status.MPI_TAG;
                C[anstag] = ans;
            }
            //print the result
        
            for (i = 0; i < rows; i++)
            {
                printf("%d ",C[i]);
                if((i+1)%20 == 0)
                    printf("\n");
            }
        
        }
    }
    endtime = MPI_Wtime();
    totaltime = endtime-starttime;
    //printf("cost time:%f s.\n",tmp); 
    //MPI_Reduce(&tmp,&totaltime,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    if(myid == masterpro)
        printf("total time:%f s.\n",totaltime);
    MPI_Finalize();
    return 0;
}