#ifndef __S_CPROC_IDENTIFIER_H__
#define __S_CPROC_IDENTIFIER_H__
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"


__BEGIN_C_DECLS
ENUM_RETURN s_cproc_identifier_init(_VOID);
ENUM_RETURN s_cproc_identifier_release(_VOID);
ENUM_RETURN s_cproc_identifier_insert(PSTR p_string);
ENUM_RETURN s_cproc_identifier_print(_VOID);



__END_C_DECLS

#endif

