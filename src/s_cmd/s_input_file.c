#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s_log.h"
#include "s_cmd.h"
#include "s_cmd_proc.h"

#include "s_option.h"
#include "s_error.h"
#include "s_subcmd.h"
#include "s_input_file.h"


#define MAX_NUM_OF_INPUT_FILES 64
PRIVATE int num_of_input_files = 0;
PRIVATE char* input_files[MAX_NUM_OF_INPUT_FILES] = {NULL};


PRIVATE ENUM_RETURN add_input_file(const char *file)
{
    R_ASSERT(file != NULL, RETURN_FAILURE);
    S_LOG("add file: %s", file);
    
    input_files[num_of_input_files] = malloc(strlen(file) + 1);
    R_ASSERT(input_files[num_of_input_files] != NULL, RETURN_FAILURE);
    strcpy(input_files[num_of_input_files], file);
    num_of_input_files++;
    
    return RETURN_SUCCESS;
}

int get_current_running_subcmd_input_file_num(_VOID)
{
    return num_of_input_files;
}

const char *get_input_file_of_current_running_subcmd(_VOID)
{
    return input_files[0];
}

/* 将文件参数处理并保存 */
ENUM_RETURN parse_input_files(int argc, char **argv)
{
    int i = get_argv_indicator();
    ENUM_RETURN ret_val = RETURN_SUCCESS;

    /* do noting when there is any error */
    S_R_FALSE_LOG(BOOLEAN_FALSE == whether_any_error_exists(), RETURN_SUCCESS,"");
 
    while(i < argc)
    {
        S_LOG("i = %d, argv = %s", i, argv[i]);
   
        /* if an option occured, break */
        if(whether_string_is_in_option_format(argv[i]) == BOOLEAN_TRUE)
        {
            break;
        }

        //处理输入的文件列表
        ret_val = add_input_file(argv[i]);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        i++;
    }

    set_argv_indicator(i);
    
    return RETURN_SUCCESS;
}

