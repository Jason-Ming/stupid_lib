#ifndef __DEFINES_H__
#define __DEFINES_H__
#include <limits.h>

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
#define PRIVATE static
#define INVALID_INT INT_MAX

#endif

