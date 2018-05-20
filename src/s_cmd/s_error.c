#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s_log.h"
#include "s_cmd.h"
#include "s_error.h"

typedef struct TAG_STRU_ERROR_INFO
{
    int code;
    const char* info;
    ENUM_BOOLEAN need_addtional_info;
}STRU_ERROR_INFO;

PRIVATE STRU_ERROR_INFO system_error_infos[] = 
{
    {ERROR_CODE_SUCCESS, "Run succeed", BOOLEAN_FALSE},
    {ERROR_CODE_MISSING_SUBCMD, "No input sub command", BOOLEAN_FALSE},
    {ERROR_CODE_UNKONWN_SUBCMD, "Unrecognized sub command", BOOLEAN_TRUE},
    {ERROR_CODE_REPETITIVE_SUBCMD, "Repetitive sub command", BOOLEAN_TRUE},
    {ERROR_CODE_NO_INPUT_FILES, "No input files", BOOLEAN_FALSE},
    {ERROR_CODE_UNEXPECTED_INPUT_FILES, "Unexpected input files", BOOLEAN_FALSE},
    {ERROR_CODE_UNKONWN_OPTION, "Unrecognized command line option", BOOLEAN_TRUE},
    {ERROR_CODE_MISSING_ARGS, "Missing argument to", BOOLEAN_TRUE},
    {ERROR_CODE_MULTIPLE_ARGS, "Multiple arguments to", BOOLEAN_TRUE},
    {ERROR_CODE_FILE_NOT_EXIST, "No such file or directory", BOOLEAN_TRUE},
    {ERROR_CODE_REPETITIVE_OPTION, "Repetitive option", BOOLEAN_TRUE},
};

#define MAX_NUM_OF_ERROR_INFO (ERROR_CODE_MAX + MAX_NUM_OF_USER_DEFINE_ERROR)

#define INVALID_ERROR_CODE (-1)

PRIVATE STRU_ERROR_INFO error_info_array[MAX_NUM_OF_ERROR_INFO];

typedef struct TAG_STRU_CURRENT_ERROR_INFO
{
    int code;
    char* additional_info;
    struct TAG_STRU_CURRENT_ERROR_INFO *next;
}STRU_CURRENT_ERROR_INFO;

PRIVATE STRU_CURRENT_ERROR_INFO *p_current_error_list_head = NULL;
PRIVATE STRU_CURRENT_ERROR_INFO *p_current_error_list_tail = NULL;

PRIVATE STRU_CURRENT_ERROR_INFO *get_current_error_list_head(void)
{
    return p_current_error_list_head;
}

PRIVATE ENUM_BOOLEAN is_error_code_valid(int code)
{
    return (code >= 0 && code < MAX_NUM_OF_ERROR_INFO)?BOOLEAN_TRUE:BOOLEAN_FALSE;
}

PRIVATE ENUM_BOOLEAN is_error_code_registered(int code)
{
    R_FALSE_RET(is_error_code_valid(code) == BOOLEAN_TRUE, BOOLEAN_FALSE);
    
    R_FALSE_RET(error_info_array[code].code != INVALID_ERROR_CODE, BOOLEAN_FALSE);

    return BOOLEAN_TRUE;
}

PRIVATE const char * get_error_info(int code)
{
    R_ASSERT(is_error_code_valid(code) == BOOLEAN_TRUE, NULL);
    return error_info_array[code].info;
}

PRIVATE ENUM_BOOLEAN is_error_need_additional_info(int code)
{
    R_ASSERT(is_error_code_valid(code) == BOOLEAN_TRUE, BOOLEAN_FALSE);
    return error_info_array[code].need_addtional_info;
}

PRIVATE void debug_print_current_error(STRU_CURRENT_ERROR_INFO *p)
{
    R_LOG("code: %d, info: %s, additional_info: %s", p->code, get_error_info(p->code), p->additional_info);
}

ENUM_BOOLEAN is_current_error_exist(void)
{
    return (p_current_error_list_head == NULL)?BOOLEAN_FALSE:BOOLEAN_TRUE;
}

