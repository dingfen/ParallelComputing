#include "sort.h"

int getpow2(int n) {
    int ans = 0;
    while((n = n >> 1))
        ans++;
    return ans;
}

int partition(int *a, int k, int l)
{
    int pivof = a[l];
    int i = k-1;
    for(int j = k; j < l; j++) {
        if (a[j] <= pivof) {
            i++;
            int temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
    }
    int temp = a[i+1];
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

void quicksort(int *a, int beg, int end, int m, int id, int id_procs)
{
    if (beg < end || m == 0) {
        if (id == id_procs) {
            do_sort(a, beg, end);
        }
    } else {
        if (id == id_procs) {
            int r = partition(a, beg, end);
            int size = m-1-(r+1)+1;
            MPI_Send(&a[r+1], size, MPI_INT, id+(1 << (m-1)), ,MPI_COMM_WORLD);
        }

    }
}

int main(int argc, char *argv[])
{
    int num_procs, id_procs;
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
    if (id_procs == 0) {
        a = (int*)malloc((n+2)*sizeof(int));
        b = (int*)malloc((n+2)*sizeof(int));
        random_array(a, n);
        memcpy(b, a, n*sizeof(int));
        qsort(b, n, sizeof(int), compare);
    }

    quicksort(a, 0, n-1, m, 0, id_procs);

    if (id_procs == 0) {
        free(a);
        free(b);
    }
    MPI_Finalize();
    return 0;
}
