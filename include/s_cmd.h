#ifndef __S_CMD_H__
#define __S_CMD_H__
#include "s_clinkage.h"
#include "s_defines.h"


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

typedef struct TAG_STRU_ARG
{
    char* value;
    struct TAG_STRU_ARG *next;
} STRU_ARG;

typedef struct TAG_STRU_OPTION_RUN_BLOCK
{
    const char* subcmd;
    const char* option;
    struct TAG_STRU_ARG *arg;
    struct TAG_STRU_OPTION_RUN_BLOCK *next;
} STRU_OPTION_RUN_BLOCK;

typedef  ENUM_RETURN (*FUNC_SUBCMD_PROC)(STRU_OPTION_RUN_BLOCK *option_rb);

/* check and save options */
typedef  ENUM_RETURN (*FUNC_OPTION_PROC)(STRU_ARG *arg);

typedef enum TAG_ENUM_ERROR_CODE
{
    ERROR_CODE_SUCCESS = 0,
    ERROR_CODE_MISSING_SUBCMD,
    ERROR_CODE_UNKONWN_SUBCMD,
    ERROR_CODE_REPETITIVE_SUBCMD,
    ERROR_CODE_NO_INPUT_FILES,
    ERROR_CODE_UNEXPECTED_INPUT_FILES,
    ERROR_CODE_UNKONWN_OPTION,
    ERROR_CODE_MISSING_ARGS,
    ERROR_CODE_MULTIPLE_ARGS,
    ERROR_CODE_FILE_NOT_EXIST,
    ERROR_CODE_REPETITIVE_OPTION,
    ERROR_CODE_MAX,
}ENUM_ERROR_CODE;

#define USER_DEFINE_ERROR_CODE_MIN ERROR_CODE_MAX
#define MAX_NUM_OF_USER_DEFINE_ERROR 200

__BEGIN_C_DECLS
ENUM_RETURN default_option_proc_handler(STRU_ARG *arg);
ENUM_RETURN register_user_error_info(int code, const char * info, ENUM_BOOLEAN need_additional_info);
ENUM_RETURN add_current_user_error(int code, const char* additional_info);


ENUM_RETURN register_introduction(const char *introduction);

ENUM_RETURN register_usage(const char *usage);

const char *get_bin_name(void);
const char *get_introduction(void);
const char *get_usage(void);

int get_argv_indicator(void);
void set_argv_indicator(int new_value);

const char *get_input_file_of_subcmd(const char *subcmd_name);

ENUM_RETURN register_subcmd(
    const char* subcmd_name,
    ENUM_BOOLEAN need_input_file,
    FUNC_SUBCMD_PROC handler, 
    const char* help_info);

ENUM_RETURN register_option(
    const char* subcmd_name,
    const char* option_name,
    ENUM_OPTION_TYPE option_type,
    ENUM_ARG_TYPE arg_type,
    FUNC_OPTION_PROC handler, 
    const char* help_info);

ENUM_RETURN process(int argc, char **argv);
__END_C_DECLS

#endif

