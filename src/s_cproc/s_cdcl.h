#ifndef __S_CDCL_H__
#define __S_CDCL_H__
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"

typedef enum TAG_ENUM_DCL_TYPE
{
    DCL_TYPE_ARRAY = 0,
    DCL_TYPE_POINTER,
    DCL_TYPE_FUNCTION,
    DCL_TYPE_STRUCT,
    DCL_TYPE_ENUM,
    DCL_TYPE_UNION,
    DCL_TYPE_NORMAL,/* system type like int, char... */
    DCL_TYPE_INVALID,
    DCL_TYPE_MAX,
}ENUM_DCL_TYPE;

__BEGIN_C_DECLS
const _S8 * get_dcl_type_str(ENUM_DCL_TYPE type);

ENUM_RETURN s_cdcl(
	_S8 *p_text_buffer,
	STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_tail);

__END_C_DECLS
#endif


