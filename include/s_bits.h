#ifndef __S_BITS_H__
#define __S_BITS_H__
#include <limits.h>
#include <stdio.h>

#include "s_clinkage.h"

/* GET_BITS: get n bits from position p */ 
#define GET_BITS(x, p, n) (x & ((~(~0 << n)) << (p+1-n)))
#define SET_BITS(x, p, n, y) ((x & (~((~(~0 << n)) << (p+1-n)))) | ( (y & ~(~0 << n)) << (p+1-n)))
#define INVERT_BITS(x, p, n) ((x & (~((~(~0 << n)) << (p+1-n)))) | ((~x) & ((~(~0 << n)) << (p+1-n))))
#define RIGHT_ROT(x, n) ((sizeof(x)*8 == n)?(x):(((x >> n) & (~(~0 << (sizeof(x)*8 - n)))) | (x << (sizeof(x)*8 - n))))

#endif


