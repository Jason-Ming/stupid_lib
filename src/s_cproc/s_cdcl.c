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
struct TAG_STRU_DCL_TYPE;

typedef struct TAG_STRU_DCL_TYPE_QUALIFIER
{
    ENUM_BOOLEAN is_const;
    ENUM_BOOLEAN is_auto;
    ENUM_BOOLEAN is_extern;
    ENUM_BOOLEAN is_register;
    ENUM_BOOLEAN is_static;
    ENUM_BOOLEAN is_volatile;
}STRU_DCL_TYPE_QUALIFIER;

typedef struct TAG_STRU_DCL_TYPE_NORMAL
{
    const _S8 *type;
}STRU_DCL_TYPE_NORMAL;

typedef struct TAG_STRU_DCL_TYPE_UNION
{
    const _S8 *tag;
    struct TAG_STRU_DCL_VAR *members;
}STRU_DCL_TYPE_UNION;

typedef struct TAG_STRU_DCL_TYPE_ENUM_MEMBER
{
    const _S8 *name;
    const _S32 value;
}STRU_DCL_TYPE_ENUM_MEMBER;

typedef struct TAG_STRU_DCL_TYPE_ENUM
{
    const _S8 *tag;
    size_t mem_num;
    STRU_DCL_TYPE_ENUM_MEMBER *members;
}STRU_DCL_TYPE_ENUM;

typedef struct TAG_STRU_DCL_TYPE_STRUCT
{
    const _S8 *tag;
    struct TAG_STRU_DCL_VAR *members;
}STRU_DCL_TYPE_STRUCT;

typedef struct TAG_STRU_DCL_TYPE_POINTER
{
    struct TAG_STRU_DCL_TYPE *pointing;
}STRU_DCL_TYPE_POINTER;

typedef struct TAG_STRU_DCL_TYPE_FUNCTION
{
    struct TAG_STRU_DCL_TYPE *returning;
    struct TAG_STRU_DCL_VAR *parameters;
}STRU_DCL_TYPE_FUNCTION;

#define MAX_ARRAY_DIMENSION 256
typedef struct TAG_STRU_DCL_TYPE_ARRAY
{
    struct TAG_STRU_DCL_TYPE *storing;
    size_t dimension;
    size_t size[MAX_ARRAY_DIMENSION];
}STRU_DCL_TYPE_ARRAY;

typedef union TAG_UNION_DCL_TYPE_INFO
{
    STRU_DCL_TYPE_POINTER pointer_info;
    STRU_DCL_TYPE_FUNCTION function_info;
    STRU_DCL_TYPE_ARRAY array_info;
    STRU_DCL_TYPE_STRUCT struct_info;
    STRU_DCL_TYPE_ENUM enum_info;
    STRU_DCL_TYPE_UNION union_info;
    STRU_DCL_TYPE_NORMAL normal_info;
}UNION_DCL_TYPE_INFO;

typedef struct TAG_STRU_DCL_TYPE
{
    ENUM_DCL_TYPE type;
    STRU_DCL_TYPE_QUALIFIER qualifier;
    UNION_DCL_TYPE_INFO info;
}STRU_DCL_TYPE;

typedef struct TAG_STRU_DCL_VAR
{
    const char *name;
    STRU_DCL_TYPE *type;
    struct TAG_STRU_DCL_VAR *next;
}STRU_DCL_VAR;

#define MAX_TOKEN_LEN 256

typedef struct TAG_STRU_DCL_TOKEN
{
    _S8 *p_string;
    ENUM_DCL_TOKEN token_type;
    struct TAG_STRU_DCL_TOKEN *next;
    struct TAG_STRU_DCL_TOKEN *previous;
}STRU_DCL_TOKEN;

