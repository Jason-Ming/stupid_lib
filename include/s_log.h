#ifndef __S_LOG_H__
#define __S_LOG_H__

#include <stdio.h>

#include "s_clinkage.h"
#include "s_time.h"
#include "s_type.h"
#include "s_print.h"



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

//#define DEBUG_SWITCH 1

#ifdef DEBUG_SWITCH
#define DEBUG_PRINT(fmt, args...)\
    LOG_PRINT(LIGHT_GRAY"debuginfo: "fmt""NONE"\n", ##args);
#else
#define DEBUG_PRINT(fmt, args...)
#endif

//不能用编译时间，蠢货!
#define R_LOG(fmt, args...)\
        LOG_PRINT(GREEN"[%s] [file: %s, line: %ld] [function: %s]\n   "\
            fmt"\n"NONE,\
            get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
            ##args);\

#define R_RET_LOG(value, fmt, args...)\
    do{\
            LOG_PRINT(GREEN"[%s] [file: %s, line: %ld] [function: %s]\n   "\
                fmt"\n"NONE,\
                get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
            ##args);\
                return value;}while(0);

#define R_FALSE_LOG(condition, fmt, args...)\
    if(!(condition))\
    {\
        LOG_PRINT(GREEN"[%s] [file: %s, line: %ld] [function: %s]\n   "\
            "FALSE("#condition")!, "fmt"\n"NONE,\
            get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
            ##args);\
    }

#define R_FALSE_DO_LOG(condition, action, fmt, args...)\
    if(!(condition))\
    {\
        LOG_PRINT(GREEN"[%s] [file: %s, line: %ld] [function: %s]\n   "\
            "FALSE("#condition")!, "fmt"\n"NONE,\
            get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
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
        LOG_PRINT(GREEN"[%s] [file: %s, line: %ld] [function: %s]\n   "\
            "FALSE("#condition")!, return  = %ld, "fmt"\n"NONE,\
            get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
            (_SL)value, ##args);\
        return value;\
    }
    
#define R_FALSE_RET_DO_LOG(condition, value, action, fmt, args...)\
    if(!(condition))\
    {\
        LOG_PRINT(GREEN"[%s] [file: %s, line: %ld] [function: %s]\n   "\
            "FALSE("#condition")!, return  = %ld, "fmt"\n"NONE,\
            get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
            (_SL)value, ##args);\
        action;\
        return value;\
    }

#define S_ASSERT(fmt, args...)\
    LOG_PRINT(LIGHT_RED"[%s] [file: %s, line: %ld] [function: %s]\n   "\
        "ASSERT! "fmt"\n"NONE,\
        get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
        ##args);

#define V_ASSERT(condition)\
    if(!(condition))\
    {\
        LOG_PRINT(LIGHT_RED"[%s] [file: %s, line: %ld] [function: %s]\n   "\
            "ASSERT("#condition")!\n"NONE,\
            get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__);\
        return;\
    }

#define R_ASSERT(condition, value)\
    if(!(condition))\
    {\
        LOG_PRINT(LIGHT_RED"[%s] [file: %s, line: %ld] [function: %s]\n   "\
            "ASSERT("#condition")!, return  = %ld.\n"NONE,\
            get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
            (_SL)value);\
        return value;\
    }

#define R_ASSERT_DO(condition, value, action)\
    if(!(condition))\
    {\
        LOG_PRINT(LIGHT_RED"[%s] [file: %s, line: %ld] [function: %s]\n   "\
            "ASSERT("#condition")!, return  = %ld.\n"NONE,\
            get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
            (_SL)value);\
        action;\
        return value;\
    }

#define R_ASSERT_LOG(condition, value, fmt, args...)\
    if(!(condition))\
    {\
        LOG_PRINT(LIGHT_RED"[%s] [file: %s, line: %ld] [function: %s]\n   "\
            "ASSERT("#condition")!, return  = %ld, "fmt"\n"NONE,\
            get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
            (_SL)value, ##args);\
        return value;\
    }

#define R_ASSERT_DO_LOG(condition, value, action, fmt, args...)\
    if(!(condition))\
    {\
        LOG_PRINT(LIGHT_RED"[%s] [file: %s, line: %ld] [function: %s]\n   "\
            "ASSERT("#condition")!, return  = %ld, "fmt"\n"NONE,\
            get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
            (_SL)value, ##args);\
        action;\
        return value;\
    }

#endif

