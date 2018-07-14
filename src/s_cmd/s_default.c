#include <stdio.h>

#include "s_log.h"

#include "s_cmd.h"

#include "s_cmd_proc.h"
#include "s_subcmd.h"
#include "s_option.h"
#include "s_error.h"

#include "s_default.h"

PRIVATE ENUM_RETURN subcmd_help_proc(_VOID)
{
    display_all_subcmd_help_info();
    
    return RETURN_SUCCESS;
}

ENUM_RETURN default_option_proc_handler(const char *arg)
{
    R_LOG("Do nothing..");
    return RETURN_SUCCESS;
}

ENUM_RETURN default_subcmd_proc_handler(_VOID)
{
    if(get_current_running_option_number(get_current_running_subcmd_name()) == 0)
    {
        printf(YELLOW"This is the default sub command handler, did you miss any option?\n");
        printf("See '%s %s -h'.\n"NONE, get_bin_name(), get_current_running_subcmd_name());
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN default_option_h_proc_handler(const char *arg)
{
    display_current_running_subcmd_help_info();
    
    return RETURN_SUCCESS;
}

ENUM_RETURN register_default_option(const char *subcmd_name)
{
    R_ASSERT(subcmd_name != NULL, RETURN_FAILURE);
    
    ENUM_RETURN ret_val = register_option(
        subcmd_name, 
        "-h",
        OPTION_TYPE_OPTIONAL,
        ARG_TYPE_SWITCH,
        default_option_h_proc_handler,
        BOOLEAN_TRUE,
        "display help information of this subcmd");
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

ENUM_RETURN register_default_subcmd(void)
{
    ENUM_RETURN ret_val = register_subcmd(
        "help", 
        subcmd_help_proc, 
        "display help information");
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

