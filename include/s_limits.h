#ifndef __S_LIMITS_H__
#define __S_LIMITS_H__


#include "s_type.h"

#define S8_MAX SCHAR_MAX
#define S8_MIN SCHAR_MIN
#define S8_INVALID SCHAR_MAX

#define UC_MAX UCHAR_MAX
#define UC_INVLAID UCHAR_MAX

#define SC_MAX SCHAR_MAX
#define SC_MIN SCHAR_MIN
#define SC_INVALID SCHAR_MAX

#define UC_MAX UCHAR_MAX
#define UC_INVLAID UCHAR_MAX


#define S16_MAX SHRT_MAX
#define S16_MIN SHRT_MIN
#define S16_INVALID SHRT_MAX

#define U16_MAX USHRT_MAX
#define U16_INVLAID USHRT_MAX

#define SS_MAX SHRT_MAX
#define SS_MIN SHRT_MIN
#define SS_INVALID SHRT_MAX

#define US_MAX USHRT_MAX
#define US_INVLAID USHRT_MAX

#define S32_MAX INT_MAX
#define S32_MIN INT_MIN
#define S32_INVALID INT_MAX

#define U32_MAX UINT_MAX
#define U32_INVLAID UINT_MAX

#define SI_MAX INT_MAX
#define SI_MIN INT_MIN
#define SI_INVALID INT_MAX

#define UI_MAX UINT_MAX
#define UI_INVLAID UINT_MAX

#define SL_MAX LONG_MAX
#define SL_MIN LONG_MIN
#define SL_INVALID LONG_MAX

#define UL_MAX ULONG_MAX
#define UL_INVLAID ULONG_MAX

#define SLL_MAX LLONG_MAX
#define SLL_MIN LLONG_MIN
#define SLL_INVALID LLONG_MAX

#define ULL_MAX ULLONG_MAX
#define ULL_INVLAID ULLONG_MAX

#define S64_MAX LLONG_MAX
#define S64_MIN LLONG_MIN
#define S64_INVALID LLONG_MAX

#define U64_MAX ULLONG_MAX
#define U64_INVLAID ULLONG_MAX

/* 
Sign-bit：0表示正，1表示负。占1bit；
Biased-exponent：首先exponent表示该域用于表示指数，也就是数值可表示数值范围，
而biased则表示它采用偏移的编码方式。那么什么是采用偏移的编码方式呢？
也就是位模式中没有设立sign-bit，而是通过设置一个中间值作为0，小于该中间值则为负数，
大于改中间值则为正数。IEEE 754中规定bias = 2^e-1 - 1，e为Biased-exponent所占位数；
Significant：表示有效数，也就是数值可表示的精度。（注意：Significant采用原码编码；
假设有效数位模式为0101，那么其值为0*2-1+1*2-2+0*2-3+1*2-4，即有效数域的指数为负数）
*/

/*                                         1 8------- 23---------------------  */
#define FLOAT_ZERO_MINUS     0x80000000 /* 1 00000000 00000000000000000000000  */
#define FLOAT_ZERO_PLUS      0x00000000 /* 1 00000000 00000000000000000000000  */
#define FLOAT_MIN_MINUS      0xff7fffff /* 1 11111110 11111111111111111111111  */
#define FLOAT_MAX_MINUS      0x80800000 /* 1 00000001 00000000000000000000001  */
#define FLOAT_MIN_PLUS       0x00800001 /* 0 00000001 00000000000000000000001  */
#define FLOAT_MAX_PLUS       0x7f7fffff /* 0 11111110 11111111111111111111111  */
#define FLOAT_INFINITY_MINUS 0xFF800000 /* 1 11111111 00000000000000000000000  */
#define FLOAT_INFINITY_PLUS  0x7F800000 /* 0 11111111 00000000000000000000000  */
#define FLOAT_NAN_MINUS      0xFFFFFFFF /* 1 11111111 11111111111111111111111  */
#define FLOAT_NAN_PLUS       0x7FFFFFFF /* 0 11111111 11111111111111111111111  */

/*                                                   1 11--------- 52--------------------------------------------------  */
#define DOUBLE_ZERO_MINUS      0x8000000000000000 /* 1 00000000000 0000000000000000000000000000000000000000000000000000  */
#define DOUBLE_ZERO_PLUS       0x0000000000000000 /* 0 00000000000 0000000000000000000000000000000000000000000000000000  */
#define DOUBLE_MIN_MINUS       0xffefffffffffffff /* 1 11111111110 1111111111111111111111111111111111111111111111111111  */
#define DOUBLE_MAX_MINUS       0x8010000000000000 /* 1 00000000001 0000000000000000000000000000000000000000000000000000  */
#define DOUBLE_MIN_PLUS        0x0010000000000000 /* 0 00000000001 0000000000000000000000000000000000000000000000000000  */
#define DOUBLE_MAX_PLUS        0x7fefffffffffffff /* 0 11111111110 1111111111111111111111111111111111111111111111111111  */
#define DOUBLE_INFINITY_MINUS  0xfff0000000000000 /* 1 11111111111 0000000000000000000000000000000000000000000000000000  */
#define DOUBLE_INFINITY_PLUS   0x7ff0000000000000 /* 0 11111111111 0000000000000000000000000000000000000000000000000000  */
#define DOUBLE_NAN_MINUS       0xffffffffffffffff /* 1 11111111111 1111111111111111111111111111111111111111111111111111  */
#define DOUBLE_NAN_PLUS        0x7fffffffffffffff /* 0 11111111111 1111111111111111111111111111111111111111111111111111  */


