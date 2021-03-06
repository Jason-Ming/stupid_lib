#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s_log.h"
#include "s_mem.h"
#include "s_cmd.h"
#include "s_subcmd.h"
#include "s_cmd_proc.h"
#include "s_error.h"

#define MAX_LEN_OF_TIP_STRING 64
typedef enum TAG_ENUM_ERROR_TIP
{
    ERROR_TIP_OPTION = 0,
    ERROR_TIP_SUBCMD,
    ERROR_TIP_NULL,
}ENUM_ERROR_TIP;

typedef struct TAG_STRU_ERROR_INFO
{
    int code;
    const char* info;
    ENUM_BOOLEAN whether_error_need_addtional_info;
    ENUM_ERROR_TIP tip;
}STRU_ERROR_INFO;

PRIVATE STRU_ERROR_INFO system_error_infos[] = 
{
    {ERROR_CODE_SUCCESS,                 "Run succeed",                      BOOLEAN_FALSE, ERROR_TIP_NULL},
    {ERROR_CODE_OPTION_PROC_FAIL,        "option process failed",            BOOLEAN_FALSE, ERROR_TIP_NULL},
    {ERROR_CODE_SUBCMD_PROC_FAIL,        "Run failed",                       BOOLEAN_FALSE, ERROR_TIP_NULL},
    {ERROR_CODE_MISSING_SUBCMD,          "Missing sub command",              BOOLEAN_FALSE, ERROR_TIP_SUBCMD},
    {ERROR_CODE_UNKONWN_SUBCMD,          "Unrecognized sub command '%s'",    BOOLEAN_TRUE,  ERROR_TIP_SUBCMD},
    {ERROR_CODE_REPETITIVE_SUBCMD,       "Repetitive sub command '%s'",      BOOLEAN_TRUE,  ERROR_TIP_NULL},
    {ERROR_CODE_NO_INPUT_FILES,          "'%s' requires input file",         BOOLEAN_TRUE,  ERROR_TIP_OPTION},
    {ERROR_CODE_UNEXPECTED_INPUT_FILES,  "Unexpected input files to '%s'",   BOOLEAN_TRUE,  ERROR_TIP_NULL},
    {ERROR_CODE_UNKONWN_OPTION,          "Unrecognized option '%s'",         BOOLEAN_TRUE,  ERROR_TIP_OPTION},
    {ERROR_CODE_MISSING_OPTION,          "Missing command line option '%s'", BOOLEAN_TRUE,  ERROR_TIP_OPTION},
    {ERROR_CODE_MISSING_ARGS,            "option '%s' requires an argument", BOOLEAN_TRUE,  ERROR_TIP_OPTION},
    {ERROR_CODE_MULTIPLE_ARGS,           "Multiple arguments to '%s'",       BOOLEAN_TRUE,  ERROR_TIP_OPTION},
    {ERROR_CODE_INVALID_ARGS,            "Invalid argument '%s'",            BOOLEAN_TRUE,  ERROR_TIP_OPTION},
    {ERROR_CODE_FILE_NOT_EXIST,          "No such file or directory '%s'",   BOOLEAN_TRUE,  ERROR_TIP_NULL},
    {ERROR_CODE_FILE_CAN_NOT_BE_CREATED, "File '%s' can not be created",     BOOLEAN_TRUE,  ERROR_TIP_NULL},
    {ERROR_CODE_REPETITIVE_OPTION,       "Repetitive option '%s'",           BOOLEAN_TRUE,  ERROR_TIP_OPTION},
};

#define MAX_NUM_OF_ERROR_INFO (ERROR_CODE_MAX + USER_DEFINE_ERROR_CODE_MAX)

#define INVALID_ERROR_CODE (-1)

PRIVATE STRU_ERROR_INFO error_info_array[MAX_NUM_OF_ERROR_INFO];

typedef struct TAG_STRU_GENERATED_ERROR_INFO
{
    int code;
    char* additional_info;
    struct TAG_STRU_GENERATED_ERROR_INFO *next;
}STRU_GENERATED_ERROR_INFO;

