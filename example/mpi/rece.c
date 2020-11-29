#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(size==1){
        printf("Too less of process.\n");
        MPI_Abort(MPI_COMM_WORLD, MPI_ERR_COUNT);
    }
    char buf[] = "hello world\n";
    MPI_Status status;
    if(rank!=0){
        MPI_Send(buf, 13, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    else{
        char *ans = (char *)malloc(13*sizeof(char));
        MPI_Recv(ans, 13, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        printf("ans is %s\n", ans);
    }
    
    MPI_Finalize();
    return 0;
}
