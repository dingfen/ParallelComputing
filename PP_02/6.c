#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef PNum
#define PNum 2
#endif

enum MsgTag {
    RANDATA,
    AVERAGE
};

void work(int id) {
    int randata;
    int recvdata;
    MPI_Status status;

    while(1) {
        srand(time(NULL)+id);
        randata = rand() % 100;
        MPI_Send(&randata, 1, MPI_INT, id % PNum, RANDATA, MPI_COMM_WORLD);
        MPI_Recv(&recvdata, 1, MPI_INT, id % PNum, AVERAGE, MPI_COMM_WORLD, &status);
        printf("Proc#%d receive average data = %d\n", id, recvdata);
    }
}

void serve(int id, int num) {
    int input[num];
    int average;
    int sum, ctn;
    MPI_Status status;

    while (1) {
        sum = 0;
        ctn = 0;
        for(int i = 1; i*PNum+id<num; i++) {
            MPI_Recv(input+i, 1, MPI_INT, i*PNum+id, RANDATA, MPI_COMM_WORLD, &status);
            ctn++;
        }
        printf("Proc#%d, input data = ", id);
        for(int i = 1; i <= ctn; i++) {
            sum += input[i];
            printf("%d, ", input[i]);
        }
        average = sum / ctn;
        printf("Proc#%d send average data = %d\n", id, average);
        for(int i = 1; i <= ctn; i++) {
            MPI_Send(&average, 1, MPI_INT, i*PNum+id, AVERAGE, MPI_COMM_WORLD);
        }
    }
    
}

int main(int argc, char *argv[])
{
    int id_procs, num_procs;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_procs);

    int P, Q;
    P = PNum;
    Q = num_procs - P;
    
    if (id_procs > P-1) {
        work(id_procs);
    }
    else {
        serve(id_procs, num_procs);
    }

    MPI_Finalize();
    return 0;
}
