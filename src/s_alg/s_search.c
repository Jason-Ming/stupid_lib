#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "s_defines.h"
#include "s_log.h"
#include "s_alg.h"

/* binsearch: find x in v[0] <= v[1] <= ... <= v[n-1] */ 
_S32 binsearch(_S32 x, _S32 v[], _S32 n) 
{ 
    _S32 low, high, mid; 
    low = 0; 
    high = n - 1; 
    
    while (low <= high) 
    { 

        mid = (low+high)/2;
        
        if (x < v[mid]) 
        {
            high = mid - 1; 
        }
        else if (x > v[mid]) 
        {
            low = mid + 1; 
        }
        else /* found match */ 
        {
            return mid; 
        }
    } 
    return -1; /* no match */ 
} 


_S32 binsearch2(_S32 x, _S32 v[], _S32 n) 
{ 
    int low, high, mid;
    low = -1;
    high = n;
    while (low + 1 < high) {
    mid = (low + high) / 2;
    if (v[mid] < x)
    low = mid;
    else
    high = mid;
    }
    if (high == n || v[high] != x)
    return -1;
    else
    return high;

} 

