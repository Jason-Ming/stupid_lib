#include <stdio.h>
#include <stdlib.h>

#include "s_log.h"
#include "s_mem.h"
#include "s_list.h"

#include "s_cproc.h"
#include "s_ctoken.h"
#include "s_cerror.h"
#include "s_cproc_stm.h"
#include "s_cproc_token.h"

#include "s_cproc_macro.h"
typedef struct TAG_STRU_MACRO
{
    STRU_C_TOKEN_NODE name_head;
    size_t parameter_separator_num;
    ENUM_BOOLEAN parameter_part_exist;
    STRU_C_TOKEN_NODE parameter_list_head;
    STRU_C_TOKEN_NODE replacement_list_head;
}STRU_MACRO;

typedef struct TAG_STRU_MACRO_NODE
{
    STRU_MACRO info;
    struct list_head list;
}STRU_MACRO_NODE;

PRIVATE STRU_MACRO_NODE g_macro_node_list_head;


_VOID s_cproc_macro_print_list_debug_info(_VOID)
{

	STRU_MACRO_NODE *p_macro_temp;
    struct list_head *pos_macro;
    printf("\n\n--MACRO LIST DEBUG INFO BEGIN------------------------------------------------------\n");
    list_for_each_all(pos_macro, &g_macro_node_list_head.list)
	{
        p_macro_temp = list_entry(pos_macro, STRU_MACRO_NODE, list);

        STRU_C_TOKEN_NODE *p_token_temp;
        struct list_head *pos_token;

        list_for_each_all(pos_token, &p_macro_temp->info.name_head.list)
        {
            p_token_temp = list_entry(pos_token, STRU_C_TOKEN_NODE, list);
		    printf(LIGHT_BLUE"%s"NONE, p_token_temp->info.p_string);
        }
        
		if(p_macro_temp->info.parameter_part_exist == BOOLEAN_TRUE)
        {
            printf(LIGHT_BLUE"("NONE);
            ENUM_BOOLEAN first_parameter = BOOLEAN_TRUE;
            list_for_each_all(pos_token, &p_macro_temp->info.parameter_list_head.list)
            {
                p_token_temp = list_entry(pos_token, STRU_C_TOKEN_NODE, list);
    		    printf(LIGHT_BLUE"%s%s"NONE, first_parameter?"":", ", p_token_temp->info.p_string);
                first_parameter = BOOLEAN_FALSE;
            }
            printf(LIGHT_BLUE")"NONE);
        }

        printf(LIGHT_BLUE": "NONE);

        list_for_each_all(pos_token, &p_macro_temp->info.replacement_list_head.list)
        {
            p_token_temp = list_entry(pos_token, STRU_C_TOKEN_NODE, list);
            printf(LIGHT_BLUE"%s"NONE, p_token_temp->info.p_string);
        };
		printf("\n");
	}

    printf("--MACRO LIST DEBUG INFO END--------------------------------------------------------\n");

}

_VOID s_cproc_macro_list_init(_VOID)
{
    INIT_LIST_HEAD(&g_macro_node_list_head.list);
    s_ctoken_init_head(&g_macro_node_list_head.info.name_head, "macro-name-head");
    s_ctoken_init_head(&g_macro_node_list_head.info.parameter_list_head, "macro-parameter-head");
    s_ctoken_init_head(&g_macro_node_list_head.info.replacement_list_head, "macro-replacement_head");
}

PRIVATE size_t s_cproc_macro_get_parameter_num(STRU_MACRO *macro)
{
    size_t parameter_num = 0;
    struct list_head *pos;
    DEBUG_PRINT("in...");
    list_for_each_all(pos, &macro->parameter_list_head.list)
    {
        //DEBUG_PRINT("%s", list_entry(pos, STRU_C_TOKEN_NODE, list)->info.p_string);
        parameter_num++;
    };
    DEBUG_PRINT("out...");

    return parameter_num;
}

