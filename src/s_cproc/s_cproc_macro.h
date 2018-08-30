#ifndef __S_CPROC_MACRO_H__
#define __S_CPROC_MACRO_H__
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"

#include "s_cproc.h"

__BEGIN_C_DECLS
_VOID s_cproc_macro_list_init(_VOID);

_VOID s_cproc_macro_print_list_debug_info(_VOID);

ENUM_RETURN s_cproc_macro_name_exist(_S8 *p_macro_name, ENUM_BOOLEAN *exist);
ENUM_BOOLEAN s_cproc_macro_parameter_part_exist();

_VOID s_cproc_macro_release_list(_VOID);

ENUM_RETURN s_cproc_macro_add_name(
    const STRU_C_TOKEN_NODE* p_macro_name_token_node);

ENUM_RETURN s_cproc_macro_remove_name(
    const STRU_C_TOKEN_NODE* p_macro_name_token_node);


ENUM_RETURN s_cproc_macro_add_parameter_separator_comma(ENUM_RETURN *result);

ENUM_RETURN s_cproc_macro_add_parameter(
    STRU_C_TOKEN_NODE* p_macro_parameter_token_node);

ENUM_RETURN s_cproc_macro_finish_parameter(ENUM_RETURN *p_check_result);
ENUM_RETURN s_cproc_macro_finish_replacement();

__END_C_DECLS

#endif
