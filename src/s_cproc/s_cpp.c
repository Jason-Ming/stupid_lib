
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "s_defines.h"
#include "s_text.h"
#include "s_cmd.h"
#include "s_log.h"
#include "s_stack.h"
#include "s_stm.h"
#include "s_mem.h"
#include "s_alg.h"

#include "s_ctoken.h"
#include "s_cpp.h"
#include "s_cproc_stm.h"
#include "s_cproc_macro.h"

#define CPP_PRINT(fpw, fmt, args...)\
    do\
    {\
        if(fpw != NULL)\
        {\
            fprintf(fpw, fmt, ##args);\
        }\
        else\
        {\
            printf(fmt, ##args);\
        }\
    }while(0);

_VOID s_cpp_print(
    FILE *pfw,
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_tail)
{
    S_V_ASSERT(p_token_list_head != NULL);
    S_V_ASSERT(p_token_list_tail != NULL);

	while(p_token_list_head != NULL)
	{

	    CPP_PRINT(pfw, "%s", p_token_list_head->info.p_string);
		
		if(p_token_list_head == p_token_list_tail)
		{
			break;
		}
		
		p_token_list_head = p_token_list_head->next;
	}
}

ENUM_RETURN s_cpp(
    FILE *pfr,
    _S8 **pp_text_buffer, 
    size_t *p_text_buffer_size, 
    STRU_C_TOKEN_NODE **pp_token_list_head,
    STRU_C_TOKEN_NODE **pp_token_list_tail)
{
    S_R_ASSERT(pfr != NULL, RETURN_FAILURE);
    S_R_ASSERT(pp_text_buffer != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_text_buffer_size != NULL, RETURN_FAILURE);
    S_R_ASSERT(pp_token_list_head != NULL, RETURN_FAILURE);
    S_R_ASSERT(pp_token_list_tail != NULL, RETURN_FAILURE);
    
    ENUM_RETURN ret_val = RETURN_SUCCESS;
   
    ret_val = s_save_file_to_text_buffer(pfr, pp_text_buffer, p_text_buffer_size);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ENUM_RETURN check_result;
    ret_val = s_cproc_stm(
		get_input_file_of_current_running_subcmd(), 
    	*pp_text_buffer, 
    	*p_text_buffer_size, 
    	pp_token_list_head, 
    	pp_token_list_tail, 
    	&check_result);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

	debug_print_token_list(*pp_token_list_head, *pp_token_list_tail);

    s_cproc_macro_print_list_debug();

    return RETURN_SUCCESS;
}


