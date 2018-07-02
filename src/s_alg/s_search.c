#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "s_defines.h"
#include "s_log.h"
#include "s_alg.h"


_S32 binsearch(_S32 x, _S32 v[], _S32 n) 
{
    clock_t time_taken = clock();
    _S32 low, high, mid, result = -1/* no match */; 
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
            result = mid;
            break;
        }
    }
    
    time_taken = clock() - time_taken;
    DEBUG_PRINT("binsearch took %lu clocks (%lu seconds)\n", (_UL)time_taken, (_UL)time_taken/CLOCKS_PER_SEC);
    return result;
} 


_S32 binsearch2(_S32 x, _S32 v[], _S32 n) 
{
    clock_t time_taken = clock();
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

    time_taken = clock() - time_taken;
    DEBUG_PRINT("binsearch2 took %lu clocks (%lu seconds)\n", (_UL)time_taken, (_UL)time_taken/CLOCKS_PER_SEC);
    
    if (high == n || v[high] != x)
    return -1;
    else
    return high;

} 