#define DCL_PRINT(fmt, args...)\
    printf(LIGHT_BLUE""fmt""NONE, ##args);

#ifdef DCL_DBG
#define DCL_DBG_PRINT(head, tail)\
    printf("\n%s:\n", __FUNCTION__);\
    print_token_list(head, tail);
#else
#define DCL_DBG_PRINT(head, tail)
#endif
PRIVATE ENUM_RETURN add_token_to_list(
    const _S8 *token_string, 
    ENUM_DCL_TOKEN token_type, 
    STRU_DCL_TOKEN **pp_token_list_head, 
    STRU_DCL_TOKEN **pp_token_list_tail)
{
    R_ASSERT(token_string != NULL, RETURN_FAILURE);
    R_ASSERT(pp_token_list_head != NULL, RETURN_FAILURE);
    R_ASSERT(pp_token_list_tail != NULL, RETURN_FAILURE);
    
    STRU_DCL_TOKEN *p_new_token_node = (STRU_DCL_TOKEN*)malloc(sizeof(STRU_DCL_TOKEN));
    R_ASSERT(p_new_token_node != NULL, RETURN_FAILURE);

    p_new_token_node->p_string = (_S8*)malloc(strlen(token_string) + 1);
    R_ASSERT_DO(p_new_token_node->p_string != NULL, RETURN_FAILURE, FREE(p_new_token_node));
    
    strcpy(p_new_token_node->p_string, token_string);
    p_new_token_node->token_type = token_type;
    p_new_token_node->next = NULL;
    p_new_token_node->previous = NULL;

    if(*pp_token_list_head == NULL)
    {
        *pp_token_list_head = p_new_token_node;
        *pp_token_list_tail = p_new_token_node;
    }
    else
    {
        p_new_token_node->previous = *pp_token_list_tail;
        (*pp_token_list_tail)->next = p_new_token_node;
        *pp_token_list_tail = p_new_token_node;
    }
    
    return RETURN_SUCCESS;
}
PRIVATE ENUM_RETURN build_token_list(const _S8 *statement, 
    STRU_DCL_TOKEN **pp_token_list_head, 
    STRU_DCL_TOKEN **pp_token_list_tail)
{
    R_ASSERT(statement != NULL, RETURN_FAILURE);
    R_ASSERT(pp_token_list_head != NULL, RETURN_FAILURE);
    R_ASSERT(pp_token_list_tail != NULL, RETURN_FAILURE);

    _S8 *p_token_buf = (_S8 *)malloc(MAX_TOKEN_LEN);
    R_ASSERT(p_token_buf != NULL, RETURN_FAILURE);
    size_t len = 0;
    ENUM_DCL_TOKEN token;
    
    while(s_cget_token(statement, p_token_buf, MAX_TOKEN_LEN, &len, &token, &statement) == RETURN_SUCCESS && len > 0)
    {
        R_ASSERT(add_token_to_list(p_token_buf, token, pp_token_list_head, pp_token_list_tail) == RETURN_SUCCESS, RETURN_FAILURE);
    }
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN release_token_list(STRU_DCL_TOKEN **pp_token_list_head, STRU_DCL_TOKEN **pp_token_list_tail)
{
    R_ASSERT(pp_token_list_head != NULL, RETURN_FAILURE);
    R_ASSERT(pp_token_list_tail != NULL, RETURN_FAILURE);
    STRU_DCL_TOKEN *p_token_list_head = *pp_token_list_head;
    STRU_DCL_TOKEN *p_token_temp;
    
    while(p_token_list_head != NULL)
    {
        p_token_temp = p_token_list_head;
        FREE(p_token_temp->p_string);
        p_token_list_head = p_token_list_head->next;
        FREE(p_token_temp);
    }

    *pp_token_list_head = NULL;
    *pp_token_list_tail = NULL;
    
    return RETURN_SUCCESS;
}

PRIVATE _VOID print_token_list(STRU_DCL_TOKEN *p_token_list_head, STRU_DCL_TOKEN *p_token_list_tail)
{
    if(p_token_list_head == NULL)
    {
        printf("    Nothing\n");
        return;
    }

    printf("    ");
    
    while(p_token_list_head != NULL)
    {
        printf("%s ", p_token_list_head->p_string);

        if(p_token_list_head == p_token_list_tail)
        {
            break;
        }
        
        p_token_list_head = p_token_list_head->next;
    }

    printf("\n");
}

PRIVATE _VOID display_dcl_error(
    STRU_DCL_TOKEN *p_token_list_head,
    STRU_DCL_TOKEN *p_token_list_tail,
    const _S8 *info)
{
    printf(LIGHT_RED"Error: %s\n"NONE, info);
    print_token_list(p_token_list_head, p_token_list_tail);
};

PRIVATE ENUM_RETURN dcl_get_prelist_and_direct_dcl_list(
    STRU_DCL_TOKEN *p_token_list_head,
    STRU_DCL_TOKEN *p_token_list_tail,
    STRU_DCL_TOKEN **pp_prelist_head,
    STRU_DCL_TOKEN **pp_prelist_tail,
    STRU_DCL_TOKEN **pp_direct_dcl_list_head,
    STRU_DCL_TOKEN **pp_direct_dcl_list_tail)
{
    R_ASSERT(pp_prelist_head != NULL, RETURN_FAILURE);
    R_ASSERT(pp_prelist_tail != NULL, RETURN_FAILURE);
    R_ASSERT(pp_direct_dcl_list_head != NULL, RETURN_FAILURE);
    R_ASSERT(pp_direct_dcl_list_tail != NULL, RETURN_FAILURE);

    DCL_DBG_PRINT(p_token_list_head, p_token_list_tail);

    *pp_prelist_head = NULL;
    *pp_prelist_tail = NULL;
    *pp_direct_dcl_list_head = NULL;
    *pp_direct_dcl_list_tail = NULL;
    ENUM_BOOLEAN has_direct_dcl_list = BOOLEAN_FALSE;
    
    while(p_token_list_head != NULL)
    {
        if(p_token_list_head->token_type == DCL_TOKEN_IDENTIFIER 
            || p_token_list_head->token_type == DCL_TOKEN_PARENTHESIS_LEFT
            || p_token_list_head->token_type == DCL_TOKEN_BRACKET_LEFT)
        {
            has_direct_dcl_list = BOOLEAN_TRUE;
            break;
        }
        else
        {
            if(*pp_prelist_head == NULL)
            {
                *pp_prelist_head = p_token_list_head;
            }

            *pp_prelist_tail = p_token_list_head;
        }

        if(p_token_list_head == p_token_list_tail)
        {
            break;
        }
        
        p_token_list_head = p_token_list_head->next;
    };

    if(!has_direct_dcl_list)
    {
        *pp_direct_dcl_list_head = NULL;
        *pp_direct_dcl_list_tail = NULL;
    }
    else
    {
        *pp_direct_dcl_list_head = p_token_list_head;
        *pp_direct_dcl_list_tail = p_token_list_tail;
    }
    
    return RETURN_SUCCESS;
};

PRIVATE ENUM_RETURN dcl_get_next_premeter(
    STRU_DCL_TOKEN *p_token_list_head,
    STRU_DCL_TOKEN *p_token_list_tail,
    STRU_DCL_TOKEN **pp_premeter_head,
    STRU_DCL_TOKEN **pp_premeter_tail)
{
    R_ASSERT(p_token_list_head != NULL, RETURN_FAILURE);
    R_ASSERT(p_token_list_tail != NULL, RETURN_FAILURE);
    R_ASSERT(pp_premeter_head != NULL, RETURN_FAILURE);
    R_ASSERT(pp_premeter_tail != NULL, RETURN_FAILURE);

    DCL_DBG_PRINT(p_token_list_head, p_token_list_tail);

    *pp_premeter_head = p_token_list_head;
    
    /* not include () */
    ENUM_BOOLEAN in_parenthesis = BOOLEAN_FALSE;
    ENUM_BOOLEAN find_comma = BOOLEAN_FALSE;
    
    while(p_token_list_head != NULL)
    {
        switch(p_token_list_head->token_type)
        {
            case DCL_TOKEN_PARENTHESIS_LEFT:
            {
                in_parenthesis = BOOLEAN_TRUE;
                break;
            }
            case DCL_TOKEN_PARENTHESIS_RIGHT:
            {
                in_parenthesis = BOOLEAN_FALSE;
                break;
            }
            case DCL_TOKEN_COMMA:
            {
                if(in_parenthesis == BOOLEAN_FALSE)
                {
                    find_comma = BOOLEAN_TRUE;
                }
                break;
            }
            default:
            {
                break;
            }
        }

        
        if(BOOLEAN_TRUE == find_comma)
        {
            break;
        }
        
        if(p_token_list_head == p_token_list_tail)
        {
            break;
        }
        
        p_token_list_head = p_token_list_head->next;
    }

    R_ASSERT(p_token_list_head != NULL, RETURN_FAILURE);

    if(find_comma)
    {
        *pp_premeter_tail = p_token_list_head->previous;
    }
    else
    {
        *pp_premeter_tail = p_token_list_tail;
    }
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN proc_dcl_list(
    STRU_DCL_TOKEN *p_token_list_head,
    STRU_DCL_TOKEN *p_token_list_tail,
    ENUM_BOOLEAN in_function_premeters);


PRIVATE ENUM_RETURN proc_prelist(
    STRU_DCL_TOKEN *p_prelist_head,
    STRU_DCL_TOKEN *p_prelist_tail)
{
    DCL_DBG_PRINT(p_prelist_head, p_prelist_tail);
    
    R_FALSE_RET(p_prelist_head != NULL, RETURN_SUCCESS);

    while(p_prelist_tail != NULL)
    {
        switch(p_prelist_tail->token_type)
        {
            case DCL_TOKEN_STAR:
            {
                DCL_PRINT("a pointer to ");
                break;
            }
            case DCL_TOKEN_TYPE:
            {
                DCL_PRINT("type-'%s'", p_prelist_tail->p_string);
                break;
            }
            case DCL_TOKEN_TYPE_QUALIFIER:
            {
                DCL_PRINT(" of %s", p_prelist_tail->p_string);
                break;
            }
            default:
            {
                display_dcl_error(p_prelist_head, p_prelist_tail, "");
                return RETURN_FAILURE;
            }
        }

        if(p_prelist_tail == p_prelist_head)
        {
            break;
        }

        p_prelist_tail = p_prelist_tail->previous;
    }
    
    return RETURN_SUCCESS;
}
char (*(*x(int()))[])();

PRIVATE ENUM_RETURN proc_function_premeters(
    STRU_DCL_TOKEN *p_token_list_head,
    STRU_DCL_TOKEN *p_token_list_tail)
{
    DCL_DBG_PRINT(p_token_list_head, p_token_list_tail);

    R_ASSERT(p_token_list_head != NULL, RETURN_FAILURE);
    R_ASSERT(p_token_list_tail != NULL, RETURN_FAILURE);
    STRU_DCL_TOKEN *p_token_list_head_temp = p_token_list_head;
    STRU_DCL_TOKEN *p_token_list_tail_temp = p_token_list_tail;
    STRU_DCL_TOKEN *p_token_list_head_premeter;
    STRU_DCL_TOKEN *p_token_list_tail_premeter;
    ENUM_RETURN ret_val;

    DCL_PRINT("a function(");

    if(p_token_list_head->next != p_token_list_tail)
    {
        p_token_list_head_temp = p_token_list_head_temp->next;
        p_token_list_tail_temp = p_token_list_tail_temp->previous;
        while(RETURN_SUCCESS == dcl_get_next_premeter(p_token_list_head_temp, p_token_list_tail_temp, &p_token_list_head_premeter, &p_token_list_tail_premeter))
        {
            ret_val = proc_dcl_list(p_token_list_head_premeter, p_token_list_tail_premeter, BOOLEAN_TRUE);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

            if(p_token_list_tail_premeter == p_token_list_tail_temp)
            {
                break;
            }

            if(p_token_list_head_premeter->next == NULL 
                || p_token_list_head_premeter->next->next == NULL)
            {
                display_dcl_error(p_token_list_head_premeter, p_token_list_tail_premeter, "function premeter error");
                return RETURN_FAILURE;
            }
            
            p_token_list_head_temp = p_token_list_head_premeter->next->next;
        }
    }
    else
    {
        
    }

    DCL_PRINT(") returning ");
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN proc_array_size(
    STRU_DCL_TOKEN *p_token_list_head,
    STRU_DCL_TOKEN *p_token_list_tail)
{
    DCL_DBG_PRINT(p_token_list_head, p_token_list_tail);

    R_ASSERT(p_token_list_head != NULL, RETURN_FAILURE);
    R_ASSERT(p_token_list_tail != NULL, RETURN_FAILURE);
    STRU_DCL_TOKEN *p_token_list_head_temp = p_token_list_head;

    _S32 size = 0;
    ENUM_RETURN ret_val;
    _S32 parenthesis_level = 0;

    DCL_PRINT("a array");
    while(p_token_list_head_temp != NULL)
    {
        switch(p_token_list_head_temp->token_type)
        {
            case DCL_TOKEN_BRACKET_LEFT:
            {
                if(parenthesis_level != 0)
                {
                    display_dcl_error(p_token_list_head, p_token_list_tail, "array size format error");
                    return RETURN_FAILURE;
                }
                DCL_PRINT("[");
                parenthesis_level++;
                break;
            }

            case DCL_TOKEN_BRACKET_RIGHT:
            {
                if(parenthesis_level != 1)
                {
                    display_dcl_error(p_token_list_head, p_token_list_tail, "array size format error");
                    return RETURN_FAILURE;
                }
                DCL_PRINT("]");
                parenthesis_level--;
                break;
            }

            case DCL_TOKEN_NUMBER:
            {
                if(parenthesis_level != 1)
                {
                    display_dcl_error(p_token_list_head, p_token_list_tail, "array size format error");
                    return RETURN_FAILURE;
                }

                ret_val = s_strtos32(p_token_list_head_temp->p_string, &size);
                if(ret_val == RETURN_FAILURE)
                {
                    display_dcl_error(p_token_list_head, p_token_list_tail, "array size is not a number");
                    return RETURN_FAILURE;
                }

                DCL_PRINT("%d", size);
                break;
            }

            default:
            {
                display_dcl_error(p_token_list_head, p_token_list_tail, "array size format error");
                return RETURN_FAILURE;
            }
        }
        
        if(p_token_list_head_temp == p_token_list_tail)
        {
            break;
        }
        p_token_list_head_temp = p_token_list_head_temp->next;
    }
    
    DCL_PRINT(" of ");

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN proc_direct_dcl_list(
    STRU_DCL_TOKEN *p_token_list_head,
    STRU_DCL_TOKEN *p_token_list_tail,
    ENUM_BOOLEAN in_function_premeters)
{
    DCL_DBG_PRINT(p_token_list_head, p_token_list_tail);

    ENUM_RETURN ret_val;

    if(p_token_list_head == NULL && p_token_list_tail == NULL)
    {
        if(in_function_premeters)
        {
            DCL_PRINT("identifier-'not given' is ");
            return RETURN_SUCCESS;
        }
        else
        {
            display_dcl_error(p_token_list_head, p_token_list_tail, "missing identifier");
            return RETURN_FAILURE;
        }
    }

    STRU_DCL_TOKEN *p_token_list_tail_temp = p_token_list_tail;
    _S32 parenthesis_level = 0;
    if(p_token_list_tail_temp->token_type == DCL_TOKEN_PARENTHESIS_RIGHT)
    {
        /* find one ()' '(' from tail */
        while(p_token_list_tail_temp != NULL)
        {
            switch(p_token_list_tail_temp->token_type)
            {
                case DCL_TOKEN_PARENTHESIS_RIGHT:
                {
                    parenthesis_level++;
                    break;
                }

                case DCL_TOKEN_PARENTHESIS_LEFT:
                {
                    parenthesis_level--;
                    break;
                }
                
                default:
                {
                    break;
                }
            }
            
            if(parenthesis_level == 0)
            {
                break;
            }

            p_token_list_tail_temp = p_token_list_tail_temp->previous;
        }

        /* not have a pair */
        if(p_token_list_tail_temp == NULL)
        {
            display_dcl_error(p_token_list_head, p_token_list_tail, "parenthesis pairing error");
            return RETURN_FAILURE;
        }

        /* the first token is '(' */
        if(p_token_list_tail_temp == p_token_list_head)
        {
            /* empty() */
            if(p_token_list_head->next == p_token_list_tail)
            {
                if(in_function_premeters)
                {
                    ret_val = proc_direct_dcl_list(NULL, NULL, in_function_premeters);
                    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                    ret_val = proc_function_premeters(p_token_list_tail_temp, p_token_list_tail);
                    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                    return RETURN_SUCCESS;
                }
                else
                {
                    display_dcl_error(p_token_list_head, p_token_list_tail, "missing delaration");
                    return RETURN_FAILURE;
                }
            }
            else
            {
                /* (dcl_list) */
                ret_val = proc_dcl_list(p_token_list_head->next, p_token_list_tail->previous, in_function_premeters);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                return RETURN_SUCCESS;
            }
        }
        else
        {
            /* direct_dcl_list (function premeters) */
            ret_val = proc_direct_dcl_list(p_token_list_head, p_token_list_tail_temp->previous, in_function_premeters);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            
            ret_val = proc_function_premeters(p_token_list_tail_temp, p_token_list_tail);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            return RETURN_SUCCESS;
        }
    }

    p_token_list_tail_temp = p_token_list_tail;
    /* array */
    if(p_token_list_tail->token_type == DCL_TOKEN_BRACKET_RIGHT)
    {
        p_token_list_tail_temp = p_token_list_tail_temp->previous;
        
        while(p_token_list_tail_temp != NULL)
        {
            if(p_token_list_tail_temp->token_type != DCL_TOKEN_BRACKET_LEFT
                &&p_token_list_tail_temp->token_type != DCL_TOKEN_BRACKET_RIGHT
                &&p_token_list_tail_temp->token_type != DCL_TOKEN_NUMBER)
            {
                break;
            }
            
            p_token_list_tail_temp = p_token_list_tail_temp->previous;
        }

        /* direct_dcl_list only contain [][], the array identifier is not given */
        if(p_token_list_tail_temp == p_token_list_head->previous)
        {
            ret_val = proc_direct_dcl_list(NULL, NULL, in_function_premeters);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        }
        else
        {
            /* dcl_list [array size] */
            ret_val = proc_direct_dcl_list(p_token_list_head, p_token_list_tail_temp, in_function_premeters);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        }
        
        ret_val = proc_array_size(p_token_list_tail_temp->next, p_token_list_tail);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

        return RETURN_SUCCESS;
    }

    /* ID */
    if(p_token_list_tail->token_type == DCL_TOKEN_IDENTIFIER)
    {
        if(p_token_list_head == p_token_list_tail)
        {
            DCL_PRINT("identifier-'%s' is ", p_token_list_head->p_string);
            return RETURN_SUCCESS;
        }
        else
        {
            display_dcl_error(p_token_list_head, p_token_list_tail, "");
            return RETURN_FAILURE;
        }
    }

    display_dcl_error(p_token_list_head, p_token_list_tail, "");
    return RETURN_FAILURE;
}



PRIVATE ENUM_RETURN proc_dcl_list(
    STRU_DCL_TOKEN *p_token_list_head,
    STRU_DCL_TOKEN *p_token_list_tail,
    ENUM_BOOLEAN in_function_premeters)
{
    STRU_DCL_TOKEN *p_prelist_head = NULL;
    STRU_DCL_TOKEN *p_prelist_tail = NULL;
    STRU_DCL_TOKEN *p_direct_dcl_list_head = NULL;
    STRU_DCL_TOKEN *p_direct_dcl_list_tail = NULL;

    DCL_DBG_PRINT(p_token_list_head, p_token_list_tail);
    
    ENUM_RETURN ret_val = dcl_get_prelist_and_direct_dcl_list(
        p_token_list_head, 
        p_token_list_tail,
        &p_prelist_head, 
        &p_prelist_tail, 
        &p_direct_dcl_list_head, 
        &p_direct_dcl_list_tail);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = proc_direct_dcl_list(p_direct_dcl_list_head, p_direct_dcl_list_tail, in_function_premeters);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    ret_val = proc_prelist(p_prelist_head, p_prelist_tail);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

ENUM_RETURN s_cdcl(const _S8 *statement)
{
    R_ASSERT(statement != NULL, RETURN_FAILURE);
    ENUM_RETURN ret_val;
    STRU_DCL_TOKEN *p_token_list_head = NULL;
    STRU_DCL_TOKEN *p_token_list_tail = NULL;

    printf(LIGHT_CYAN"declaration: %s\n"NONE, statement);

    ret_val = build_token_list(statement, &p_token_list_head, &p_token_list_tail);
    R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, release_token_list(&p_token_list_head, &p_token_list_tail));
    
    ret_val = proc_dcl_list(p_token_list_head, p_token_list_tail, BOOLEAN_FALSE);
    R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, release_token_list(&p_token_list_head, &p_token_list_tail));

    DCL_PRINT("\n");
    

    release_token_list(&p_token_list_head, &p_token_list_tail);

    return RETURN_SUCCESS;
}


