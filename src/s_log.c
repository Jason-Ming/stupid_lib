#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "s_defines.h"
#include "s_log.h"

PRIVATE FILE *logfp = NULL;
#define MAX_LENGTH_OF_LOG_FILE_NAME 128
char log_file_name[MAX_LENGTH_OF_LOG_FILE_NAME] = {'\0'};

ENUM_BOOLEAN log_isready(void)
{
    if(logfp == NULL)
    {
        return BOOLEAN_FALSE;
    }
    else
    {
        return BOOLEAN_TRUE;
    }
}

FILE *log_getfp(void)
{
    return logfp;
}

ENUM_RETURN log_init(void)
{
    log_file_name[0] = '\0';
    sprintf(log_file_name, "LOG_FILE_%s_%s.txt", get_date_string(), get_time_string());

    //printf("logfile: %s\n", log_file_name);
    
    logfp = fopen(log_file_name, "w");
    R_ASSERT(logfp != NULL, RETURN_FAILURE);
    fclose(logfp);
    return RETURN_SUCCESS;
}

char* log_getfn(void)
{
    if(strlen(log_file_name) != 0)
    {
        return log_file_name;
    }
    else
    {
        assert(log_init() == RETURN_SUCCESS);
        
        return log_file_name;
    }
}



void f(void)
{
    R_LOG("format= %d.\n", 5);
}