PRIVATE ENUM_RETURN s_cproc_macro_get_by_name(
    const _S8* p_macro_name, 
    STRU_MACRO_NODE **pp_macro_node)
{
    S_R_ASSERT(p_macro_name != NULL, RETURN_FAILURE);
    S_R_ASSERT(pp_macro_node != NULL, RETURN_FAILURE);
    *pp_macro_node = NULL;
    
    STRU_MACRO_NODE *p_macro_temp;
    struct list_head *pos_macro;
    
    list_for_each_all(pos_macro, &g_macro_node_list_head.list)
	{
        p_macro_temp = list_entry(pos_macro, STRU_MACRO_NODE, list);

        STRU_C_TOKEN_NODE *p_token_temp;
        struct list_head *pos_token;

        list_for_each_all(pos_token, &p_macro_temp->info.name_head.list)
        {
            p_token_temp = list_entry(pos_token, STRU_C_TOKEN_NODE, list);

            S_R_ASSERT(p_token_temp->info.p_string != NULL, RETURN_FAILURE);
            if( strcmp(p_token_temp->info.p_string, p_macro_name) == 0)
            {
                *pp_macro_node = p_macro_temp;
                return RETURN_SUCCESS;
            }
        }
    }

    return RETURN_SUCCESS;
}
    
