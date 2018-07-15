#ifndef __S_CMD_H__
#define __S_CMD_H__
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"


typedef enum TAG_ENUM_OPTION_TYPE
{
    OPTION_TYPE_MANDATORY = 0,
    OPTION_TYPE_OPTIONAL,
    OPTION_TYPE_MAX,
}ENUM_OPTION_TYPE;

typedef enum TAG_ENUM_ARG_TYPE
{
    ARG_TYPE_SWITCH = 0,
    ARG_TYPE_DATA,
    ARG_TYPE_MAX,
}ENUM_ARG_TYPE;

typedef  ENUM_RETURN (*FUNC_SUBCMD_PROC)(_VOID);

/* check and save options */
typedef  ENUM_RETURN (*FUNC_OPTION_PROC)(const char *arg);

typedef enum TAG_ENUM_ERROR_CODE
{
    ERROR_CODE_SUCCESS = 0,
    ERROR_CODE_SUBCMD_PROC_FAIL,
    ERROR_CODE_OPTION_PROC_FAIL,
    ERROR_CODE_MISSING_SUBCMD,
    ERROR_CODE_UNKONWN_SUBCMD,
    ERROR_CODE_REPETITIVE_SUBCMD,
    ERROR_CODE_NO_INPUT_FILES,
    ERROR_CODE_UNEXPECTED_INPUT_FILES,
    ERROR_CODE_UNKONWN_OPTION,
    ERROR_CODE_MISSING_OPTION,
    ERROR_CODE_MISSING_ARGS,
    ERROR_CODE_MULTIPLE_ARGS,
    ERROR_CODE_INVALID_ARGS, 
    ERROR_CODE_FILE_NOT_EXIST,
    ERROR_CODE_FILE_CAN_NOT_BE_CREATED,
    ERROR_CODE_REPETITIVE_OPTION,
    ERROR_CODE_MAX,
}ENUM_ERROR_CODE;

#define USER_DEFINE_ERROR_CODE_MAX 200

__BEGIN_C_DECLS
_VOID debug_print_all_subcmds_and_its_options(_VOID);
_VOID debug_print_errors(_VOID);

ENUM_RETURN default_option_proc_handler(const char *arg);
ENUM_RETURN default_subcmd_proc_handler(_VOID);

/* code must be 0~(USER_DEFINE_ERROR_CODE_MAX-1) */
ENUM_RETURN register_user_error_info(int code, const char * info, ENUM_BOOLEAN need_additional_info);

/* code must be 0~(USER_DEFINE_ERROR_CODE_MAX-1) */
ENUM_RETURN generate_user_error(int code, const char* additional_info);

/* code must be ENUM_ERROR_CODE */
ENUM_RETURN generate_system_error(int code, const char* additional_info);

const char *get_input_file_of_current_running_subcmd(_VOID);

ENUM_RETURN register_subcmd(
    const char* subcmd_name,
    FUNC_SUBCMD_PROC handler, 
    const char* help_info);

ENUM_RETURN register_option(
    const char* subcmd_name,
    const char* option_name,
    ENUM_OPTION_TYPE option_type,
    ENUM_ARG_TYPE arg_type,
    FUNC_OPTION_PROC handler, 
    ENUM_BOOLEAN finish_handle,
    const char* help_info);

ENUM_RETURN prepare(const char * bin_name, const char *introduction);

ENUM_RETURN process(int argc, char **argv);
__END_C_DECLS

#define FALSE_GEN_SYSTEM_ERROR_DO(condition, error_code, additional_info, action)\
    if(!(condition))\
    {\
        ENUM_RETURN ret_val = generate_system_error(\
            error_code, additional_info);\
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);\
        action;\
    }

#define FALSE_GEN_USER_ERROR_DO(condition, error_code, additional_info, action)\
    if(!(condition))\
    {\
        ENUM_RETURN ret_val = generate_user_error(\
            error_code, additional_info);\
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);\
        action;\
    }

#endif

