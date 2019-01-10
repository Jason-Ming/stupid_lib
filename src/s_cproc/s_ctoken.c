#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "s_log.h"
#include "s_mem.h"
#include "s_limits.h"
#include "s_text.h"
#include "s_stack.h"
#include "s_stm.h"
#include "s_list.h"
#include "s_cproc.h"
#include "s_cpp.h"
# include "s_ctoken.h"

ENUM_RETURN s_ctoken_check_list_node_order(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_start, 
    STRU_C_TOKEN_NODE *p_token_list_end,
    ENUM_RETURN *result)
{
    S_R_ASSERT(p_token_list_head != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_token_list_start != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_token_list_end != NULL, RETURN_FAILURE);
    S_R_ASSERT(result != NULL, RETURN_FAILURE);
    *result = RETURN_FAILURE;

    //make sure p_source_token_list_start is the previous node of p_source_token_list_end
    STRU_C_TOKEN_NODE *p_dest_token_list_temp;

    ENUM_BOOLEAN find_start_node = BOOLEAN_FALSE;
    ENUM_BOOLEAN find_end_node = BOOLEAN_FALSE;
    LIST_FOR_EACH_ALL(p_token_list_head)
    {
        p_dest_token_list_temp = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);
        if(p_token_list_start == p_dest_token_list_temp)
        {
            if(find_start_node == BOOLEAN_FALSE)
            {
                S_R_FALSE(find_end_node == BOOLEAN_FALSE, RETURN_SUCCESS);
            }
            find_start_node = BOOLEAN_TRUE;
        }
        
        if(p_token_list_end == p_dest_token_list_temp)
        {
            S_R_FALSE(find_start_node == BOOLEAN_TRUE, RETURN_SUCCESS);
            find_end_node = BOOLEAN_TRUE;
            break;
        }
    }

    S_R_FALSE(find_start_node == BOOLEAN_TRUE && find_end_node == BOOLEAN_TRUE, RETURN_SUCCESS);

    *result = RETURN_SUCCESS;

    return RETURN_SUCCESS;
}

_VOID s_ctoken_init_head(STRU_C_TOKEN_NODE *p_head, _S8 * head_name)
{
    S_V_ASSERT(p_head != NULL);
    S_V_ASSERT(head_name != NULL);
    
    p_head->info.p_string = head_name;
    p_head->info.token_type = C_TOKEN_LIST_HEAD;
    p_head->info.c_position.offset = 0;
    p_head->info.c_position.len = strlen(head_name);
    p_head->info.c_position.line = 0;
    p_head->info.c_position.column= 0;
    p_head->info.qualifier.is_const = BOOLEAN_FALSE;
    p_head->info.qualifier.is_auto = BOOLEAN_FALSE;
    p_head->info.qualifier.is_extern = BOOLEAN_FALSE;
    p_head->info.qualifier.is_register = BOOLEAN_FALSE;
    p_head->info.qualifier.is_static = BOOLEAN_FALSE;
    p_head->info.qualifier.is_volatile = BOOLEAN_FALSE;
    
    INIT_LIST_HEAD(&p_head->list);
}

