#include "sort.h"

//Merge函数合并两个子数组形成单一的已排好序的字数组
//并代替当前的子数组A[p..r]
void merge(int *a, int p, int q, int r)
{
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
    for (k = p; k <= r; k++)
    {
        if (L[i] <= R[j])
        {
            a[k] = L[i];
            i++;
        }
        else
        {
            a[k] = R[j];
            j++;
        }
    }
}


//归并排序
void merge_sort(int *a, int p, int r)
{
    if (p < r)
    {
        int q = (p + r) / 2;
        merge_sort(a, p, q);
        merge_sort(a, q + 1, r);
        merge(a, p, q, r);
    }
}


int main(int argc, char *argv[])
{
    int *array;
    int *result;
    int len;
    int processors;
    int id;
    int group;
    int group_len;
    int mod;

    int i, index;
    // 产生随机数据部分
    if (argc != 2) {
        fprintf(stderr, "Please input the num of array!\n");
        return 1;
    }
    i = atoi(argv[1]);
    array = (int*)malloc((i+2)*sizeof(int));
    result = (int*)malloc((i+2)*sizeof(int));
    random_array(array, i);

    // 开始mpi
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &processors);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Status status;
    MPI_Request request;
    
    len = i;
    group = len / processors;
    mod = len % processors;
    
    int sample[processors * processors];
    int pivot[processors];
    
    // 均匀划分 局部排序
    if (id != processors - 1) {
        qsort(array + id * group, group, sizeof(int), compare);
        group_len = group;
    }
    else {
        qsort(array + id * group, group + mod, sizeof(int), compare);
        group_len = group+mod;
    }
    // 正则采样
    for (i = 0; i < processors; i++)
        sample[i + id * group / processors] = array[id * group + i * group / processors];

    // 采样到的样本全部放到0号线程
    if (id == 0)
        for (i = 1; i < processors; i++)
            MPI_Recv(sample + processors * i, processors, MPI_INT, i, 100 + i, MPI_COMM_WORLD, &status);
    else
        MPI_Send(sample + id * group / processors, processors, MPI_INT, 0, 100 + id, MPI_COMM_WORLD);

    // 采样排序
    if (id == 0)
    {
        qsort(sample, processors * processors, sizeof(int), compare);
        for (i = 0; i < processors-1; i++) 
            pivot[i] = sample[(i+1) * processors];
    // 分发主元
        for(i = 1;i < processors; i++)
            MPI_Send(pivot, processors, MPI_INT, i, 110+i, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // 接收主元
    if(id != 0)
        MPI_Recv(pivot, processors, MPI_INT, 0, 110+id, MPI_COMM_WORLD, &status);

    // 进行主元划分
    index = 0;
    int partionSize[processors];
    for(i = 0;i < processors;i++)
        partionSize[i] = 0;
    
    for(i = 0; i < group_len;i++) {
        if(array[i+id*group] > pivot[index])
            index++;
        if(index == processors) {
            partionSize[processors-1] = group_len -i+1;
            break;
        }
        partionSize[index]++;
    }

    // 广播分组 先发送各个段的长度
    int newSize[processors];
    MPI_Alltoall(partionSize, 1, MPI_INT, newSize, 1, MPI_INT, MPI_COMM_WORLD);

    // 计算位移
    int sendIndex[processors];
    int recvIndex[processors];

    sendIndex[0] = 0;
    recvIndex[0] = 0;
    for(i = 1;i < processors;i++) {
        sendIndex[i] = sendIndex[i-1]+partionSize[i-1];
        recvIndex[i] = recvIndex[i-1]+newSize[i-1];
    }
    // 计算总长度
    int totalSize = 0;
    for(i = 0;i < processors;i++)
        totalSize += newSize[i];


    // 每个处理器发送数据给其他所有处理器，且每个处理发送的数据长度都不同
    // 故有长度数组和位移数组
    MPI_Alltoallv(array+id*group, partionSize, sendIndex, MPI_INT, 
        result, newSize, recvIndex, MPI_INT, MPI_COMM_WORLD);

    // 归并排序
    merge_sort(result, 0, totalSize-1);

    MPI_Gather(&totalSize, 1, MPI_INT, recvIndex, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int recvDisp[processors];
    recvDisp[0];
    for(i = 1;i < processors;i++)
        recvDisp[i] = recvIndex[i-1] + recvDisp[i-1];

    MPI_Gatherv(result, totalSize, MPI_INT, array, recvIndex, recvDisp, MPI_INT, 0, MPI_COMM_WORLD);

    if(id == 0)
        print_array(array, len);
    
    MPI_Finalize();
    return 0;
}
