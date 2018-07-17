#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "s_defines.h"
#include "s_text.h"
#include "s_cmd.h"
#include "s_log.h"
#include "s_cproc.h"

typedef enum TAG_ENUM_DELCOM_STM
{
    DELCOM_STM_NORMAL = 0,
    DELCOM_STM_STRING_DOUBLE_QUOTE,
    DELCOM_STM_STRING_SINGLE_QUOTE,
    DELCOM_STM_ONELINE_COMMENT,
    DELCOM_STM_PAIR_COMMENT,
    DELCOM_STM_INTERMEDIATE,
    DELCOM_STM_MAX,
}ENUM_DELCOM_STM;

PRIVATE ENUM_DELCOM_STM delcom_stm_state = DELCOM_STM_NORMAL;

typedef ENUM_RETURN (*DELCOM_STM_PROC)(FILE *pfr, FILE *pfw, int c);
typedef struct TAG_STRU_DELCOM_STM_PROC
{
    ENUM_DELCOM_STM state;
    DELCOM_STM_PROC handler;
    const char *info;
}STRU_DELCOM_STM_PROC;

PRIVATE ENUM_RETURN del_stm_proc_normal(FILE *pfr, FILE *pfw, int c);
PRIVATE ENUM_RETURN del_stm_proc_string_double_quote(FILE *pfr, FILE *pfw, int c);
PRIVATE ENUM_RETURN del_stm_proc_string_single_quote(FILE *pfr, FILE *pfw, int c);
PRIVATE ENUM_RETURN del_stm_proc_oneline_comment(FILE *pfr, FILE *pfw, int c);
PRIVATE ENUM_RETURN del_stm_proc_pair_comment(FILE *pfr, FILE *pfw, int c);
PRIVATE ENUM_RETURN del_stm_proc_intermediate(FILE *pfr, FILE *pfw, int c);


PRIVATE STRU_DELCOM_STM_PROC delcom_stm_proc[DELCOM_STM_MAX] = 
{
    {DELCOM_STM_NORMAL, del_stm_proc_normal, "normal"},
    {DELCOM_STM_STRING_DOUBLE_QUOTE, del_stm_proc_string_double_quote, "string double quote"},
    {DELCOM_STM_STRING_SINGLE_QUOTE, del_stm_proc_string_single_quote, "string single quote"},
    {DELCOM_STM_ONELINE_COMMENT, del_stm_proc_oneline_comment, "oneline comment"},
    {DELCOM_STM_PAIR_COMMENT, del_stm_proc_pair_comment, "pair comment"},
    {DELCOM_STM_INTERMEDIATE, del_stm_proc_intermediate, "intermediate"},
};

PRIVATE ENUM_BOOLEAN whether_delcom_stm_state_is_valid(int arg)
{
    R_FALSE_RET(arg >= DELCOM_STM_NORMAL && arg < DELCOM_STM_MAX, BOOLEAN_FALSE);

    return BOOLEAN_TRUE;
}

PRIVATE ENUM_RETURN delcom_stm_init(void)
{
    for(int i = 0; i < DELCOM_STM_MAX; i++)
    {
        R_ASSERT_LOG(delcom_stm_proc[i].state == i, 
            RETURN_FAILURE, 
            "i = %d, state = %d", 
            i, delcom_stm_proc[i].state);
    }

    return RETURN_SUCCESS;
}

PRIVATE const char* get_delcom_stm_string(ENUM_DELCOM_STM state)
{
    R_ASSERT(whether_delcom_stm_state_is_valid(state) == BOOLEAN_TRUE, NULL);

    return delcom_stm_proc[(state)].info;
}

PRIVATE DELCOM_STM_PROC get_delcom_stm_proc(ENUM_DELCOM_STM state)
{
    R_ASSERT(whether_delcom_stm_state_is_valid(state) == BOOLEAN_TRUE, NULL);

    return delcom_stm_proc[(state)].handler;
}

PRIVATE ENUM_DELCOM_STM get_current_stm_state(void)
{
    return delcom_stm_state;
}

PRIVATE void set_current_stm_state(ENUM_DELCOM_STM arg)
{
    V_ASSERT(arg >= DELCOM_STM_NORMAL && arg < DELCOM_STM_MAX);
    R_LOG("state change from %s to %s", 
        get_delcom_stm_string(delcom_stm_state),
        get_delcom_stm_string(arg));
    
    delcom_stm_state = arg;
}

