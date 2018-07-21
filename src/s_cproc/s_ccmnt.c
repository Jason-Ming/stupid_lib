#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "s_defines.h"
#include "s_text.h"
#include "s_cmd.h"
#include "s_log.h"
#include "s_cproc.h"
#include "s_stm.h"
#include "s_mem.h"

typedef enum TAG_ENUM_DEL_CMNT_STM
{
    DEL_CMNT_STM_NORMAL = 0,
    DEL_CMNT_STM_STRING_DOUBLE_QUOTE,
    DEL_CMNT_STM_STRING_SINGLE_QUOTE,
    DEL_CMNT_STM_ONELINE_COMMENT,
    DEL_CMNT_STM_PAIR_COMMENT,
    DEL_CMNT_STM_INTERMEDIATE,
    DEL_CMNT_STM_END,
    DEL_CMNT_STM_MAX,
}ENUM_DEL_CMNT_STM;

typedef struct TAG_STRU_DEL_CMNT_STM_PROC
{
    STM_STATE state;
    STM_PROC handler;
    const char *info;
}STRU_DEL_CMNT_STM_PROC;

typedef struct TAG_STRU_DEL_CMNT_STM_RUN_DATA
{
    FILE *pfr;
    FILE *pfw;
    _S32 c;
    ENUM_BOOLEAN whether_any_error_exists;
}STRU_DEL_CMNT_STM_RUN_DATA;


PRIVATE STM stm = NULL;
PRIVATE STRU_DEL_CMNT_STM_RUN_DATA run_data;

