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
/*

dcl: optional *'s direct-dcl 
direct-dcl:
  name 
  (dcl) 
  direct-dcl() 
  direct-dcl[optional size] 

简而言之，声明符 dcl 就是前面可能带有多个*的 direct-dcl。direct-dcl 可以是 name、由一对圆括号括起来的 
dcl、后面跟有一对圆括号的 direct-dcl、后面跟有用方括号括起来的表示可选长度的 direct-dcl。

该语法可用来对 C 语言的声明进行分析。例如，考虑下面的声明符：
    (*pfa[])() 
按照该语法分析，pfa 将被识别为一个 name，从而被认为是一个 direct-dcl。于是，pfa[]也
是一个 direct-dcl。接着，*pfa[]被识别为一个 dcl，因此，判定(*pfa[])是一个 direct-dcl。
再接着，(*pfa[])()被识别为一个 direct-dcl，因此也是一个 dcl。可以用图 5-12 所示的语法
分析树来说明分析的过程（其中 direct-dcl 缩写为 dir-dcl）。

*/


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
    printf(LIGHT_YELLOW""fmt""NONE, ##args);

#define DCL_DBG 0
#if DCL_DBG
#define DCL_DBG_PRINT(info, head, tail)\
    printf("\n%s: %s\n", __FUNCTION__, info);\
    if(head == NULL)\
    {\
        printf("NULL\n");\
    }\
    else\
    {\
        s_ctoken_print_list(NULL, s_cproc_token_get_list_head(), head, tail);\
    }
#else
#define DCL_DBG_PRINT(info, head, tail)
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
    printf(LIGHT_RED"\nError: %s\n"NONE, info);
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

    DCL_DBG_PRINT("input:", p_token_list_head, p_token_list_tail);

    *pp_prelist_head = NULL;
    *pp_prelist_tail = NULL;
    *pp_direct_dcl_list_head = NULL;
    *pp_direct_dcl_list_tail = NULL;
    ENUM_BOOLEAN has_direct_dcl_list = BOOLEAN_FALSE;
    
    STRU_C_TOKEN_NODE *p_token_list_node;
    list_for_each(PREV_TOKEN(p_token_list_head), NEXT_TOKEN(p_token_list_tail))
    {
        p_token_list_node = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);
        
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

    DCL_DBG_PRINT("prelist:", *pp_prelist_head, *pp_prelist_tail);

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

    DCL_DBG_PRINT("direct_dcl_list:", *pp_direct_dcl_list_head, *pp_direct_dcl_list_tail);

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

    DCL_DBG_PRINT("input:", p_token_list_head, p_token_list_tail);

    *pp_parameter_head = p_token_list_head;
    
    /* not include () */
    ENUM_BOOLEAN in_parenthesis = BOOLEAN_FALSE;
    ENUM_BOOLEAN find_comma = BOOLEAN_FALSE;
    
    STRU_C_TOKEN_NODE *p_token_list_node = NULL;
    list_for_each(PREV_TOKEN(p_token_list_head), NEXT_TOKEN(p_token_list_tail))
    {
        p_token_list_node = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);

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

    //R_ASSERT(p_token_list_node != NEXT_TOKEN(p_token_list_tail), RETURN_FAILURE);

    if(find_comma)
    {
        *pp_parameter_tail = PREV_TOKEN(p_token_list_node);
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

    STRU_C_TOKEN_NODE *p_token_list_node = NULL;
    list_for_each_reverse(PREV_TOKEN(p_prelist_head), NEXT_TOKEN(p_token_loop_previous_type))
    {
        p_token_list_node = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);

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

    list_for_each(PREV_TOKEN(p_token_loop_next_type), NEXT_TOKEN(p_prelist_tail))
    {
        p_token_list_node = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);

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
    STRU_C_TOKEN_NODE *p_token_list_node = NULL;
    list_for_each(PREV_TOKEN(p_prelist_head), NEXT_TOKEN(p_prelist_tail))
    {
        p_token_list_node = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);

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
    
    DCL_DBG_PRINT("input:", p_prelist_head, p_prelist_tail);
    R_ASSERT(need_type != NULL, RETURN_FAILURE);

    S_R_FALSE(p_prelist_head != s_cproc_token_get_list_head(), RETURN_SUCCESS);

    ENUM_RETURN ret_val = proc_type_qualifier(p_prelist_head, p_prelist_tail);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    STRU_C_TOKEN_NODE *p_token_list_node = NULL;
    list_for_each_reverse(PREV_TOKEN(p_prelist_head), NEXT_TOKEN(p_prelist_tail))
    {
        p_token_list_node = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);

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
                display_dcl_error(p_prelist_head, p_token_list_node, "unexpected case!");
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
    DCL_DBG_PRINT("input:", p_token_list_head, p_token_list_tail);

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

    //"()" OR NOT
    if(NEXT_TOKEN(p_token_list_head) != p_token_list_tail)
    {
        //relocation the list in parentheses
        p_token_list_head_temp = NEXT_TOKEN(p_token_list_head);
        p_token_list_tail_temp = PREV_TOKEN(p_token_list_tail);

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

            if(NEXT_TOKEN(p_token_list_tail_parameter) == p_token_list_tail 
                || NEXT_NEXT_TOKEN(p_token_list_tail_parameter) == p_token_list_tail)
            {
                display_dcl_error(p_token_list_tail_parameter, p_token_list_tail, "function premeter error");
                return RETURN_FAILURE;
            }
            
            p_token_list_head_temp = NEXT_NEXT_TOKEN(p_token_list_tail_parameter);
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
    DCL_DBG_PRINT("input:", p_token_list_head, p_token_list_tail);

    R_ASSERT(p_token_list_head != NULL, RETURN_FAILURE);
    R_ASSERT(p_token_list_tail != NULL, RETURN_FAILURE);
    R_ASSERT(need_type != NULL, RETURN_FAILURE);
    
    _S32 size = 0;
    ENUM_RETURN ret_val;
    _S32 parenthesis_level = 0;

    DCL_PRINT("a array");
    *need_type = BOOLEAN_FALSE;

    STRU_C_TOKEN_NODE *p_token_list_node = NULL;
    list_for_each(PREV_TOKEN(p_token_list_head), NEXT_TOKEN(p_token_list_tail))
    {
        p_token_list_node = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);

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
    R_ASSERT((p_token_list_head != NULL && p_token_list_tail != NULL) || (p_token_list_head == NULL && p_token_list_tail == NULL), RETURN_FAILURE);

    DCL_DBG_PRINT("input:", p_token_list_head, p_token_list_tail);

    ENUM_RETURN ret_val;

    if(p_token_list_head == NULL 
        && p_token_list_tail == NULL)
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

    //STRU_C_TOKEN_NODE *p_token_list_node = NULL;

    STRU_C_TOKEN_NODE *p_token_iterator = NULL;
    _S32 parenthesis_level = 0;
    _S32 parenthesis_number = 0;
    if(p_token_list_tail->info.token_type == C_TOKEN_PARENTHESIS_RIGHT)
    {
        /* find one ()' '(' from tail */
        list_for_each_reverse(PREV_TOKEN(p_token_list_head), NEXT_TOKEN(p_token_list_tail))
        {
            p_token_iterator = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);

            switch(p_token_iterator->info.token_type)
            {
                case C_TOKEN_PARENTHESIS_RIGHT:
                {
                    parenthesis_level++;
                    parenthesis_number++;
                    break;
                }

                case C_TOKEN_PARENTHESIS_LEFT:
                {
                    parenthesis_level--;
                    parenthesis_number++;
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

        /* not have a pair or pairing error */
        if(parenthesis_number == 0 || 
            (parenthesis_number != 0 && parenthesis_level != 0))
        {
            display_dcl_error(p_token_list_head, p_token_list_tail, "parenthesis pairing error");
            return RETURN_FAILURE;
        }

        /* the first token is '(' */
        if(p_token_iterator == p_token_list_head)
        {
            /* empty() */
            if(NEXT_TOKEN(p_token_list_head) == p_token_list_tail)
            {
                if(in_function_parameters)
                {
                    ret_val = proc_direct_dcl_list(NULL, NULL, in_function_parameters, need_type);
                    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                    ret_val = proc_function_parameters(p_token_iterator, p_token_list_tail, need_type);
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
                ret_val = proc_dcl_list(NEXT_TOKEN(p_token_list_head), PREV_TOKEN(p_token_list_tail),
                    in_function_parameters, need_type);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                return RETURN_SUCCESS;
            }
        }
        else
        {
            /* direct_dcl_list (function premeters) */
            ret_val = proc_direct_dcl_list(p_token_list_head, PREV_TOKEN(p_token_iterator), 
                in_function_parameters, need_type);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            
            ret_val = proc_function_parameters(p_token_iterator, p_token_list_tail, need_type);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            return RETURN_SUCCESS;
        }
    }

    /* array */
    ENUM_BOOLEAN has_identifier = BOOLEAN_FALSE;
    if(p_token_list_tail->info.token_type == C_TOKEN_BRACKET_RIGHT)
    {
        list_for_each_reverse(PREV_TOKEN(p_token_list_head),  p_token_list_tail)
        {
            p_token_iterator = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);
            
            if(p_token_iterator->info.token_type != C_TOKEN_BRACKET_LEFT
                &&p_token_iterator->info.token_type != C_TOKEN_BRACKET_RIGHT
                &&p_token_iterator->info.token_type != C_TOKEN_CONSTANT_INTEGER)
            {
                has_identifier = BOOLEAN_TRUE;
                break;
            }
        }

        /* direct_dcl_list only contain [][], the array identifier is not given */
        if(has_identifier == BOOLEAN_FALSE)
        {
            ret_val = proc_direct_dcl_list(NULL, NULL, in_function_parameters, need_type);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        }
        else
        {
            /* dcl_list [array size] */
            ret_val = proc_direct_dcl_list(p_token_list_head, p_token_iterator, in_function_parameters, need_type);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

            p_token_iterator = NEXT_TOKEN(p_token_iterator);
        }
        
        ret_val = proc_array_size(p_token_iterator, p_token_list_tail, need_type);
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
            display_dcl_error(p_token_list_head, p_token_list_tail, "head does not equals to tail!");
            return RETURN_FAILURE;
        }
    }

    display_dcl_error(p_token_list_head, p_token_list_tail, "missing identifier!");
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

    DCL_DBG_PRINT("input:", p_token_list_head, p_token_list_tail);
    
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

ENUM_RETURN s_cproc_dcl_statement(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_tail)
{
    R_ASSERT(p_token_list_head != NULL, RETURN_FAILURE);
    R_ASSERT(p_token_list_tail != NULL, RETURN_FAILURE);

    DCL_DBG_PRINT("input:", p_token_list_head, p_token_list_tail);

    ENUM_RETURN ret_val;
    ENUM_BOOLEAN need_type = BOOLEAN_FALSE;
    
    ret_val = proc_dcl_list(p_token_list_head, p_token_list_tail, 
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

ENUM_RETURN s_cdcl(_VOID)
{
    ENUM_RETURN ret_val;
    STRU_C_TOKEN_NODE *p_token_list_head = s_cproc_token_get_list_head();
    
    printf(LIGHT_CYAN"declaration: \n");
	s_ctoken_print_list(NULL, p_token_list_head, NEXT_TOKEN(p_token_list_head), PREV_TOKEN(p_token_list_head));
	printf(NONE"\n");
    
    //seperate statement by semicolon
    STRU_C_TOKEN_NODE *p_token_node_temp = NULL;
    STRU_C_TOKEN_NODE *p_token_list_head_temp = NULL;

    LIST_FOR_EACH_ALL(p_token_list_head)
    {
        p_token_node_temp = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);
        if(p_token_node_temp->info.token_type != C_TOKEN_SEMICOLON)
        {
            if(p_token_list_head_temp == NULL)
            {
                p_token_list_head_temp = p_token_node_temp;
            }
            continue;
        }

        if(p_token_list_head_temp == NULL)
        {
            continue;
        }
        
        ret_val = s_cproc_dcl_statement(p_token_list_head_temp, PREV_TOKEN(p_token_node_temp));
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        p_token_list_head_temp = NULL;
    }

    if(p_token_list_head_temp != NULL)
    {
        ret_val = s_cproc_dcl_statement(p_token_list_head_temp, PREV_TOKEN(p_token_node_temp));
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }
        

    return RETURN_SUCCESS;
}


