#ifndef __S_MEM_H__
#define __S_MEM_H__

#include <string.h>
#include <endian.h>
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"


__BEGIN_C_DECLS
void display_mem(void *addr, size_t size, ENUM_BOOLEAN whether_display_bits);
void display_var(void *addr, size_t size);
_VOID s_swap_ptr(_VOID * v[], _S32 i, _S32 j);
_VOID s_swap_s32(_S32 v[], _S32 i, _S32 j);


__END_C_DECLS

    
#define SIZE_OF_ARRAY(a) (sizeof(a)/sizeof(a[0]))

#define S_MALLOC 
#define S_FREE(p) do{ if((p) != NULL){free(p); (p) = NULL;}}while(0)

#define S_FCLOSE(p) do{ if((p) != NULL){fclose(p); (p) = NULL;}}while(0)

#define SWAP(x, y)\
    do\
    {\
        _U8 * x##temp_p = (_U8 *)&x;\
        _U8 * y##temp_p = (_U8 *)&y;\
        size_t size##x##y = sizeof(x);\
        _U8 x##y##temp;\
        while(size##x##y--)\
        {\
            x##y##temp = *x##temp_p;\
            *x##temp_p = *y##temp_p;\
            *y##temp_p = x##y##temp;\
            x##temp_p++;\
            y##temp_p++;\
        }\
    }while(0)
        

#define DISPLAY_VAR_MEM_ONE_LINE(var)\
    do\
    {\
        display_var((void *)&var, sizeof(var));\
    }while(0)

#define DISPLAY_VAR_MEM_MULTI_LINES(var)\
    do\
    {\
        printf("    "#var":\n");\
        display_mem((void *)&var, sizeof(var), BOOLEAN_TRUE);\
    }while(0)

#define  VALUE_S8_OF_ADDR( x )  ( *( (_S8 *) (x) ) )
#define  VALUE_U8_OF_ADDR( x )  ( *( (_U8 *) (x) ) )
#define  VALUE_SC_OF_ADDR( x )  ( *( (_SC *) (x) ) )
#define  VALUE_UC_OF_ADDR( x )  ( *( (_UC *) (x) ) )


#define  VALUE_S16_OF_ADDR( x )  ( *( (_S16 *) (x) ) )
#define  VALUE_U16_OF_ADDR( x )  ( *( (_U16 *) (x) ) )
#define  VALUE_SS_OF_ADDR( x )  ( *( (_SS *) (x) ) )
#define  VALUE_US_OF_ADDR( x )  ( *( (_US *) (x) ) )

#define  VALUE_S32_OF_ADDR( x )  ( *( (_S32 *) (x) ) )
#define  VALUE_U32_OF_ADDR( x )  ( *( (_U32 *) (x) ) )
#define  VALUE_SI_OF_ADDR( x )  ( *( (_SI *) (x) ) )
#define  VALUE_UI_OF_ADDR( x )  ( *( (_UI *) (x) ) )

#define  VALUE_SL_OF_ADDR( x )  ( *( (_SL *) (x) ) )
#define  VALUE_UL_OF_ADDR( x )  ( *( (_UL*) (x) ) )

#define  VALUE_S64_OF_ADDR( x )  ( *( (_S64 *) (x) ) )
#define  VALUE_U64_OF_ADDR( x )  ( *( (_U64  *) (x) ) )
#define  VALUE_SLL_OF_ADDR( x )  ( *( (_SLL *) (x) ) )
#define  VALUE_ULL_OF_ADDR( x )  ( *( (_ULL*) (x) ) )

#define  VALUE_SF_OF_ADDR( x )  ( *( (_SF *) (x) ) )
#define  VALUE_SD_OF_ADDR( x )  ( *( (_SD *) (x) ) )
#define  VALUE_SLD_OF_ADDR( x )  ( *( (_SLD*) (x) ) )
#if defined __USE_MISC && !defined __ASSEMBLER__
//#error this is a compile error!
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
//#error this is a compile error!
    /* set value to dest, if value size is larger than dest,k keep the lower bits of value */
#define SET_VAR_VALUE(dest, value)\
    do{\
        _U64 x = value;\
        size_t __l_dest = sizeof(dest);\
        memcpy((void*)&(dest), (void*)&(x), __l_dest);\
    }while(0);

#define SET_LONG_DOUBLE_VALUE(dest, value_u64, value_l64)\
    {\
        size_t __l_dest = sizeof(dest);\
        _U64 x = value_l64;\
        memcpy(((void*)&(dest)), (void*)&(x), 8);\
        x = value_u64;\
        memcpy(((void*)&(dest)) + 8, (void*)&(x), __l_dest - 8);\
    }
#else
    /* set value to dest, if value size is larger than dest,k keep the lower bits of value */
#define SET_VAR_VALUE(dest, value)\
    do{\
        _U64 x = value;\
        SWAP_BITS_64(x);\
        size_t __l_dest = sizeof(dest);\
        memcpy((void*)&(dest), (void*)&(x), __l_dest);\
        if(__l_dest == 2)\
        {\
            SWAP_BITS_16(dest);\
        }else if(__l_dest == 4)\
        {\
            SWAP_BITS_32(dest);\
        }else if(__l_dest == 8)\
        {\
            SWAP_BITS_64(dest);\
        }\
    }while(0);

#define SET_LONG_DOUBLE_VALUE(dest, value_u64, value_l64)\
    {\
        size_t __l_dest = sizeof(dest);\

        _U64 x = value_u64;\
        SWAP_BITS_64(x);\
        memcpy(((void*)&(dest)), (void*)&(x) + __l_dest - 8, __l_dest - 8);\
        x = value_l64;\
        SWAP_BITS_64(x);\
        memcpy(((void*)&(dest)) + __l_dest - 8, (void*)&(x), 8);\
    }
#endif

#endif

