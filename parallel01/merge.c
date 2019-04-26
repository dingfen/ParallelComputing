#include <stdio.h>
#include <stdlib.h>
//Merge函数合并两个子数组形成单一的已排好序的字数组
//并代替当前的子数组A[p..r] 
void Merge(int *a, int p, int q, int r)
{
    int i,j,k;
    int n1 = q - p + 1;
    int n2 = r - q;
    int *L = (int*)malloc((n1+1)*sizeof(int));
    int *R = (int*)malloc((n2+1)*sizeof(int));
    for(i=0; i<n1; i++)
        L[i] = a[p+i];
    L[i] = 65536;
    for(j=0; j<n2; j++)
        R[j] = a[q+j+1];
    R[j] = 65536;
    i=0,j=0;
    for(k=p; k<=r; k++){
        if(L[i]<=R[j]){
            a[k] = L[i];
            i++;
        }
        else{
            a[k] = R[j];
            j++;
        }
    }
    free(L);
    free(R);
} 
//归并排序
void MergeSort(int *a, int p, int r)
{
    if(p<r){
        int q = (p+r)/2;
        MergeSort(a,p,q);
        MergeSort(a,q+1,r);
        Merge(a,p,q,r);
    }
} 

int main()
{
    int a[] = {15,46,48, 93, 39,
     6, 72, 91, 14, 36, 69, 40, 89,
      61, 97, 12, 21, 54, 53, 97, 
      84, 58, 32, 27, 33, 72, 20};
    MergeSort(a, 0, 9);
    for(int i=0;i<27;i++)
        printf("%d ",a[i]);
}