/* 在linux 32/64位 前8/24bit固定为0,1bit符号位，15位biased-exponent，
72位significant，第一个bit为整数位，0时为0，其他值为1
                                                                       8        1 15------------- 72----------------------------------------------------------------------  */
#define LONG_DOUBLE_ZERO_MINUS_U64     0x0000000000008000  /* 00000000 1 000000000000000 000000000000000000000000000000000000000000000000000000000000000000000000  */
#define LONG_DOUBLE_ZERO_MINUS_L64     0x0000000000000000  /* 00000000 1 000000000000000 000000000000000000000000000000000000000000000000000000000000000000000000  */
#define LONG_DOUBLE_ZERO_PLUS_U64      0x0000000000000000  /* 00000000 0 000000000000000 000000000000000000000000000000000000000000000000000000000000000000000000  */
#define LONG_DOUBLE_ZERO_PLUS_L64      0x0000000000000000  /* 00000000 0 000000000000000 000000000000000000000000000000000000000000000000000000000000000000000000  */
#define LONG_DOUBLE_MIN_MINUS_U64      0x000000000000fffe  /* 00000000 1 111111111111110 111111111111111111111111111111111111111111111111111111111111111111111111  */
#define LONG_DOUBLE_MIN_MINUS_L64      0xffffffffffffffff  /* 00000000 1 111111111111110 111111111111111111111111111111111111111111111111111111111111111111111111  */
#define LONG_DOUBLE_MAX_MINUS_U64      0x0000000000008001  /* 00000000 1 000000000000001 100000000000000000000000000000000000000000000000000000000000000000000000  */
#define LONG_DOUBLE_MAX_MINUS_L64      0x8000000000000000  /* 00000000 1 000000000000001 100000000000000000000000000000000000000000000000000000000000000000000000  */
#define LONG_DOUBLE_MIN_PLUS_U64       0x0000000000000001  /* 00000000 0 000000000000001 100000000000000000000000000000000000000000000000000000000000000000000000  */
#define LONG_DOUBLE_MIN_PLUS_L64       0x8000000000000000  /* 00000000 0 000000000000001 100000000000000000000000000000000000000000000000000000000000000000000000  */
#define LONG_DOUBLE_MAX_PLUS_U64       0x0000000000007ffe  /* 00000000 0 111111111111110 111111111111111111111111111111111111111111111111111111111111111111111111  */
#define LONG_DOUBLE_MAX_PLUS_L64       0xffffffffffffffff  /* 00000000 0 111111111111110 111111111111111111111111111111111111111111111111111111111111111111111111  */
#define LONG_DOUBLE_INFINITY_MINUS_U64 0x000000000000ffff  /* 00000000 1 111111111111111 100000000000000000000000000000000000000000000000000000000000000000000000  */
#define LONG_DOUBLE_INFINITY_MINUS_L64 0x8000000000000000  /* 00000000 1 111111111111111 100000000000000000000000000000000000000000000000000000000000000000000000  */
#define LONG_DOUBLE_INFINITY_PLUS_U64  0x0000000000007fff  /* 00000000 0 111111111111111 100000000000000000000000000000000000000000000000000000000000000000000000  */
#define LONG_DOUBLE_INFINITY_PLUS_L64  0x8000000000000000  /* 00000000 0 111111111111111 100000000000000000000000000000000000000000000000000000000000000000000000  */
#define LONG_DOUBLE_NAN_MINUS_U64      0x000000000000ffff  /* 00000000 1 111111111111111 111111111111111111111111111111111111111111111111111111111111111111111111  */
#define LONG_DOUBLE_NAN_MINUS_L64      0xffffffffffffffff  /* 00000000 1 111111111111111 111111111111111111111111111111111111111111111111111111111111111111111111  */
#define LONG_DOUBLE_NAN_PLUS_U64       0x0000000000007fff  /* 00000000 0 111111111111111 1111111111111111111111111111111111111111111111111111111111111111111111111  */
#define LONG_DOUBLE_NAN_PLUS_L64       0xffffffffffffffff  /* 00000000 0 111111111111111 111111111111111111111111111111111111111111111111111111111111111111111111  */

#endif
