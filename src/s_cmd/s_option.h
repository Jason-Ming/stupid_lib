#ifndef __S_OPTION_H__
#define __S_OPTION_H__
#include "s_clinkage.h"
#include "s_defines.h"

#include "s_cmd.h"

__BEGIN_C_DECLS

ENUM_RETURN parse_options(int argc, char **argv);

void display_option_help_info(void);

ENUM_RETURN process_options(void);

__END_C_DECLS

#endif
