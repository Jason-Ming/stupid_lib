#ifndef __DEFINES_H__
#define __DEFINES_H__
#include <limits.h>
#include <stdio.h>
#include "clinkage.h"

typedef enum TAG_ENUM_RETURN
{
    RETURN_SUCCESS = 0,
    RETURN_FAILURE = -1,
}ENUM_RETURN;

typedef enum TAG_ENUM_BOOLEAN
{
    BOOLEAN_TURE = 1,
    BOOLEAN_FALSE = 0,
}ENUM_BOOLEAN;

#define SIZE_OF_ARRAY(a) (sizeof(a)/sizeof(a[0]))
#define PRIVATE static
#define INVALID_INT INT_MAX
#define INVALID_UINT (~(UINT)0)

typedef int          INT;
typedef unsigned int UINT;
typedef char*        PSTR;

char* get_time_string_without_usec(void);
char* get_time_string_with_usec(void);

ENUM_BOOLEAN log_isready(void);
ENUM_RETURN log_init(void);
FILE *log_getfp(void);
char* log_getfn(void);

#define LOG_TYPE_FILE 11

#ifdef LOG_TYPE_FILE

#define LOG_PRINT(fmt, args...)\
    do\
    {\
        FILE *fp = fopen(log_getfn(), "a");\
        if(fp != NULL)\
        {\
            fprintf(fp, fmt, ##args);\
            fclose(fp);\
        }\
        else\
        {\
            printf(fmt, ##args);\
        }\
    }while(0);
#else
#define LOG_PRINT(fmt, args...)\
    printf(fmt, ##args);
#endif

//不能用编译时间，蠢货!
#define R_LOG(fmt, args...)\
    do\
    {\
        LOG_PRINT("[%s] [file: %s, line: %u] [function: %s]"\
            fmt"\n",\
            get_time_string_with_usec(), __FILE__, __LINE__, __FUNCTION__,\
            ##args);\
    }while(0)


#define R_FALSE_LOG(condition, fmt, args...)\
    if(!(condition))\
    {\
        LOG_PRINT("[%s] [file: %s, line: %u] [function: %s]"\
            "FALSE("#condition")!, return  = %d, "fmt"\n",\
            get_time_string_with_usec(), __FILE__, __LINE__, __FUNCTION__,\
            (INT)value, ##args);\
    }

#define R_FALSE_DO_LOG(condition, action, fmt, args...)\
    if(!(condition))\
    {\
        LOG_PRINT("[%s] [file: %s, line: %u] [function: %s]"\
            "FALSE("#condition")!, return  = %d, "fmt"\n",\
            get_time_string_with_usec(), __FILE__, __LINE__, __FUNCTION__,\
            (INT)value, ##args);\
        action;\
    }

#define R_FALSE_RET(condition, value)\
    if(!(condition))\
    {\
        return value;\
    }

#define R_FALSE_RET_DO(condition, value, action)\
    if(!(condition))\
    {\
        action;\
        return value;\
    }

#define R_FALSE_RET_LOG(condition, value, fmt, args...)\
    if(!(condition))\
    {\
        LOG_PRINT("[%s] [file: %s, line: %u] [function: %s]"\
            "FALSE("#condition")!, return  = %d, "fmt"\n",\
            get_time_string_with_usec(), __FILE__, __LINE__, __FUNCTION__,\
            (INT)value, ##args);\
        return value;\
    }
    
#define R_FALSE_RET_DO_LOG(condition, value, action, fmt, args...)\
    if(!(condition))\
    {\
        LOG_PRINT("[%s] [file: %s, line: %u] [function: %s]"\
            "FALSE("#condition")!, return  = %d, "fmt"\n",\
            get_time_string_with_usec(), __FILE__, __LINE__, __FUNCTION__,\
            (INT)value, ##args);\
        action;\
        return value;\
    }
        

#define R_ASSERT(condition, value)\
    if(!(condition))\
    {\
        LOG_PRINT("[%s] [file: %s, line: %u] [function: %s]"\
            "ASSERT("#condition")!, return  = %d.\n",\
            get_time_string_with_usec(), __FILE__, __LINE__, __FUNCTION__,\
            (INT)value);\
        return value;\
    }

#define R_ASSERT_DO(condition, value, action)\
    if(!(condition))\
    {\
        LOG_PRINT("[%s] [file: %s, line: %u] [function: %s]"\
            "ASSERT("#condition")!, return  = %d.\n",\
            get_time_string_with_usec(), __FILE__, __LINE__, __FUNCTION__,\
            (INT)value);\
        action;\
        return value;\
    }

#define R_ASSERT_LOG(condition, value, fmt, args...)\
    if(!(condition))\
    {\
        LOG_PRINT("[%s] [file: %s, line: %u] [function: %s]"\
            "ASSERT("#condition")!, return  = %d, "fmt"\n",\
            get_time_string_with_usec(), __FILE__, __LINE__, __FUNCTION__,\
            (INT)value, ##args);\
        return value;\
    }

#define R_ASSERT_DO_LOG(condition, value, action, fmt, args...)\
    if(!(condition))\
    {\
        LOG_PRINT("[%s] [file: %s, line: %u] [function: %s]"\
            "ASSERT("#condition")!, return  = %d, "fmt"\n",\
            get_time_string_with_usec(), __FILE__, __LINE__, __FUNCTION__,\
            (INT)value, ##args);\
        action;\
        return value;\
    }

#endif

