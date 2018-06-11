#ifndef __S_DEFINES_H__
#define __S_DEFINES_H__

#include "s_clinkage.h"
#include "s_type.h"

    
#define FREE(p) do{ if(p != NULL){free(p); p = NULL;}}while(0);
#define MIN(x, y) ((x) < (y)? (x):(y))
#define MAX(x, y) ((x) > (y)? (x):(y))

#define SIZE_OF_ARRAY(a) (sizeof(a)/sizeof(a[0]))

#define PRIVATE static

#endif

