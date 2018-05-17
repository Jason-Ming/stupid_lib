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
    BOOLEAN_TURE = 1,
    BOOLEAN_FALSE = 0,
}ENUM_BOOLEAN;

#define SIZE_OF_ARRAY(a) (sizeof(a)/sizeof(a[0]))
#define IS_ALPHABET(c) (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))?(BOOLEAN_TURE):(BOOLEAN_FALSE))

#define PRIVATE static
#define INVALID_INT INT_MAX
#define INVALID_UINT (~(UINT)0)

typedef int          INT;
typedef unsigned int UINT;
typedef char*        PSTR;


#endif

