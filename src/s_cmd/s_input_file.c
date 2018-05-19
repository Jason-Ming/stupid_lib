#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s_log.h"
#include "s_cmd.h"
#include "s_error.h"
#include "s_input_file.h"


#define MAX_NUM_OF_INPUT_FILES 64
PRIVATE int num_of_input_files = 0;
PRIVATE char* input_files[MAX_NUM_OF_INPUT_FILES] = {NULL};


PRIVATE ENUM_RETURN add_input_file(const char *file)
{
    R_ASSERT(file != NULL, RETURN_FAILURE);
    R_LOG("add file: %s", file);
    
    input_files[num_of_input_files] = malloc(strlen(file) + 1);
    R_ASSERT(input_files[num_of_input_files] != NULL, RETURN_FAILURE);
    strcpy(input_files[num_of_input_files], file);
    num_of_input_files++;
    
    return RETURN_SUCCESS;
}

int get_input_file_num(void)
{
    return num_of_input_files;
}

/* 将文件参数处理并保存 */
ENUM_RETURN parse_input_files(int argc, char **argv)
{
    int i = 1;
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    
    while(i < argc)
    {
        if(argv[i][0] == '-')
        {
            break;
        }

        //处理输入的文件列表
        ret_val = add_input_file(argv[i]);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        i++;
    }
    
    if(get_input_file_num() == 0)
    {
        ret_val = add_current_error(ERROR_CODE_NO_INPUT_FILES, NULL);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }
    
    return RETURN_SUCCESS;
}

