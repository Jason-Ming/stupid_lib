#include "s_log.h"
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

STRU_C_TOKEN_NODE *s_cproc_token_get_last_token(_VOID)
{
    STRU_C_TOKEN_NODE *p_token_temp = list_entry(g_c_token_list_head.list.prev, STRU_C_TOKEN_NODE, list);
    if(p_token_temp == &g_c_token_list_head)
    {
        return NULL;
    }
    else
    {
        return p_token_temp;
    }
}
STRU_C_TOKEN_NODE *s_cproc_token_get_last_node_by_type(ENUM_C_TOKEN token_type)
{
    return s_ctoken_get_last_node_by_type(token_type, &g_c_token_list_head,&g_c_token_list_head,&g_c_token_list_head);
}

_VOID s_cproc_token_list_init(_VOID)
{
    s_ctoken_init_head(&g_c_token_list_head, "c-token-head");
}

_VOID s_cproc_token_print_list_debug_info(_VOID)
{
    s_ctoken_print_list_debug_info(&g_c_token_list_head,&g_c_token_list_head,&g_c_token_list_head);
}

_VOID s_cproc_token_print_list(    FILE *pfw)
{
    printf("\n\nprint tokens to output file%s start...\n", pfw==NULL?"(NULL)":"");
	s_ctoken_print_list(pfw, &g_c_token_list_head,&g_c_token_list_head,&g_c_token_list_head);
    printf("print tokens to output file finish.\n");
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

_VOID s_cproc_token_release_list_after_last_newline(_VOID)
{
    DEBUG_PRINT("delete tokens after last newline");
    STRU_C_TOKEN_NODE *p_token_last_newline = s_cproc_token_get_last_node_by_type(C_TOKEN_NEWLINE_LINUX);
    if(p_token_last_newline == NULL)
    {
        p_token_last_newline = s_cproc_token_get_last_node_by_type(C_TOKEN_NEWLINE_WINDOWS);
    }

    if(p_token_last_newline == NULL)
    {
        p_token_last_newline = s_cproc_token_get_last_node_by_type(C_TOKEN_NEWLINE_MAC);
    }

    if(p_token_last_newline == NULL)
    {
        p_token_last_newline = s_cproc_token_get_list_head();
    }

    s_cproc_token_release_list_after_node(p_token_last_newline);
}

