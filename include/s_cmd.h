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

typedef  ENUM_RETURN (*FUNC_OPTION_PROC)(struct TAG_STRU_ARG *arg);
typedef  ENUM_RETURN (*FUNC_MAIN_PROC)(void);

__BEGIN_C_DECLS
ENUM_RETURN register_error_info(int code, const char * info);
ENUM_RETURN add_current_error(int code, const char* additional_info);

ENUM_RETURN register_introduction(const char *introduction);
ENUM_RETURN register_usage(const char *usage);

ENUM_RETURN register_option(
    const char* option_name,
    ENUM_OPTION_TYPE option_type,
    ENUM_ARG_TYPE arg_type,
    FUNC_OPTION_PROC handler,
    const char* help_info);

ENUM_RETURN register_main_proc(FUNC_MAIN_PROC handler);

ENUM_RETURN process(int argc, char **argv);
__END_C_DECLS

#endif