ENUM_RETURN s_ctoken_make_new_node(
    const _S8 *token_string, 
    ENUM_C_TOKEN token_type, 
    size_t text_offset,
    size_t line_index,
    size_t line_column,
    STRU_C_TOKEN_NODE **pp_new_token_node)
{
    R_ASSERT(token_string != NULL, RETURN_FAILURE);
    R_ASSERT(pp_new_token_node != NULL, RETURN_FAILURE);
    *pp_new_token_node = NULL;

    STRU_C_TOKEN_NODE *p_new_token_temp = (STRU_C_TOKEN_NODE*)malloc(sizeof(STRU_C_TOKEN_NODE));
    R_ASSERT(p_new_token_temp != NULL, RETURN_FAILURE);

    p_new_token_temp->info.p_string = (_S8*)malloc(strlen(token_string) + 1);
    R_ASSERT_DO(p_new_token_temp->info.p_string != NULL, RETURN_FAILURE, S_FREE(p_new_token_temp));

    strcpy(p_new_token_temp->info.p_string, token_string);
    p_new_token_temp->info.token_type = token_type;
    p_new_token_temp->info.qualifier.is_auto = BOOLEAN_FALSE;
    p_new_token_temp->info.qualifier.is_const = BOOLEAN_FALSE;
    p_new_token_temp->info.qualifier.is_extern = BOOLEAN_FALSE;
    p_new_token_temp->info.qualifier.is_register = BOOLEAN_FALSE;
    p_new_token_temp->info.qualifier.is_static = BOOLEAN_FALSE;
    p_new_token_temp->info.qualifier.is_volatile = BOOLEAN_FALSE;
    p_new_token_temp->info.c_position.offset = text_offset;
    p_new_token_temp->info.c_position.len = strlen(token_string);
    p_new_token_temp->info.c_position.line = line_index;
    p_new_token_temp->info.c_position.column = line_column;
    p_new_token_temp->list.next = NULL;
    p_new_token_temp->list.prev = NULL;
    *pp_new_token_node = p_new_token_temp;

    return RETURN_SUCCESS;
}

ENUM_RETURN s_ctoken_duplicate_node(
    const STRU_C_TOKEN_NODE *p_token_node_source, 
    STRU_C_TOKEN_NODE **pp_token_node_dest)
{
    R_ASSERT(p_token_node_source != NULL, RETURN_FAILURE);
    R_ASSERT(pp_token_node_dest != NULL, RETURN_FAILURE);

    DEBUG_PRINT("copy token: "TOKEN_INFO_FORMAT, 
        TOKEN_INFO_VALUE(p_token_node_source));

    *pp_token_node_dest = NULL;

    STRU_C_TOKEN_NODE *p_token_node_temp = NULL;

    p_token_node_temp = (STRU_C_TOKEN_NODE*)malloc(sizeof(STRU_C_TOKEN_NODE));
    S_R_ASSERT(p_token_node_temp != NULL, RETURN_FAILURE);

    p_token_node_temp->list = p_token_node_source->list;
    p_token_node_temp->info = p_token_node_source->info;
    p_token_node_temp->info.p_string = NULL;
    p_token_node_temp->info.p_string = (_S8*)malloc(strlen(p_token_node_source->info.p_string) + 1);
    S_R_ASSERT_DO(p_token_node_temp->info.p_string != NULL, RETURN_FAILURE, S_FREE(p_token_node_temp));

    strcpy(p_token_node_temp->info.p_string, p_token_node_source->info.p_string);
    *pp_token_node_dest = p_token_node_temp;

    return RETURN_SUCCESS;
}

ENUM_RETURN s_ctoken_add_node_after_node(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_node,
    STRU_C_TOKEN_NODE *p_new_token_node)
{
    R_ASSERT(p_token_list_head != NULL, RETURN_FAILURE);
    R_ASSERT(p_token_node != NULL, RETURN_FAILURE);
    R_ASSERT(p_new_token_node != NULL, RETURN_FAILURE);
    
    DEBUG_PRINT("LIST HEAD: "TOKEN_INFO_FORMAT", add token "TOKEN_INFO_FORMAT" after token "TOKEN_INFO_FORMAT, 
            TOKEN_INFO_VALUE(p_token_list_head),
            TOKEN_INFO_VALUE(p_new_token_node),
            TOKEN_INFO_VALUE(p_token_node));

    list_add(&p_new_token_node->list, &p_token_node->list);

    return RETURN_SUCCESS;
}

ENUM_RETURN s_ctoken_add_node_to_list_tail(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_new_token_node)
{
    R_ASSERT(p_new_token_node != NULL, RETURN_FAILURE);
    
    DEBUG_PRINT("LIST HEAD: "TOKEN_INFO_FORMAT", add token "TOKEN_INFO_FORMAT, 
            TOKEN_INFO_VALUE(p_token_list_head),
            TOKEN_INFO_VALUE(p_new_token_node));

    list_add_tail(&p_new_token_node->list, &p_token_list_head->list);

    return RETURN_SUCCESS;
}

