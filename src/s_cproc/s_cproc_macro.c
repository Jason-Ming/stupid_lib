#include <stdio.h>
#include <stdlib.h>

#include "s_log.h"
#include "s_mem.h"

#include "s_cproc.h"
#include "s_ctoken.h"
#include "s_cerror.h"
#include "s_cproc_stm.h"
#include "s_cproc_macro.h"
typedef struct TAG_STRU_MACRO
{
    STRU_C_TOKEN_NODE * p_name;
    size_t parameter_separator_num;
    ENUM_BOOLEAN parameter_part_exist;
    STRU_C_TOKEN_NODE *p_parameter_list_head;
    STRU_C_TOKEN_NODE *p_parameter_list_tail;
    STRU_C_TOKEN_NODE *p_replacement_list_head;
    STRU_C_TOKEN_NODE *p_replacement_list_tail;    
}STRU_MACRO;

typedef struct TAG_STRU_MACRO_NODE
{
    STRU_MACRO info;
    struct TAG_STRU_MACRO_NODE *next;
    struct TAG_STRU_MACRO_NODE *previous;
}STRU_MACRO_NODE;

PRIVATE STRU_MACRO_NODE *g_p_macro_node_list_head = NULL;
PRIVATE STRU_MACRO_NODE *g_p_macro_node_list_tail = NULL;

PRIVATE size_t s_cproc_macro_get_parameter_num(STRU_MACRO macro)
{
    STRU_C_TOKEN_NODE *p_parameter_list_head = macro.p_parameter_list_head;
    size_t parameter_num = 0;
    
    while(p_parameter_list_head != NULL)
    {
        parameter_num++;
        p_parameter_list_head = p_parameter_list_head->next;
    };

    return parameter_num;
}

PRIVATE ENUM_RETURN s_cproc_macro_add_node_to_list(
    STRU_MACRO_NODE *p_macro_node_to_be_added)
{
    S_R_ASSERT(p_macro_node_to_be_added != NULL, RETURN_FAILURE);

    if(g_p_macro_node_list_head == NULL)
    {
        R_ASSERT(g_p_macro_node_list_tail == NULL, RETURN_FAILURE);
        
        g_p_macro_node_list_head = p_macro_node_to_be_added;
        g_p_macro_node_list_tail = p_macro_node_to_be_added;
    }
    else
    {
        R_ASSERT(g_p_macro_node_list_tail != NULL, RETURN_FAILURE);

        p_macro_node_to_be_added->previous = g_p_macro_node_list_tail;
        g_p_macro_node_list_tail->next = p_macro_node_to_be_added;
        g_p_macro_node_list_tail = p_macro_node_to_be_added;
    }
    
    return RETURN_SUCCESS;
};

PRIVATE ENUM_RETURN s_cproc_macro_delete_node_from_list(
    STRU_MACRO_NODE *p_macro_node_to_be_deleted)
{
    S_R_ASSERT(p_macro_node_to_be_deleted != NULL, RETURN_FAILURE);
    S_R_ASSERT(g_p_macro_node_list_head != NULL, RETURN_FAILURE);
    S_R_ASSERT(g_p_macro_node_list_tail != NULL, RETURN_FAILURE);

    /* the macro to be deleted is the list head and list tail */
    if(g_p_macro_node_list_head == p_macro_node_to_be_deleted 
        && g_p_macro_node_list_tail == p_macro_node_to_be_deleted)
    {
        g_p_macro_node_list_head = NULL;
        g_p_macro_node_list_tail = NULL;
    }
    /* the macro to be deleted is the list head */
    else if(g_p_macro_node_list_head == p_macro_node_to_be_deleted)
    {
        g_p_macro_node_list_head = g_p_macro_node_list_head->next;
        g_p_macro_node_list_head->previous = NULL;
    }
    /* the token to be deleted is the list tail */
    else if(g_p_macro_node_list_tail == p_macro_node_to_be_deleted)
    {
        g_p_macro_node_list_tail = g_p_macro_node_list_tail->previous;
        g_p_macro_node_list_tail->next = NULL;
    }
    else
    {
        p_macro_node_to_be_deleted->previous->next = p_macro_node_to_be_deleted->next;
        p_macro_node_to_be_deleted->next->previous = p_macro_node_to_be_deleted->previous;
    }
    
    ENUM_RETURN ret_val;
    ret_val = release_token_list(&(p_macro_node_to_be_deleted->info.p_name), &(p_macro_node_to_be_deleted->info.p_name));
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    ret_val = release_token_list(&(p_macro_node_to_be_deleted->info.p_parameter_list_head), &(p_macro_node_to_be_deleted->info.p_parameter_list_tail));
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    ret_val = release_token_list(&(p_macro_node_to_be_deleted->info.p_replacement_list_head), &(p_macro_node_to_be_deleted->info.p_replacement_list_tail));
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    FREE(p_macro_node_to_be_deleted);
    return RETURN_SUCCESS;
};

