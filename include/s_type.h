#ifndef __S_TYPE_H__
#define __S_TYPE_H__
#include <limits.h>
#include <stdio.h>

#include "s_clinkage.h"
typedef void                    _VOID;
typedef char                    _S8;
typedef unsigned char           _U8;

typedef char                    _SC;
typedef unsigned char           _UC;

typedef short                   _S16;
typedef unsigned short          _U16;

typedef short                   _SS;
typedef unsigned short          _US;

typedef int                     _S32;
typedef unsigned int            _U32;

typedef int                     _SI;
typedef unsigned int            _UI;

/* long and unsigned long has different bits in 32 and b4 bit systems */

typedef long                    _SL;
typedef unsigned long           _UL;

typedef long long               _SLL;
typedef unsigned long long      _ULL;

typedef long long               _S64;
typedef unsigned long long      _U64;

typedef float                   _SF;
typedef double                  _SD;
typedef long double             _SLD;

typedef char*        PSTR;

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

__BEGIN_C_DECLS

void s_print_types(void);
__END_C_DECLS

#endif

#
