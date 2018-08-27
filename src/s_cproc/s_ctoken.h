#ifndef __S_C_TOKEN_H__
#define __S_C_TOKEN_H__
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"
#include "s_list.h"
#include "s_cproc.h"
#include "s_cproc_parse.h"

#define MAX_TOKEN_LEN 1024
#define TOKEN_INFO_FORMAT "[%p-"REVERSE"%s"NONE"-%s]"
#define TOKEN_INFO_VALUE(p_token) p_token, p_token->info.p_string, s_ctoken_get_str(p_token->info.token_type)

#define NEXT_TOKEN(p_token_node) (list_entry((p_token_node)->list.next, STRU_C_TOKEN_NODE, list))
#define PREV_TOKEN(p_token_node) (list_entry((p_token_node)->list.prev, STRU_C_TOKEN_NODE, list))

typedef struct TAG_STRU_C_POSITION
{
	size_t offset;
    size_t line;
    size_t column;
}STRU_C_POSITION;

typedef struct TAG_STRU_C_TYPE_QUALIFIER
{
    ENUM_BOOLEAN is_const;
    ENUM_BOOLEAN is_auto;
    ENUM_BOOLEAN is_extern;
    ENUM_BOOLEAN is_register;
    ENUM_BOOLEAN is_static;
    ENUM_BOOLEAN is_volatile;
}STRU_C_TYPE_QUALIFIER;

typedef struct TAG_STRU_C_TOKEN
{
    _S8 *p_string;
    _SL inode;
    STRU_C_POSITION c_position;
    ENUM_C_TOKEN token_type;
    STRU_C_TYPE_QUALIFIER qualifier;
}STRU_C_TOKEN;

typedef struct TAG_STRU_C_TOKEN_NODE
{
	STRU_C_TOKEN info;
    struct list_head list;
}STRU_C_TOKEN_NODE;

__BEGIN_C_DECLS

_VOID s_ctoken_init_head(STRU_C_TOKEN_NODE *p_head, _S8 * head_name);

const _S8 * s_ctoken_get_str(ENUM_C_TOKEN token);
ENUM_RETURN s_ctoken_copy(
    const STRU_C_TOKEN_NODE *p_token_node_source, 
    STRU_C_TOKEN_NODE **pp_token_node_dest);

ENUM_RETURN s_ctoken_make_new(
    const _S8 *token_string, 
    ENUM_C_TOKEN token_type, 
    size_t text_offset,
    size_t line_index,
    size_t line_column,
    STRU_C_TOKEN_NODE **pp_new_token_node);

ENUM_RETURN s_ctoken_make_new_node_and_add_to_list(
    STRU_C_TOKEN_NODE *p_token_list_head,
    const _S8 *token_string, 
    ENUM_C_TOKEN token_type, 
    size_t text_offset,
    size_t line_index,
    size_t line_column);

ENUM_RETURN s_ctoken_add_node_to_list(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_new_token_node);

ENUM_RETURN s_ctoken_delete_node_from_list(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_to_be_deleted);

ENUM_RETURN s_ctoken_get_token_type(const _S8 *p_string, ENUM_C_TOKEN *p_type);
ENUM_RETURN s_ctoken_get_token(
	const _S8 *p_text_buffer, 
	size_t *len, 
	ENUM_C_TOKEN *token);
_VOID s_ctoken_delete_blanks_and_newline_from_list(STRU_C_TOKEN_NODE *p_token_list_head);

_VOID s_ctoken_release_list(STRU_C_TOKEN_NODE *p_token_list_head);
_VOID s_ctoken_release_list_after_node(STRU_C_TOKEN_NODE *p_token_list_head, 
    STRU_C_TOKEN_NODE *p_token_list_node);

_VOID s_ctoken_free_node(STRU_C_TOKEN_NODE *p_token_to_be_deleted);

_VOID s_ctoken_print_list(
    FILE *fpw,
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_start, 
    STRU_C_TOKEN_NODE *p_token_list_end);

_VOID s_ctoken_print_list_debug_info(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_start, 
    STRU_C_TOKEN_NODE *p_token_list_end);

STRU_C_TOKEN_NODE * s_ctoken_get_last_node_by_type(
    ENUM_C_TOKEN token_type,
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_start, 
    STRU_C_TOKEN_NODE *p_token_list_end);

STRU_C_TOKEN_NODE *s_ctoken_get_last_node_by_name(
    const _S8 *p_string, 
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_start, 
    STRU_C_TOKEN_NODE *p_token_list_end);

STRU_C_TOKEN_NODE * s_ctoken_get_last_node_by_type_after_node(
    ENUM_C_TOKEN token_type,
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_node);

ENUM_RETURN s_ctoken_mod_node_type(STRU_C_TOKEN_NODE *p_token, ENUM_C_TOKEN new_type);
ENUM_RETURN s_ctoken_mod_node_string(STRU_C_TOKEN_NODE *p_token, const _S8*p_new_string);
ENUM_RETURN s_ctoken_mod_node_string_and_type(
    STRU_C_TOKEN_NODE *p_token, const _S8*p_new_string, ENUM_C_TOKEN new_type);

STRU_C_TOKEN_NODE *s_ctoken_get_last_node(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_start, 
    STRU_C_TOKEN_NODE *p_token_list_end);
STRU_C_TOKEN_NODE *s_ctoken_get_last_last_node(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_start, 
    STRU_C_TOKEN_NODE *p_token_list_end);

STRU_C_TOKEN_NODE *s_ctoken_get_next_node(
    STRU_C_TOKEN_NODE *p_token_list_head, 
    STRU_C_TOKEN_NODE *p_token_list_node);

ENUM_BOOLEAN s_ctoken_all_same_type_after_node(
    STRU_C_TOKEN_NODE *p_token_list_head, 
    STRU_C_TOKEN_NODE *p_token_list_node,
    ENUM_C_TOKEN token_type);

ENUM_RETURN s_ctoken_move_list_to_another_list(
    STRU_C_TOKEN_NODE *p_source_token_list_head, 
    STRU_C_TOKEN_NODE *p_source_token_list_start,
    STRU_C_TOKEN_NODE *p_source_token_list_end,
    STRU_C_TOKEN_NODE *p_dest_token_list_head,
    STRU_C_TOKEN_NODE *p_dest_token_list_node);


__END_C_DECLS
#endif

