#ifndef __S_ERROR_H__
#define __S_ERROR_H__
#include "s_clinkage.h"
#include "s_defines.h"

typedef enum TAG_ENUM_ERROR_CODE
{
    ERROR_CODE_SUCCESS = 0,
    ERROR_CODE_NO_INPUT_FILES,
    ERROR_CODE_UNKONWN_OPTION,
    ERROR_CODE_MISSING_ARGS,
    ERROR_CODE_FILE_NOT_EXIST,
    ERROR_CODE_REPETITIVE_OPTION,
    ERROR_CODE_MAX,
}ENUM_ERROR_CODE;

__BEGIN_C_DECLS
void display_error_info(void);
ENUM_RETURN init_error_info(void);
ENUM_BOOLEAN is_current_error_exist(void);

__END_C_DECLS

#endif