ENUM_RETURN s_ctoken_make_new_node_and_add_to_list_tail(
    STRU_C_TOKEN_NODE *p_token_list_head,
    const _S8 *token_string, 
    ENUM_C_TOKEN token_type, 
    size_t text_offset,
    size_t line_index,
    size_t line_column)
{
    R_ASSERT(token_string != NULL, RETURN_FAILURE);

    STRU_C_TOKEN_NODE *p_new_token_node = NULL;
    ENUM_RETURN ret_val;
    ret_val = s_ctoken_make_new_node(
        token_string,
        token_type, 
        text_offset, 
        line_index, 
        line_column, 
        &p_new_token_node);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    S_R_ASSERT(p_new_token_node != NULL, RETURN_FAILURE);
    
    ret_val = s_ctoken_add_node_to_list_tail(
        p_token_list_head,
        p_new_token_node);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    return RETURN_SUCCESS;
};

_VOID s_ctoken_free_node(STRU_C_TOKEN_NODE *p_token_to_be_deleted)
{
    S_V_FALSE(p_token_to_be_deleted != NULL);
    DEBUG_PRINT("free token: "TOKEN_INFO_FORMAT, TOKEN_INFO_VALUE(p_token_to_be_deleted));
    
    S_FREE(p_token_to_be_deleted->info.p_string);
    S_FREE(p_token_to_be_deleted);
}

ENUM_RETURN s_ctoken_delete_node_from_list(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_to_be_deleted)
{
    R_ASSERT(p_token_to_be_deleted != NULL, RETURN_FAILURE);
    R_ASSERT(p_token_list_head != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_token_to_be_deleted != p_token_list_head, RETURN_FAILURE);

    DEBUG_PRINT("LIST HEAD: "TOKEN_INFO_FORMAT", delete token "TOKEN_INFO_FORMAT, 
        TOKEN_INFO_VALUE(p_token_list_head),
        TOKEN_INFO_VALUE(p_token_to_be_deleted));
    
    LIST_RMV_NODE(p_token_to_be_deleted);
    
    s_ctoken_free_node(p_token_to_be_deleted);

    return RETURN_SUCCESS;
};

_VOID s_ctoken_delete_list_after_node(
    STRU_C_TOKEN_NODE *p_token_list_head, 
    STRU_C_TOKEN_NODE *p_token_list_node)
{
    S_V_ASSERT(p_token_list_head != NULL);
    S_V_ASSERT(p_token_list_node != NULL);
    STRU_C_TOKEN_NODE *p_token_temp;

    DEBUG_PRINT("LIST HEAD: "TOKEN_INFO_FORMAT", release tokens after "TOKEN_INFO_FORMAT, 
            TOKEN_INFO_VALUE(p_token_list_head),
            TOKEN_INFO_VALUE(p_token_list_node));
    
    list_for_each_safe(p_token_list_node, p_token_list_head)
    {
        p_token_temp = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);
        LIST_RMV_NODE(p_token_temp);
        s_ctoken_free_node(p_token_temp);
    }
}

_VOID s_ctoken_delete_list(STRU_C_TOKEN_NODE *p_token_list_head)
{
    S_V_ASSERT(p_token_list_head != NULL);
    STRU_C_TOKEN_NODE *p_token_temp;

    DEBUG_PRINT("LIST HEAD: "TOKEN_INFO_FORMAT", release the whole token list", 
            TOKEN_INFO_VALUE(p_token_list_head));
    LIST_FOR_EACH_ALL_SAFE(p_token_list_head)
    {
        p_token_temp = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);
        LIST_RMV_NODE(p_token_temp);
        s_ctoken_free_node(p_token_temp);
    }
}

