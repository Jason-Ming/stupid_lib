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

#include "s_cproc.h"

typedef enum TAG_ENUM_CHECKPAIR_STM
{
    CHECKPAIR_STM_NORMAL = 0,
    CHECKPAIR_STM_STRING_DOUBLE_QUOTE,
    CHECKPAIR_STM_STRING_SINGLE_QUOTE,
    CHECKPAIR_STM_ONELINE_COMMENT,
    CHECKPAIR_STM_PAIR_COMMENT,
    CHECKPAIR_STM_INTERMEDIATE,
    CHECKPAIR_STM_END,
    CHECKPAIR_STM_MAX,
}ENUM_CHECKPAIR_STM;

typedef struct TAG_STRU_CHECKPAIR_STM_PROC
{
    STM_STATE state;
    STM_PROC handler;
    const char *info;
}STRU_CHECKPAIR_STM_PROC;

typedef struct TAG_STRU_CHECKPAIR_STM_RUN_DATA
{
    STACK stack;
    int line;
    int column;
    FILE *pfr;
    int c;
    ENUM_BOOLEAN whether_any_error_exists;
}STRU_CHECKPAIR_STM_RUN_DATA;


PRIVATE STM stm = NULL;
PRIVATE STRU_CHECKPAIR_STM_RUN_DATA run_data;

PRIVATE void display_check_error_info(const int c)
{
    printf("Error: \'%c\', at line: %d, column :%d\n", 
        c, run_data.line, run_data.column);
}

PRIVATE void display_check_correct_info(void)
{
    printf("Check correct!\n");
}

PRIVATE void generate_check_error(const _S8 c)
{
    display_check_error_info(c);
    run_data.whether_any_error_exists = BOOLEAN_TRUE;
}

