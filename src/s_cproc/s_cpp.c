
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

ENUM_RETURN s_cpp(const _S8 * file_name, ENUM_RETURN *check_result)
{
    S_R_ASSERT(file_name != NULL, RETURN_FAILURE);
    S_R_ASSERT(check_result != NULL, RETURN_FAILURE);
    *check_result = RETURN_FAILURE;
    
    DEBUG_PRINT(LIGHT_YELLOW">>>>>=====cpp file name: %s====================\n"NONE, file_name);
    printf(LIGHT_YELLOW">>>>>=====cpp file name: %s====================\n"NONE, file_name);
    
    ENUM_RETURN ret_val = RETURN_SUCCESS;

    const _S8 *p_text_buffer = NULL;
    ret_val = s_cproc_text_read_file_to_buffer(file_name, &p_text_buffer);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    S_R_ASSERT(p_text_buffer != NULL, RETURN_FAILURE);

    ret_val = s_cproc_stm(
		file_name,
    	p_text_buffer,
    	check_result);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    DEBUG_PRINT(LIGHT_YELLOW"<<<<<=====cpp file name: %s====================check_result: %s\n"NONE, file_name, 
        *check_result == RETURN_SUCCESS?"success":"fail");
    printf(LIGHT_YELLOW"<<<<<=====cpp file name: %s====================check_result: %s\n"NONE, file_name, *check_result 
        == RETURN_SUCCESS?"success":"fail");
    
    return RETURN_SUCCESS;
}


