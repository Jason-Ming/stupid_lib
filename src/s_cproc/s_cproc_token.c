#include <stdlib.h>

#include "s_log.h"
#include "s_mem.h"
#include "s_text.h"

#include "s_ctoken.h"
#include "s_cproc_token.h"

PRIVATE STRU_C_TOKEN_NODE g_c_token_list_head;

STRU_C_TOKEN_NODE *s_cproc_token_get_list_head(_VOID)
{
    return &g_c_token_list_head;
}

ENUM_RETURN s_cproc_token_add_node_to_list(
    const _S8 *token_string, 
    ENUM_C_TOKEN token_type, 
    size_t text_offset,
    size_t line_index,
    size_t line_column)
{
    return s_ctoken_make_new_node_and_add_to_list(
        &g_c_token_list_head,
        token_string, 
        token_type, 
        text_offset,
        line_index,
        line_column);
}
STRU_C_TOKEN_NODE *s_cproc_token_get_next_node(STRU_C_TOKEN_NODE *p_token_list_node)
{
    return s_ctoken_get_next_node(&g_c_token_list_head, p_token_list_node);
}

STRU_C_TOKEN_NODE *s_cproc_token_get_last_token(_VOID)
{
    return s_ctoken_get_last_node(&g_c_token_list_head, NEXT_TOKEN(&g_c_token_list_head), PREV_TOKEN(&g_c_token_list_head));
}

STRU_C_TOKEN_NODE *s_cproc_token_get_last_last_token(_VOID)
{
    return s_ctoken_get_last_last_node(&g_c_token_list_head, NEXT_TOKEN(&g_c_token_list_head), PREV_TOKEN(&g_c_token_list_head));
}

STRU_C_TOKEN_NODE *s_cproc_token_get_last_node_by_type(ENUM_C_TOKEN token_type)
{
    return s_ctoken_get_last_node_by_type(token_type, &g_c_token_list_head, NEXT_TOKEN(&g_c_token_list_head), PREV_TOKEN(&g_c_token_list_head));
}

STRU_C_TOKEN_NODE *s_cproc_token_get_last_node_by_type_after_node(ENUM_C_TOKEN token_type, STRU_C_TOKEN_NODE *p_token_list_node)
{
    return s_ctoken_get_last_node_by_type_after_node(token_type, &g_c_token_list_head,p_token_list_node);
}

_VOID s_cproc_token_list_init(_VOID)
{
    s_ctoken_init_head(&g_c_token_list_head, "c-token-head");
}

_VOID s_cproc_token_print_list_debug_info(_VOID)
{
    s_ctoken_print_list_debug_info(&g_c_token_list_head, NEXT_TOKEN(&g_c_token_list_head), PREV_TOKEN(&g_c_token_list_head));
}

_VOID s_cproc_token_print_list(    FILE *pfw)
{
    printf("\n\nprint tokens to output file%s start...\n", pfw==NULL?"(NULL)":"");
	s_ctoken_print_list(pfw, &g_c_token_list_head, NEXT_TOKEN(&g_c_token_list_head), PREV_TOKEN(&g_c_token_list_head));
    printf("\nprint tokens to output file finish.\n");
}

_VOID s_cproc_token_release_list(_VOID)
{
    s_ctoken_release_list(&g_c_token_list_head);
}

_VOID s_cproc_token_delete_blanks_and_newline(_VOID)
{
    s_ctoken_delete_blanks_and_newline_from_list(&g_c_token_list_head);
}

_VOID s_cproc_token_release_list_after_node(STRU_C_TOKEN_NODE *p_token_list_node)
{
    s_ctoken_release_list_after_node(&g_c_token_list_head, p_token_list_node);
}

STRU_C_TOKEN_NODE *s_cproc_token_get_last_newline(_VOID)
{
    return s_cproc_token_get_last_node_by_type(C_TOKEN_NEWLINE);
}

_VOID s_cproc_token_release_list_after_last_newline(_VOID)
{
    DEBUG_PRINT("delete tokens after last newline");
    
    STRU_C_TOKEN_NODE *p_token_last_newline = s_cproc_token_get_last_newline();

    if(p_token_last_newline == NULL)
    {
        p_token_last_newline = s_cproc_token_get_list_head();
    }

    s_cproc_token_release_list_after_node(p_token_last_newline);
}

ENUM_RETURN s_cproc_token_mod_last_node_type(ENUM_C_TOKEN new_type)
{
    STRU_C_TOKEN_NODE *p_token_last =  s_cproc_token_get_last_token();
    S_R_ASSERT(p_token_last != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_token_last->info.token_type != C_TOKEN_LIST_HEAD, RETURN_FAILURE);
    
    return s_ctoken_mod_node_type(p_token_last, new_type);
}

