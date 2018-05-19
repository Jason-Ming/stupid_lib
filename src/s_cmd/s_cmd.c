#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "s_log.h"
#include "s_cmd.h"
#include "s_option.h"
#include "s_error.h"
#include "s_input_file.h"

PRIVATE const char *p_bin = NULL;
PRIVATE char *p_usage = NULL;
PRIVATE char *p_introduction = NULL;
PRIVATE FUNC_MAIN_PROC p_main_proc = NULL;

PRIVATE void save_bin_name(const char *bin_name)
{
    p_bin = bin_name;
}

PRIVATE const char *get_bin_name(void)
{
    return p_bin;
}

ENUM_RETURN register_introduction(const char *introduction)
{
    R_ASSERT(introduction != NULL, RETURN_FAILURE);

    p_introduction = malloc(strlen(introduction) + 1);
    R_ASSERT(p_introduction != NULL, RETURN_FAILURE);
    strcpy(p_introduction, introduction);
    
    return RETURN_SUCCESS;
}

ENUM_RETURN register_usage(const char *usage)
{
    R_ASSERT(usage != NULL, RETURN_FAILURE);

    p_usage = malloc(strlen(usage) + 1);
    R_ASSERT(p_usage != NULL, RETURN_FAILURE);
    strcpy(p_usage, usage);
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN display_help_info(struct TAG_STRU_ARG *value)
{
    printf("       %s\n", p_introduction);
    printf("Usage: %s %s\n", get_bin_name(), p_usage);

    display_option_help_info();
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN register_option_help(void)
{
    ENUM_RETURN ret_val = register_option("-h", OPTION_TYPE_OPTIONAL, ARG_TYPE_SWITCH, display_help_info, "display this information");
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN register_default_data(void)
{
    ENUM_RETURN ret_val;
    ret_val = register_option_help();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = init_error_info();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN prepare(const char * bin_name)
{
    save_bin_name(bin_name);
    
    ENUM_RETURN ret_val;
    ret_val = register_default_data();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN process_do(void)
{
    ENUM_RETURN ret_val;
    ret_val = process_options();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    if(is_current_error_exist() == BOOLEAN_TURE)
    {
        display_error_info();
        return RETURN_FAILURE;
    }
    
    //处理主函数
    R_ASSERT(p_main_proc != NULL, RETURN_FAILURE);
    ret_val = p_main_proc();
    return ret_val;
}

PRIVATE ENUM_RETURN parse_args(int argc, char **argv)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    ret_val = parse_input_files(argc, argv);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = parse_options(argc, argv);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

ENUM_RETURN register_main_proc(FUNC_MAIN_PROC handler)
{
    R_ASSERT(handler != NULL, RETURN_FAILURE);
    p_main_proc = handler;
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

    return ret_val;
    
}

