#ifndef __S_DEFINES_H__
#define __S_DEFINES_H__
#include <limits.h>
#include <stdio.h>

#include "s_clinkage.h"

typedef enum TAG_ENUM_RETURN
{
    RETURN_SUCCESS = 0,
    RETURN_FAILURE = -1,
}ENUM_RETURN;

typedef enum TAG_ENUM_BOOLEAN
{
    BOOLEAN_TRUE = 1,
    BOOLEAN_FALSE = 0,
}ENUM_BOOLEAN;
    
#define FREE(p) do{ if(p != NULL){free(p);}}while(0);
#define MIN(x, y) ((x) < (y)? (x):(y))
#define SIZE_OF_ARRAY(a) (sizeof(a)/sizeof(a[0]))

/* GET_BITS: get n bits from position p */ 
#define GET_BITS(x, p, n) (x & ((~(~0 << n)) << (p+1-n)))
//1000 1011 p = 2 n = 2
//      **
//0101 1110
//1000 1101
#define SET_BITS(x, p, n, y) ((x & (~((~(~0 << n)) << (p+1-n)))) | ( (y & ~(~0 << n)) << (p+1-n)))

#define INVERT_BITS(x, p, n) ((x & (~((~(~0 << n)) << (p+1-n)))) | ((~x) & ((~(~0 << n)) << (p+1-n))))

#define PRIVATE static

#endif

