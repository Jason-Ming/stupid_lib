#ifndef __S_CPROC_TOKEN_H__
#define __S_CPROC_TOKEN_H__
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"

#include "s_cproc_parse.h"

#define S_CPROC_LAST_TOKEN_POINTER (s_cproc_token_get_last_token())
#define S_CPROC_LAST_TOKEN_C_POSITION (S_CPROC_LAST_TOKEN_POINTER == NULL?NULL:&S_CPROC_LAST_TOKEN_POINTER->info.c_position)
#define S_CPROC_LAST_TOKEN_STRING (S_CPROC_LAST_TOKEN_POINTER==NULL?"NULL":S_CPROC_LAST_TOKEN_POINTER->info.p_string)
#define S_CPROC_LAST_TOKEN_TYPE (S_CPROC_LAST_TOKEN_POINTER==NULL?C_TOKEN_INVALID:S_CPROC_LAST_TOKEN_POINTER->info.token_type)

#define S_CPROC_LAST_LAST_TOKEN_POINTER (s_cproc_token_get_last_last_token())
#define S_CPROC_LAST_LAST_TOKEN_C_POSITION (S_CPROC_LAST_LAST_TOKEN_POINTER == NULL?NULL:&S_CPROC_LAST_LAST_TOKEN_POINTER->info.c_position)
#define S_CPROC_LAST_LAST_TOKEN_STRING (S_CPROC_LAST_LAST_TOKEN_POINTER==NULL?"NULL":S_CPROC_LAST_LAST_TOKEN_POINTER->info.p_string)
#define S_CPROC_LAST_LAST_TOKEN_TYPE (S_CPROC_LAST_LAST_TOKEN_POINTER==NULL?C_TOKEN_INVALID:S_CPROC_LAST_LAST_TOKEN_POINTER->info.token_type)

__BEGIN_C_DECLS
ENUM_RETURN s_cproc_token_add_node_to_list(
    const _S8 *token_string, 
    ENUM_C_TOKEN token_type, 
    size_t text_offset,
    size_t line_index,
    size_t line_column);

STRU_C_TOKEN_NODE *s_cproc_token_get_list_head(_VOID);
_VOID s_cproc_token_init_list(_VOID);
_VOID s_cproc_token_print_list_debug_info(_VOID);
_VOID s_cproc_token_print_list(    FILE *pfw);

_VOID s_cproc_token_release_list(_VOID);
_VOID s_cproc_token_delete_node(STRU_C_TOKEN_NODE *p_token_node);
_VOID s_cproc_token_delete_last_node(_VOID);
_VOID s_cproc_token_delete_blanks_and_newline(_VOID);
_VOID s_cproc_token_release_list_after_node(STRU_C_TOKEN_NODE *p_token_list_node);
_VOID s_cproc_token_release_list_after_last_newline(_VOID);

STRU_C_TOKEN_NODE *s_cproc_token_get_next_node(STRU_C_TOKEN_NODE *p_token_list_node);

STRU_C_TOKEN_NODE *s_cproc_token_get_last_newline(_VOID);
STRU_C_TOKEN_NODE *s_cproc_token_get_last_token(_VOID);
STRU_C_TOKEN_NODE *s_cproc_token_get_last_last_token(_VOID);

STRU_C_TOKEN_NODE *s_cproc_token_get_last_node_by_type(ENUM_C_TOKEN token_type);
STRU_C_TOKEN_NODE *s_cproc_token_get_last_node_by_type_after_node(
    ENUM_C_TOKEN token_type, STRU_C_TOKEN_NODE *p_token_list_node);

ENUM_RETURN s_cproc_token_mod_last_node_type(ENUM_C_TOKEN new_type);
ENUM_RETURN s_cproc_token_mod_last_node_string(const _S8 *p_new_string);
ENUM_RETURN s_cproc_token_mod_last_node_string_and_type(const _S8 *p_new_string, ENUM_C_TOKEN new_type);

ENUM_RETURN s_cproc_token_merge_last_2_nodes(ENUM_C_TOKEN new_type);
ENUM_RETURN s_cproc_token_merge_last_3_nodes(ENUM_C_TOKEN new_type);
ENUM_BOOLEAN s_cproc_token_all_blank_in_line(_VOID);


__END_C_DECLS
#endif

