#ifndef __S_CPROC_STM_H__
#define __S_CPROC_STM_H__
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"

#include "s_cpp.h"
#include "s_ctoken.h"

__BEGIN_C_DECLS

ENUM_RETURN s_cproc_stm(
	const _S8 *filename, 
	const _S8 *p_text_buffer, 
	const size_t text_buffer_size,
	STRU_C_TOKEN_NODE **pp_token_list_head, 
    STRU_C_TOKEN_NODE **pp_token_list_tail,
	ENUM_RETURN *check_result);

_VOID s_cproc_generate_error();
const _S8 *s_cproc_get_text_buffer();
const _S8 *s_cproc_get_filename();
size_t s_cproc_get_current_offset();
size_t s_cproc_get_current_line();
size_t s_cproc_get_current_column();

__END_C_DECLS
#endif

