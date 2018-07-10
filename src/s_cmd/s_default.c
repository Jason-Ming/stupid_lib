#include <stdio.h>

#include "s_log.h"

#include "s_cmd.h"

#include "s_cmd_proc.h"
#include "s_subcmd.h"
#include "s_option.h"
#include "s_error.h"

#include "s_default.h"

PRIVATE ENUM_RETURN subcmd_help_proc(STRU_OPTION_RUN_BLOCK *p_option_rb)
{
    if(p_option_rb == NULL)
    {
        display_all_subcmd_help_info();
    }
    
    return RETURN_SUCCESS;
}

ENUM_RETURN default_option_proc_handler(STRU_ARG *arg)
{
    R_LOG("Do nothing..");
    return RETURN_SUCCESS;
}

ENUM_RETURN default_subcmd_proc_handler(STRU_OPTION_RUN_BLOCK *value)
{
    if(is_current_subcmd_option_rb_existed() == BOOLEAN_FALSE)
    {
        display_subcmd_help_info(get_current_subcmd_name());
    }
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN default_option_h_proc_handler(STRU_ARG *arg)
{
    display_subcmd_help_info(get_current_subcmd_name());
    
    return RETURN_SUCCESS;
}

ENUM_RETURN register_default_option(const char *subcmd_name)
{
    R_ASSERT(subcmd_name != NULL, RETURN_FAILURE);
    
    ENUM_RETURN ret_val = register_option(
        subcmd_name, 
        "-h",
        BOOLEAN_FALSE,
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
        BOOLEAN_FALSE,
        subcmd_help_proc, 
        "display help information");
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

