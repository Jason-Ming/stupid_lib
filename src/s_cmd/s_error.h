#ifndef __S_ERROR_H__
#define __S_ERROR_H__
#include "s_clinkage.h"
#include "s_defines.h"


__BEGIN_C_DECLS
ENUM_RETURN register_error_info(int code, const char * info);
ENUM_RETURN set_error(int code);
void print_error_info(void);

__END_C_DECLS

#endif

