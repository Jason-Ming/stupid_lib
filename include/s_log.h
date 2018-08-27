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

#define LOG_PRINT(fmt, ...)\
    do\
    {\
        FILE *fp = fopen(log_getfn(), "a");\
        if(fp != NULL)\
        {\
            fprintf(fp, fmt, __VA_ARGS__);\
            fclose(fp);\
        }\
        else\
        {\
            printf(fmt, __VA_ARGS__);\
        }\
    }while(0);
#else
#define LOG_PRINT(fmt, args...)\
    printf(fmt, ##args);
#endif

#define DEBUG_SWITCH 1

#if (DEBUG_SWITCH == 1)
#define DEBUG_PRINT(fmt, ...)\
    LOG_PRINT(LIGHT_GRAY"[%s] [file: %s, line: %ld] [function: %s]\n"NONE"   debuginfo: "fmt""NONE"\n", \
        get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__, ##__VA_ARGS__);
#else
#define DEBUG_PRINT(fmt, args...)
#endif

//不能用编译时间，蠢货!
#define S_LOG(fmt, args...)\
	LOG_PRINT(GREEN"[%s] [file: %s, line: %ld] [function: %s]\n   "\
		fmt"\n"NONE,\
		get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
		##args);\

#define S_FALSE_LOG(condition, fmt, args...)\
	if(!(condition))\
	{\
		LOG_PRINT(GREEN"[%s] [file: %s, line: %ld] [function: %s]\n   "\
			"FALSE("#condition")!, "fmt"\n"NONE,\
			get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
        ##args);\
	}
#define S_FALSE_DO(condition, action)\
	if(!(condition))\
	{\
		action;\
	}

#define S_FALSE_LOG_DO(condition, action, fmt, args...)\
	if(!(condition))\
	{\
		LOG_PRINT(GREEN"[%s] [file: %s, line: %ld] [function: %s]\n   "\
			"FALSE("#condition")!, "fmt"\n"NONE,\
			get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
        ##args);\
		action;\
	}

#define S_V_FALSE(condition)\
    if(!(condition))\
    {\
        return;\
    }

#define S_R_FALSE(condition, value)\
    if(!(condition))\
    {\
        return value;\
    }

#define S_R_FALSE_DO(condition, value, action)\
    if(!(condition))\
    {\
        action;\
        return value;\
    }

#define S_R_FALSE_LOG(condition, value, fmt, args...)\
    if(!(condition))\
    {\
        LOG_PRINT(GREEN"[%s] [file: %s, line: %ld] [function: %s]\n   "\
            "FALSE("#condition")!, return  = %ld, "fmt"\n"NONE,\
            get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
            (_SL)value, ##args);\
        return value;\
    }
    
#define S_R_FALSE_LOG_DO(condition, value, action, fmt, args...)\
    if(!(condition))\
    {\
        LOG_PRINT(GREEN"[%s] [file: %s, line: %ld] [function: %s]\n   "\
            "FALSE("#condition")!, return  = %ld, "fmt"\n"NONE,\
            get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
            (_SL)value, ##args);\
        action;\
        return value;\
    }

#define S_ASSERT(condition)\
    if(!(condition))\
    {\
        LOG_PRINT(LIGHT_RED"[%s] [file: %s, line: %ld] [function: %s]\n   "\
            "ASSERT("#condition")!\n"NONE,\
            get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__);\
    }

#define S_ASSERT_DO(condition, action)\
	if(!(condition))\
	{\
		LOG_PRINT(LIGHT_RED"[%s] [file: %s, line: %ld] [function: %s]\n   "\
			"ASSERT("#condition")!\n"NONE,\
			get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__);\
		action;\
	}

#define S_ASSERT_LOG(condition, fmt, args...)\
	if(!(condition))\
	{\
		LOG_PRINT(LIGHT_RED"[%s] [file: %s, line: %ld] [function: %s]\n   "\
			"ASSERT("#condition")! "fmt"\n"NONE,\
			get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
			##args);\
	}


#define S_ASSERT_LOG_DO(condition, action, fmt, args...)\
	if(!(condition))\
	{\
		LOG_PRINT(LIGHT_RED"[%s] [file: %s, line: %ld] [function: %s]\n   "\
			"ASSERT("#condition")! "fmt"\n"NONE,\
			get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
			##args);\
		action;\
	}

#define S_V_ASSERT(condition)\
	if(!(condition))\
	{\
		LOG_PRINT(LIGHT_RED"[%s] [file: %s, line: %ld] [function: %s]\n   "\
			"ASSERT("#condition")!\n"NONE,\
			get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__);\
		return;\
	}

#define S_V_ASSERT_DO(condition, action)\
	if(!(condition))\
	{\
		LOG_PRINT(LIGHT_RED"[%s] [file: %s, line: %ld] [function: %s]\n   "\
			"ASSERT("#condition")!, return	= %ld.\n"NONE,\
			get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
			(_SL)value);\
		action;\
		return;\
	}

#define S_V_ASSERT_LOG(condition, fmt, args...)\
	if(!(condition))\
	{\
		LOG_PRINT(LIGHT_RED"[%s] [file: %s, line: %ld] [function: %s]\n   "\
			"ASSERT("#condition")!, return	= %ld, "fmt"\n"NONE,\
			get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
			(_SL)value, ##args);\
		return;\
	}

#define S_V_ASSERT_LOG_DO(condition, action, fmt, args...)\
	if(!(condition))\
	{\
		LOG_PRINT(LIGHT_RED"[%s] [file: %s, line: %ld] [function: %s]\n   "\
			"ASSERT("#condition")!, return	= %ld, "fmt"\n"NONE,\
			get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
			(_SL)value, ##args);\
		action;\
		return;\
	}

#define S_R_ASSERT(condition, value)\
	if(!(condition))\
	{\
		LOG_PRINT(LIGHT_RED"[%s] [file: %s, line: %ld] [function: %s]\n   "\
			"ASSERT(" # condition")!, return  = %ld.\n"NONE,\
			get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
			(_SL)value);\
		return value;\
	}

#define S_R_ASSERT_DO(condition, value, action)\
	if(!(condition))\
	{\
		LOG_PRINT(LIGHT_RED"[%s] [file: %s, line: %ld] [function: %s]\n   "\
			"ASSERT("#condition")!, return	= %ld.\n"NONE,\
			get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
			(_SL)value);\
		action;\
		return value;\
	}

#define S_R_ASSERT_LOG(condition, value, fmt, args...)\
	if(!(condition))\
	{\
		LOG_PRINT(LIGHT_RED"[%s] [file: %s, line: %ld] [function: %s]\n   "\
			"ASSERT("#condition")!, return	= %ld, "fmt"\n"NONE,\
			get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
			(_SL)value, ##args);\
		return value;\
	}

#define S_R_ASSERT_LOG_DO(condition, value, action, fmt, args...)\
	if(!(condition))\
	{\
		LOG_PRINT(LIGHT_RED"[%s] [file: %s, line: %ld] [function: %s]\n   "\
			"ASSERT("#condition")!, return	= %ld, "fmt"\n"NONE,\
			get_time_stamp(), __FILE__, (_SL)__LINE__, __FUNCTION__,\
			(_SL)value, ##args);\
		action;\
		return value;\
	}

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
            "ASSERT(" # condition")!, return  = %ld.\n"NONE,\
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

