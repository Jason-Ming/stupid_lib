#ifndef __S_OPTION_H__
#define __S_OPTION_H__
#include "s_clinkage.h"
#include "s_defines.h"


typedef enum TAG_ENUM_OPTION_TYPE
{
    OPTION_TYPE_MANDATORY = 0,
    OPTION_TYPE_OPTIONAL,
    OPTION_TYPE_MAX,
}ENUM_OPTION_TYPE;

typedef enum TAG_ENUM_VALUE_TYPE
{
    VALUE_TYPE_SWITCH = 0,
    VALUE_TYPE_DATA,
    VALUE_TYPE_MAX,
}ENUM_VALUE_TYPE;

typedef struct TAG_STRU_VALUE
{
    char* value;
    struct TAG_STRU_VALUE *next;
} STRU_VALUE;

typedef  ENUM_RETURN (*FUNC_OPTION_PROC)(struct TAG_STRU_VALUE *value);

__BEGIN_C_DECLS

ENUM_RETURN register_option(
    const char* option_name,
    ENUM_OPTION_TYPE option_type,
    ENUM_VALUE_TYPE value_type,
    FUNC_OPTION_PROC handler,
    const char* help_info);

__END_C_DECLS

#endif
