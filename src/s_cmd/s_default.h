#ifndef __S_DEFAULT_H__
#define __S_DEFAULT_H__
#include "s_clinkage.h"
#include "s_defines.h"

__BEGIN_C_DECLS
ENUM_RETURN register_default_option(const char *subcmd_name);
ENUM_RETURN register_default_subcmd(void);
__END_C_DECLS

#endif

