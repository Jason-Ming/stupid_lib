#include "s_bits.h"

_U64 get_bits(_U64 x, _S32 p, _S32 n)
{
    return (x & ((~(~0 << n)) << (p+1-n)));
}

_U64 set_bits(_U64 x, _S32 p, _S32 n, _U64 y)
{
    return ((x & (~((~(~0 << n)) << (p+1-n)))) | ( (y & ~(~0 << n)) << (p+1-n)));
}

_U64 invert_bits(_U64 x, _S32 p, _S32 n)
{
    return ((x & (~((~(~0 << n)) << (p+1-n)))) | ((~x) & ((~(~0 << n)) << (p+1-n))));
}
_U64 right_rot_bits(_U64 x, _S32 n)
{
    return ((sizeof(x)*8 == n)?(x):(((x >> n) & (~(~0 << (sizeof(x)*8 - n)))) | (x << (sizeof(x)*8 - n))));
}

_U32 bits_count(_U64 x)
{
    _U32 b;
    for (b = 0; x != 0; x >>= 1)
    {
        if (x & 01)
        {
            b++;
        }
    }
    
    return b; 
}

