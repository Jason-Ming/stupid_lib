#ifndef __S_C_PROC_CTOKEN_PP_H__
#define __S_C_PROC_CTOKEN_PP_H__
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"

#include "s_cproc.h"
#include "s_ctoken.h"

#define C_TOKEN_IS_IDENTIFIER_STARTER(c) (c == '_' || IS_ALPHABET(c) || c == '$')
#define C_TOKEN_IS_IDENTIFIER_CHAR(c) (c == '_' || IS_ALPHABET(c) || c == '$' || IS_DEC(c))

__BEGIN_C_DECLS
ENUM_RETURN s_cproc_get_ctoken_pp_keyword(const _S8 *p_string, size_t len, ENUM_C_TOKEN *token_type);

__END_C_DECLS


#endif

