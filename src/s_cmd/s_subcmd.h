#ifndef __S_SUBCMD_H__
#define __S_SUBCMD_H__
#include "s_clinkage.h"
#include "s_defines.h"

#include "s_cmd.h"
#include "s_option.h"

typedef struct TAG_STRU_SUBCMD_CONTROL_BLOCK
{
    char* subcmd;
    ENUM_BOOLEAN need_input_file;
    FUNC_SUBCMD_PROC handler;
    struct TAG_STRU_OPTION_CONTROL_BLOCK *option_cbs;
    char* help_info;
    struct TAG_STRU_SUBCMD_CONTROL_BLOCK *next;
} STRU_SUBCMD_CONTROL_BLOCK;

__BEGIN_C_DECLS

STRU_SUBCMD_CONTROL_BLOCK *get_subcmd_cb_list_head(void);
STRU_SUBCMD_CONTROL_BLOCK *get_subcmd_cb_by_name(const char* subcmd_name);
ENUM_BOOLEAN is_subcmd_registered(const char *subcmd_name);
ENUM_BOOLEAN is_subcmd_need_input_files(const char *subcmd_name);

void debug_print_subcmd_cb_list(void);

const char *get_current_subcmd_name(void);

STRU_OPTION_CONTROL_BLOCK *get_option_cb_list_head(const char *subcmd_name);
ENUM_RETURN add_a_new_option_cb_to_subcmd_cb(STRU_OPTION_CONTROL_BLOCK *p_new);

STRU_OPTION_RUN_BLOCK *get_option_rb_list_head(const char *subcmd_name);
ENUM_RETURN add_a_new_option_rb_to_subcmd_rb(STRU_OPTION_RUN_BLOCK *p_new);

ENUM_RETURN parse_subcmds(int argc, char **argv);
ENUM_RETURN parse_options(int argc, char **argv);
ENUM_RETURN process_subcmd(void);

__END_C_DECLS

#endif