ENUM_RETURN s_cproc_token_mod_last_node_string(const _S8 *p_new_string)
{
    STRU_C_TOKEN_NODE *p_token_last =  s_cproc_token_get_last_token();
    S_R_ASSERT(p_token_last != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_new_string != NULL, RETURN_FAILURE);
    
    return s_ctoken_mod_node_string(p_token_last, p_new_string);
}

ENUM_RETURN s_cproc_token_mod_last_node_string_and_type(const _S8 *p_new_string, ENUM_C_TOKEN new_type)
{
    STRU_C_TOKEN_NODE *p_token_last =  s_cproc_token_get_last_token();
    S_R_ASSERT(p_token_last != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_new_string != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_token_last->info.token_type != C_TOKEN_LIST_HEAD, RETURN_FAILURE);
    
    return s_ctoken_mod_node_string_and_type(p_token_last, p_new_string, new_type);
}

ENUM_RETURN s_cproc_token_merge_last_2_nodes(ENUM_C_TOKEN new_type)
{
    S_R_ASSERT(new_type >= C_TOKEN_NORMAL && new_type < C_TOKEN_MAX && C_TOKEN_LIST_HEAD != new_type, RETURN_FAILURE);
    DEBUG_PRINT("merge last 2 tokens");
    ENUM_RETURN ret_val;
    STRU_C_TOKEN_NODE *p_token_last =  s_cproc_token_get_last_token();
    S_R_ASSERT(p_token_last != NULL, RETURN_FAILURE);

    STRU_C_TOKEN_NODE *p_token_last_last =  s_cproc_token_get_last_last_token();
    S_R_ASSERT(p_token_last_last != NULL, RETURN_FAILURE);

    STRU_C_TOKEN_NODE *p_token_new = NULL;
    ret_val = s_ctoken_copy(p_token_last_last, &p_token_new);
    S_R_ASSERT(p_token_new != NULL, RETURN_FAILURE);

    S_FREE(p_token_new->info.p_string);
    p_token_new->info.p_string = s_concatenate_string(p_token_last_last->info.p_string, p_token_last->info.p_string);
    S_R_ASSERT_DO(p_token_new->info.p_string != NULL, RETURN_FAILURE, s_ctoken_free_node(p_token_new));
    p_token_new->info.token_type = new_type;

    ret_val = s_ctoken_delete_node_from_list(&g_c_token_list_head, p_token_last);
    S_R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, s_ctoken_free_node(p_token_new));

    ret_val = s_ctoken_delete_node_from_list(&g_c_token_list_head, p_token_last_last);
    S_R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, s_ctoken_free_node(p_token_new));

    ret_val = s_ctoken_add_node_to_list(&g_c_token_list_head, p_token_new);
    S_R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, s_ctoken_free_node(p_token_new));
    
    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_token_merge_last_3_nodes(ENUM_C_TOKEN new_type)
{
    S_R_ASSERT(new_type >= C_TOKEN_NORMAL && new_type < C_TOKEN_MAX && C_TOKEN_LIST_HEAD != new_type, RETURN_FAILURE);
    DEBUG_PRINT("merge last 3 tokens");
    ENUM_RETURN ret_val;
    ret_val = s_cproc_token_merge_last_2_nodes(new_type);
    S_R_ASSERT(ret_val != RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = s_cproc_token_merge_last_2_nodes(new_type);
    S_R_ASSERT(ret_val != RETURN_SUCCESS, RETURN_FAILURE);
    
    return RETURN_SUCCESS;
}

ENUM_BOOLEAN s_cproc_token_all_blank_in_line(_VOID)
{
    STRU_C_TOKEN_NODE *p_token_last_newline = s_cproc_token_get_last_newline();

    if(p_token_last_newline == NULL)
    {
        p_token_last_newline = s_cproc_token_get_list_head();
    }

    return s_ctoken_all_same_type_after_node(&g_c_token_list_head, p_token_last_newline, C_TOKEN_BLANK);
}

ENUM_RETURN s_cproc_token_move_replacement_list_to_another_list(
    STRU_C_TOKEN_NODE *p_dest_token_list_head,
    STRU_C_TOKEN_NODE *p_dest_token_list_node)
{
    
    STRU_C_TOKEN_NODE *p_token_replacement_list_head = s_cproc_token_get_last_node_by_type(C_TOKEN_PP_PARAMETER_FINISH);
    if(p_token_replacement_list_head == NULL)
    {
        p_token_replacement_list_head = s_cproc_token_get_last_node_by_type(C_TOKEN_PP_MACRO);
    }
    S_R_ASSERT(p_token_replacement_list_head != NULL, RETURN_FAILURE);
    
    return s_ctoken_move_list_to_another_list(&g_c_token_list_head, NEXT_TOKEN(p_token_replacement_list_head), 
        PREV_TOKEN(&g_c_token_list_head),p_dest_token_list_head, p_dest_token_list_node);
}

