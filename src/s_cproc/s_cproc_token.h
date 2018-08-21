#ifndef __S_CPROC_TOKEN_H__
#define __S_CPROC_TOKEN_H__
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"

__BEGIN_C_DECLS
ENUM_RETURN s_cproc_token_add_node_to_list(
    const _S8 *token_string, 
    ENUM_C_TOKEN token_type, 
    size_t text_offset,
    size_t line_index,
    size_t line_column);

STRU_C_TOKEN_NODE *s_cproc_token_get_list_head(_VOID);
_VOID s_cproc_token_list_init(_VOID);
_VOID s_cproc_token_print_list_debug_info(_VOID);
_VOID s_cproc_token_print_list(    FILE *pfw);
_VOID s_cproc_token_release_list(_VOID);
_VOID s_cproc_token_delete_blanks_and_newline(_VOID);
_VOID s_cproc_token_release_list_after_node(STRU_C_TOKEN_NODE *p_token_list_node);
_VOID s_cproc_token_release_list_after_last_newline(_VOID);

STRU_C_TOKEN_NODE *s_cproc_token_get_last_token(_VOID);
STRU_C_TOKEN_NODE *s_cproc_token_get_last_node_by_type(ENUM_C_TOKEN token_type);

__END_C_DECLS
#endif

