#ifndef __S_MEM_H__
#define __S_MEM_H__

#include <string.h>

#include "s_clinkage.h"
#include "s_defines.h"


__BEGIN_C_DECLS
void display_mem(void *addr, size_t size);
void display_var(void *addr, size_t size);

__END_C_DECLS

#define DISPLAY_VAR_MEM_ONE_LINE(var)\
    do\
    {\
        display_var((void *)&var, sizeof(var));\
    }while(0)

#define DISPLAY_VAR_MEM_MULTI_LINES(var)\
    do\
    {\
        printf("    "#var":\n");\
        display_mem((void *)&var, sizeof(var));\
    }while(0)

#define  VALUE_CHAR_OF_ADDR( x )  ( *( (char *) (x) ) )
#define  VALUE_SHORT_OF_ADDR( x )  ( *( (short *) (x) ) )
#define  VALUE_INT_OF_ADDR( x )  ( *( (int *) (x) ) )
#define  VALUE_LONG_INT_OF_ADDR( x )  ( *( (long *) (x) ) )
#define  VALUE_LONG_LONG_INT_OF_ADDR( x )  ( *( (long long *) (x) ) )
#define  VALUE_UINT_OF_ADDR( x )  ( *( (unsigned int *) (x) ) )
#define  VALUE_LONG_UINT_OF_ADDR( x )  ( *( (long unsigned *) (x) ) )
#define  VALUE_LONG_LONG_UINT_OF_ADDR( x )  ( *( (long long unsigned *) (x) ) )

#define  VALUE_FLOAT_OF_ADDR( x )  ( *( (float *) (x) ) )
#define  VALUE_DOUBLE_OF_ADDR( x )  ( *( (double *) (x) ) )
#define  VALUE_LONG_DOUBLE_OF_ADDR( x )  ( *( (long double*) (x) ) )

#define SET_VAR_MEM(dest, value)  {long long int x = value; memcpy((void*)&(dest), (void*)&(x), sizeof(dest));}


#endif

