#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>

int compare(const void *p1, const void *p2)
{
    return (*(int *)p1 - *(int *)p2);
}

//Merge函数合并两个子数组形成单一的已排好序的字数组
//并代替当前的子数组A[p..r]
void Merge(int *a, int p, int q, int r)
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
void MergeSort(int *a, int p, int r)
{
    if (p < r)
    {
        int q = (p + r) / 2;
        MergeSort(a, p, q);
        MergeSort(a, q + 1, r);
        Merge(a, p, q, r);
    }
}

int main()
{
    int a[1024];      // 输入的数组
    int result[1024]; // 排好顺序的数组
    int i = 0, j, k = 0;
    int p = 3;
    FILE *fin;
    fin = fopen("input.txt", "r");
    while (fscanf(fin, "%d", &a[i]) != EOF)
    {
        i++;
    }

    int len = i;
    int group = len / p;
    int mod = len % p;

    omp_set_num_threads(p);
    int b[p * p];          // 采样排序
    int pivot[p + 1];      // 主元数组
    int pivot_a[p][p + 1]; // 重排序用到的 记录位置的数组
    int newpivot[p + 1];
    for (i = 0; i < p + 1; i++)
        newpivot[i] = 0;

#pragma omp parallel private(i, j, k) shared(b, pivot, pivot_a, result, newpivot)
    {
        // 均匀划分 局部排序
        int id = omp_get_thread_num();
        if(id!=p-1)
            qsort(a + id * group, group, sizeof(int), compare);
        else qsort(a + id * group, group + mod, sizeof(int), compare);

        // 正则采样
        for (j = 0; j < p; j++)
            b[j + id * group / p] = a[id * group + j * group / p];

// 采样排序
#pragma omp barrier
#pragma omp single
        {
            qsort(b, p * p, sizeof(int), compare);
            // 选择p-1个主元
            for (i = 1; i < p; i++)
                pivot[i] = b[i * p];
        }
        // pivot_a 计算出小于主元的数的位置
        // 6 14 15 | 39 46 48 | 72 91 93
        // 12 21 | 36 40 54 61 69 | 89 97
        // 20 27 32 33 | 53 58 | 72 84 97 
        // 3 6 
        // 2 7
        // 4 6
        for (j = 0; j < group; j++)
        {
            for (i = 0; i <= p; i++)
            {
                if (i == p)
                {
                    if(id != p-1)
                        pivot_a[id][i] = group;
                    else pivot_a[id][i] = group + mod;
                    continue;
                }
                if (i == 0)
                {
                    pivot_a[id][i] = 0;
                    continue;
                }
                if (a[id * group + j] <= pivot[i])
                    pivot_a[id][i] = j + 1;
            }
        }
#pragma omp critical
        for (j = 1; j <= p; j++)
        {
            int sumoffset = 0;
            for (i = 0; i < p; i++)
                if (i >= id)
                    sumoffset += pivot_a[i][j - 1];
                else
                    sumoffset += pivot_a[i][j];
            memcpy(result + sumoffset, &a[id * group + pivot_a[id][j - 1]], (pivot_a[id][j] - pivot_a[id][j - 1]) * sizeof(int));
        }

#pragma omp barrier
#pragma omp single
        {
            for (i = 1; i <= p; i++)
                for (j = 0; j < p; j++)
                    newpivot[i] += pivot_a[j][i];
        }
        MergeSort(result, newpivot[id], newpivot[id + 1] - 1);
    }

    printf("原输入的数据：\n");
    for (int i = 0; i < len; i++)
        printf("%d,", a[i]);

    printf("\n 打印主元：%d, %d\n", pivot[1], pivot[2]);
    for (i = 0; i < p; i++)
    {
        for (j = 0; j <= p; j++)
            printf("%d,", pivot_a[i][j]);
        printf("\n");
    }

    printf("排序结果：\n");
    for (i = 0; i < len; i++)
        printf("%d ", result[i]);
}
