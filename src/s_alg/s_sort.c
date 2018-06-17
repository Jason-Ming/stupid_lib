
#include "s_alg.h"

_VOID shell_sort(_S32 a[], _S32 n)
{
    _S32 i, j, k, h, tmp;
    _S32 cols[] = {1391376, 463792, 198768, 86961, 33936, 13776, 4592,
                    1968, 861, 336, 112, 48, 21, 7, 3, 1};
    for (k=0; k < SIZE_OF_ARRAY(cols); k++)
    {
        h=cols[k];
        for (i=h; i<n; i++)           //i=h,��ζ�Ŵӵڶ��п�ʼ��ÿ��InsertSort
        {
            tmp=a[i];                 //����������Ҫ����ʱ������
            j=i;
            while (j>=h && a[j-h]>tmp)
            {
                a[j]=a[j-h];
                j=j-h;
            }
            a[j]=tmp;
        }
    }
}

