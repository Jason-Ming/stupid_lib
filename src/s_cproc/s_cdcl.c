#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "s_log.h"
#include "s_mem.h"
#include "s_text.h"
#include "s_stack.h"
#include "s_stm.h"

#include "s_ctoken.h"
#include "s_cpp.h"
#include "s_cproc_token.h"
#include "s_cdcl.h"

struct TAG_STRU_DCL_VAR;
struct TAG_STRU_DCL_TYPE;

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
    STRU_C_TYPE_QUALIFIER qualifier;
    UNION_DCL_TYPE_INFO info;
}STRU_DCL_TYPE;

typedef struct TAG_STRU_DCL_VAR
{
    const char *name;
    STRU_DCL_TYPE *type;
    struct TAG_STRU_DCL_VAR *next;
}STRU_DCL_VAR;

#define DCL_PRINT(fmt, args...)\
    printf(LIGHT_BLUE""fmt""NONE, ##args);

#define DCL_DBG 0
#if DCL_DBG
#define DCL_DBG_PRINT(head, tail)\
    printf("\n%s:\n", __FUNCTION__);\
    s_ctoken_print_list(NULL, s_cproc_token_get_list_head(), head, tail);
#else
#define DCL_DBG_PRINT(head, tail)
#endif


typedef struct TAG_STRU_DCL_TYPE_ENUM_INFO
{
    ENUM_DCL_TYPE type;
    const _S8 *info;
}STRU_DCL_TYPE_ENUM_INFO;

PRIVATE STRU_DCL_TYPE_ENUM_INFO g_dcl_type_enum_info[DCL_TYPE_MAX] = 
{
    {DCL_TYPE_ARRAY, "array"},
    {DCL_TYPE_POINTER, "pointer"},
    {DCL_TYPE_FUNCTION, "function"},
    {DCL_TYPE_STRUCT, "struct"},
    {DCL_TYPE_ENUM, "enum"},
    {DCL_TYPE_UNION, "union"},
    {DCL_TYPE_NORMAL, "normal"}, /* system type like int, char... */
    {DCL_TYPE_INVALID, "invalid"},
};

const _S8 * get_dcl_type_str(ENUM_DCL_TYPE type)
{
    R_ASSERT(type >= DCL_TYPE_ARRAY && type < DCL_TYPE_MAX, "type out of range");
    R_ASSERT(g_dcl_type_enum_info[type].type == type, "type value in private data error");
    R_ASSERT(g_dcl_type_enum_info[type].info != NULL, "type string in private data error");
    
    return g_dcl_type_enum_info[type].info;
}

PRIVATE _VOID display_dcl_error(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_tail,
    const _S8 *info)
{
    printf(LIGHT_RED"Error: %s\n"NONE, info);
    s_ctoken_print_list(NULL, s_cproc_token_get_list_head(), p_token_list_head, p_token_list_tail);
};

PRIVATE ENUM_RETURN dcl_get_prelist_and_direct_dcl_list(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_tail,
    STRU_C_TOKEN_NODE **pp_prelist_head,
    STRU_C_TOKEN_NODE **pp_prelist_tail,
    STRU_C_TOKEN_NODE **pp_direct_dcl_list_head,
    STRU_C_TOKEN_NODE **pp_direct_dcl_list_tail)
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
    
    struct list_head *pos;
    STRU_C_TOKEN_NODE *p_token_list_node;
    list_for_each(pos, &s_cproc_token_get_list_head()->list, &p_token_list_head->list, &p_token_list_tail->list)
    {
        p_token_list_node = list_entry(pos, STRU_C_TOKEN_NODE, list);
        if(p_token_list_node->info.token_type == C_TOKEN_IDENTIFIER 
            || p_token_list_node->info.token_type == C_TOKEN_PARENTHESIS_LEFT
            || p_token_list_node->info.token_type == C_TOKEN_BRACKET_LEFT)
        {
            has_direct_dcl_list = BOOLEAN_TRUE;
            break;
        }
        else
        {
            if(*pp_prelist_head == NULL)
            {
                *pp_prelist_head = p_token_list_node;
            }

            *pp_prelist_tail = p_token_list_node;
        }
    };

    if(!has_direct_dcl_list)
    {
        *pp_direct_dcl_list_head = NULL;
        *pp_direct_dcl_list_tail = NULL;
    }
    else
    {
        *pp_direct_dcl_list_head = p_token_list_node;
        *pp_direct_dcl_list_tail = p_token_list_tail;
    }
    
    return RETURN_SUCCESS;
};