PRIVATE ENUM_RETURN del_stm_proc_normal(FILE *pfr, FILE *pfw, int c)
{

    switch (c)
    {
        case '"'://this is a test comment
        {/* this is a test comment */
            fputc(c, pfw);
            set_current_stm_state(DELCOM_STM_STRING_DOUBLE_QUOTE);
            break;
        }
        case '/':
        {
            set_current_stm_state(DELCOM_STM_INTERMEDIATE);
            break;
        }
        case '\'':
        {
            fputc(c, pfw);
            set_current_stm_state(DELCOM_STM_STRING_SINGLE_QUOTE);
            break;
        }
        case '\n':
        default:
        {
            fputc(c, pfw);
            break;
        }
    }
    
    return RETURN_SUCCESS;
}
//this is a test comment
PRIVATE ENUM_RETURN del_stm_proc_string_double_quote(FILE *pfr, FILE *pfw, int c)
{
    static ENUM_BOOLEAN whether_backslash_occured = BOOLEAN_FALSE;
    
    switch (c)
    {
        case '"'://a line comment
        {
            fputc(c, pfw);
            if(whether_backslash_occured == BOOLEAN_FALSE)
            {
                set_current_stm_state(DELCOM_STM_NORMAL);
            }

            whether_backslash_occured = BOOLEAN_FALSE;
            break;
        }
        case '\\':
        {
            whether_backslash_occured = BOOLEAN_TRUE;
            fputc(c, pfw);
            break;
        }
        case '\n':
        default:
        {
            whether_backslash_occured = BOOLEAN_FALSE;
            fputc(c, pfw);
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN del_stm_proc_string_single_quote(FILE *pfr, FILE *pfw, int c)
{
    static ENUM_BOOLEAN whether_backslash_occured = BOOLEAN_FALSE;
    
    switch (c)
    {
        case '\''://a line comment
        {
            fputc(c, pfw);
            if(whether_backslash_occured == BOOLEAN_FALSE)
            {
                set_current_stm_state(DELCOM_STM_NORMAL);
            }

            whether_backslash_occured = BOOLEAN_FALSE;
            break;
        }
        case '\\':
        {
            whether_backslash_occured = BOOLEAN_TRUE;
            fputc(c, pfw);
            break;
        }
        case '\n':
        default:
        {
            whether_backslash_occured = BOOLEAN_FALSE;
            fputc(c, pfw);
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN del_stm_proc_oneline_comment(FILE *pfr, FILE *pfw, int c)
{
    switch (c)
    {
        case '\n':
        {
            fputc(c, pfw);
            set_current_stm_state(DELCOM_STM_NORMAL);
            break;
        }
        default:
        {
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN del_stm_proc_pair_comment(FILE *pfr, FILE *pfw, int c)
{
    static ENUM_BOOLEAN whether_star_occured = BOOLEAN_FALSE;
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
                set_current_stm_state(DELCOM_STM_NORMAL);
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

PRIVATE ENUM_RETURN del_stm_proc_intermediate(FILE *pfr, FILE *pfw, int c)
{
    switch (c)
    {
        case '*':
        {
            set_current_stm_state(DELCOM_STM_PAIR_COMMENT);
            break;
        }
        case '/':
        {
            set_current_stm_state(DELCOM_STM_ONELINE_COMMENT);
            break;
        }
        case '\n':
        default:
        {
            fputc('/', pfw);
            fputc(c, pfw);
            set_current_stm_state(DELCOM_STM_NORMAL);
            break;
        }
    }

    return RETURN_SUCCESS;
}

ENUM_RETURN s_cdel_cmnt(FILE *pfr, FILE *pfw)
{
    R_ASSERT(pfr != NULL, RETURN_FAILURE);
    R_ASSERT(pfw != NULL, RETURN_FAILURE);

    ENUM_RETURN ret_val = RETURN_SUCCESS;
    int c;

    ret_val = delcom_stm_init();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    while((c = fgetc(pfr)) != EOF)
    {
        ENUM_DELCOM_STM state = get_current_stm_state();
        DELCOM_STM_PROC handler = get_delcom_stm_proc(state);
        R_ASSERT(handler != NULL, RETURN_FAILURE);
        
        ret_val = handler(pfr, pfw, c);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }
    
    return RETURN_SUCCESS;
}