ENUM_RETURN s_cproc_macro_release_list(_VOID)
{
    STRU_MACRO_NODE *p_macro_list_head = g_p_macro_node_list_head;
    STRU_MACRO_NODE *p_macro_temp;
    ENUM_RETURN ret_val;
    while(p_macro_list_head != NULL)
    {
        p_macro_temp = p_macro_list_head;
        ret_val = release_token_list(&(p_macro_temp->info.p_name), &(p_macro_temp->info.p_name));
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        ret_val = release_token_list(&(p_macro_temp->info.p_parameter_list_head), &(p_macro_temp->info.p_parameter_list_tail));
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        ret_val = release_token_list(&(p_macro_temp->info.p_replacement_list_head), &(p_macro_temp->info.p_replacement_list_tail));
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

        p_macro_list_head = p_macro_list_head->next;
        FREE(p_macro_temp);
    }

    g_p_macro_node_list_head = NULL;
    g_p_macro_node_list_tail = NULL;
    
    return RETURN_SUCCESS;
}

_VOID s_cproc_macro_print_list_debug(_VOID)
{
	STRU_MACRO_NODE *p_macro_list_head = g_p_macro_node_list_head;

	printf("macro list:\n");
	while(p_macro_list_head != NULL)
	{
		printf(LIGHT_BLUE"%s"NONE, p_macro_list_head->info.p_name->info.p_string);
		if(p_macro_list_head->info.parameter_part_exist == BOOLEAN_TRUE)
        {
            printf(LIGHT_BLUE"("NONE);
            STRU_C_TOKEN_NODE *p_parameter = p_macro_list_head->info.p_parameter_list_head;
            ENUM_BOOLEAN first_parameter = BOOLEAN_TRUE;
            while(p_parameter != NULL)
            {
                printf(LIGHT_BLUE"%s%s"NONE, first_parameter?"":", ", p_parameter->info.p_string);
                first_parameter = BOOLEAN_FALSE;
                p_parameter = p_parameter->next;
            };

            printf(LIGHT_BLUE")"NONE);
        }

        printf(LIGHT_BLUE": "NONE);

        STRU_C_TOKEN_NODE *p_replacement = p_macro_list_head->info.p_replacement_list_head;
        while(p_replacement != NULL)
        {
            printf(LIGHT_BLUE"%s"NONE, p_replacement->info.p_string);
            p_replacement = p_replacement->next;
        };
		printf("\n");
		p_macro_list_head = p_macro_list_head->next;
	}

	printf("\n");

}