_VOID s_ctoken_delete_blanks_and_newline_from_list(STRU_C_TOKEN_NODE *p_token_list_head)
{
    S_V_ASSERT(p_token_list_head != NULL);
    STRU_C_TOKEN_NODE *p_token_temp;

    printf("\n\ndelete blank tokens start...\n");

    DEBUG_PRINT("LIST HEAD: "TOKEN_INFO_FORMAT", delete all blank tokens", 
            TOKEN_INFO_VALUE(p_token_list_head));
    size_t token_num = 0;
    LIST_FOR_EACH_ALL_SAFE(p_token_list_head)
    {
        p_token_temp = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);
        if(p_token_temp->info.token_type == C_TOKEN_BLANK
            || p_token_temp->info.token_type == C_TOKEN_NEWLINE)
        {
            token_num++;
            LIST_RMV_NODE(p_token_temp);
            s_ctoken_free_node(p_token_temp);
        }
    }
    printf("%zd blank tokens had been deleted.\n", token_num);
    DEBUG_PRINT("%zd blank tokens had been deleted", token_num);
}

_VOID s_ctoken_print_list(
    FILE *fpw,
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_start, 
    STRU_C_TOKEN_NODE *p_token_list_end)
{
	S_V_ASSERT(p_token_list_head != NULL);
    S_V_ASSERT(p_token_list_start != NULL);
    S_V_ASSERT(p_token_list_end != NULL);
    
    STRU_C_TOKEN_NODE *p_token_list_node;
    
    DEBUG_PRINT("LIST HEAD: "TOKEN_INFO_FORMAT", print tokens to file or IO from "TOKEN_INFO_FORMAT"to "TOKEN_INFO_FORMAT, 
            TOKEN_INFO_VALUE(p_token_list_head),
            TOKEN_INFO_VALUE(p_token_list_start),
            TOKEN_INFO_VALUE(p_token_list_end));

    list_for_each(PREV_TOKEN(p_token_list_start), NEXT_TOKEN(p_token_list_end))
	{
        p_token_list_node = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);
		CPP_PRINT(fpw, "%s", p_token_list_node->info.p_string);
	}

	//CPP_PRINT(fpw, "\n");
}

_VOID s_ctoken_print_list_debug_info(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_start, 
    STRU_C_TOKEN_NODE *p_token_list_end)
{
    S_V_ASSERT(p_token_list_head != NULL);
    S_V_ASSERT(p_token_list_start != NULL);
    S_V_ASSERT(p_token_list_end != NULL);
    
    STRU_C_TOKEN_NODE *p_token_list_node; 

    DEBUG_PRINT("LIST HEAD: "TOKEN_INFO_FORMAT", display tokens from "TOKEN_INFO_FORMAT"to "TOKEN_INFO_FORMAT, 
        TOKEN_INFO_VALUE(p_token_list_head),
        TOKEN_INFO_VALUE(p_token_list_start),
        TOKEN_INFO_VALUE(p_token_list_end));
    
	printf("\n\n--TOKEN LIST DEBUG INFO BEGIN------------------------------------------------------\n");
    list_for_each(PREV_TOKEN(p_token_list_start), NEXT_TOKEN(p_token_list_end))
    {
        p_token_list_node = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);

		printf(REVERSE"%s"NONE, p_token_list_node->info.p_string);
		size_t len = strlen(p_token_list_node->info.p_string);
		_S32 print_space_num = (len > 20)?0:(20 - len);
		for(_S32 i = 0; i < print_space_num; i++)
		{
			printf(" ");
		}

		printf(" %s\n", s_ctoken_get_str(p_token_list_node->info.token_type));
	}

    printf("--TOKEN LIST DEBUG INFO END--------------------------------------------------------\n");

}


STRU_C_TOKEN_NODE * s_ctoken_get_last_node_by_type_after_node(
    ENUM_C_TOKEN token_type,
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_node)
{
    return s_ctoken_get_last_node_by_type(token_type, p_token_list_head, 
        list_entry(p_token_list_node->list.next, STRU_C_TOKEN_NODE, list), list_entry(p_token_list_head->list.prev, STRU_C_TOKEN_NODE, list));
}