ENUM_RETURN s_cproc_macro_get_parameter_list(_S8 *p_macro_name, STRU_C_TOKEN_NODE **pp_parameter_list_head)
{
    S_R_ASSERT(p_macro_name != NULL, RETURN_FAILURE);
    S_R_ASSERT(pp_parameter_list_head != NULL, RETURN_FAILURE);
    *pp_parameter_list_head = NULL;

    /* get macro node by name */
    STRU_MACRO_NODE *p_macro_node = NULL;
    ENUM_RETURN ret_val;

    ret_val = s_cproc_macro_get_by_name(p_macro_name, &p_macro_node);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    S_R_ASSERT(p_macro_node != NULL, RETURN_SUCCESS);

    if(p_macro_node->info.parameter_part_exist)
    {
        *pp_parameter_list_head = &p_macro_node->info.parameter_list_head;
    }
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_cproc_macro_get_replacement_list(STRU_MACRO_NODE *p_macro_node, STRU_C_TOKEN_NODE **pp_replacement_token_list_head)
{
    S_R_ASSERT(p_macro_node != NULL, RETURN_FAILURE);
    S_R_ASSERT(pp_replacement_token_list_head != NULL, RETURN_FAILURE);
    *pp_replacement_token_list_head = NULL;

    *pp_replacement_token_list_head = &p_macro_node->info.replacement_list_head;
    return RETURN_SUCCESS;
}


ENUM_RETURN s_cproc_macro_move_replacement_list_from_token_list(
    STRU_C_TOKEN_NODE *p_dest_token_list_head)
{
    
    STRU_C_TOKEN_NODE *p_token_replacement_list_head = s_cproc_token_get_last_node_by_type(C_TOKEN_PP_PARAMETER_FINISH);
    if(p_token_replacement_list_head == NULL)
    {
        p_token_replacement_list_head = s_cproc_token_get_last_node_by_type(C_TOKEN_PP_MACRO);
    }
    S_R_ASSERT(p_token_replacement_list_head != NULL, RETURN_FAILURE);
    
    return s_ctoken_move_list_to_another_list_tail(s_cproc_token_get_list_head(), NEXT_TOKEN(p_token_replacement_list_head), 
        PREV_TOKEN(s_cproc_token_get_list_head()),p_dest_token_list_head);
}

ENUM_BOOLEAN s_cproc_macro_va_in_parameter_list(_VOID)
{
    STRU_MACRO_NODE *p_macro_node = list_entry(g_macro_node_list_head.list.prev, STRU_MACRO_NODE, list);
    S_R_ASSERT(p_macro_node != &g_macro_node_list_head, BOOLEAN_FALSE);

    STRU_C_TOKEN_NODE *p_parameter_token;
    struct list_head *pos;
    list_for_each_all(pos, &p_macro_node->info.parameter_list_head.list)
    {
        p_parameter_token = list_entry(pos, STRU_C_TOKEN_NODE, list);
        if(p_parameter_token->info.token_type == C_TOKEN_PP_PARAMETER_VA)
        {
            return BOOLEAN_TRUE;
        }
    }
    
    return BOOLEAN_FALSE;
}

ENUM_BOOLEAN s_cproc_macro_identifier_name_in_parameter_list(const _S8 *p_identifier)
{
    S_R_ASSERT(p_identifier != NULL, BOOLEAN_FALSE);

    STRU_MACRO_NODE *p_macro_node = list_entry(g_macro_node_list_head.list.prev, STRU_MACRO_NODE, list);
    S_R_ASSERT(p_macro_node != &g_macro_node_list_head, BOOLEAN_FALSE);

    STRU_C_TOKEN_NODE *p_parameter_token;
    struct list_head *pos;
    list_for_each_all(pos, &p_macro_node->info.parameter_list_head.list)
    {
        p_parameter_token = list_entry(pos, STRU_C_TOKEN_NODE, list);
        if(strcmp(p_parameter_token->info.p_string, p_identifier) == 0)
        {
            return BOOLEAN_TRUE;
        }
    }
    
    return BOOLEAN_FALSE;
}

ENUM_BOOLEAN s_cproc_macro_replacement_identifier_in_parameter_list(const STRU_C_TOKEN_NODE *p_replacement_token)
{
    S_R_ASSERT(p_replacement_token != NULL, BOOLEAN_FALSE);
    S_R_FALSE(p_replacement_token->info.token_type == C_TOKEN_PP_IDENTIFIER 
        || p_replacement_token->info.token_type == C_TOKEN_PP_IDENTIFIER_VA, BOOLEAN_FALSE);

    if(p_replacement_token->info.token_type == C_TOKEN_PP_IDENTIFIER_VA)
    {
        if(s_cproc_macro_va_in_parameter_list() == BOOLEAN_FALSE)
        {
            return BOOLEAN_FALSE;
        }
        else
        {
            return BOOLEAN_TRUE;
        }
    }

    STRU_MACRO_NODE *p_macro_node = list_entry(g_macro_node_list_head.list.prev, STRU_MACRO_NODE, list);
    S_R_ASSERT(p_macro_node != &g_macro_node_list_head, BOOLEAN_FALSE);

    STRU_C_TOKEN_NODE *p_parameter_token;
    struct list_head *pos;
    list_for_each_all(pos, &p_macro_node->info.parameter_list_head.list)
    {
        p_parameter_token = list_entry(pos, STRU_C_TOKEN_NODE, list);
        DEBUG_PRINT(TOKEN_INFO_FORMAT, TOKEN_INFO_VALUE(p_parameter_token));
        
        if(p_parameter_token->info.token_type == C_TOKEN_PP_PARAMETER_ID
            && strcmp(p_parameter_token->info.p_string, p_replacement_token->info.p_string) == 0)
        {
            return BOOLEAN_TRUE;
        }
        else if(p_parameter_token->info.token_type == C_TOKEN_PP_PARAMETER_ID_VA
            && memcmp(p_parameter_token->info.p_string, p_replacement_token->info.p_string, strlen(p_replacement_token->info.p_string)) == 0)
        {
            return BOOLEAN_TRUE;
        }
    }
    
    return BOOLEAN_FALSE;
}


ENUM_BOOLEAN s_cproc_macro_parameter_identifier_in_parameter_list(const STRU_C_TOKEN_NODE *p_parameter_token)
{
    S_R_ASSERT(p_parameter_token != NULL, BOOLEAN_FALSE);
    S_R_FALSE(p_parameter_token->info.token_type == C_TOKEN_PP_PARAMETER_ID
        || p_parameter_token->info.token_type == C_TOKEN_PP_PARAMETER_ID_VA, BOOLEAN_FALSE);

    STRU_MACRO_NODE *p_macro_node = list_entry(g_macro_node_list_head.list.prev, STRU_MACRO_NODE, list);
    S_R_ASSERT(p_macro_node != &g_macro_node_list_head, BOOLEAN_FALSE);

    STRU_C_TOKEN_NODE *p_parameter_token_loop;
    struct list_head *pos;
    list_for_each_all(pos, &p_macro_node->info.parameter_list_head.list)
    {
        p_parameter_token_loop = list_entry(pos, STRU_C_TOKEN_NODE, list);
        DEBUG_PRINT(TOKEN_INFO_FORMAT, TOKEN_INFO_VALUE(p_parameter_token_loop));

        S_R_ASSERT(p_parameter_token_loop->info.token_type == C_TOKEN_PP_PARAMETER_ID, BOOLEAN_FALSE);
        
        if(p_parameter_token->info.token_type == C_TOKEN_PP_PARAMETER_ID
            && strcmp(p_parameter_token->info.p_string, p_parameter_token_loop->info.p_string) == 0)
        {
            return BOOLEAN_TRUE;
        }
        else if(p_parameter_token->info.token_type == C_TOKEN_PP_PARAMETER_ID_VA
            && memcmp(p_parameter_token->info.p_string, p_parameter_token_loop->info.p_string, strlen(p_parameter_token_loop->info.p_string)) == 0)
        {
            return BOOLEAN_TRUE;
        }
    }
    
    return BOOLEAN_FALSE;
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

ENUM_BOOLEAN s_cproc_macro_parameter_part_exist()
{
    STRU_MACRO_NODE *p_macro_node = list_entry(g_macro_node_list_head.list.prev, STRU_MACRO_NODE, list);
    S_R_ASSERT(p_macro_node != &g_macro_node_list_head, BOOLEAN_FALSE);
    
    return p_macro_node->info.parameter_part_exist;
}

PRIVATE ENUM_RETURN s_cproc_macro_add_node_to_list(
    STRU_MACRO_NODE *p_macro_node_to_be_added)
{
    S_R_ASSERT(p_macro_node_to_be_added != NULL, RETURN_FAILURE);

    list_add_tail(&(p_macro_node_to_be_added->list), &(g_macro_node_list_head.list));

    return RETURN_SUCCESS;
};

PRIVATE _VOID s_cproc_macro_delete_node(STRU_MACRO_NODE *p_macro_node_to_be_deleted)
{
    S_V_FALSE(p_macro_node_to_be_deleted != NULL);
    s_ctoken_release_list(&(p_macro_node_to_be_deleted->info.name_head));
    s_ctoken_release_list(&(p_macro_node_to_be_deleted->info.parameter_list_head));
    s_ctoken_release_list(&(p_macro_node_to_be_deleted->info.replacement_list_head));

    S_FREE(p_macro_node_to_be_deleted);
}
PRIVATE ENUM_RETURN s_cproc_macro_delete_node_from_list(
    STRU_MACRO_NODE *p_macro_node_to_be_deleted)
{
    S_R_ASSERT(p_macro_node_to_be_deleted != NULL, RETURN_FAILURE);
    list_del_init(&p_macro_node_to_be_deleted->list);

    s_cproc_macro_delete_node(p_macro_node_to_be_deleted);

    return RETURN_SUCCESS;
};

_VOID s_cproc_macro_release_list(_VOID)
{
    STRU_MACRO_NODE *p_macro_temp;
    struct list_head *pos, *next;
    
    list_for_each_all_safe(pos, next, &g_macro_node_list_head.list)
    {
        p_macro_temp = list_entry(pos, STRU_MACRO_NODE, list);
        s_cproc_macro_delete_node_from_list(p_macro_temp);
    }
}

PRIVATE ENUM_RETURN s_cproc_macro_make_new(
    STRU_C_TOKEN_NODE* p_macro_token_node, 
    STRU_MACRO_NODE **pp_macro_node)
{
    S_R_ASSERT(pp_macro_node != NULL, RETURN_FAILURE);

    STRU_MACRO_NODE *p_macro_node_temp = NULL;
    p_macro_node_temp = (STRU_MACRO_NODE*)malloc(sizeof(STRU_MACRO_NODE));
    S_R_ASSERT(p_macro_node_temp != NULL, RETURN_FAILURE);

    p_macro_node_temp->info.parameter_separator_num = 0;
    p_macro_node_temp->info.parameter_part_exist = BOOLEAN_FALSE;
    INIT_LIST_HEAD(&p_macro_node_temp->list);

    s_ctoken_init_head(&p_macro_node_temp->info.name_head, "macro-name-head");
    s_ctoken_init_head(&p_macro_node_temp->info.parameter_list_head, "macro-parameter-head");
    s_ctoken_init_head(&p_macro_node_temp->info.replacement_list_head, "macro-replacement_head");

    s_ctoken_add_node_to_list_tail(&p_macro_node_temp->info.name_head,
        p_macro_token_node);
    *pp_macro_node = p_macro_node_temp;
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

    STRU_C_TOKEN_NODE *p_token_node_new = NULL;
    ret_val = s_ctoken_duplicate_node(p_macro_name_token_node, &p_token_node_new);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    S_R_ASSERT(p_token_node_new != NULL, RETURN_FAILURE);

    ret_val = s_cproc_macro_make_new(p_token_node_new, &p_macro_node);
    S_R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, s_ctoken_free_node(p_token_node_new));
    S_R_ASSERT_DO(p_macro_node != NULL, RETURN_FAILURE, s_ctoken_free_node(p_token_node_new));

    ret_val = s_cproc_macro_add_node_to_list(p_macro_node);
    S_R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, 
        s_cproc_macro_delete_node(p_macro_node);s_ctoken_free_node(p_token_node_new));

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

ENUM_RETURN s_cproc_macro_add_parameter_separator_comma(
    ENUM_RETURN *result)
{
    S_R_ASSERT(result != NULL, RETURN_FAILURE);
    *result = RETURN_FAILURE;
    
    STRU_MACRO_NODE *p_macro_node = 
        list_entry(g_macro_node_list_head.list.prev, STRU_MACRO_NODE, list);
    S_R_ASSERT(p_macro_node != &g_macro_node_list_head, RETURN_FAILURE);
    S_R_ASSERT(p_macro_node->info.name_head.list.next != &p_macro_node->info.name_head.list, RETURN_FAILURE);
    size_t parameter_num = s_cproc_macro_get_parameter_num(&p_macro_node->info);

    (p_macro_node->info.parameter_separator_num)++;

    DEBUG_PRINT("parameter_num = %zd, parameter_separator_num = %zd", parameter_num, 
        p_macro_node->info.parameter_separator_num);
    
    if(parameter_num == p_macro_node->info.parameter_separator_num)
    {
        *result = RETURN_SUCCESS;
    }
    
    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_macro_add_parameter(
    STRU_C_TOKEN_NODE* p_macro_parameter_token_node)
{
    S_R_ASSERT(p_macro_parameter_token_node != NULL, RETURN_FAILURE);
    
    /* get last macro node */
    STRU_MACRO_NODE *p_last_macro_node = list_entry(g_macro_node_list_head.list.prev, STRU_MACRO_NODE, list);
    ENUM_RETURN ret_val;

    S_R_ASSERT(p_last_macro_node != &g_macro_node_list_head, RETURN_FAILURE);
    DEBUG_PRINT("macro name: %s", 
        list_entry(p_last_macro_node->info.name_head.list.next, STRU_C_TOKEN_NODE, list)->info.p_string);
        
    if(p_last_macro_node->info.parameter_separator_num != s_cproc_macro_get_parameter_num(&p_last_macro_node->info))
    {
        S_CPROC_STM_GEN_ERROR(&p_macro_parameter_token_node->info.c_position, "macro parameters must be comma-separated");
        return RETURN_SUCCESS;
    }

    if(s_cproc_macro_parameter_identifier_in_parameter_list(p_macro_parameter_token_node))
    {
        S_CPROC_STM_GEN_ERROR(&p_macro_parameter_token_node->info.c_position, "duplicate macro parameter \"%s\"", 
            p_macro_parameter_token_node->info.p_string);
        return RETURN_SUCCESS;
    }
    
    STRU_C_TOKEN_NODE *p_token_node_new = NULL;
    ret_val = s_ctoken_duplicate_node(p_macro_parameter_token_node, &p_token_node_new);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    S_R_ASSERT(p_token_node_new != NULL, RETURN_FAILURE);
    
    ret_val = s_ctoken_add_node_to_list_tail(        
        &(p_last_macro_node->info.parameter_list_head),
        p_token_node_new);
    S_R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, s_ctoken_free_node(p_token_node_new));

    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_macro_finish_parameter(ENUM_RETURN *result)
{
    S_R_ASSERT(result != NULL, RETURN_FAILURE);

    *result = RETURN_FAILURE;
    STRU_MACRO_NODE *p_macro_node = list_entry(g_macro_node_list_head.list.prev, STRU_MACRO_NODE, list);

    S_R_ASSERT(p_macro_node != &g_macro_node_list_head, RETURN_FAILURE);

    DEBUG_PRINT("separator_num: %zd, parameter_num: %zd\n", 
        p_macro_node->info.parameter_separator_num, 
        s_cproc_macro_get_parameter_num(&p_macro_node->info));
    size_t separator_num = p_macro_node->info.parameter_separator_num;
    size_t parameter_num = s_cproc_macro_get_parameter_num(&p_macro_node->info);
    
    if((separator_num == 0 && parameter_num <= 1)
        ||(separator_num > 0 && (separator_num == (parameter_num - 1))))
    {
        *result = RETURN_SUCCESS;
    }

    p_macro_node->info.parameter_part_exist = BOOLEAN_TRUE;

    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_macro_finish_replacement()
{
    ENUM_RETURN ret_val;
    STRU_MACRO_NODE *p_macro_node = list_entry(g_macro_node_list_head.list.prev, STRU_MACRO_NODE, list);
    S_R_ASSERT(p_macro_node != &g_macro_node_list_head, BOOLEAN_FALSE);
   
    ret_val = s_cproc_macro_move_replacement_list_from_token_list(&p_macro_node->info.replacement_list_head);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    //delete blanks at the begining and end
    DEBUG_PRINT("delete blanks at the begining and end");
    
    STRU_C_TOKEN_NODE *p_replacement_token_loop = NULL, *p_replacement_token_first_nonblank = NULL;
    
    struct list_head *pos, *next, *prev;
    size_t replacement_token_num = 0;
    size_t first_blank_replacement_token_num = 0;
    
    list_for_each_all_safe(pos, next, &p_macro_node->info.replacement_list_head.list)
    {
        replacement_token_num++;
        p_replacement_token_loop = list_entry(pos, STRU_C_TOKEN_NODE, list);
        if(p_replacement_token_loop->info.token_type == C_TOKEN_BLANK)
        {
            first_blank_replacement_token_num++;
            ret_val = s_ctoken_delete_node_from_list(
                &p_macro_node->info.replacement_list_head,
                p_replacement_token_loop);
            S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        }
        else
        {
            p_replacement_token_first_nonblank = p_replacement_token_loop;
            break;
        }
    };

    if(p_macro_node->info.parameter_part_exist == BOOLEAN_FALSE 
        && replacement_token_num > 0
        && first_blank_replacement_token_num == 0)
    {
        S_CPROC_STM_GEN_WARNING(&p_replacement_token_first_nonblank->info.c_position, "ISO C99 requires whitespace after the macro name");
    }

    p_replacement_token_loop = NULL;
    list_for_each_all_reverse_safe(pos, prev, &p_macro_node->info.replacement_list_head.list)
    {
        p_replacement_token_loop = list_entry(pos, STRU_C_TOKEN_NODE, list);
        if(p_replacement_token_loop->info.token_type == C_TOKEN_BLANK)
        {
            ret_val = s_ctoken_delete_node_from_list(
                &p_macro_node->info.replacement_list_head,
                p_replacement_token_loop);
            S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        }
        else
        {
            break;
        }
    };

    //continue blanks between the replacement tokens will be replaced by one space
    DEBUG_PRINT("replace continued blanks by one space");
    ENUM_BOOLEAN first_continued_blank = BOOLEAN_FALSE;
    list_for_each_all_safe(pos, next, &p_macro_node->info.replacement_list_head.list)
    {
        p_replacement_token_loop = list_entry(pos, STRU_C_TOKEN_NODE, list);
        if(p_replacement_token_loop->info.token_type == C_TOKEN_BLANK)
        {
            if(first_continued_blank == BOOLEAN_FALSE)
            {
                first_continued_blank = BOOLEAN_TRUE;
                ret_val = s_ctoken_mod_node_string(p_replacement_token_loop, " ");
                S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            }
            else
            {
                ret_val = s_ctoken_delete_node_from_list(
                    &p_macro_node->info.replacement_list_head,
                    p_replacement_token_loop);
                S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            }
        }
        else
        {
            first_continued_blank = BOOLEAN_FALSE;
        }
    };

    //processing # and ## rules
    DEBUG_PRINT("processing # and ##");
    p_replacement_token_loop = NULL;
    replacement_token_num = 0;
    ENUM_BOOLEAN last_stingification = BOOLEAN_FALSE;
    list_for_each_all(pos, &p_macro_node->info.replacement_list_head.list)
    {
        p_replacement_token_loop = list_entry(pos, STRU_C_TOKEN_NODE, list);

        DEBUG_PRINT(TOKEN_INFO_FORMAT, TOKEN_INFO_VALUE(p_replacement_token_loop));
        //'#' is not followed by a macro parameter
        if(last_stingification == BOOLEAN_TRUE)
        {
            if(p_replacement_token_loop->info.token_type != C_TOKEN_BLANK
                && p_macro_node->info.parameter_part_exist
                && !s_cproc_macro_replacement_identifier_in_parameter_list(p_replacement_token_loop))
            {
                S_CPROC_STM_GEN_ERROR(&p_replacement_token_loop->info.c_position, "'#' is not followed by a macro parameter");
                return RETURN_SUCCESS;
            }
        }

        if(p_replacement_token_loop->info.token_type == C_TOKEN_PPD_CONCATENATE && replacement_token_num == 0)
        {
            S_CPROC_STM_GEN_ERROR(&p_replacement_token_loop->info.c_position, "'##' cannot appear at either end of a macro expansion");
            return RETURN_SUCCESS;
        }

        if(p_replacement_token_loop->info.token_type == C_TOKEN_PP_IDENTIFIER_VA)
        {
            if(s_cproc_macro_va_in_parameter_list() == BOOLEAN_FALSE)
            {
                S_CPROC_STM_GEN_WARNING(&p_replacement_token_loop->info.c_position, "__VA_ARGS__ can only appear in the expansion of a C99 variadic macro");
            }
        }

        if(p_replacement_token_loop->info.token_type == C_TOKEN_PPD_STRINGIFICATION)
        {
            last_stingification = BOOLEAN_TRUE;
        }
        else
        {
            last_stingification = BOOLEAN_FALSE;
        }
        replacement_token_num++;
    };

    S_R_FALSE(p_replacement_token_loop != NULL, RETURN_SUCCESS);
    
    //'##' cannot appear at either end of a macro expansion
    if(p_replacement_token_loop->info.token_type == C_TOKEN_PPD_CONCATENATE)
    {
        S_CPROC_STM_GEN_ERROR(&p_replacement_token_loop->info.c_position, "'##' cannot appear at either end of a macro expansion");
        return RETURN_SUCCESS;
    }

    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_macro_expand(_VOID)
{
    //expand last token if the type is identifier
    STRU_C_TOKEN_NODE *p_last_token = S_CPROC_LAST_TOKEN_POINTER;
    S_R_ASSERT(p_last_token != NULL, RETURN_FAILURE);

    //not a identifier
    S_R_ASSERT(p_last_token->info.token_type == C_TOKEN_IDENTIFIER, RETURN_FAILURE);

    /* get macro node by name */
    STRU_MACRO_NODE *p_macro_node = NULL;
    ENUM_RETURN ret_val;

    ret_val = s_cproc_macro_get_by_name(p_last_token->info.p_string, &p_macro_node);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    //the macro doesn't exist
    S_R_FALSE(p_macro_node != NULL, RETURN_SUCCESS);
    
    STRU_C_TOKEN_NODE *p_replacement_token_list_head = NULL;
    ret_val = s_cproc_macro_get_replacement_list(p_macro_node, &p_replacement_token_list_head);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    S_R_ASSERT(p_replacement_token_list_head != NULL, RETURN_FAILURE);

    s_cproc_token_delete_last_node();

    ret_val = s_ctoken_copy_list_to_another_list_tail(p_replacement_token_list_head, NEXT_TOKEN(p_replacement_token_list_head), 
        PREV_TOKEN(p_replacement_token_list_head), s_cproc_token_get_list_head());
    
    //pasting """" and """" does not give a valid preprocessing token

    return RETURN_SUCCESS;
}

