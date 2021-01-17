#include "sort.h"

//Merge函数合并两个子数组形成单一的已排好序的字数组
//并代替当前的子数组A[p..r]
void merge(int *a, int p, int q, int r) {
    int i, j, k;
    int n1 = q - p + 1;
    int n2 = r - q;
    int L[n1 + 1];
    int R[n2 + 1];
    for (i = 0; i < n1; i++)
        L[i] = a[p + i];
    L[i] = 65536;
    for (j = 0; j < n2; j++)
        R[j] = a[q + j + 1];
    R[j] = 65536;
    i = 0, j = 0;
    for (k = p; k <= r; k++) {
        if (L[i] <= R[j]) {
            a[k] = L[i];
            i++;
        }
        else {
            a[k] = R[j];
            j++;
        }
    }
}


//归并排序
void merge_sort(int *a, int p, int r) {
    if (p < r) {
        int q = (p + r) / 2;
        merge_sort(a, p, q);
        merge_sort(a, q + 1, r);
        merge(a, p, q, r);
    }
}


int main(int argc, char *argv[])
{
    int *array, *a;
    int *result, *ans;
    int n;
    int num_procs, id_procs;
    int group, group_len;
    int mod;

    int i, index;

    // 开始mpi
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_procs);
    MPI_Status status;
    MPI_Request request;

    if (argc != 2) {
        fprintf(stderr, "Please input the num of array!\n");
        return 1;
    }
    n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Please, the size must > 0.\n");
        return 1;
    }

    group = n / num_procs;
    mod = n % num_procs;
    a = (int *)malloc((group+mod)*sizeof(int));

    if (id_procs == 0) {
        // 产生随机数据
        // 串行运行 得出结果
        array = (int*)malloc((n+2)*sizeof(int));
        result = (int*)malloc((n+2)*sizeof(int));
        ans = (int*)malloc((n+2)*sizeof(int));
        random_array(array, n);
        memcpy(ans, array, (n)*sizeof(int));
        qsort(ans, n, sizeof(int), compare);
    }

    // 分段数据 移交各个处理器
    int sendcounts[num_procs];
    int sdispls[num_procs];
    for(int i = 0; i < num_procs; i++) {
        sendcounts[i] = group;
        sdispls[i] = i*group;
    }
    sendcounts[num_procs-1] = group+mod;

    MPI_Scatterv(array, sendcounts, sdispls, MPI_INT, a, group+mod, MPI_INT, 0, MPI_COMM_WORLD);

    group_len = sendcounts[id_procs];
    // 均匀划分 局部排序
    qsort(a, group_len, sizeof(int), compare);

    // 正则采样
    int samples[num_procs * num_procs];
    int s[num_procs];    
    for (i = 1; i < num_procs; i++) {
        s[i-1] = a[i * group / num_procs];
    }
    // 采到样本 收集到Proc#0
    MPI_Gather(s, num_procs-1, MPI_INT, samples, num_procs-1, MPI_INT, 0, MPI_COMM_WORLD);

    // 采样排序
    int pivot[num_procs];
    if (id_procs == 0) {
        qsort(samples, (num_procs-1)*num_procs, sizeof(int), compare);
        for (i = 1; i < num_procs; i++) 
            pivot[i-1] = samples[i*(num_procs-1)];
    }
    // 广播主元
    MPI_Bcast(pivot, num_procs-1, MPI_INT, 0, MPI_COMM_WORLD);

    // 进行主元划分
    index = 0;
    int pcounts[num_procs];
    for(i = 0; i < num_procs; i++)
        pcounts[i] = 0;
    pivot[num_procs-1] = INT32_MAX;

    for(i = 0; i < group_len; i++) {
        if(a[i] <= pivot[index])
            pcounts[index]++;
        else {
            i--;
            index++;
        }
    }

    // 发送各个段的长度
    int rcounts[num_procs];
    MPI_Alltoall(pcounts, 1, MPI_INT, rcounts, 1, MPI_INT, MPI_COMM_WORLD);

    // 计算位移
    int rdispls[num_procs];
    sdispls[0] = 0;
    rdispls[0] = 0;
    for(i = 1;i < num_procs;i++) {
        sdispls[i] = sdispls[i-1] + pcounts[i-1];
        rdispls[i] = rdispls[i-1] + rcounts[i-1];
    }
    // 计算总长度
    int totalcounts = 0;
    for(i = 0; i < num_procs; i++)
        totalcounts += rcounts[i];

    int *b = (int *)malloc(totalcounts*sizeof(int));

    // 每个处理器发送数据给其他所有处理器，且每个处理发送的数据长度都不同
    // 故有长度数组和位移数组
    MPI_Alltoallv(a, pcounts, sdispls, MPI_INT, 
        b, rcounts, rdispls, MPI_INT, MPI_COMM_WORLD);

    // 归并排序
    merge_sort(b, 0, totalcounts-1);

    // Proc#0 收集有序数组
    MPI_Gather(&totalcounts, 1, MPI_INT, rcounts, 1, MPI_INT, 0, MPI_COMM_WORLD);

    rdispls[0] = 0;
    for(i = 1; i < num_procs; i++)
        rdispls[i] = rdispls[i-1] + rcounts[i-1];

    MPI_Gatherv(b, totalcounts, MPI_INT, result, rcounts, rdispls, MPI_INT, 0, MPI_COMM_WORLD);

    if(id_procs == 0) {
        if (check_array(result, ans, n))
            printf("Done.\n");
        free(ans);
        free(result);
        free(array);
    }
    if (b != NULL) {
        free(b);
        b = NULL;
    }
    if (a != NULL) {
        free(a);
        a = NULL;
    }
    
    MPI_Finalize();
    return 0;
}