STRU_C_TOKEN_NODE * s_ctoken_get_last_node_by_type(
    ENUM_C_TOKEN token_type,
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_start, 
    STRU_C_TOKEN_NODE *p_token_list_end)
{
    S_R_ASSERT(token_type >= C_TOKEN_NORMAL && token_type < C_TOKEN_MAX, NULL);
    S_R_ASSERT(p_token_list_head != NULL, NULL);
    S_R_ASSERT(p_token_list_start != NULL, NULL);
    S_R_ASSERT(p_token_list_end != NULL, NULL);
    
    STRU_C_TOKEN_NODE *p_token_list_node; 

    DEBUG_PRINT("LIST HEAD: "TOKEN_INFO_FORMAT", find token type: %s from "TOKEN_INFO_FORMAT"to "TOKEN_INFO_FORMAT, 
        TOKEN_INFO_VALUE(p_token_list_head),
        s_ctoken_get_str(token_type), 
        TOKEN_INFO_VALUE(p_token_list_start),
        TOKEN_INFO_VALUE(p_token_list_end));
    
    list_for_each_reverse(PREV_TOKEN(p_token_list_start), NEXT_TOKEN(p_token_list_end))
    {
        p_token_list_node = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);
        DEBUG_PRINT("iterator token: "TOKEN_INFO_FORMAT, 
            TOKEN_INFO_VALUE(p_token_list_node));
        if(p_token_list_node->info.token_type == token_type)
        {
            return p_token_list_node;
        }
	}

    return NULL;
}

STRU_C_TOKEN_NODE *s_ctoken_get_last_node_by_name(
    const _S8 *p_string, 
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_start, 
    STRU_C_TOKEN_NODE *p_token_list_end)
{
    S_R_ASSERT(p_string != NULL, NULL);
    S_R_ASSERT(p_token_list_head != NULL, NULL);
    S_R_ASSERT(p_token_list_start != NULL, NULL);
    S_R_ASSERT(p_token_list_end != NULL, NULL);
    
    STRU_C_TOKEN_NODE *p_token_list_node; 

    DEBUG_PRINT("LIST HEAD: "TOKEN_INFO_FORMAT", find name: %s from "TOKEN_INFO_FORMAT"to "TOKEN_INFO_FORMAT, 
        TOKEN_INFO_VALUE(p_token_list_head),
        p_string, 
        TOKEN_INFO_VALUE(p_token_list_start),
        TOKEN_INFO_VALUE(p_token_list_end));
    
    list_for_each_reverse(PREV_TOKEN(p_token_list_start), NEXT_TOKEN(p_token_list_end))
    {
        p_token_list_node = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);
        DEBUG_PRINT("iterator token: "TOKEN_INFO_FORMAT, 
            TOKEN_INFO_VALUE(p_token_list_node));
        if(strcmp(p_token_list_node->info.p_string, p_string) == 0)
        {
            return p_token_list_node;
        }
	}

    return NULL;
}

ENUM_RETURN s_ctoken_mod_node_type(STRU_C_TOKEN_NODE *p_token, ENUM_C_TOKEN new_type)
{
    S_R_ASSERT(p_token != NULL, RETURN_FAILURE);
    S_R_ASSERT(new_type >= C_TOKEN_NORMAL && new_type < C_TOKEN_MAX && C_TOKEN_LIST_HEAD != new_type, RETURN_FAILURE);
    DEBUG_PRINT("mod token type: "TOKEN_INFO_FORMAT", from %s to %s", 
        TOKEN_INFO_VALUE(p_token),
        s_ctoken_get_str(p_token->info.token_type),
        s_ctoken_get_str(new_type));
    
    p_token->info.token_type = new_type;

    return RETURN_SUCCESS;
    
}

ENUM_RETURN s_ctoken_mod_node_string(STRU_C_TOKEN_NODE *p_token, const _S8*p_new_string)
{
    S_R_ASSERT(p_token != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_new_string != NULL, RETURN_FAILURE);
    
    DEBUG_PRINT("mod token string: "TOKEN_INFO_FORMAT", from %s to %s", 
        TOKEN_INFO_VALUE(p_token),
        p_token->info.p_string,
        p_new_string);

    S_FREE(p_token->info.p_string);
    p_token->info.p_string = s_duplicate_string(p_new_string);
    S_R_ASSERT(p_token->info.p_string != NULL, RETURN_FAILURE);
    p_token->info.c_position.len = strlen(p_token->info.p_string);

    return RETURN_SUCCESS;
    
}

