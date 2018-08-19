
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "s_defines.h"
#include "s_text.h"
#include "s_cmd.h"
#include "s_log.h"
#include "s_stm.h"
#include "s_mem.h"
#include "s_alg.h"

#include "s_ctoken.h"
#include "s_cpp.h"
#include "s_cproc_stm.h"
#include "s_cproc_text.h"

#include "s_cproc_macro.h"

ENUM_RETURN s_cpp(const _S8 * file_name, STACK stack)
{
    S_R_ASSERT(file_name != NULL, RETURN_FAILURE);
    S_R_ASSERT(stack != NULL, RETURN_FAILURE);

    ENUM_RETURN ret_val = RETURN_SUCCESS;

    _S8 *p_text_buffer = NULL;
    ret_val = s_cproc_text_read_file_to_buffer(file_name, &p_text_buffer);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    S_R_ASSERT(p_text_buffer != NULL, RETURN_FAILURE);
    
    ENUM_RETURN check_result;
    ret_val = s_cproc_stm(
        stack,
		file_name,
    	p_text_buffer,
    	&check_result);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}


