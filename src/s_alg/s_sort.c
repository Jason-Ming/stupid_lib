#include <time.h>

#include "s_log.h"
#include "s_alg.h"

_VOID insert_sort(_S32 a[], _S32 n)
{
    clock_t time_taken = clock();
    _S32 i, j, tmp;
    for (i=1; i<n; i++)
    {
        tmp=a[i];
        j=i;
        while (j>=1 && a[j-1]>tmp)
        {
            a[j]=a[j-1];
            j=j-1;
        }
        a[j]=tmp;
    }

    time_taken = clock() - time_taken;
    DEBUG_PRINT("insert_sort took %lu clocks (%lu seconds)\n", (_UL)time_taken, (_UL)time_taken/CLOCKS_PER_SEC);
}

_VOID shell_sort(_S32 a[], _S32 n)
{
    clock_t time_taken = clock();
    _S32 i, j, k, h, tmp;
    _S32 cols[] = {1391376, 463792, 198768, 86961, 33936, 13776, 4592,
                    1968, 861, 336, 112, 48, 21, 7, 3, 1};
    for (k=0; k < SIZE_OF_ARRAY(cols); k++)
    {
        h=cols[k];
        for (i=h; i<n; i++)           //i=h,意味着从第二行开始对每列InsertSort
        {
            tmp=a[i];                 //插入排序需要的临时变量。
            j=i;
            while (j>=h && a[j-h]>tmp)
            {
                a[j]=a[j-h];
                j=j-h;
            }
            a[j]=tmp;
        }
    }

    time_taken = clock() - time_taken;
    DEBUG_PRINT("shell_sort took %lu clocks (%lu seconds)\n", (_UL)time_taken, (_UL)time_taken/CLOCKS_PER_SEC);
}