ENUM_RETURN s_ctoken_mod_node_string_and_type(STRU_C_TOKEN_NODE *p_token, const _S8*p_new_string, ENUM_C_TOKEN new_type)
{
    S_R_ASSERT(p_token != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_new_string != NULL, RETURN_FAILURE);
    S_R_ASSERT(new_type >= C_TOKEN_NORMAL && new_type < C_TOKEN_MAX && C_TOKEN_LIST_HEAD != new_type, RETURN_FAILURE);
    
    DEBUG_PRINT("mod token string and type from: "TOKEN_INFO_FORMAT", to %s, %s", 
        TOKEN_INFO_VALUE(p_token),
        p_new_string,
        s_ctoken_get_str(new_type));

    p_token->info.token_type = new_type;

    S_FREE(p_token->info.p_string);
    p_token->info.p_string = s_duplicate_string(p_new_string);
    S_R_ASSERT(p_token->info.p_string != NULL, RETURN_FAILURE);
    p_token->info.c_position.len = strlen(p_token->info.p_string);

    return RETURN_SUCCESS;
    
}

STRU_C_TOKEN_NODE *s_ctoken_get_last_node(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_start, 
    STRU_C_TOKEN_NODE *p_token_list_end)
{
    S_R_ASSERT(p_token_list_head != NULL, NULL);
    S_R_ASSERT(p_token_list_start != NULL, NULL);
    S_R_ASSERT(p_token_list_end != NULL, NULL);
    if(p_token_list_end == p_token_list_head)
    {
        return NULL;
    }
    
    return p_token_list_end;
}

STRU_C_TOKEN_NODE *s_ctoken_get_last_last_node(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_start, 
    STRU_C_TOKEN_NODE *p_token_list_end)
{
    S_R_ASSERT(p_token_list_head != NULL, NULL);
    S_R_ASSERT(p_token_list_start != NULL, NULL);
    S_R_ASSERT(p_token_list_end != NULL, NULL);
    if(p_token_list_end == p_token_list_head)
    {
        return NULL;
    }

    if(p_token_list_end == p_token_list_start)
    {
        return NULL;
    }
    
    STRU_C_TOKEN_NODE *p_token_node = list_entry(p_token_list_end->list.prev, STRU_C_TOKEN_NODE, list);
    if(p_token_node == p_token_list_head)
    {
        return NULL;
    }

    return p_token_node;
}

STRU_C_TOKEN_NODE *s_ctoken_get_next_node(STRU_C_TOKEN_NODE *p_token_list_head, STRU_C_TOKEN_NODE *p_token_list_node)
{
    S_R_ASSERT(p_token_list_head != NULL, NULL);
    S_R_ASSERT(p_token_list_node != NULL, NULL);

    return (NEXT_TOKEN(p_token_list_node) != p_token_list_head)?NEXT_TOKEN(p_token_list_node):NULL;
}

ENUM_BOOLEAN s_ctoken_all_same_type_after_node(
    STRU_C_TOKEN_NODE *p_token_list_head, 
    STRU_C_TOKEN_NODE *p_token_list_node,
    ENUM_C_TOKEN token_type)
{
    S_R_ASSERT(p_token_list_head != NULL, BOOLEAN_FALSE);
    S_R_ASSERT(p_token_list_node != NULL, BOOLEAN_FALSE);
    
    DEBUG_PRINT("LIST HEAD: "TOKEN_INFO_FORMAT", check all same type: %s after "TOKEN_INFO_FORMAT, 
        TOKEN_INFO_VALUE(p_token_list_head),
        s_ctoken_get_str(token_type), 
        TOKEN_INFO_VALUE(p_token_list_node));
    
    STRU_C_TOKEN_NODE *p_token_list_temp;
    ENUM_BOOLEAN all_same_type = BOOLEAN_TRUE;

    list_for_each_reverse(p_token_list_node, p_token_list_head)
    {
        p_token_list_temp = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);

        DEBUG_PRINT("iterator token: "TOKEN_INFO_FORMAT, 
            TOKEN_INFO_VALUE(p_token_list_temp));
        
		if(p_token_list_temp->info.token_type != token_type)
		{
			all_same_type = BOOLEAN_FALSE;
			break;
		}
    }

    return all_same_type;
}