PRIVATE ENUM_RETURN s_cproc_macro_get_by_name(
    const _S8* p_macro_name, 
    STRU_MACRO_NODE **pp_macro_node)
{
    S_R_ASSERT(p_macro_name != NULL, RETURN_FAILURE);
    S_R_ASSERT(pp_macro_node != NULL, RETURN_FAILURE);
    *pp_macro_node = NULL;
    
     STRU_MACRO_NODE *p_macro_node = g_p_macro_node_list_head;
     while(p_macro_node != NULL)
     {
        S_R_ASSERT(p_macro_node->info.p_name != NULL, RETURN_FAILURE);
        S_R_ASSERT(p_macro_node->info.p_name->info.p_string != NULL, RETURN_FAILURE);
        if( strcmp(p_macro_node->info.p_name->info.p_string, p_macro_name) == 0)
        {
            *pp_macro_node = p_macro_node;
            return RETURN_SUCCESS;
        }
        p_macro_node = p_macro_node->next;
     }

     return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_cproc_macro_make_new(STRU_C_TOKEN_NODE* p_macro_token_node, STRU_MACRO_NODE **pp_macro_node)
{
    S_R_ASSERT(pp_macro_node != NULL, RETURN_FAILURE);

    STRU_MACRO_NODE *p_macro_node_temp = NULL;
    p_macro_node_temp = (STRU_MACRO_NODE*)malloc(sizeof(STRU_MACRO_NODE));
    S_R_ASSERT(p_macro_node_temp != NULL, RETURN_FAILURE);

    p_macro_node_temp->next = NULL;
    p_macro_node_temp->previous = NULL;
    p_macro_node_temp->info.p_name = p_macro_token_node;
    p_macro_node_temp->info.parameter_separator_num = 0;
    p_macro_node_temp->info.parameter_part_exist = BOOLEAN_FALSE;
    p_macro_node_temp->info.p_parameter_list_head = NULL;
    p_macro_node_temp->info.p_parameter_list_tail = NULL;
    p_macro_node_temp->info.p_replacement_list_head = NULL;
    p_macro_node_temp->info.p_replacement_list_tail = NULL;

    *pp_macro_node = p_macro_node_temp;
    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_macro_name_exist(_S8 *p_macro_name, ENUM_BOOLEAN *exist)
{
    S_R_ASSERT(p_macro_name != NULL, RETURN_FAILURE);
    S_R_ASSERT(exist != NULL, RETURN_FAILURE);
    *exist = BOOLEAN_FALSE;

    /* get macro node by name */
    STRU_MACRO_NODE *p_macro_node = NULL;
    ENUM_RETURN ret_val;

    ret_val = s_cproc_macro_get_by_name(p_macro_name, &p_macro_node);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    S_R_FALSE(p_macro_node != NULL, RETURN_SUCCESS);

    *exist = BOOLEAN_TRUE;
    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_macro_add_name(
    const STRU_C_TOKEN_NODE* p_macro_name_token_node)
{
    S_R_ASSERT(p_macro_name_token_node != NULL, RETURN_FAILURE);

    /* get macro node by name */
    STRU_MACRO_NODE *p_macro_node = NULL;
    ENUM_RETURN ret_val;

    ret_val = s_cproc_macro_get_by_name(p_macro_name_token_node->info.p_string, &p_macro_node);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    if(p_macro_node != NULL)
    {
        ret_val = s_cproc_macro_delete_node_from_list(p_macro_node);
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }

    STRU_C_TOKEN_NODE *p_macro_token_node_temp = NULL;
    ret_val = s_cproc_token_copy(p_macro_name_token_node, &p_macro_token_node_temp);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    S_R_ASSERT(p_macro_token_node_temp != NULL, RETURN_FAILURE);

    ret_val = s_cproc_macro_make_new(p_macro_token_node_temp, &p_macro_node);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    S_R_ASSERT(p_macro_node != NULL, RETURN_FAILURE);

    ret_val = s_cproc_macro_add_node_to_list(p_macro_node);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_macro_remove_name(
    const STRU_C_TOKEN_NODE* p_macro_name_token_node)
{
    S_R_ASSERT(p_macro_name_token_node != NULL, RETURN_FAILURE);

    /* get macro node by name */
    STRU_MACRO_NODE *p_macro_node = NULL;
    ENUM_RETURN ret_val;

    ret_val = s_cproc_macro_get_by_name(p_macro_name_token_node->info.p_string, &p_macro_node);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    S_R_FALSE(p_macro_node != NULL, RETURN_SUCCESS);

    s_cproc_macro_delete_node_from_list(p_macro_node);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_macro_add_parameter_separator_comma(ENUM_RETURN *result)
{
    S_R_ASSERT(result != NULL, RETURN_FAILURE);
    *result = RETURN_FAILURE;
    
    STRU_MACRO_NODE *p_macro_node = g_p_macro_node_list_tail;
    S_R_ASSERT(p_macro_node != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_macro_node->info.p_name != NULL, RETURN_FAILURE);
    size_t parameter_num = s_cproc_macro_get_parameter_num(p_macro_node->info);

    (p_macro_node->info.parameter_separator_num)++;

    if(parameter_num == p_macro_node->info.parameter_separator_num)
    {
        *result = RETURN_SUCCESS;
    }
    
    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_macro_add_parameter(
    const STRU_C_TOKEN_NODE* p_macro_parameter_token_node, 
    ENUM_RETURN *result)
{
    S_R_ASSERT(p_macro_parameter_token_node != NULL, RETURN_FAILURE);
    S_R_ASSERT(result != NULL, RETURN_FAILURE);
    *result = RETURN_FAILURE;
    
    /* get macro node by name */
    STRU_MACRO_NODE *p_macro_node = g_p_macro_node_list_tail;
    ENUM_RETURN ret_val;

    S_R_ASSERT(p_macro_node != NULL, RETURN_FAILURE);

    if(p_macro_node->info.parameter_separator_num != s_cproc_macro_get_parameter_num(p_macro_node->info))
    {
        return RETURN_SUCCESS;
    }

    STRU_C_TOKEN_NODE *p_new_token_node = NULL;
    ret_val = s_cproc_token_copy(p_macro_parameter_token_node, &p_new_token_node);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    S_R_ASSERT(p_new_token_node != NULL, RETURN_FAILURE);
    
    ret_val = s_cproc_token_add_node_to_list(
        p_new_token_node,
        &(p_macro_node->info.p_parameter_list_head),
        &(p_macro_node->info.p_parameter_list_tail));
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    *result = RETURN_SUCCESS;

    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_macro_finish_parameter(ENUM_RETURN *result)
{
    S_R_ASSERT(result != NULL, RETURN_FAILURE);

    *result = RETURN_FAILURE;
    STRU_MACRO_NODE *p_macro_node = g_p_macro_node_list_tail;

    S_R_ASSERT(p_macro_node != NULL, RETURN_FAILURE);

    DEBUG_PRINT("separator_num: %zd, parameter_num: %zd\n", 
        p_macro_node->info.parameter_separator_num, 
        s_cproc_macro_get_parameter_num(p_macro_node->info));
    size_t separator_num = p_macro_node->info.parameter_separator_num;
    size_t parameter_num = s_cproc_macro_get_parameter_num(p_macro_node->info);
    
    if((separator_num == 0 && parameter_num <= 1)
        ||(separator_num > 0 && (separator_num == (parameter_num - 1))))
    {
        *result = RETURN_SUCCESS;
    }

    p_macro_node->info.parameter_part_exist = BOOLEAN_TRUE;

    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_macro_add_replacement(
    const STRU_C_TOKEN_NODE* p_macro_replacement_token_node)
{
    S_R_ASSERT(p_macro_replacement_token_node != NULL, RETURN_FAILURE);

    STRU_MACRO_NODE *p_macro_node = g_p_macro_node_list_tail;
    S_R_ASSERT(p_macro_node != NULL, RETURN_FAILURE);
    ENUM_RETURN ret_val;

    STRU_C_TOKEN_NODE *p_new_token_node = NULL;
    ret_val = s_cproc_token_copy(p_macro_replacement_token_node, &p_new_token_node);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    S_R_ASSERT(p_new_token_node != NULL, RETURN_FAILURE);
    
    ret_val = s_cproc_token_add_node_to_list(
        p_new_token_node,
        &(p_macro_node->info.p_replacement_list_head),
        &(p_macro_node->info.p_replacement_list_tail));
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

ENUM_BOOLEAN s_cproc_macro_identifier_in_parameter_list(const STRU_C_TOKEN_NODE *p_replacement_token)
{
    S_R_ASSERT(p_replacement_token != NULL, BOOLEAN_FALSE);
    S_R_FALSE(p_replacement_token->info.token_type == C_TOKEN_PP_IDENTIFIER, BOOLEAN_FALSE);

    STRU_MACRO_NODE *p_macro_node = g_p_macro_node_list_tail;
    S_R_ASSERT(p_macro_node != NULL, BOOLEAN_FALSE);

    STRU_C_TOKEN_NODE *p_parameter_token = p_macro_node->info.p_parameter_list_head;

    while(p_parameter_token != NULL)
    {
        if(strcmp(p_parameter_token->info.p_string, p_replacement_token->info.p_string) == 0)
        {
            return BOOLEAN_TRUE;
        }
        p_parameter_token = p_parameter_token->next;
    }
    
    return BOOLEAN_FALSE;
}

ENUM_BOOLEAN s_cproc_macro_parameter_part_exist()
{
    STRU_MACRO_NODE *p_macro_node = g_p_macro_node_list_tail;
    S_R_ASSERT(p_macro_node != NULL, BOOLEAN_FALSE);
    
    return p_macro_node->info.parameter_part_exist;
}

ENUM_RETURN s_cproc_macro_finish_replacement()
{
    ENUM_RETURN ret_val;
    STRU_MACRO_NODE *p_macro_node = g_p_macro_node_list_tail;
    S_R_ASSERT(p_macro_node != NULL, RETURN_FAILURE);

    //delete blanks at the begining and end 
    STRU_C_TOKEN_NODE *p_replacement_token_temp;
    STRU_C_TOKEN_NODE *p_replacement_token;

    p_replacement_token = p_macro_node->info.p_replacement_list_head;
    while(p_replacement_token != NULL)
    {
        p_replacement_token_temp = p_replacement_token->next;
        if(p_replacement_token->info.token_type == C_TOKEN_BLANK)
        {
            ret_val = delete_token_from_list(p_replacement_token, 
                &(p_macro_node->info.p_replacement_list_head),
                &(p_macro_node->info.p_replacement_list_tail));
            S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        }
        else
        {
            break;
        }
        p_replacement_token = p_replacement_token_temp;
    };

    p_replacement_token = p_macro_node->info.p_replacement_list_tail;
    while(p_replacement_token != NULL)
    {
        p_replacement_token_temp = p_replacement_token->previous;
        if(p_replacement_token->info.token_type == C_TOKEN_BLANK)
        {
            ret_val = delete_token_from_list(p_replacement_token, 
                &(p_macro_node->info.p_replacement_list_head),
                &(p_macro_node->info.p_replacement_list_tail));
            S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        }
        else
        {
            break;
        }
        p_replacement_token = p_replacement_token_temp;
    };
    
    //'#' is not followed by a macro parameter
    p_replacement_token = p_macro_node->info.p_replacement_list_head;
    ENUM_BOOLEAN last_stingification = BOOLEAN_FALSE;
    while(p_replacement_token != NULL)
    {
        p_replacement_token_temp = p_replacement_token->next;
        if(last_stingification == BOOLEAN_TRUE)
        {
            if(p_replacement_token->info.token_type != C_TOKEN_BLANK
                && p_macro_node->info.parameter_part_exist
                && !s_cproc_macro_identifier_in_parameter_list(p_replacement_token))
            {
                CPROC_GEN_ERROR(p_replacement_token, "'#' is not followed by a macro parameter");
                return RETURN_SUCCESS;
            }
        }
        
        if(p_replacement_token->info.token_type == C_TOKEN_PPD_STRINGIFICATION)
        {
            last_stingification = BOOLEAN_TRUE;
        }
        else
        {
            last_stingification = BOOLEAN_FALSE;
        }
        
        p_replacement_token = p_replacement_token_temp;
    };

    //'##' cannot appear at either end of a macro expansion
    p_replacement_token = p_macro_node->info.p_replacement_list_head;
    if(p_replacement_token->info.token_type == C_TOKEN_PPD_CONCATENATE)
    {
        CPROC_GEN_ERROR(p_replacement_token, "'##' cannot appear at either end of a macro expansion");
        return RETURN_SUCCESS;
    }
    p_replacement_token = p_macro_node->info.p_replacement_list_tail;
    if(p_replacement_token->info.token_type == C_TOKEN_PPD_CONCATENATE)
    {
        CPROC_GEN_ERROR(p_replacement_token, "'##' cannot appear at either end of a macro expansion");
        return RETURN_SUCCESS;
    }

    return RETURN_SUCCESS;
}

_VOID s_cproc_macro_expand(_VOID)
{
    //pasting """" and """" does not give a valid preprocessing token
}

