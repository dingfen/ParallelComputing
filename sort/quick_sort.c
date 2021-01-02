#include "sort.h"

enum TagType {
    ORDER = 1,
    UNORDER,
    MMSG,
    LEN
};


int getpow2(int n) {
    int ans = 0;
    while((n = n >> 1))
        ans++;
    return ans;
}


int partition(int *a, int k, int l)
{
    srand(clock());
    int r = rand() % (l-k+1);
    int pivof = a[r];
    int temp = a[l];
    a[l] = a[r];
    a[r] = temp;
    int i = k-1;
    for(int j = k; j < l; j++) {
        if (a[j] <= pivof) {
            i++;
            temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
    }
    temp = a[i+1];
    a[i+1] = a[l];
    a[l] = temp;
    return i+1;
}


void do_sort(int *a, int i, int j)
{
    if (i < j) {
        int r = partition(a, i, j);
        do_sort(a, i, r-1);
        do_sort(a, r+1, j);
    }
}


void quicksort(int *a, int beg, int end, int m, int id_procs)
{
    int pivot;
    int dest;
    int len;
    MPI_Status status;
    if (beg < end) {
        if (m > 0) {
            dest = id_procs + (1 << (m-1));
            pivot = partition(a, beg, end);
            len = end-(pivot+1)+1;
            m = m-1 > 0 ? m-1:0;
            // send info to Proc#id+2^m-1, ask it run latter array
            MPI_Send(&len, 1, MPI_INT, dest, LEN, MPI_COMM_WORLD);
            MPI_Send(&m, 1, MPI_INT, dest, MMSG, MPI_COMM_WORLD);
            MPI_Send(&a[pivot+1], len, MPI_INT, dest, UNORDER, MPI_COMM_WORLD);
            printf("Proc#%d send %d data to Proc#%d\n", id_procs, len, dest);

            // run former array local
            quicksort(a, beg, pivot-1, m, id_procs);
            
            // Get the order array from Proc#id+2^m-1
            if (len > 1) {
                int *b = (int *)malloc(len*sizeof(int));
                MPI_Recv(b, len, MPI_INT, dest, ORDER, MPI_COMM_WORLD, &status);
                printf("Proc#%d recv %d data from Proc#%d\n", id_procs, len, dest);
                // Merge
                memcpy(&a[pivot+1], b, len*sizeof(int));
                free(b);
            }

            if ((id_procs >> (m+1)) % 2) {
                dest = id_procs - (1 << (m+1));
                len = end -beg +1;
                MPI_Send(a, len, MPI_INT, dest, ORDER, MPI_COMM_WORLD);
                printf("Proc#%d send %d data to Proc#%d\n", id_procs, len, dest);
            }
        }
        else {
            pivot = partition(a, beg, end);
            do_sort(a, beg, pivot-1);
            do_sort(a, pivot+1, end);

            // return the order array
            if (id_procs % 2) {
                dest = id_procs -1;
                len = end - beg +1;
                MPI_Send(a, len, MPI_INT, dest, ORDER, MPI_COMM_WORLD);
                printf("Proc#%d send %d data to Proc#%d\n", id_procs, len, dest);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int num_procs, id_procs;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_procs);

    if (argc != 2) {
        fprintf(stderr, "Please input the size of data array.\n");
        return 1;
    }
    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Please, the size must > 0.\n");
        return 1;
    }

    int *a, *b;
    int m = getpow2(num_procs);
    int len;
    a = (int*)malloc((n+2)*sizeof(int));
    if (id_procs == 0) {
        b = (int*)malloc((n+2)*sizeof(int));
        random_array(a, n);
        memcpy(b, a, n*sizeof(int));
        qsort(b, n, sizeof(int), compare);
    }


    if (id_procs != 0) {
        // None-0 Proc Get UnOrder Data from other
        MPI_Recv(&len, 1, MPI_INT, MPI_ANY_SOURCE, LEN, MPI_COMM_WORLD, &status);
        printf("Proc#%d Run!\n", id_procs);
        MPI_Recv(&m, 1, MPI_INT, MPI_ANY_SOURCE, MMSG, MPI_COMM_WORLD, &status);
        MPI_Recv(a, len, MPI_INT, MPI_ANY_SOURCE, UNORDER, MPI_COMM_WORLD, &status);
        quicksort(a, 0, len-1, m, id_procs);
    } else {
        quicksort(a, 0, n-1, m, id_procs);
    }

    if (id_procs == 0) {
        print_array(a, n);
    }

    free(a);
    if (id_procs == 0) {
        free(b);
    }
    MPI_Finalize();
    return 0;
}
