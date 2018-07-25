#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "s_log.h"
#include "s_mem.h"
#include "s_text.h"
#include "s_stack.h"
#include "s_stm.h"

#include "s_cproc.h"

struct TAG_STRU_DCL_VAR;

typedef struct TAG_STRU_DCL_TYPE
{
    ENUM_DCL_TYPE type;
    
    /* only for array */
    size_t size;

    /* means: 
    1.array's elelment type; 
    2.pointer's pointing type;
    3.function's returnning type;
    */
    struct TAG_STRU_DCL_TYPE *p_contain_type;

    /* only for function */
    struct TAG_STRU_DCL_VAR *parameters;
}STRU_DCL_TYPE;

typedef struct TAG_STRU_DCL_VAR
{
    const char *name;
    struct TAG_STRU_DCL_TYPE *p_type;
    struct TAG_STRU_DCL_VAR *next;
}STRU_DCL_VAR;

#define MAX_TOKEN_LEN 256

typedef struct TAG_STRU_DCL_STM_PROC
{
    STM_STATE state;
    STM_PROC handler;
    const char *info;
}STRU_DCL_STM_PROC;

typedef struct TAG_STRU_DCL_TOKEN
{
    _S8 token_string[MAX_TOKEN_LEN];
    ENUM_DCL_TOKEN token_type;
}STRU_DCL_TOKEN;

typedef struct TAG_STRU_DCL_STM_RUN_DATA
{
    STACK stack;
    const _S8 *statement;
    STRU_DCL_TOKEN token_buffer;
    STRU_DCL_TOKEN token;
    int c;
    ENUM_BOOLEAN foward;
    ENUM_BOOLEAN whether_any_error_exists;
}STRU_DCL_STM_RUN_DATA;


PRIVATE STM g_dcl_stm = NULL;
PRIVATE STRU_DCL_STM_RUN_DATA g_dcl_run_data;
typedef enum TAG_ENUM_DCL_STM_STATE
{
    DCL_STM_STATE_FIND_IDENTIFIER = 0,
    DCL_STM_STATE_PROC_IDENTIFIER,
    DCL_STM_STATE_PROC_IDENTIFIER_TYPE,
    DCL_STM_STATE_PROC_IDENTIFIER_TYPE_BACKWARD,
    DCL_STM_STATE_PROC_FUNCTION_PARAMETERS,
    DCL_STM_STATE_PROC_ARRAY_SIZE,
    DCL_STM_STATE_END,
    DCL_STM_STATE_MAX,
}ENUM_DCL_STM_STATE;


PRIVATE void dcl_display_error_info(_VOID)
{
    printf("Error! %s\n", g_dcl_run_data.statement);
}

PRIVATE void dcl_display_correct_info(void)
{
    printf("Correct!\n");
}

PRIVATE ENUM_RETURN dcl_push_token(const STRU_DCL_TOKEN t)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;

    ret_val = stack_push(g_dcl_run_data.stack, (void *)&t, sizeof(t));
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN dcl_pop_token(STRU_DCL_TOKEN *t)
{
    R_ASSERT(t != NULL, RETURN_FAILURE);

    ENUM_RETURN ret_val = RETURN_FAILURE;
    size_t len = 0;
    size_t stack_data_count = 0;
    ret_val = stack_get_element_count(g_dcl_run_data.stack, &stack_data_count);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    if(stack_data_count > 0)
    {
        ret_val = stack_pop(g_dcl_run_data.stack, (void *)t, &len, sizeof(STRU_DCL_TOKEN));
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }
    else
    {
        t->token_type = DCL_TOKEN_INVALID;
        t->token_string[0] = '\0';
    }

    DEBUG_PRINT("pop: %s, %d", t->token_string, t->token_type);
    
    return RETURN_SUCCESS;
}

PRIVATE _VOID dcl_get_token_foward(_VOID)
{
    g_dcl_run_data.foward = BOOLEAN_TRUE;
}

PRIVATE _VOID dcl_get_token_backward(_VOID)
{
    g_dcl_run_data.foward = BOOLEAN_FALSE;
}

