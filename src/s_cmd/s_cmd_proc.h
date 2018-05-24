#ifndef __S_CMD_PROC_H__
#define __S_CMD_PROC_H__
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_cmd.h"

__BEGIN_C_DECLS
const char *get_introduction(void);
const char *get_usage(void);

int get_argv_indicator(void);
void set_argv_indicator(int new_value);

__END_C_DECLS

#endif


