#include <stdio.h>

#include "s_log.h"

#include "s_cmd.h"

#include "s_subcmd.h"
#include "s_option.h"
#include "s_error.h"

#include "s_default.h"

PRIVATE void display_all_subcmd_help_info(void)
{
    STRU_SUBCMD_CONTROL_BLOCK *p = get_subcmd_cb_list_head();

    while(p != NULL)
    {
        printf("   %-10s %s\n", p->subcmd, p->help_info);

        p = p->next;
    }
}

PRIVATE void display_option_help_info(STRU_OPTION_CONTROL_BLOCK *p_option_cb)
{
    while(p_option_cb != NULL)
    {
        printf("    %-20s %s\n", p_option_cb->option, p_option_cb->help_info);

        p_option_cb = p_option_cb->next;
    }
}

PRIVATE void display_subcmd_help_info(const char *subcmd_name)
{
    V_ASSERT(subcmd_name != NULL);
    
    STRU_SUBCMD_CONTROL_BLOCK *p_subcmd_cb = NULL;
    
    p_subcmd_cb = get_subcmd_cb_by_name(subcmd_name);
    V_ASSERT(p_subcmd_cb != NULL);

    printf("   %-10s %s\n", p_subcmd_cb->subcmd, p_subcmd_cb->help_info);

    display_option_help_info(p_subcmd_cb->option_cbs);
}

PRIVATE ENUM_RETURN display_help_info(STRU_OPTION_RUN_BLOCK *p_option_rb)
{
    printf("       %s\n", get_introduction());
    printf("Usage: %s %s\n", get_bin_name(), get_usage());

    if(p_option_rb == NULL)
    {
        display_all_subcmd_help_info();
    }
    else
    {
        while(p_option_rb != NULL)
        {
            display_subcmd_help_info(p_option_rb->arg->value);
            p_option_rb = p_option_rb->next;
        }
    }
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN help_option_proc_handler(STRU_ARG *arg)
{
    R_ASSERT(arg != NULL, RETURN_FAILURE);
    R_ASSERT(arg->value != NULL, RETURN_FAILURE);

    ENUM_RETURN ret_val = RETURN_SUCCESS;
    STRU_SUBCMD_CONTROL_BLOCK *p_subcmd_cb = get_subcmd_cb_by_name(arg->value);
    if(p_subcmd_cb == NULL)
    {
        ret_val = add_current_system_error(ERROR_CODE_UNKONWN_SUBCMD, arg->value);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        return RETURN_FAILURE;
    }
    
    return RETURN_SUCCESS;
}

ENUM_RETURN default_option_proc_handler(STRU_ARG *arg)
{
    R_LOG("Do nothing..");
    return RETURN_SUCCESS;
}

ENUM_RETURN register_subcmd_help(void)
{
    ENUM_RETURN ret_val = register_subcmd("help", BOOLEAN_FALSE, display_help_info, "display help information");
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = register_option("help", "-s", OPTION_TYPE_OPTIONAL, ARG_TYPE_DATA, help_option_proc_handler, "display help information of specify subcmd");
    return RETURN_SUCCESS;
}