PRIVATE ENUM_RETURN dcl_get_next_token(_VOID)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    size_t token_len = 0;
    if(g_dcl_run_data.token_buffer.token_type != DCL_TOKEN_INVALID)
    {
        g_dcl_run_data.token = g_dcl_run_data.token_buffer;
        g_dcl_run_data.token_buffer.token_type = DCL_TOKEN_INVALID;
        return RETURN_SUCCESS;
    }
    
    ret_val = s_cget_token(g_dcl_run_data.statement, 
        g_dcl_run_data.token.token_string, 
        MAX_TOKEN_LEN,
        &token_len, &(g_dcl_run_data.token.token_type), 
        &(g_dcl_run_data.statement));
    
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN dcl_send_back_token(_VOID)
{
    R_ASSERT(g_dcl_run_data.token_buffer.token_type == DCL_TOKEN_INVALID, RETURN_FAILURE);
    
    g_dcl_run_data.token_buffer = g_dcl_run_data.token;
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN dcl_stm_prepare_proc()
{
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN dcl_stm_clear_proc()
{
    if(g_dcl_run_data.whether_any_error_exists == BOOLEAN_FALSE)
    {
        dcl_display_correct_info();
    }
    else
    {
        dcl_display_error_info();
    }

    return RETURN_SUCCESS;
}


PRIVATE ENUM_RETURN dcl_stm_preproc()
{
	ENUM_RETURN ret_val = RETURN_SUCCESS;
    

    if(g_dcl_run_data.whether_any_error_exists == BOOLEAN_TRUE)
    {
        DEBUG_PRINT("Error!");
        ret_val = set_current_stm_state(g_dcl_stm, DCL_STM_STATE_END);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        
        return RETURN_SUCCESS;
    }

    DEBUG_PRINT("get token: %s", g_dcl_run_data.foward?"--->>>":"<<<---");
    
    if(g_dcl_run_data.foward)
    {
        ret_val = dcl_get_next_token();
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }
    else
    {
        ret_val = dcl_pop_token(&(g_dcl_run_data.token));
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }

    DEBUG_PRINT("get token: %s, %d", g_dcl_run_data.token.token_string, g_dcl_run_data.token.token_type);
    
    if(g_dcl_run_data.token.token_type == DCL_TOKEN_INVALID)
    {
        ret_val = set_current_stm_state(g_dcl_stm, DCL_STM_STATE_END);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN dcl_stm_postproc()
{
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN dcl_stm_state_notifier()
{
    return RETURN_SUCCESS;
}


PRIVATE ENUM_RETURN dcl_stm_proc_find_identifier()
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    STRU_DCL_TOKEN token = g_dcl_run_data.token;
    
    switch(token.token_type)
    {
        /* ( * type */
        case DCL_TOKEN_PARENTHESIS_LEFT:
        case DCL_TOKEN_STAR:
        case DCL_TOKEN_TYPE:
        {
            ret_val = dcl_push_token(token);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            dcl_get_token_foward();
            break;
        }

        case DCL_TOKEN_IDENTIFIER:
        {
            ret_val = dcl_push_token(token);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            
            ret_val = set_current_stm_state(g_dcl_stm, DCL_STM_STATE_PROC_IDENTIFIER);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            dcl_get_token_backward();
            break;
        }

        default:
        {
            g_dcl_run_data.whether_any_error_exists = BOOLEAN_TRUE;
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN dcl_stm_proc_proc_identifier()
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    STRU_DCL_TOKEN token_current;

    token_current = g_dcl_run_data.token;

    switch(token_current.token_type)
    {
        /* ) */
        case DCL_TOKEN_IDENTIFIER:
        {
            printf("identifier '%s' is ", token_current.token_string);
            dcl_get_token_foward();
            ret_val = set_current_stm_state(g_dcl_stm, DCL_STM_STATE_PROC_IDENTIFIER_TYPE);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }

        default:
        {
            g_dcl_run_data.whether_any_error_exists = BOOLEAN_TRUE;
            break;
        }
    }
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN dcl_stm_proc_proc_identifier_type()
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    STRU_DCL_TOKEN token_current;

    token_current = g_dcl_run_data.token;

    switch(token_current.token_type)
    {
        case DCL_TOKEN_END:
        {
            dcl_get_token_backward();
            ret_val = set_current_stm_state(g_dcl_stm, DCL_STM_STATE_PROC_IDENTIFIER_TYPE_BACKWARD);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }
        
        /* ) */
        case DCL_TOKEN_PARENTHESIS_RIGHT:
        {
            dcl_get_token_backward();
            ret_val = set_current_stm_state(g_dcl_stm, DCL_STM_STATE_PROC_IDENTIFIER_TYPE_BACKWARD);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }
        
        /* ( */
        case DCL_TOKEN_PARENTHESIS_LEFT:
        {
            printf("a function");
            ret_val = dcl_push_token(token_current);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            
            dcl_get_token_backward();
            ret_val = set_current_stm_state(g_dcl_stm, DCL_STM_STATE_PROC_FUNCTION_PARAMETERS);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }

        /* [ */
        case DCL_TOKEN_BRACKET_LEFT:
        {
            printf("a array");
            ret_val = dcl_push_token(token_current);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            
            dcl_get_token_backward();
            ret_val = set_current_stm_state(g_dcl_stm, DCL_STM_STATE_PROC_ARRAY_SIZE);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }
        
        default:
        {
            g_dcl_run_data.whether_any_error_exists = BOOLEAN_TRUE;
            break;
        }
    }
    
    return RETURN_SUCCESS;
}


PRIVATE ENUM_RETURN dcl_stm_proc_proc_identifier_backward()
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    STRU_DCL_TOKEN token_current;

    token_current = g_dcl_run_data.token;

    switch(token_current.token_type)
    {
        /* ( */
        case DCL_TOKEN_PARENTHESIS_LEFT:
        {
            dcl_get_token_foward();
            ret_val = set_current_stm_state(g_dcl_stm, DCL_STM_STATE_PROC_IDENTIFIER_TYPE);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }
        
        /* * */
        case DCL_TOKEN_STAR:
        {
            dcl_get_token_backward();
            printf("a pointer to ");
            break;
        }

        /* type */
        case DCL_TOKEN_TYPE:
        {
            dcl_get_token_backward();
            printf("type-'%s' ", token_current.token_string);
            break;
        }
        
        default:
        {
            g_dcl_run_data.whether_any_error_exists = BOOLEAN_TRUE;
            break;
        }
    }
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN dcl_stm_proc_proc_identifier_function_parameters()
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;

    STRU_DCL_TOKEN token_current;
    PRIVATE _S32 parenthesis_num = 0;
    token_current = g_dcl_run_data.token;

    printf("%s ", token_current.token_string);

    DEBUG_PRINT("token_string: %s, parenthesis_num = %d", token_current.token_string, parenthesis_num);
    
    switch(token_current.token_type)
    {
        /* ) */
        case DCL_TOKEN_PARENTHESIS_RIGHT:
        {
            parenthesis_num--;
            if(parenthesis_num == 0)
            {
                printf("returning ");
                dcl_get_token_foward();
                ret_val = set_current_stm_state(g_dcl_stm, DCL_STM_STATE_PROC_IDENTIFIER_TYPE);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                parenthesis_num = 0;
            }
            else
            {
                dcl_get_token_foward();
            }

            break;
        }

        case DCL_TOKEN_PARENTHESIS_LEFT:
        {
            parenthesis_num++;
            dcl_get_token_foward();

            break;
        }
        
        case DCL_TOKEN_INVALID:
        {
            g_dcl_run_data.whether_any_error_exists = BOOLEAN_TRUE;
            break;
        }

        default:
        {
            dcl_get_token_foward();
            break;
        }
    }
    
    return RETURN_SUCCESS;
}


PRIVATE ENUM_RETURN dcl_stm_proc_proc_identifier_array_size()
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;

    STRU_DCL_TOKEN token_current;
    PRIVATE _S32 bracket_num = 0;
    token_current = g_dcl_run_data.token;

    switch(token_current.token_type)
    {
        /* [ */
        case DCL_TOKEN_BRACKET_LEFT:
        {
            if(bracket_num != 0)
            {
                g_dcl_run_data.whether_any_error_exists = BOOLEAN_TRUE;
            }
            else
            {
                bracket_num++;
                printf("[");
            }
            dcl_get_token_foward();
            break;
        }
        
        /* ] */
        case DCL_TOKEN_BRACKET_RIGHT:
        {
            if(bracket_num != 1)
            {
                g_dcl_run_data.whether_any_error_exists = BOOLEAN_TRUE;
            }
            else
            {
                bracket_num--;
                printf("]");
            }
            dcl_get_token_foward();
            break;
        }
        case DCL_TOKEN_NUMBER:
        {
            if(bracket_num != 1)
            {
                g_dcl_run_data.whether_any_error_exists = BOOLEAN_TRUE;
            }
            else
            {
                printf("%s", token_current.token_string);
            }
            dcl_get_token_foward();
            break;
        }
        
        case DCL_TOKEN_INVALID:
        {
            g_dcl_run_data.whether_any_error_exists = BOOLEAN_TRUE;
            break;
        }

        default:
        {
            if(bracket_num != 0)
            {
                g_dcl_run_data.whether_any_error_exists = BOOLEAN_TRUE;
            }
            else
            {
                printf(" of ");
                ret_val = dcl_send_back_token();
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                dcl_get_token_foward();
                ret_val = set_current_stm_state(g_dcl_stm, DCL_STM_STATE_PROC_IDENTIFIER_TYPE);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            }

            break;
        }

    }
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN dcl_stm_proc_end()
{
    return RETURN_SUCCESS;
}

PRIVATE STRU_DCL_STM_PROC dcl_stm_proc[DCL_STM_STATE_MAX] = 
{
    {DCL_STM_STATE_FIND_IDENTIFIER, dcl_stm_proc_find_identifier, "find identifier"},
    {DCL_STM_STATE_PROC_IDENTIFIER, dcl_stm_proc_proc_identifier, "process identifier"},
    {DCL_STM_STATE_PROC_IDENTIFIER_TYPE, dcl_stm_proc_proc_identifier_type, "process identifier type"},
    {DCL_STM_STATE_PROC_IDENTIFIER_TYPE_BACKWARD, dcl_stm_proc_proc_identifier_backward, "process identifier type backward"},
    {DCL_STM_STATE_PROC_FUNCTION_PARAMETERS, dcl_stm_proc_proc_identifier_function_parameters, "process identifier function parameters"},
    {DCL_STM_STATE_PROC_ARRAY_SIZE, dcl_stm_proc_proc_identifier_array_size, "process identifier array size"},
    {DCL_STM_STATE_END, dcl_stm_proc_end, "end"},
};

ENUM_RETURN dcl_stm_init(const _S8 *statement)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    ret_val = stm_create(&g_dcl_stm, DCL_STM_STATE_MAX);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = set_stm_start_state(g_dcl_stm, DCL_STM_STATE_FIND_IDENTIFIER);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = set_stm_end_state(g_dcl_stm, DCL_STM_STATE_END);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_prepare_handler(g_dcl_stm, dcl_stm_prepare_proc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_clear_handler(g_dcl_stm, dcl_stm_clear_proc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_preproc_handler(g_dcl_stm, dcl_stm_preproc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_postproc_handler(g_dcl_stm, dcl_stm_postproc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_state_notifier(g_dcl_stm, dcl_stm_state_notifier);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    for(int i = 0; i < SIZE_OF_ARRAY(dcl_stm_proc); i++)
    {
        ret_val = add_stm_state_handler(
            g_dcl_stm, 
            dcl_stm_proc[i].state, 
            dcl_stm_proc[i].handler, 
            dcl_stm_proc[i].info);
        
        R_ASSERT_LOG(
            ret_val == RETURN_SUCCESS, 
            RETURN_FAILURE, 
            "i = %d, state = %d",
            i, 
            dcl_stm_proc[i].state);
    }

    g_dcl_run_data.foward = BOOLEAN_TRUE;
    g_dcl_run_data.stack = NULL;
    g_dcl_run_data.statement = statement;
    g_dcl_run_data.token.token_type = DCL_TOKEN_INVALID;
    g_dcl_run_data.token_buffer.token_type = DCL_TOKEN_INVALID;
    g_dcl_run_data.whether_any_error_exists = BOOLEAN_FALSE;

    ret_val = stack_create(&(g_dcl_run_data.stack));
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

PRIVATE _VOID dcl_stm_clear(_VOID)
{
    ENUM_RETURN ret_val = stm_delete(&g_dcl_stm);
    V_ASSERT(ret_val == RETURN_SUCCESS);
    
    ret_val = stack_delete(&(g_dcl_run_data.stack));
    V_ASSERT(ret_val == RETURN_SUCCESS);
}

ENUM_RETURN s_cdcl(const _S8 *statement)
{
    R_ASSERT(statement != NULL, RETURN_FAILURE);
    ENUM_RETURN ret_val;

    ret_val = dcl_stm_init(statement);
    R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, dcl_stm_clear());
    
    ret_val = stm_run(g_dcl_stm);
    R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, dcl_stm_clear());

    dcl_stm_clear();

    return RETURN_SUCCESS;
}


