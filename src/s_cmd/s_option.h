#ifndef __S_OPTION_H__
#define __S_OPTION_H__
#include "s_clinkage.h"
#include "s_defines.h"

#include "s_cmd.h"

typedef struct TAG_STRU_OPTION_CONTROL_BLOCK
{
    char *subcmd;
    char* option;
    ENUM_OPTION_TYPE option_type;
    ENUM_ARG_TYPE arg_type;
    FUNC_OPTION_PROC handler;
    char* help_info;
    struct TAG_STRU_OPTION_CONTROL_BLOCK *next;
} STRU_OPTION_CONTROL_BLOCK;

__BEGIN_C_DECLS
void debug_print_option_cb_list(STRU_OPTION_CONTROL_BLOCK *p);

ENUM_RETURN parse_options(int argc, char **argv);

ENUM_RETURN process_options(STRU_OPTION_RUN_BLOCK *p_option_rb);

__END_C_DECLS

#endif