ENUM_RETURN s_ctoken_move_list_to_another_list_tail(
    STRU_C_TOKEN_NODE *p_source_token_list_head, 
    STRU_C_TOKEN_NODE *p_source_token_list_start,
    STRU_C_TOKEN_NODE *p_source_token_list_end,
    STRU_C_TOKEN_NODE *p_dest_token_list_head)
{
    S_R_ASSERT(p_source_token_list_head != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_source_token_list_start != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_source_token_list_end != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_dest_token_list_head != NULL, RETURN_FAILURE);

    DEBUG_PRINT("move list: LIST HEAD: "TOKEN_INFO_FORMAT", from "TOKEN_INFO_FORMAT"to "TOKEN_INFO_FORMAT"\n"
                "  to list: LIST HEAD: "TOKEN_INFO_FORMAT, 
            TOKEN_INFO_VALUE(p_source_token_list_head),
            TOKEN_INFO_VALUE(p_source_token_list_start),
            TOKEN_INFO_VALUE(p_source_token_list_end),
            TOKEN_INFO_VALUE(p_dest_token_list_head));

    //start in the end of the list means the source list is empty, not need to move 
    S_R_FALSE(p_source_token_list_head != p_source_token_list_start, RETURN_SUCCESS);
    S_R_ASSERT(p_source_token_list_head != p_source_token_list_end, RETURN_FAILURE);
    
    ENUM_RETURN ret_val, check_result;
    ret_val = s_ctoken_check_list_node_order(p_source_token_list_head, p_source_token_list_start, 
        p_source_token_list_end, &check_result);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    S_R_ASSERT(check_result == RETURN_SUCCESS, RETURN_FAILURE);

    p_source_token_list_start->list.prev->next = p_source_token_list_end->list.next;
    p_source_token_list_end->list.next->prev = p_source_token_list_start->list.prev;

    p_source_token_list_end->list.next = &p_dest_token_list_head->list;
    p_dest_token_list_head->list.prev = &p_source_token_list_end->list;
    p_dest_token_list_head->list.next = &p_source_token_list_start->list;
    p_source_token_list_start->list.prev = &p_dest_token_list_head->list;
    
    return RETURN_SUCCESS;
}


