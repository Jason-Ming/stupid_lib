#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "s_log.h"
#include "s_mem.h"
#include "s_text.h"
#include "s_stack.h"
#include "s_stm.h"
#include "s_cproc.h"
#include "s_cproc_ctoken_pp.h"

typedef struct  TAG_STRU_C_TOKEN_PP_KEYWORD_INFO
{
    ENUM_C_TOKEN token;
    _S8 * p_keyword;
}STRU_C_TOKEN_PP_KEYWORD_INFO;

STRU_C_TOKEN_PP_KEYWORD_INFO g_c_token_pp_keyword_info[] =
{
    {C_TOKEN_PP_CONTROL_DEFINE,                                "define"},
    {C_TOKEN_PPD_DEFINED,                                      "defined"},
    {C_TOKEN_PPD_ELIF,                                         "elif"},
    {C_TOKEN_PPD_ENDIF,                                        "endif"},
    {C_TOKEN_PP_CONTROL_ERROR,                                 "error"},
    {C_TOKEN_PPD_IFDEF,                                        "ifdef"},
    {C_TOKEN_PPD_IFNDEF,                                       "ifndef"},
    {C_TOKEN_PP_CONTROL_INCLUDE,                               "include"},
    {C_TOKEN_PP_CONTROL_LINE,                                  "line"},
    {C_TOKEN_PP_CONTROL_PRAGMA,                                "pragma"},
    {C_TOKEN_PP_CONTROL_UNDEF,                                 "undef"},
};

ENUM_RETURN s_cproc_get_ctoken_pp_keyword(const _S8 *p_string, size_t len, ENUM_C_TOKEN *token_type)
{
    R_ASSERT(p_string != NULL, RETURN_FAILURE);
    R_ASSERT(token_type != NULL, RETURN_FAILURE);

    size_t size = SIZE_OF_ARRAY(g_c_token_pp_keyword_info);
    
    for(size_t i = 0; i < size; i++)
    {
        if(memcmp(g_c_token_pp_keyword_info[i].p_keyword, p_string, len) == 0)
        {
            *token_type = g_c_token_pp_keyword_info[i].token;
            return RETURN_SUCCESS;
        }
    }

    *token_type = C_TOKEN_PP_IDENTIFIER;
    return RETURN_SUCCESS;
}

