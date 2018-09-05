#ifndef __S_CPROC_STM_H__
#define __S_CPROC_STM_H__
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"

#include "s_cpp.h"
#include "s_ctoken.h"

#define S_CPROC_STM_GEN_ERROR(p_c_position, fmt, args...)\
    do{\
        S_R_ASSERT((p_c_position) != NULL, RETURN_FAILURE);\
        S_C_EMIT_ERROR(s_cproc_stm_get_filename(), \
            s_cproc_stm_get_text_buffer(), \
            (p_c_position)->offset,\
            (p_c_position)->len,\
            (p_c_position)->line, \
            (p_c_position)->column, \
            fmt, ##args);\
        s_cproc_stm_generate_error();\
    }while(0)

#define S_CPROC_STM_GEN_WARNING(p_c_position, fmt, args...)\
    do{\
        S_R_ASSERT((p_c_position) != NULL, RETURN_FAILURE);\
        S_C_EMIT_WARNING(s_cproc_stm_get_filename(), \
            s_cproc_stm_get_text_buffer(), \
            (p_c_position)->offset,\
            (p_c_position)->len,\
            (p_c_position)->line, \
            (p_c_position)->column, \
            fmt, ##args);\
    }while(0)

#define S_CPROC_STM_CURRENT_C_POSITION (s_cproc_stm_get_current_c_position())
#define S_CPROC_STM_LAST_C_POSITION (s_cproc_stm_get_last_c_position())

#define S_CPROC_STM_CURRENT_TOKEN_C_POSITION (s_cproc_stm_get_current_token_c_position())

__BEGIN_C_DECLS

ENUM_RETURN s_cproc_stm(
    const _S8 *filename, 
    const _S8 *p_text_buffer, 
    ENUM_RETURN *check_result);
        
STRU_C_POSITION *s_cproc_stm_get_current_c_position(_VOID);
STRU_C_POSITION *s_cproc_stm_get_last_c_position(_VOID);

STRU_C_POSITION *s_cproc_stm_get_current_token_c_position(_VOID);

_VOID s_cproc_stm_generate_error(_VOID);
const _S8 *s_cproc_stm_get_text_buffer(_VOID);
const _S8 *s_cproc_stm_get_filename(_VOID);

__END_C_DECLS
#endif