ENUM_RETURN s_ctoken_copy_list_to_another_list_after_node(
    STRU_C_TOKEN_NODE *p_source_token_list_head, 
    STRU_C_TOKEN_NODE *p_source_token_list_start,
    STRU_C_TOKEN_NODE *p_source_token_list_end,
    STRU_C_TOKEN_NODE *p_dest_token_list_head,
    STRU_C_TOKEN_NODE *p_dest_token_list_node)
{
    S_R_ASSERT(p_source_token_list_head != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_source_token_list_start != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_source_token_list_end != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_dest_token_list_head != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_dest_token_list_node != NULL, RETURN_FAILURE);

    DEBUG_PRINT("copy list: LIST HEAD: "TOKEN_INFO_FORMAT", from "TOKEN_INFO_FORMAT"to "TOKEN_INFO_FORMAT"\n"
                "  to list: LIST HEAD: "TOKEN_INFO_FORMAT", after "TOKEN_INFO_FORMAT, 
            TOKEN_INFO_VALUE(p_source_token_list_head),
            TOKEN_INFO_VALUE(p_source_token_list_start),
            TOKEN_INFO_VALUE(p_source_token_list_end),
            TOKEN_INFO_VALUE(p_dest_token_list_head),
            TOKEN_INFO_VALUE(p_dest_token_list_node));

    //start in the end of the list means the source list is empty, not need to move 
    S_R_FALSE(p_source_token_list_head != p_source_token_list_start, RETURN_SUCCESS);
    S_R_ASSERT(p_source_token_list_head != p_source_token_list_end, RETURN_FAILURE);
    
    ENUM_RETURN ret_val, check_result;
    ret_val = s_ctoken_check_list_node_order(p_source_token_list_head, p_source_token_list_start, 
        p_source_token_list_end, &check_result);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    S_R_ASSERT(check_result == RETURN_SUCCESS, RETURN_FAILURE);

    STRU_C_TOKEN_NODE *p_token_list_node;
    STRU_C_TOKEN_NODE *p_token_node_new;
    STRU_C_TOKEN_NODE *p_token_node_after = p_dest_token_list_node;

    list_for_each(PREV_TOKEN(p_source_token_list_start), NEXT_TOKEN(p_source_token_list_end))
	{
        p_token_list_node = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);
		ret_val = s_ctoken_duplicate_node(p_token_list_node, &p_token_node_new);
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        S_R_ASSERT(p_token_node_new != NULL, RETURN_FAILURE);

        ret_val = s_ctoken_add_node_after_node(p_dest_token_list_head, p_token_node_after, p_token_node_new);
        S_R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, s_ctoken_free_node(p_token_node_new));

        p_token_node_after = p_token_node_new;
	}

    return RETURN_SUCCESS;
}

ENUM_RETURN s_ctoken_copy_list_to_another_list_tail(
    STRU_C_TOKEN_NODE *p_source_token_list_head, 
    STRU_C_TOKEN_NODE *p_source_token_list_start,
    STRU_C_TOKEN_NODE *p_source_token_list_end,
    STRU_C_TOKEN_NODE *p_dest_token_list_head)
{
    S_R_ASSERT(p_source_token_list_head != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_source_token_list_start != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_source_token_list_end != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_dest_token_list_head != NULL, RETURN_FAILURE);

    DEBUG_PRINT("copy list: LIST HEAD: "TOKEN_INFO_FORMAT", from "TOKEN_INFO_FORMAT"to "TOKEN_INFO_FORMAT"\n"
                "  to list: LIST HEAD: "TOKEN_INFO_FORMAT, 
            TOKEN_INFO_VALUE(p_source_token_list_head),
            TOKEN_INFO_VALUE(p_source_token_list_start),
            TOKEN_INFO_VALUE(p_source_token_list_end),
            TOKEN_INFO_VALUE(p_dest_token_list_head));

    //start in the end of the list means the source list is empty, not need to move 
    S_R_FALSE(p_source_token_list_head != p_source_token_list_start, RETURN_SUCCESS);
    S_R_ASSERT(p_source_token_list_head != p_source_token_list_end, RETURN_FAILURE);
    
    ENUM_RETURN ret_val, check_result;
    ret_val = s_ctoken_check_list_node_order(p_source_token_list_head, p_source_token_list_start, 
        p_source_token_list_end, &check_result);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    S_R_ASSERT(check_result == RETURN_SUCCESS, RETURN_FAILURE);

    STRU_C_TOKEN_NODE *p_token_list_node;
    STRU_C_TOKEN_NODE *p_token_node_new;

    list_for_each(PREV_TOKEN(p_source_token_list_start), NEXT_TOKEN(p_source_token_list_end))
	{
        p_token_list_node = LIST_GET_ITERATOR(STRU_C_TOKEN_NODE);
		ret_val = s_ctoken_duplicate_node(p_token_list_node, &p_token_node_new);
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        S_R_ASSERT(p_token_node_new != NULL, RETURN_FAILURE);

        ret_val = s_ctoken_add_node_to_list_tail(p_dest_token_list_head, p_token_node_new);
        S_R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, s_ctoken_free_node(p_token_node_new));
	}

    return RETURN_SUCCESS;
}