PRIVATE ENUM_RETURN dcl_get_next_parameter(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_tail,
    STRU_C_TOKEN_NODE **pp_parameter_head,
    STRU_C_TOKEN_NODE **pp_parameter_tail)
{
    R_ASSERT(p_token_list_head != NULL, RETURN_FAILURE);
    R_ASSERT(p_token_list_tail != NULL, RETURN_FAILURE);
    R_ASSERT(pp_parameter_head != NULL, RETURN_FAILURE);
    R_ASSERT(pp_parameter_tail != NULL, RETURN_FAILURE);

    DCL_DBG_PRINT(p_token_list_head, p_token_list_tail);

    *pp_parameter_head = p_token_list_head;
    
    /* not include () */
    ENUM_BOOLEAN in_parenthesis = BOOLEAN_FALSE;
    ENUM_BOOLEAN find_comma = BOOLEAN_FALSE;
    
    struct list_head *pos;
    STRU_C_TOKEN_NODE *p_token_list_node = NULL;
    list_for_each(pos, &s_cproc_token_get_list_head()->list, &p_token_list_head->list, &p_token_list_tail->list)
    {
        p_token_list_node = list_entry(pos, STRU_C_TOKEN_NODE, list);

        switch(p_token_list_node->info.token_type)
        {
            case C_TOKEN_PARENTHESIS_LEFT:
            {
                in_parenthesis = BOOLEAN_TRUE;
                break;
            }
            case C_TOKEN_PARENTHESIS_RIGHT:
            {
                in_parenthesis = BOOLEAN_FALSE;
                break;
            }
            case C_TOKEN_COMMA:
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
    }

    R_ASSERT(p_token_list_node != s_cproc_token_get_list_head(), RETURN_FAILURE);

    if(find_comma)
    {
        *pp_parameter_tail = list_entry(p_token_list_node->list.prev, STRU_C_TOKEN_NODE, list);
    }
    else
    {
        *pp_parameter_tail = p_token_list_tail;
    }
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN proc_dcl_list(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_tail,
    ENUM_BOOLEAN in_function_parameters,
    ENUM_BOOLEAN *need_type);


PRIVATE ENUM_RETURN find_type_for_qualifier(
    STRU_C_TOKEN_NODE *p_qualifier,
    STRU_C_TOKEN_NODE *p_prelist_head,
    STRU_C_TOKEN_NODE *p_prelist_tail,
    ENUM_BOOLEAN *find_type)
{
    R_ASSERT(p_qualifier != NULL, RETURN_FAILURE);
    R_ASSERT(p_prelist_head != NULL, RETURN_FAILURE);
    R_ASSERT(p_prelist_tail != NULL, RETURN_FAILURE);
    R_ASSERT(find_type != NULL, RETURN_FAILURE);

    STRU_C_TOKEN_NODE *p_token_loop_next_type = p_qualifier;
    STRU_C_TOKEN_NODE *p_token_loop_previous_type = p_qualifier;
    *find_type = BOOLEAN_FALSE;
    
    ENUM_BOOLEAN stop_loop = BOOLEAN_FALSE;
    ENUM_BOOLEAN has_previous_type = BOOLEAN_FALSE;

    struct list_head *pos;
    STRU_C_TOKEN_NODE *p_token_list_node = NULL;
    list_for_each_reverse(pos, &s_cproc_token_get_list_head()->list, &p_prelist_head->list, &p_token_loop_previous_type->list)
    {
        p_token_list_node = list_entry(pos, STRU_C_TOKEN_NODE, list);

        switch(p_token_list_node->info.token_type)
        {
            case C_TOKEN_KEYWORD_TYPE:
            {
                p_token_list_node->info.qualifier.is_const = BOOLEAN_TRUE;
                stop_loop = BOOLEAN_TRUE;
                has_previous_type = BOOLEAN_TRUE;
                break;
            }

            case C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY:
            {
                p_token_list_node->info.qualifier.is_const = BOOLEAN_TRUE;
                stop_loop = BOOLEAN_TRUE;
                has_previous_type = BOOLEAN_TRUE;
                break;
            }

            case C_TOKEN_KEYWORD_TYPE_QUALIFIER:
            {
                stop_loop = BOOLEAN_FALSE;
                break;
            }

            default:
            {
                stop_loop = BOOLEAN_TRUE;
                break;
            }
        }
        
        if(stop_loop)
        {
            break;
        }
    }

    if(has_previous_type == BOOLEAN_TRUE)
    {
        *find_type = BOOLEAN_TRUE;
        return RETURN_SUCCESS;
    }
    
    /* find next type */
    stop_loop = BOOLEAN_FALSE;
    ENUM_BOOLEAN has_next_type = BOOLEAN_FALSE;

    list_for_each(pos, &s_cproc_token_get_list_head()->list, &p_token_loop_next_type->list, &p_prelist_tail->list)
    {
        p_token_list_node = list_entry(pos, STRU_C_TOKEN_NODE, list);

        switch(p_token_list_node->info.token_type)
        {
            case C_TOKEN_KEYWORD_TYPE:
            {
                p_token_list_node->info.qualifier.is_const = BOOLEAN_TRUE;
                stop_loop = BOOLEAN_TRUE;
                has_next_type = BOOLEAN_TRUE;
                break;
            }

            case C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY:
            {
                p_token_list_node->info.qualifier.is_const = BOOLEAN_TRUE;
                stop_loop = BOOLEAN_TRUE;
                has_next_type = BOOLEAN_TRUE;
                break;
            }

            case C_TOKEN_KEYWORD_TYPE_QUALIFIER:
            {
                stop_loop = BOOLEAN_FALSE;
                break;
            }

            default:
            {
                stop_loop = BOOLEAN_TRUE;
                break;
            }
        }
        
        if(stop_loop)
        {
            break;
        }
    }

    if(has_next_type == BOOLEAN_TRUE)
    {
        *find_type = BOOLEAN_TRUE;
        return RETURN_SUCCESS;
    }

    return RETURN_SUCCESS;
}
PRIVATE ENUM_RETURN proc_type_qualifier(
    STRU_C_TOKEN_NODE *p_prelist_head,
    STRU_C_TOKEN_NODE *p_prelist_tail)
{
    R_ASSERT(p_prelist_head != NULL, RETURN_FAILURE);
    R_ASSERT(p_prelist_tail != NULL, RETURN_FAILURE);

    ENUM_RETURN ret_val;
    
    /* delete continuing const */
    struct list_head *pos;
    STRU_C_TOKEN_NODE *p_token_list_node = NULL;
    list_for_each(pos, &s_cproc_token_get_list_head()->list, &p_prelist_head->list, &p_prelist_tail->list)
    {
        p_token_list_node = list_entry(pos, STRU_C_TOKEN_NODE, list);

        ENUM_BOOLEAN find_type = BOOLEAN_FALSE;
        if(p_token_list_node->info.token_type == C_TOKEN_KEYWORD_TYPE_QUALIFIER 
            && (strcmp("const", p_token_list_node->info.p_string) == 0))
        {
            ret_val = find_type_for_qualifier(p_token_list_node, p_prelist_head, p_prelist_tail, &find_type);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

            if(find_type == BOOLEAN_FALSE)
            {
                display_dcl_error(p_prelist_head, p_prelist_tail, "missing type for qualifier");
                return RETURN_FAILURE;
            }
        }
    };
    
    return RETURN_SUCCESS;
}
PRIVATE ENUM_RETURN proc_prelist(
    STRU_C_TOKEN_NODE *p_prelist_head,
    STRU_C_TOKEN_NODE *p_prelist_tail,
    ENUM_BOOLEAN *need_type)
{
    
    DCL_DBG_PRINT(p_prelist_head, p_prelist_tail);
    R_ASSERT(need_type != NULL, RETURN_FAILURE);

    S_R_FALSE(p_prelist_head != s_cproc_token_get_list_head(), RETURN_SUCCESS);

    ENUM_RETURN ret_val = proc_type_qualifier(p_prelist_head, p_prelist_tail);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    struct list_head *pos;
    STRU_C_TOKEN_NODE *p_token_list_node = NULL;
    list_for_each_reverse(pos, &s_cproc_token_get_list_head()->list, &p_prelist_head->list, &p_prelist_tail->list)
    {
        p_token_list_node = list_entry(pos, STRU_C_TOKEN_NODE, list);

        switch(p_token_list_node->info.token_type)
        {
            case C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY:
            {   
                DCL_PRINT("a %spointer to ", p_token_list_node->info.qualifier.is_const?"const ":"");
                *need_type = BOOLEAN_TRUE;
                break;
            }
            case C_TOKEN_KEYWORD_TYPE:
            {
                DCL_PRINT("%stype-'%s'", p_token_list_node->info.qualifier.is_const?"const ":"", p_token_list_node->info.p_string);
                *need_type = BOOLEAN_FALSE;
                break;
            }
            case C_TOKEN_KEYWORD_TYPE_QUALIFIER:
            {
                break;
            }
            default:
            {
                display_dcl_error(p_prelist_head, p_token_list_node, "");
                return RETURN_FAILURE;
            }
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN proc_function_parameters(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_tail,
    ENUM_BOOLEAN *need_type)
{
    DCL_DBG_PRINT(p_token_list_head, p_token_list_tail);

    R_ASSERT(p_token_list_head != NULL, RETURN_FAILURE);
    R_ASSERT(p_token_list_tail != NULL, RETURN_FAILURE);
    R_ASSERT(need_type != NULL, RETURN_FAILURE);
    
    STRU_C_TOKEN_NODE *p_token_list_head_temp = p_token_list_head;
    STRU_C_TOKEN_NODE *p_token_list_tail_temp = p_token_list_tail;
    STRU_C_TOKEN_NODE *p_token_list_head_parameter;
    STRU_C_TOKEN_NODE *p_token_list_tail_parameter;
    ENUM_RETURN ret_val;

    DCL_PRINT("a function(");
    *need_type = BOOLEAN_FALSE;

    if(p_token_list_head->list.next != &p_token_list_tail->list)
    {
        p_token_list_head_temp = list_entry(p_token_list_head->list.next, STRU_C_TOKEN_NODE, list);
        p_token_list_tail_temp = list_entry(p_token_list_tail->list.prev, STRU_C_TOKEN_NODE, list);

        ENUM_BOOLEAN first_parameter = BOOLEAN_TRUE;
        
        while(RETURN_SUCCESS == dcl_get_next_parameter(p_token_list_head_temp, p_token_list_tail_temp, &p_token_list_head_parameter, &p_token_list_tail_parameter))
        {
            if(!first_parameter)
            {
                DCL_PRINT(", ");
            }
            first_parameter = BOOLEAN_FALSE;
            ret_val = proc_dcl_list(p_token_list_head_parameter, p_token_list_tail_parameter, BOOLEAN_TRUE, need_type);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

            if(p_token_list_tail_parameter == p_token_list_tail_temp)
            {
                break;
            }

            if(p_token_list_tail_parameter->list.next == &s_cproc_token_get_list_head()->list 
                || p_token_list_tail_parameter->list.next->next == &s_cproc_token_get_list_head()->list)
            {
                display_dcl_error(p_token_list_tail_parameter, p_token_list_tail, "function premeter error");
                return RETURN_FAILURE;
            }
            
            p_token_list_head_temp = list_entry(p_token_list_tail_parameter->list.next->next, STRU_C_TOKEN_NODE, list);
            p_token_list_tail_temp = list_entry(p_token_list_tail->list.prev, STRU_C_TOKEN_NODE, list);
        }
    }
    else
    {
        
    }

    DCL_PRINT(") returning ");
    *need_type = BOOLEAN_TRUE;
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN proc_array_size(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_tail,
    ENUM_BOOLEAN *need_type)
{
    DCL_DBG_PRINT(p_token_list_head, p_token_list_tail);

    R_ASSERT(p_token_list_head != NULL, RETURN_FAILURE);
    R_ASSERT(p_token_list_tail != NULL, RETURN_FAILURE);
    R_ASSERT(need_type != NULL, RETURN_FAILURE);
    
    _S32 size = 0;
    ENUM_RETURN ret_val;
    _S32 parenthesis_level = 0;

    DCL_PRINT("a array");
    *need_type = BOOLEAN_FALSE;

    struct list_head *pos;
    STRU_C_TOKEN_NODE *p_token_list_node = NULL;
    list_for_each(pos, &s_cproc_token_get_list_head()->list, &p_token_list_head->list, &p_token_list_tail->list)
    {
        p_token_list_node = list_entry(pos, STRU_C_TOKEN_NODE, list);

        switch(p_token_list_node->info.token_type)
        {
            case C_TOKEN_BRACKET_LEFT:
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

            case C_TOKEN_BRACKET_RIGHT:
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

            case C_TOKEN_CONSTANT_INTEGER:
            {
                if(parenthesis_level != 1)
                {
                    display_dcl_error(p_token_list_head, p_token_list_tail, "array size format error");
                    return RETURN_FAILURE;
                }

                ret_val = s_strtos32(p_token_list_node->info.p_string, &size);
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
    }
    
    DCL_PRINT(" of ");
    *need_type = BOOLEAN_TRUE;

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN proc_direct_dcl_list(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_tail,
    ENUM_BOOLEAN in_function_parameters,
    ENUM_BOOLEAN *need_type)
{
    R_ASSERT(p_token_list_head != NULL, RETURN_FAILURE);
    R_ASSERT(p_token_list_tail != NULL, RETURN_FAILURE);
    
    DCL_DBG_PRINT(p_token_list_head, p_token_list_tail);

    ENUM_RETURN ret_val;

    if(p_token_list_head == s_cproc_token_get_list_head() 
        && p_token_list_tail == s_cproc_token_get_list_head())
    {
        if(in_function_parameters)
        {
            DCL_PRINT("identifier-'not given' is ");
            *need_type = BOOLEAN_TRUE;
            return RETURN_SUCCESS;
        }
        else
        {
            display_dcl_error(p_token_list_head, p_token_list_tail, "missing identifier");
            return RETURN_FAILURE;
        }
    }

    struct list_head *pos;
    //STRU_C_TOKEN_NODE *p_token_list_node = NULL;

    STRU_C_TOKEN_NODE *p_token_list_tail_temp = p_token_list_tail;
    _S32 parenthesis_level = 0;
    if(p_token_list_tail_temp->info.token_type == C_TOKEN_PARENTHESIS_RIGHT)
    {
        /* find one ()' '(' from tail */
        list_for_each_reverse(pos, &s_cproc_token_get_list_head()->list, &s_cproc_token_get_list_head()->list, &p_token_list_tail->list)
        {
            p_token_list_tail_temp = list_entry(pos, STRU_C_TOKEN_NODE, list);

            switch(p_token_list_tail_temp->info.token_type)
            {
                case C_TOKEN_PARENTHESIS_RIGHT:
                {
                    parenthesis_level++;
                    break;
                }

                case C_TOKEN_PARENTHESIS_LEFT:
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
        }

        /* not have a pair */
        if(p_token_list_tail_temp == s_cproc_token_get_list_head())
        {
            display_dcl_error(p_token_list_head, p_token_list_tail, "parenthesis pairing error");
            return RETURN_FAILURE;
        }

        /* the first token is '(' */
        if(p_token_list_tail_temp == p_token_list_head)
        {
            /* empty() */
            if(p_token_list_head->list.next == &p_token_list_tail->list)
            {
                if(in_function_parameters)
                {
                    ret_val = proc_direct_dcl_list(NULL, NULL, in_function_parameters, need_type);
                    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                    ret_val = proc_function_parameters(p_token_list_tail_temp, p_token_list_tail, need_type);
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
                ret_val = proc_dcl_list(
                    list_entry(p_token_list_head->list.next, STRU_C_TOKEN_NODE,list),
                    list_entry(p_token_list_tail->list.prev, STRU_C_TOKEN_NODE,list),
                    in_function_parameters, need_type);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                return RETURN_SUCCESS;
            }
        }
        else
        {
            /* direct_dcl_list (function premeters) */
            ret_val = proc_direct_dcl_list(p_token_list_head, 
                list_entry(p_token_list_tail_temp->list.prev, STRU_C_TOKEN_NODE,list), 
                in_function_parameters, need_type);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            
            ret_val = proc_function_parameters(p_token_list_tail_temp, p_token_list_tail, need_type);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            return RETURN_SUCCESS;
        }
    }

    p_token_list_tail_temp = p_token_list_tail;
    /* array */
    if(p_token_list_tail->info.token_type == C_TOKEN_BRACKET_RIGHT)
    {
        list_for_each_reverse(pos, &s_cproc_token_get_list_head()->list, 
            &s_cproc_token_get_list_head()->list, 
            p_token_list_tail_temp->list.prev)
        {
            p_token_list_tail_temp = list_entry(pos, STRU_C_TOKEN_NODE, list);

            if(p_token_list_tail_temp->info.token_type != C_TOKEN_BRACKET_LEFT
                &&p_token_list_tail_temp->info.token_type != C_TOKEN_BRACKET_RIGHT
                &&p_token_list_tail_temp->info.token_type != C_TOKEN_CONSTANT_INTEGER)
            {
                break;
            }
        }

        /* direct_dcl_list only contain [][], the array identifier is not given */
        if(&p_token_list_tail_temp->list == p_token_list_head->list.prev)
        {
            ret_val = proc_direct_dcl_list(NULL, NULL, in_function_parameters, need_type);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        }
        else
        {
            /* dcl_list [array size] */
            ret_val = proc_direct_dcl_list(p_token_list_head, p_token_list_tail_temp, in_function_parameters, need_type);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        }
        
        ret_val = proc_array_size(list_entry(p_token_list_tail_temp->list.next, STRU_C_TOKEN_NODE, list), p_token_list_tail, need_type);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

        return RETURN_SUCCESS;
    }

    /* ID */
    if(p_token_list_tail->info.token_type == C_TOKEN_IDENTIFIER)
    {
        if(p_token_list_head == p_token_list_tail)
        {
            DCL_PRINT("identifier-'%s' is ", p_token_list_head->info.p_string);
            *need_type = BOOLEAN_TRUE;
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
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_tail,
    ENUM_BOOLEAN in_function_parameters,
    ENUM_BOOLEAN *need_type)
{
    STRU_C_TOKEN_NODE *p_prelist_head = NULL;
    STRU_C_TOKEN_NODE *p_prelist_tail = NULL;
    STRU_C_TOKEN_NODE *p_direct_dcl_list_head = NULL;
    STRU_C_TOKEN_NODE *p_direct_dcl_list_tail = NULL;

    DCL_DBG_PRINT(p_token_list_head, p_token_list_tail);
    
    ENUM_RETURN ret_val = dcl_get_prelist_and_direct_dcl_list(
        p_token_list_head, 
        p_token_list_tail,
        &p_prelist_head, 
        &p_prelist_tail, 
        &p_direct_dcl_list_head, 
        &p_direct_dcl_list_tail);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = proc_direct_dcl_list(
        p_direct_dcl_list_head, 
        p_direct_dcl_list_tail, 
        in_function_parameters,
        need_type);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    if(*need_type == BOOLEAN_FALSE)
    {
        display_dcl_error(p_token_list_head, p_token_list_tail, "declaration error");
        return RETURN_FAILURE;
    }

    ret_val = proc_prelist(p_prelist_head, p_prelist_tail, need_type);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

ENUM_RETURN s_cdcl(_VOID)
{
    ENUM_RETURN ret_val;
    STRU_C_TOKEN_NODE *p_token_list_head = s_cproc_token_get_list_head();
    printf(LIGHT_CYAN"declaration: ");
	s_ctoken_print_list(NULL, p_token_list_head, p_token_list_head, p_token_list_head);
	printf(NONE"\n");
	
    ENUM_BOOLEAN need_type = BOOLEAN_FALSE;
    
    ret_val = proc_dcl_list(
        list_entry(p_token_list_head->list.next, STRU_C_TOKEN_NODE, list), 
        list_entry(p_token_list_head->list.prev, STRU_C_TOKEN_NODE, list), 
        BOOLEAN_FALSE, 
        &need_type);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
  
    DCL_PRINT("\n");

    if(need_type == BOOLEAN_TRUE)
    {
        display_dcl_error(p_token_list_head, p_token_list_head, "missing type");
        return RETURN_FAILURE;
    }

    return RETURN_SUCCESS;
}