PRIVATE ENUM_RETURN push(const int c)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;

    R_LOG("%c, line %d, column %d", c, run_data.line, run_data.column);
    ret_val = stack_push(run_data.stack, (void *)&c, sizeof(c));
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN pop_and_check(const int c, ENUM_RETURN *result)
{
    R_ASSERT(result != NULL, RETURN_FAILURE);

    ENUM_RETURN ret_val = RETURN_FAILURE;
    int c_temp = 0;
    size_t len = 0;
    size_t stack_data_count = 0;
    ret_val = stack_get_element_count(run_data.stack, &stack_data_count);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    if(stack_data_count > 0)
    {
        ret_val = stack_pop(run_data.stack, (void *)&c_temp, &len, sizeof(c_temp));
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }
    else
    {
        c_temp = 0;
    }

    R_LOG("%c, %c, line %d, column %d", c, c_temp, run_data.line, run_data.column);
    
    switch (c_temp)
    {
        case '(':
        {
            c_temp = ')';
            break;
        }
        case '[':
        {
            c_temp = ']';
            break;
        }
        case '{':
        {
            c_temp = '}';
            break;
        }
        case 0:
        {
            break;
        }
        default:
        {
            ret_val = RETURN_FAILURE;
            break;
        }
    }

    R_ASSERT_LOG(ret_val == RETURN_SUCCESS, RETURN_FAILURE, "the data poped is error: %c", c_temp);
    
    *result = RETURN_SUCCESS;

    if(c_temp != c)
    {
        *result = RETURN_FAILURE;
        R_LOG("c_temp = %c, c = %c", c_temp, c);

        generate_check_error(c_temp);
    }
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN checkpair_stm_prepare_proc()
{
    ENUM_RETURN ret_val = stack_create(&(run_data.stack));
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    run_data.line = 1;
    run_data.column = 0;
    run_data.whether_any_error_exists = BOOLEAN_FALSE;

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN checkpair_stm_clear_proc()
{
    ENUM_RETURN ret_val = stack_delete(&(run_data.stack));
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    if(run_data.whether_any_error_exists == BOOLEAN_FALSE)
    {
        display_check_correct_info();
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN checkpair_stm_preproc()
{
    ENUM_RETURN ret_val;
    
    run_data.c = fgetc(run_data.pfr);
    if(run_data.whether_any_error_exists == BOOLEAN_TRUE)
    {
        R_LOG("check error!");
        ret_val = set_current_stm_state(stm, CHECKPAIR_STM_END);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        
        return RETURN_SUCCESS;
    }

    if(run_data.c == EOF)
    {
        ret_val = set_current_stm_state(stm, CHECKPAIR_STM_END);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        
        return RETURN_SUCCESS;
    }

    if(run_data.c == '\n')
    {
        run_data.column = 0;
        run_data.line++;
    }
    
    run_data.column++;

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN checkpair_stm_postproc()
{
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN checkpair_stm_state_notifier()
{
    return RETURN_SUCCESS;
}


PRIVATE ENUM_RETURN checkpair_stm_proc_normal()
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    int c = run_data.c;
    switch (c)
    {
        case '(':
        case '[':
        case '{':
        {
            ret_val = push(c);
            R_ASSERT_LOG(ret_val == RETURN_SUCCESS, RETURN_FAILURE, 
                "c = %c", c);
            break;
        }
        case ')':
        case ']':
        case '}':
        {
            ENUM_RETURN result = RETURN_FAILURE;
            ret_val = pop_and_check(c, &result);
            R_ASSERT_LOG(ret_val == RETURN_SUCCESS, RETURN_FAILURE, 
                "c = %c", c);
            break;
        }
        case '"'://this is a test comment
        {/* this is a test comment */
            ret_val = set_current_stm_state(stm, CHECKPAIR_STM_STRING_DOUBLE_QUOTE);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }
        case '/':
        {
            ret_val = set_current_stm_state(stm, CHECKPAIR_STM_INTERMEDIATE);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }
        case '\'':
        {
            R_LOG("line %d, column %d", run_data.line, run_data.column);
            ret_val = set_current_stm_state(stm, CHECKPAIR_STM_STRING_SINGLE_QUOTE);
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
//this is a test comment
PRIVATE ENUM_RETURN checkpair_stm_proc_string_double_quote()
{
    static ENUM_BOOLEAN whether_backslash_occured = BOOLEAN_FALSE;
    ENUM_RETURN ret_val;
    int c = run_data.c;
    switch (c)
    {
        case '"'://a line comment
        {
            if(whether_backslash_occured == BOOLEAN_FALSE)
            {
                ret_val = set_current_stm_state(stm, CHECKPAIR_STM_NORMAL);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            }

            whether_backslash_occured = BOOLEAN_FALSE;
            break;
        }
        case '\\':
        {
            whether_backslash_occured = BOOLEAN_TRUE;
            break;
        }
        default:
        {
            whether_backslash_occured = BOOLEAN_FALSE;
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN checkpair_stm_proc_string_single_quote()
{
    static ENUM_BOOLEAN whether_backslash_occured = BOOLEAN_FALSE;
    int c = run_data.c;
    ENUM_RETURN ret_val;
    switch (c)
    {
        case '\''://a line comment
        {
            if(whether_backslash_occured == BOOLEAN_FALSE)
            {
                R_LOG("line %d, column %d", run_data.line, run_data.column);
                ret_val = set_current_stm_state(stm, CHECKPAIR_STM_NORMAL);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            }

            whether_backslash_occured = BOOLEAN_FALSE;
            break;
        }
        case '\\':
        {
            if(whether_backslash_occured == BOOLEAN_TRUE)
            {
                whether_backslash_occured = BOOLEAN_FALSE;
            }
            else
            {
                whether_backslash_occured = BOOLEAN_TRUE;
            }
            
            break;
        }
        default:
        {
            whether_backslash_occured = BOOLEAN_FALSE;
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN checkpair_stm_proc_oneline_comment()
{
    int c = run_data.c;
    ENUM_RETURN ret_val;
    switch (c)
    {
        case '\n':
        {
            ret_val = set_current_stm_state(stm, CHECKPAIR_STM_NORMAL);
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

PRIVATE ENUM_RETURN checkpair_stm_proc_pair_comment()
{
    static ENUM_BOOLEAN whether_star_occured = BOOLEAN_FALSE;
    int c = run_data.c;
    ENUM_RETURN ret_val;
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
                ret_val = set_current_stm_state(stm, CHECKPAIR_STM_NORMAL);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            }
            whether_star_occured = BOOLEAN_FALSE;
            break;
        }
        default:
        {
            whether_star_occured = BOOLEAN_FALSE;
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN checkpair_stm_proc_intermediate()
{
    int c = run_data.c;
    ENUM_RETURN ret_val;
    switch (c)
    {
        case '*':
        {
            ret_val = set_current_stm_state(stm, CHECKPAIR_STM_PAIR_COMMENT);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }
        case '/':
        {
            ret_val = set_current_stm_state(stm, CHECKPAIR_STM_ONELINE_COMMENT);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }
        default:
        {
            ret_val = set_current_stm_state(stm, CHECKPAIR_STM_NORMAL);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN checkpair_stm_proc_end()
{
    //if there is any error before return to the main process to display the error
    R_FALSE_RET(run_data.whether_any_error_exists == BOOLEAN_FALSE, RETURN_SUCCESS);

    //run to here will mean missing some char in the end of the file
    size_t stack_data_count = 0;
    ENUM_RETURN ret_val = stack_get_element_count(run_data.stack, &stack_data_count);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    if(stack_data_count != 0)
    {
        int c_temp = 0;
        size_t len = 0;
        ret_val = stack_pop(run_data.stack, (void *)&c_temp, &len, sizeof(c_temp));
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
        generate_check_error(c_temp);
    }
    
    return RETURN_SUCCESS;
}

PRIVATE STRU_CHECKPAIR_STM_PROC checkpair_stm_proc[CHECKPAIR_STM_MAX] = 
{
    {CHECKPAIR_STM_NORMAL, checkpair_stm_proc_normal, "normal"},
    {CHECKPAIR_STM_STRING_DOUBLE_QUOTE, checkpair_stm_proc_string_double_quote, "string double quote"},
    {CHECKPAIR_STM_STRING_SINGLE_QUOTE, checkpair_stm_proc_string_single_quote, "string single quote"},
    {CHECKPAIR_STM_ONELINE_COMMENT, checkpair_stm_proc_oneline_comment, "oneline comment"},
    {CHECKPAIR_STM_PAIR_COMMENT, checkpair_stm_proc_pair_comment, "pair comment"},
    {CHECKPAIR_STM_INTERMEDIATE, checkpair_stm_proc_intermediate, "intermediate"},
    {CHECKPAIR_STM_END, checkpair_stm_proc_end, "end"},
};

PRIVATE ENUM_RETURN checkpair_stm_init(void)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    ret_val = stm_create(&stm, CHECKPAIR_STM_MAX);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = set_stm_start_state(stm, CHECKPAIR_STM_NORMAL);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = set_stm_end_state(stm, CHECKPAIR_STM_END);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_prepare_handler(stm, checkpair_stm_prepare_proc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_clear_handler(stm, checkpair_stm_clear_proc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_preproc_handler(stm, checkpair_stm_preproc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_postproc_handler(stm, checkpair_stm_postproc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_state_notifier(stm, checkpair_stm_state_notifier);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    for(int i = 0; i < SIZE_OF_ARRAY(checkpair_stm_proc); i++)
    {
        ret_val = add_stm_state_handler(
            stm, 
            checkpair_stm_proc[i].state, 
            checkpair_stm_proc[i].handler, 
            checkpair_stm_proc[i].info);
        
        R_ASSERT_LOG(
            ret_val == RETURN_SUCCESS, 
            RETURN_FAILURE, 
            "i = %d, state = %d",
            i, 
            checkpair_stm_proc[i].state);
    }

    return RETURN_SUCCESS;
}

ENUM_RETURN s_cchk_pair(FILE *pfr, ENUM_RETURN *check_result)
{
    R_ASSERT(check_result != NULL, RETURN_FAILURE);
    
    R_ASSERT(pfr != NULL, RETURN_FAILURE);
    ENUM_RETURN ret_val;

    *check_result = RETURN_SUCCESS;

    ret_val = checkpair_stm_init();
    R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, FREE(stm));

    run_data.pfr = pfr;
    ret_val = stm_run(stm);
    R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, FREE(stm));

    if(run_data.whether_any_error_exists)
    {
        *check_result = RETURN_FAILURE;
    }
    
    return RETURN_SUCCESS;
}

