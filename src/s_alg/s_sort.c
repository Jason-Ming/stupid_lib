#include <time.h>
#include <stdio.h>

#include "s_log.h"
#include "s_alg.h"
#include "s_mem.h"

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
    DEBUG_PRINT("insert_sort took %lu clocks (%lu seconds)", (_UL)time_taken, (_UL)time_taken/CLOCKS_PER_SEC);
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
    DEBUG_PRINT("shell_sort took %lu clocks (%lu seconds)", (_UL)time_taken, (_UL)time_taken/CLOCKS_PER_SEC);
}

/* recursive version */
/* quick_sort: sort v[left]...v[right] into increasing order */
_VOID s_qsort_s32(_S32 v[], _S32 left, _S32 right)
{
    _S32 i, last;

    /* do nothing if array contains fewer than two elements */
    if(left >= right)
    {
        return;
    }

    /* move partition elements to v[0] */
    s_swap_s32(v, left, (left + right)/2);

    last = left;
    for(i = left + 1; i <= right; i++)
    {
        if(v[i] < v[left])
        {
            s_swap_s32(v, ++last, i);
        }
    }

    s_swap_s32(v, left, last);
    s_qsort_s32(v, left, last - 1);
    s_qsort_s32(v, last + 1, right);
}

/* qsort: sort v[left]...v[right] into increasing order 
	*/ 
_VOID s_qsort_ptr(_VOID * v[], _S32 left, _S32 right, COMPARE_FUNC compare)
{
	_S32 i, last;

	if (left >= right) /* do nothing if array contains */
	{
		return; /* fewer than two elements */
	}

	s_swap_ptr(v, left, (left + right) / 2);
	last = left;

	for (i = left + 1; i <= right; i++)
	{
		if (compare(v[i], v[left]) < 0)
		{
			s_swap_ptr(v, ++last, i);
		}
	}

	s_swap_ptr(v, left, last);
	s_qsort_ptr(v, left, last - 1, compare);
	s_qsort_ptr(v, last + 1, right, compare);
}



