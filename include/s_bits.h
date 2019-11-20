#ifndef __S_BITS_H__
#define __S_BITS_H__
//#include <byteswap.h>
#include "s_clinkage.h"
#include "s_type.h"

/* GET_BITS: get n bits from position p */ 
#define GET_BITS(x, p, n) (p+1 >= n)?(x & ((~(~0 << n)) << (p+1-n))):(x & (~(~0 << (p+1)) ))
#define SET_BITS(x, p, n, y) ((x & (~((~(~0 << n)) << (p+1-n)))) | ( (y & ~(~0 << n)) << (p+1-n)))
#define INVERT_BITS(x, p, n) ((x & (~((~(~0 << n)) << (p+1-n)))) | ((~x) & ((~(~0 << n)) << (p+1-n))))
#define RIGHT_ROT(x, n) ((sizeof(x)*8 == n)?(x):(((x >> n) & (~(~0 << (sizeof(x)*8 - n)))) | (x << (sizeof(x)*8 - n))))
#define SWAP_BITS_16(x) (x = bswap_16(x))
#define SWAP_BITS_32(x) (x = bswap_32(x))
#define SWAP_BITS_64(x) (x = bswap_64(x))


__BEGIN_C_DECLS

_U64 get_bits(_U64 x, _S32 p, _S32 n);

/* returns x with the n bits that begin at
position p set to the rightmost n bits of y, leaving the other bits unchanged */
_U64 set_bits(_U64 x, _S32 p, _S32 n, _U64 y);

/* returns x with the n bits that begin at position p
inverted (i.e., 1 changed into 0 and vice versa), leaving the others unchanged */
_U64 invert_bits(_U64 x, _S32 p, _S32 n);

/* returns the value of the integer x rotated to the right by n bit positions. */
_U64 right_rot_bits(_U64 x, _S32 n);

/* counts the number of 1-bits in its integer argument */
_U32 bits_count(_U64 x);
__END_C_DECLS

#endif


