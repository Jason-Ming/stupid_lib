#ifndef __S_ERROR_H__
#define __S_ERROR_H__
#include "s_clinkage.h"
#include "s_defines.h"

__BEGIN_C_DECLS
void display_error_info(void);
ENUM_RETURN init_error_info(void);
ENUM_BOOLEAN is_current_error_exist(void);
ENUM_RETURN add_current_system_error(int code, const char* additional_info);

__END_C_DECLS

#endif

