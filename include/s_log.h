#ifndef __S_LOG_H__
#define __S_LOG_H__

#include <stdio.h>

#include "s_clinkage.h"
#include "s_time.h"

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
        LOG_PRINT("[%s] [file: %s, line: %u] [function: %s]"\
            fmt"\n",\
            get_time_stamp(), __FILE__, __LINE__, __FUNCTION__,\
            ##args);\


#define R_FALSE_LOG(condition, fmt, args...)\
    if(!(condition))\
    {\
        LOG_PRINT("[%s] [file: %s, line: %u] [function: %s]"\
            "FALSE("#condition")!, "fmt"\n",\
            get_time_stamp(), __FILE__, __LINE__, __FUNCTION__,\
            ##args);\
    }

#define R_FALSE_DO_LOG(condition, action, fmt, args...)\
    if(!(condition))\
    {\
        LOG_PRINT("[%s] [file: %s, line: %u] [function: %s]"\
            "FALSE("#condition")!, "fmt"\n",\
            get_time_stamp(), __FILE__, __LINE__, __FUNCTION__,\
            ##args);\
        action;\
    }

#define V_FALSE_RET(condition)\
    if(!(condition))\
    {\
        return;\
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
            "FALSE("#condition")!, return  = %ld, "fmt"\n",\
            get_time_stamp(), __FILE__, __LINE__, __FUNCTION__,\
            (LONG)value, ##args);\
        return value;\
    }
    
#define R_FALSE_RET_DO_LOG(condition, value, action, fmt, args...)\
    if(!(condition))\
    {\
        LOG_PRINT("[%s] [file: %s, line: %u] [function: %s]"\
            "FALSE("#condition")!, return  = %ld, "fmt"\n",\
            get_time_stamp(), __FILE__, __LINE__, __FUNCTION__,\
            (LONG)value, ##args);\
        action;\
        return value;\
    }
        

#define R_ASSERT(condition, value)\
    if(!(condition))\
    {\
        LOG_PRINT("[%s] [file: %s, line: %u] [function: %s]"\
            "ASSERT("#condition")!, return  = %ld.\n",\
            get_time_stamp(), __FILE__, __LINE__, __FUNCTION__,\
            (LONG)value);\
        return value;\
    }

#define R_ASSERT_DO(condition, value, action)\
    if(!(condition))\
    {\
        LOG_PRINT("[%s] [file: %s, line: %u] [function: %s]"\
            "ASSERT("#condition")!, return  = %ld.\n",\
            get_time_stamp(), __FILE__, __LINE__, __FUNCTION__,\
            (LONG)value);\
        action;\
        return value;\
    }

#define R_ASSERT_LOG(condition, value, fmt, args...)\
    if(!(condition))\
    {\
        LOG_PRINT("[%s] [file: %s, line: %u] [function: %s]"\
            "ASSERT("#condition")!, return  = %ld, "fmt"\n",\
            get_time_stamp(), __FILE__, __LINE__, __FUNCTION__,\
            (LONG)value, ##args);\
        return value;\
    }

#define R_ASSERT_DO_LOG(condition, value, action, fmt, args...)\
    if(!(condition))\
    {\
        LOG_PRINT("[%s] [file: %s, line: %u] [function: %s]"\
            "ASSERT("#condition")!, return  = %ld, "fmt"\n",\
            get_time_stamp(), __FILE__, __LINE__, __FUNCTION__,\
            (LONG)value, ##args);\
        action;\
        return value;\
    }

#endif

