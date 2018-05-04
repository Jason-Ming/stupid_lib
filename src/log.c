#include <stdio.h>
#include <string.h>

#include "defines.h"


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
        return BOOLEAN_TURE;
    }
}

FILE *log_getfp(void)
{
    return logfp;
}

char* log_getfn(void)
{
    if(strlen(log_file_name) != 0)
    {
        return log_file_name;
    }
    else
    {
        return NULL;
    }
}

ENUM_RETURN log_init(void)
{
    log_file_name[0] = '\0';
    sprintf(log_file_name, "LOG_FILE-%s.txt", get_time_string_without_usec());

    printf("filename = %s\n", log_file_name);
    
    logfp = fopen(log_file_name, "w");
    R_ASSERT(logfp != NULL, RETURN_FAILURE);
    fclose(logfp);
    return RETURN_SUCCESS;
}

void f(void)
{
    R_LOG("format= %d.\n", 5);
}