PRIVATE ENUM_RETURN del_cmnt_stm_prepare_proc()
{
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN del_cmnt_stm_clear_proc()
{
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN del_cmnt_stm_preproc()
{
    ENUM_RETURN ret_val;
    
    run_data.c = fgetc(run_data.pfr);

    if(run_data.c == EOF)
    {
        ret_val = set_current_stm_state(stm, DEL_CMNT_STM_END);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN del_cmnt_stm_postproc(_VOID)
{
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN del_cmnt_stm_state_notifier(_VOID)
{
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN del_cmnt_stm_proc_normal(_VOID)
{
    ENUM_RETURN ret_val;
    _S8 c = run_data.c;
    switch (c)
    {
        case '"'://this is a test comment
        {/* this is a test comment */
            fputc(c, run_data.pfw);

            ret_val = set_current_stm_state(stm, DEL_CMNT_STM_STRING_DOUBLE_QUOTE);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }
        case '/':
        {
            ret_val = set_current_stm_state(stm, DEL_CMNT_STM_INTERMEDIATE);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }
        case '\'':
        {
            fputc(c, run_data.pfw);
            
            ret_val = set_current_stm_state(stm, DEL_CMNT_STM_STRING_SINGLE_QUOTE);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }
        case '\n':
        default:
        {
            fputc(c, run_data.pfw);
            break;
        }
    }
    
    return RETURN_SUCCESS;
}
//this is a test comment
PRIVATE ENUM_RETURN del_cmnt_stm_proc_string_double_quote(_VOID)
{
    static ENUM_BOOLEAN whether_backslash_occured = BOOLEAN_FALSE;
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    _S8 c = run_data.c;
    
    switch (c)
    {
        case '"'://a line comment
        {
            fputc(c, run_data.pfw);
            if(whether_backslash_occured == BOOLEAN_FALSE)
            {
                ret_val = set_current_stm_state(stm, DEL_CMNT_STM_NORMAL);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            }

            whether_backslash_occured = BOOLEAN_FALSE;
            break;
        }
        case '\\':
        {
            whether_backslash_occured = BOOLEAN_TRUE;
            fputc(c, run_data.pfw);
            break;
        }
        case '\n':
        default:
        {
            whether_backslash_occured = BOOLEAN_FALSE;
            fputc(c, run_data.pfw);
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN del_cmnt_stm_proc_string_single_quote(_VOID)
{
    static ENUM_BOOLEAN whether_backslash_occured = BOOLEAN_FALSE;
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    _S8 c = run_data.c;
    switch (c)
    {
        case '\''://a line comment
        {
            fputc(c, run_data.pfw);
            if(whether_backslash_occured == BOOLEAN_FALSE)
            {
                ret_val = set_current_stm_state(stm, DEL_CMNT_STM_NORMAL);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            }

            whether_backslash_occured = BOOLEAN_FALSE;
            break;
        }
        case '\\':
        {
            whether_backslash_occured = BOOLEAN_TRUE;
            fputc(c, run_data.pfw);
            break;
        }
        case '\n':
        default:
        {
            whether_backslash_occured = BOOLEAN_FALSE;
            fputc(c, run_data.pfw);
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN del_cmnt_stm_proc_oneline_comment(_VOID)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    _S8 c = run_data.c;
    switch (c)
    {
        case '\n':
        {
            fputc(c, run_data.pfw);
            ret_val = set_current_stm_state(stm, DEL_CMNT_STM_NORMAL);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }
        default:
        {
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN del_cmnt_stm_proc_pair_comment(_VOID)
{
    static ENUM_BOOLEAN whether_star_occured = BOOLEAN_FALSE;
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    _S8 c = run_data.c;
    switch (c)
    {
        case '*':
        {
            whether_star_occured = BOOLEAN_TRUE;
            break;
        }
        case '/':
        {
            if(whether_star_occured == BOOLEAN_TRUE)
            {
                ret_val = set_current_stm_state(stm, DEL_CMNT_STM_NORMAL);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            }
            whether_star_occured = BOOLEAN_FALSE;
            break;
        }
        case '\n':
        default:
        {
            whether_star_occured = BOOLEAN_FALSE;
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN del_cmnt_stm_proc_intermediate(_VOID)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    _S8 c = run_data.c;
    switch (c)
    {
        case '*':
        {
            ret_val = set_current_stm_state(stm, DEL_CMNT_STM_PAIR_COMMENT);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }
        case '/':
        {
            ret_val = set_current_stm_state(stm, DEL_CMNT_STM_ONELINE_COMMENT);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }
        case '\n':
        default:
        {
            fputc('/', run_data.pfw);
            fputc(c, run_data.pfw);
            ret_val = set_current_stm_state(stm, DEL_CMNT_STM_NORMAL);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN del_cmnt_stm_proc_end(_VOID)
{
    return RETURN_SUCCESS;
}

PRIVATE STRU_DEL_CMNT_STM_PROC del_cmnt_stm_proc[DEL_CMNT_STM_MAX] = 
{
    {DEL_CMNT_STM_NORMAL, del_cmnt_stm_proc_normal, "normal"},
    {DEL_CMNT_STM_STRING_DOUBLE_QUOTE, del_cmnt_stm_proc_string_double_quote, "string double quote"},
    {DEL_CMNT_STM_STRING_SINGLE_QUOTE, del_cmnt_stm_proc_string_single_quote, "string single quote"},
    {DEL_CMNT_STM_ONELINE_COMMENT, del_cmnt_stm_proc_oneline_comment, "oneline comment"},
    {DEL_CMNT_STM_PAIR_COMMENT, del_cmnt_stm_proc_pair_comment, "pair comment"},
    {DEL_CMNT_STM_INTERMEDIATE, del_cmnt_stm_proc_intermediate, "intermediate"},
    {DEL_CMNT_STM_END, del_cmnt_stm_proc_end, "end"},
};

PRIVATE ENUM_RETURN del_cmnt_stm_init(FILE *pfr, FILE *pfw)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    ret_val = stm_create(&stm, DEL_CMNT_STM_MAX);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = set_stm_start_state(stm, DEL_CMNT_STM_NORMAL);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = set_stm_end_state(stm, DEL_CMNT_STM_END);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_prepare_handler(stm, del_cmnt_stm_prepare_proc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_clear_handler(stm, del_cmnt_stm_clear_proc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_preproc_handler(stm, del_cmnt_stm_preproc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_postproc_handler(stm, del_cmnt_stm_postproc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_state_notifier(stm, del_cmnt_stm_state_notifier);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    for(int i = 0; i < SIZE_OF_ARRAY(del_cmnt_stm_proc); i++)
    {
        ret_val = add_stm_state_handler(
            stm, 
            del_cmnt_stm_proc[i].state, 
            del_cmnt_stm_proc[i].handler, 
            del_cmnt_stm_proc[i].info);
        
        R_ASSERT_LOG(
            ret_val == RETURN_SUCCESS, 
            RETURN_FAILURE, 
            "i = %d, state = %d",
            i, 
            del_cmnt_stm_proc[i].state);
    }

    run_data.pfr = pfr;
    run_data.pfw = pfw;
    
    return RETURN_SUCCESS;
}

ENUM_RETURN s_cdel_cmnt(FILE *pfr, FILE *pfw)
{
    R_ASSERT(pfr != NULL, RETURN_FAILURE);
    R_ASSERT(pfw != NULL, RETURN_FAILURE);

    ENUM_RETURN ret_val = RETURN_SUCCESS;

    ret_val = del_cmnt_stm_init(pfr, pfw);
    R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, FREE(stm));
    
    ret_val = stm_run(stm);
    R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, FREE(stm));
    
    return RETURN_SUCCESS;
}

