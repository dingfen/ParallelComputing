#include <stdio.h>
#include <mpi.h>

int compare(int m, int n)
{
    if (m >= n) return m;
    else return n;
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int m = 2000;
    int n = 2400;
    printf("%d\n", compare(m, n));
    MPI_Finalize();
}
