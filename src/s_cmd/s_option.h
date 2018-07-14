#ifndef __S_OPTION_H__
#define __S_OPTION_H__
#include "s_clinkage.h"
#include "s_defines.h"

#include "s_cmd.h"

typedef struct TAG_STRU_OPTION_CONTROL_BLOCK
{
    char *subcmd;
    char* option;
    ENUM_BOOLEAN is_running;
    ENUM_OPTION_TYPE option_type;
    ENUM_ARG_TYPE arg_type;
    char* arg_value;
    FUNC_OPTION_PROC handler;
    ENUM_BOOLEAN finish_handle;
    char* help_info;
    struct TAG_STRU_OPTION_CONTROL_BLOCK *next;
} STRU_OPTION_CONTROL_BLOCK;

__BEGIN_C_DECLS
void debug_print_option_cb_list(STRU_OPTION_CONTROL_BLOCK *p);
void display_option_help_info(STRU_OPTION_CONTROL_BLOCK *p_option_cb);
ENUM_BOOLEAN whether_option_h_has_been_processed(void);
ENUM_BOOLEAN whether_string_is_in_option_format(const char *option_name);
_S32 get_current_running_option_number(const char* subcmd_name);

ENUM_RETURN parse_options(int argc, char **argv);
ENUM_RETURN process_options(_VOID);

__END_C_DECLS

#endif
