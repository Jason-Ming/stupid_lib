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
#define FILE_EXIST(p_filename_include)\
    if(BOOLEAN_TRUE == s_file_exist(p_filename_include))\
    {\
        *pp_real_file_name = p_filename_include;\
        return RETURN_SUCCESS;\
    }

PRIVATE STRU_C_TOKEN_NODE* s_cproc_include_file_get_header_token(_VOID)
{
    STRU_C_TOKEN_NODE *p_token_last_newline = s_cproc_token_get_last_newline();

    if(p_token_last_newline == NULL)
    {
        p_token_last_newline = s_cproc_token_get_list_head();
    }

    STRU_C_TOKEN_NODE *p_token_last_header = s_cproc_token_get_last_node_by_type_after_node(C_TOKEN_PP_HEADER_H_NAME, 
        p_token_last_newline);
    S_R_FALSE(p_token_last_header == NULL, p_token_last_header);

    return s_cproc_token_get_last_node_by_type_after_node(C_TOKEN_PP_HEADER_Q_NAME, p_token_last_newline);
}


/* if the file can't open, the output file name pointer is NULL */
PRIVATE ENUM_RETURN s_cproc_inlude_file_get_token_and_realfilename(
    _S8 ** pp_real_file_name, STRU_C_TOKEN_NODE ** pp_header_token)
{
    S_R_ASSERT(pp_real_file_name != NULL, RETURN_FAILURE);
    S_R_ASSERT(pp_header_token != NULL, RETURN_FAILURE);
    *pp_real_file_name = NULL;
    *pp_header_token = NULL;
    
    //get the include file TOKEN
    STRU_C_TOKEN_NODE *p_token_header = s_cproc_include_file_get_header_token();
    S_R_ASSERT(p_token_header != NULL, RETURN_FAILURE);
    *pp_header_token = p_token_header;

    STRU_C_TOKEN_NODE *p_token_temp = p_token_header;
    ENUM_BOOLEAN first_ignore_token = BOOLEAN_TRUE;
    while((p_token_temp = s_cproc_token_get_next_node(p_token_temp)) != NULL)
    {
        if(p_token_temp->info.token_type == C_TOKEN_PP_IGNORE && first_ignore_token)
        {
            first_ignore_token = BOOLEAN_FALSE;
            S_CPROC_STM_GEN_WARNING(&p_token_temp->info.c_position, "extra tokens at end of #include directive");
        }
        else
        {
            first_ignore_token = BOOLEAN_TRUE;
        }
    }

    DEBUG_PRINT("last include file token: "TOKEN_INFO_FORMAT, TOKEN_INFO_VALUE(p_token_header));
    
    _S8 *p_filename_include = NULL;

    // if absolute dir then open it
    if(p_token_header->info.p_string[0] == '/')
    {
        p_filename_include = s_duplicate_string(p_token_header->info.p_string);
        S_R_ASSERT(p_filename_include != NULL, RETURN_FAILURE);
        FILE_EXIST(p_filename_include);
    }

    //q header:find header in realdir
    if(p_token_header->info.token_type == C_TOKEN_PP_HEADER_Q_NAME)
    {
        //get current file realpath
        const _S8* filename_current = s_cproc_stm_get_filename();
        S_R_ASSERT(filename_current != NULL, RETURN_FAILURE);
        
        _S8 *current_realdir = s_get_realdir(filename_current);
        S_R_ASSERT(current_realdir != NULL, RETURN_FAILURE);

        DEBUG_PRINT("current dir: %s", current_realdir);
        
        //realdir+p_token_header->info.p_string
        p_filename_include = s_concatenate_string(current_realdir, p_token_header->info.p_string);
        S_R_ASSERT_DO(p_filename_include != NULL, RETURN_FAILURE, S_FREE(current_realdir));
#ifndef CPPUTEST
        S_FREE(current_realdir);
#endif
        FILE_EXIST(p_filename_include);
    }

    //iterate the -Idir
    //TO DO:

    //iterate the system include dir
    //-print-sysroot-headers-suffix
    _S8 *p_include_path[] = 
    {
        "/usr/local/include/",
        "/usr/include/"
    };

    for(_S32 i = 0; i < SIZE_OF_ARRAY(p_include_path); i++)
    {
        p_filename_include = s_concatenate_string(p_include_path[i], p_token_header->info.p_string);
        S_R_ASSERT(p_filename_include != NULL, RETURN_FAILURE);
        FILE_EXIST(p_filename_include);
    }

    S_FREE(p_filename_include);
    
    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_include_file(ENUM_RETURN *check_result)
{
    S_R_ASSERT(check_result != NULL, RETURN_FAILURE);
    *check_result = RETURN_FAILURE;

    _S8* p_include_real_file_name = NULL;
    STRU_C_TOKEN_NODE *p_header_token = NULL;
    
    ENUM_RETURN ret_val = s_cproc_inlude_file_get_token_and_realfilename(&p_include_real_file_name, &p_header_token);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    S_R_ASSERT(p_header_token != NULL, RETURN_FAILURE);
    
    if(p_include_real_file_name == NULL)
    {
        S_CPROC_STM_GEN_ERROR(&p_header_token->info.c_position, "%s: No such file or directory", 
            p_header_token->info.p_string);
        return RETURN_SUCCESS;
    }
    
    DEBUG_PRINT("delete the tokens of #include line, delete all tokens after last newline");
    s_cproc_token_release_list_after_last_newline();

    DEBUG_PRINT("recurse into another cpp to process file: %s", p_include_real_file_name);
    ret_val = s_cpp(p_include_real_file_name, check_result);
    S_ASSERT(ret_val == RETURN_SUCCESS);

    S_FREE(p_include_real_file_name);

    return RETURN_SUCCESS;
}