PRIVATE STRU_GENERATED_ERROR_INFO *p_generated_error_list_head = NULL;
PRIVATE STRU_GENERATED_ERROR_INFO *p_generated_error_list_tail = NULL;

PRIVATE STRU_GENERATED_ERROR_INFO *get_generated_error_list_head(void)
{
    return p_generated_error_list_head;
}

PRIVATE ENUM_BOOLEAN whether_error_code_is_valid(int code)
{
    return (code >= 0 && code < MAX_NUM_OF_ERROR_INFO)?BOOLEAN_TRUE:BOOLEAN_FALSE;
}

PRIVATE ENUM_BOOLEAN whether_error_has_been_registered(int code)
{
    S_R_FALSE(whether_error_code_is_valid(code) == BOOLEAN_TRUE, BOOLEAN_FALSE);
    
    S_R_FALSE(error_info_array[code].code != INVALID_ERROR_CODE, BOOLEAN_FALSE);

    return BOOLEAN_TRUE;
}

PRIVATE const char * get_error_info(int code)
{
    R_ASSERT(whether_error_code_is_valid(code) == BOOLEAN_TRUE, NULL);
    return error_info_array[code].info;
}

PRIVATE const char * get_error_tip(int code)
{
    R_ASSERT(whether_error_code_is_valid(code) == BOOLEAN_TRUE, NULL);

    PRIVATE _S8 tip_string[MAX_LEN_OF_TIP_STRING] = {'\0'};
    
    switch(error_info_array[code].tip)
    {
        case ERROR_TIP_SUBCMD:
        {
            sprintf(tip_string, ", see '%s help'", get_bin_name());
            break;
        }
        case ERROR_TIP_OPTION:
        {
            sprintf(tip_string, ", see '%s %s -h'", get_bin_name(), get_current_running_subcmd_name());
            break;
        }
        default:
        {
            tip_string[0] = '\0';
            break;
        }
    }
    return tip_string;
}

PRIVATE ENUM_BOOLEAN whether_error_needs_additional_info(int code)
{
    R_ASSERT(whether_error_code_is_valid(code) == BOOLEAN_TRUE, BOOLEAN_FALSE);
    return error_info_array[code].whether_error_need_addtional_info;
}

PRIVATE void debug_print_generated_error(STRU_GENERATED_ERROR_INFO *p)
{
    S_LOG("code: %d, info: %s, additional_info: %s", p->code, get_error_info(p->code), p->additional_info);
}

_VOID debug_print_errors(_VOID)
{
    for(int i = 0; i < SIZE_OF_ARRAY(error_info_array); i++)
    {
        printf("code: %d, info: %s, need_additional_info: %d, tip: %d\n", 
            error_info_array[i].code, 
            error_info_array[i].info, 
            error_info_array[i].whether_error_need_addtional_info,
            error_info_array[i].tip);
    }
}

ENUM_BOOLEAN whether_any_error_exists(void)
{
    return (p_generated_error_list_head == NULL)?BOOLEAN_FALSE:BOOLEAN_TRUE;
}

