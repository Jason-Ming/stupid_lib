#include <stdio.h>
#include <stdlib.h>

#include "s_log.h"
#include "s_error.h"

typedef enum TAG_ENUM_ERROR_CODE
{
    ERROR_CODE_SUCCESS = 0,
    ERROR_CODE_NO_INPUT_FILES,
    
}ENUM_ERROR_CODE;

#define MAX_NUM_OF_ERROR_INFO 200
PRIVATE const char* error_info_array[MAX_NUM_OF_ERROR_INFO] = {NULL};

typedef struct TAG_STRU_ERROR_INFO
{
    int code;
    const char* info;
}STRU_ERROR_INFO;

PRIVATE STRU_ERROR_INFO error_infos[] = 
{
    {ERROR_CODE_SUCCESS, ""},
    {ERROR_CODE_NO_INPUT_FILES, "no input files"},
};
    

PRIVATE int error_code = 0;


ENUM_RETURN set_error(int code)
{
    R_ASSERT(code >= 0 && code < MAX_NUM_OF_ERROR_INFO, RETURN_FAILURE);
    R_ASSERT(error_info_array[code] != NULL, RETURN_FAILURE);
    
    error_code = code;

    return RETURN_SUCCESS;
}

PRIVATE const char * get_error(void)
{
    return error_info_array[error_code];
}

void print_error_info(void)
{
    V_FALSE_RET(error_code != 0);
    
    printf("Error: %s\n", get_error());
}

ENUM_RETURN register_error_info(int code, const char * info)
{
    R_ASSERT(code >= 0 && code < MAX_NUM_OF_ERROR_INFO, RETURN_FAILURE);

    R_ASSERT(error_info_array[code] == NULL, RETURN_FAILURE);

    error_info_array[code] = info;

    return RETURN_SUCCESS;
}

