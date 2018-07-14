#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "s_log.h"
#include "s_cmd.h"

#include "s_cmd_proc.h"
#include "s_option.h"
#include "s_error.h"
#include "s_input_file.h"
#include "s_default.h"
#include "s_subcmd.h"

PRIVATE const char *p_bin = NULL;
PRIVATE char *p_usage = NULL;
PRIVATE char *p_introduction = NULL;

PRIVATE int argv_indicator = 0;

int get_argv_indicator(void)
{
    return argv_indicator;
}

void set_argv_indicator(int new_value)
{
    argv_indicator = new_value;
}

PRIVATE void save_bin_name(const char *bin_name)
{
    p_bin = bin_name;
}

const char *get_bin_name(void)
{
    return p_bin;
}

const char *get_introduction(void)
{
    return p_introduction;
}

const char *get_usage(void)
{
    return p_usage;
}

ENUM_RETURN register_introduction(const char *introduction)
{
    R_ASSERT(introduction != NULL, RETURN_FAILURE);

    p_introduction = malloc(strlen(introduction) + 1);
    R_ASSERT(p_introduction != NULL, RETURN_FAILURE);
    strcpy(p_introduction, introduction);
    
    return RETURN_SUCCESS;
}

/* Usage format: cmd <sub-command> [<input files>] [<options> [<args>]]*/
ENUM_RETURN register_usage(const char *usage)
{
    R_ASSERT(usage != NULL, RETURN_FAILURE);

    p_usage = malloc(strlen(usage) + 1);
    R_ASSERT(p_usage != NULL, RETURN_FAILURE);
    strcpy(p_usage, usage);
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN register_default_data(void)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    ret_val = register_usage("<sub command> [<option> [<arg>]] [<input file>] ");
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = register_default_subcmd();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN prepare(const char * bin_name)
{
    save_bin_name(bin_name);

    ENUM_RETURN ret_val = RETURN_SUCCESS;

    ret_val = init_error_info();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    ret_val = register_default_data();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN process_do(void)
{
    ENUM_RETURN ret_val;

    /* do noting when there is any error */
    R_FALSE_RET_LOG(BOOLEAN_FALSE == whether_any_error_exists(), RETURN_SUCCESS, "");
    
    ret_val = process_subcmds();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN process_errors(void)
{
    if(whether_any_error_exists() == BOOLEAN_TRUE)
    {
        display_error_info();

        display_current_running_subcmd_help_info();
        
        return RETURN_FAILURE;
    }

    display_success_info();
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN parse_args_do(int argc, char **argv)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    
    ret_val = parse_subcmds(argc, argv);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = parse_options(argc, argv);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    ret_val = parse_input_files(argc, argv);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN parse_args(int argc, char **argv)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;

    /* 从argv[1]开始处理 */
    set_argv_indicator(1);
    
    R_LOG("total args: %d", argc);
    
    ret_val = parse_args_do(argc, argv);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

ENUM_RETURN process(int argc, char **argv)
{
    R_ASSERT(argc >= 1 && argv[0] != NULL, RETURN_FAILURE);

    ENUM_RETURN ret_val = RETURN_SUCCESS;

    ret_val = prepare(argv[0]);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = parse_args(argc, argv);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = process_do();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    return process_errors();
}