PRIVATE ENUM_RETURN get_a_new_generated_error_node(STRU_GENERATED_ERROR_INFO **pp_new, 
    int code, const char* additional_info)
{
    R_ASSERT(pp_new != NULL, RETURN_FAILURE);
    
    *pp_new = (STRU_GENERATED_ERROR_INFO*)malloc(sizeof(STRU_GENERATED_ERROR_INFO));
    R_ASSERT(*pp_new != NULL, RETURN_FAILURE);

    (*pp_new)->code = code;
    (*pp_new)->additional_info = NULL;
    (*pp_new)->next = NULL;

    (*pp_new)->additional_info = (char*)malloc(strlen(additional_info) + 1);
    R_ASSERT_DO((*pp_new)->additional_info != NULL, RETURN_FAILURE, free(*pp_new); *pp_new = NULL);
    strcpy((*pp_new)->additional_info, additional_info);

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN add_node_to_generated_error_list(STRU_GENERATED_ERROR_INFO *p_new)
{
    R_ASSERT(p_new != NULL, RETURN_FAILURE);

    if(p_generated_error_list_tail == NULL)
    {
        p_generated_error_list_head = p_generated_error_list_tail = p_new;
    }
    else
    {
        p_generated_error_list_tail->next = p_new;
        p_generated_error_list_tail = p_new;
    }
    
    return RETURN_SUCCESS;
}

ENUM_RETURN generate_system_error(int code, const char* additional_info)
{    
    R_ASSERT(whether_error_code_is_valid(code) == BOOLEAN_TRUE, RETURN_FAILURE);
    R_ASSERT_LOG(whether_error_has_been_registered(code) == BOOLEAN_TRUE, RETURN_FAILURE,
        "the error code: %d is not registered", code);

    STRU_GENERATED_ERROR_INFO *p_new = NULL;

    if(error_info_array[code].whether_error_need_addtional_info == BOOLEAN_TRUE)
    {
        R_ASSERT(additional_info != NULL, RETURN_FAILURE);
    }
    else
    {
        R_ASSERT(additional_info == NULL, RETURN_FAILURE);
        additional_info = "";
    }
    
    ENUM_RETURN ret_val;
    ret_val = get_a_new_generated_error_node(&p_new, code, additional_info);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_node_to_generated_error_list(p_new);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    debug_print_generated_error(p_new);
    
    return RETURN_SUCCESS;
}

ENUM_RETURN generate_user_error(int code, const char* additional_info)
{
    return generate_system_error(code + ERROR_CODE_MAX, additional_info);
}

void display_success_info(void)
{
    printf(GREEN"Successed!\n"NONE);
}

void display_generated_error_info(void)
{
    STRU_GENERATED_ERROR_INFO *p = get_generated_error_list_head();

    while(p != NULL)
    {
        printf(LIGHT_RED"Error"NONE": ");

        if(whether_error_needs_additional_info(p->code) == BOOLEAN_TRUE)
        {
            printf(get_error_info(p->code), p->additional_info);
        }
        else
        {
            printf(get_error_info(p->code), "");
        }

        printf("%s", get_error_tip(p->code));
        
        printf("\n");
        
        p = p->next;
    }    
}

PRIVATE ENUM_RETURN register_error_info(int code, const char * info, ENUM_BOOLEAN need_additional_info, ENUM_ERROR_TIP tip)
{
    R_ASSERT(whether_error_code_is_valid(code) == BOOLEAN_TRUE, RETURN_FAILURE);

    R_ASSERT_LOG(whether_error_has_been_registered(code) == BOOLEAN_FALSE, RETURN_FAILURE,
        "the error code: %d is already registered", code);

    S_LOG("code: %d, info: %s, need_additional_info: %d, tip: %d", code, info, need_additional_info, tip);
    
    error_info_array[code].code = code;
    error_info_array[code].info = info;
    error_info_array[code].whether_error_need_addtional_info = need_additional_info;
    error_info_array[code].tip = tip;
    
    return RETURN_SUCCESS;
}

ENUM_RETURN register_user_error_info(
    int code, const char * info, 
    ENUM_BOOLEAN need_additional_info)
{
    R_ASSERT_LOG(whether_initializtion_has_been_finished(), RETURN_FAILURE, 
        "must call 'prepare' first!");
    return register_error_info(code + ERROR_CODE_MAX, info, need_additional_info, ERROR_TIP_NULL);
}

ENUM_RETURN init_error_info(void)
{
    for(int i = 0; i < MAX_NUM_OF_ERROR_INFO; i++)
    {
        error_info_array[i].code = INVALID_ERROR_CODE;
        error_info_array[i].info = NULL;
        error_info_array[i].whether_error_need_addtional_info = BOOLEAN_FALSE;
    }

    for(int i = 0; i < SIZE_OF_ARRAY(system_error_infos); i++)
    {
        ENUM_RETURN ret_val;
        ret_val = register_error_info(
            system_error_infos[i].code, 
            system_error_infos[i].info, 
            system_error_infos[i].whether_error_need_addtional_info,
            system_error_infos[i].tip);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }

    return RETURN_SUCCESS;
}

