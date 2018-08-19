#ifndef __S_CPROC_TEXT_H__
#define __S_CPROC_TEXT_H__
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"

__BEGIN_C_DECLS
_VOID s_cproc_text_list_init(_VOID);

ENUM_RETURN s_cproc_text_read_file_to_buffer(const _S8 *p_filename, _S8 **pp_text_buffer);
const _S8 * s_cproc_text_get_buffer_by_filename(const _S8 *p_filename);
_VOID s_cproc_text_release_list(_VOID);
ENUM_RETURN s_cproc_text_print_list_debug_info(_VOID);

__END_C_DECLS

#endif

