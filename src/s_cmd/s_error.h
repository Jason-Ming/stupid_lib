#ifndef __S_ERROR_H__
#define __S_ERROR_H__
#include "s_clinkage.h"
#include "s_defines.h"

__BEGIN_C_DECLS
void display_success_info(void);
void display_generated_error_info(void);
ENUM_RETURN init_error_info(void);
ENUM_BOOLEAN whether_any_error_exists(void);


__END_C_DECLS

#endif

