#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "s_log.h"
#include "s_option.h"
#include "s_error.h"
#include "s_input_file.h"

PRIVATE const char *p_bin = NULL;
PRIVATE char *p_usage = NULL;
PRIVATE char *p_introduction = NULL;


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

PRIVATE ENUM_RETURN print_help_info(struct TAG_STRU_VALUE *value)
{
    printf("       %s\n", p_introduction);
    printf("Usage: %s %s\n", get_bin_name(), p_usage);

    display_option_help_info();
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN register_option_help(void)
{
    ENUM_RETURN ret_val = register_option("-h", OPTION_TYPE_OPTIONAL, VALUE_TYPE_SWITCH, print_help_info, "display this information");
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN register_default_data(void)
{
    ENUM_RETURN ret_val;
    ret_val = register_option_help();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = register_error_info(0, "Program run succeed!");
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

ENUM_RETURN process(int argc, char **argv)
{
    save_bin_name(argv[0]);

    register_default_data();
    
    if(argc < 2)
    {
        print_help_info(NULL);
        return RETURN_FAILURE;
    }
    
    R_ASSERT(argv[1] != NULL, RETURN_FAILURE);

    R_LOG(" %d parameters: \n", argc);

    ENUM_RETURN ret_val;
    ret_val = parse_input_files(argc, argv);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = parse_options(argc, argv);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    //debug_print_option_proc_block();
    
    ret_val = process_options();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    print_error_info();
    
    return RETURN_SUCCESS;
    
}