PRIVATE ENUM_RETURN get_a_new_error_node(STRU_CURRENT_ERROR_INFO **pp_new, 
    int code, const char* additional_info)
{
    R_ASSERT(pp_new != NULL, RETURN_FAILURE);
    
    *pp_new = (STRU_CURRENT_ERROR_INFO*)malloc(sizeof(STRU_CURRENT_ERROR_INFO));
    R_ASSERT(*pp_new != NULL, RETURN_FAILURE);

    (*pp_new)->code = code;
    (*pp_new)->additional_info = NULL;
    (*pp_new)->next = NULL;

    (*pp_new)->additional_info = (char*)malloc(strlen(additional_info) + 1);
    R_ASSERT_DO((*pp_new)->additional_info != NULL, RETURN_FAILURE, free(*pp_new); *pp_new = NULL);
    strcpy((*pp_new)->additional_info, additional_info);

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN add_node_to_current_error_list(STRU_CURRENT_ERROR_INFO *p_new)
{
    R_ASSERT(p_new != NULL, RETURN_FAILURE);

    if(p_current_error_list_tail == NULL)
    {
        p_current_error_list_head = p_current_error_list_tail = p_new;
    }
    else
    {
        p_current_error_list_tail->next = p_new;
        p_current_error_list_tail = p_new;
    }
    
    return RETURN_SUCCESS;
}

ENUM_RETURN add_current_system_error(int code, const char* additional_info)
{    
    R_ASSERT(is_error_code_valid(code) == BOOLEAN_TRUE, RETURN_FAILURE);
    R_ASSERT_LOG(is_error_code_registered(code) == BOOLEAN_TRUE, RETURN_FAILURE,
        "the error code: %d is not registered", code);

    STRU_CURRENT_ERROR_INFO *p_new = NULL;

    if(error_info_array[code].need_addtional_info == BOOLEAN_TRUE)
    {
        R_ASSERT(additional_info != NULL, RETURN_FAILURE);
    }
    else
    {
        R_ASSERT(additional_info == NULL, RETURN_FAILURE);
        additional_info = "";
    }
    
    ENUM_RETURN ret_val;
    ret_val = get_a_new_error_node(&p_new, code, additional_info);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_node_to_current_error_list(p_new);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    debug_print_current_error(p_new);
    
    return RETURN_SUCCESS;
}

ENUM_RETURN add_current_user_error(int code, const char* additional_info)
{
    return add_current_system_error(code, additional_info);
}

void display_error_info(void)
{
    STRU_CURRENT_ERROR_INFO *p = get_current_error_list_head();

    while(p != NULL)
    {
        printf("Error: %s", get_error_info(p->code));

        if(is_error_need_additional_info(p->code) == BOOLEAN_TRUE)
        {
            printf(": %s", p->additional_info);
        }

        printf("\n");
        
        p = p->next;
    }    
}

PRIVATE ENUM_RETURN register_error_info(int code, const char * info, ENUM_BOOLEAN need_additional_info)
{
    R_ASSERT(is_error_code_valid(code) == BOOLEAN_TRUE, RETURN_FAILURE);

    R_ASSERT_LOG(is_error_code_registered(code) == BOOLEAN_FALSE, RETURN_FAILURE,
        "the error code: %d is already registered", code);

    R_LOG("code: %d, info: %s, need_additional_info: %d", code, info, need_additional_info);
    
    error_info_array[code].code = code;
    error_info_array[code].info = info;
    error_info_array[code].need_addtional_info = need_additional_info;
    
    return RETURN_SUCCESS;
}

ENUM_RETURN register_user_error_info(int code, const char * info, ENUM_BOOLEAN need_additional_info)
{
    return register_error_info(code, info, need_additional_info);
}

ENUM_RETURN init_error_info(void)
{
    for(int i = 0; i < MAX_NUM_OF_ERROR_INFO; i++)
    {
        error_info_array[i].code = INVALID_ERROR_CODE;
        error_info_array[i].info = NULL;
        error_info_array[i].need_addtional_info = BOOLEAN_FALSE;
    }

    for(int i = 0; i < SIZE_OF_ARRAY(system_error_infos); i++)
    {
        ENUM_RETURN ret_val;
        ret_val = register_error_info(system_error_infos[i].code, 
            system_error_infos[i].info, system_error_infos[i].need_addtional_info);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }

    return RETURN_SUCCESS;
}

