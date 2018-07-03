#ifndef __S_SUBCMD_H__
#define __S_SUBCMD_H__
#include "s_clinkage.h"
#include "s_defines.h"

#include "s_cmd.h"
#include "s_option.h"

__BEGIN_C_DECLS
void display_subcmd_help_info(const char *subcmd_name);
void display_all_subcmd_help_info(void);

ENUM_BOOLEAN is_subcmd_registered(const char *subcmd_name);

const char *get_current_subcmd_name(void);

STRU_OPTION_CONTROL_BLOCK *get_option_cb_list_head(const char *subcmd_name);
ENUM_RETURN add_a_new_option_cb_to_subcmd_cb(STRU_OPTION_CONTROL_BLOCK *p_new);

ENUM_RETURN add_a_new_option_rb_to_subcmd_rb(STRU_OPTION_RUN_BLOCK *p_new);

STRU_OPTION_RUN_BLOCK *get_option_rb_by_name(const char* subcmd_name, const char* option_name);

ENUM_RETURN check_missing_options_of_subcmd(const char *subcmd_name);

ENUM_RETURN parse_subcmds(int argc, char **argv);
ENUM_RETURN process_subcmds(void);

__END_C_DECLS

#endif

