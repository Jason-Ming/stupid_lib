#include <stdio.h>
#include <stdlib.h>

#include "s_log.h"
#include "s_limits.h"
#include "s_text.h"
#include "s_mem.h"
#include "s_cerror.h"
#include "s_ctoken.h"
#include "s_cproc_token.h"

#include "s_cproc_stm.h"
#include "s_cproc_include_file.h"
ENUM_RETURN s_cproc_include_file(ENUM_RETURN *check_result)
{
    S_R_ASSERT(check_result != NULL, RETURN_FAILURE);
    *check_result = RETURN_FAILURE;
    
    //get the file name
    _S8* filename = NULL;
    STRU_C_TOKEN_NODE *p_token_include_filename = s_cproc_token_get_last_node_by_type(C_TOKEN_PP_HEADER_NAME);
    S_R_ASSERT(p_token_include_filename != NULL, RETURN_FAILURE);

                    
    if(SL_INVALID == s_get_inode_by_filename(p_token_include_filename->info.p_string))
    {
        CPROC_GEN_ERROR(p_token_include_filename, "%s: No such file or directory", 
            p_token_include_filename->info.p_string);
        return RETURN_SUCCESS;
    }
    
    filename = s_duplicate_string(p_token_include_filename->info.p_string);
    S_R_ASSERT(filename != NULL, RETURN_FAILURE);

    DEBUG_PRINT("delete the tokens of include line, delete all tokens after last newline");
    s_cproc_token_release_list_after_last_newline();

    DEBUG_PRINT("recurse into another cpp to process file: %s", filename);
    ENUM_RETURN ret_val = s_cpp(filename, check_result);
    S_ASSERT(ret_val == RETURN_SUCCESS);

    FREE(filename);

    return RETURN_SUCCESS;
}

