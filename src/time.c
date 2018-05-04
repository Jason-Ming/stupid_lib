#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "defines.h"

#define TIME_STRING_BUFFER_SIZE 128
#define LENGTH_OF_USECS 24

char* get_time_string_usec(void)
{
    PRIVATE struct timeval tv;
    PRIVATE char time_string_buf[LENGTH_OF_USECS];
    gettimeofday(&tv, NULL);
    sprintf(time_string_buf, "%lu", tv.tv_usec);
    return time_string_buf;
}

char* get_time_string_without_usec(void)
{
    PRIVATE char time_string_buf[TIME_STRING_BUFFER_SIZE];
    time_t current_time = time(&current_time);
    
    struct tm *ptm = localtime(&current_time);

    strftime(time_string_buf, TIME_STRING_BUFFER_SIZE, "%F %T", ptm);
    
    return time_string_buf;
}

char* get_time_string_with_usec(void)
{
    PRIVATE char time_string_buf[TIME_STRING_BUFFER_SIZE];

    if(strlen(get_time_string_without_usec()) < TIME_STRING_BUFFER_SIZE - LENGTH_OF_USECS -2)
    {
        sprintf(time_string_buf, "%s %s", get_time_string_without_usec(), get_time_string_usec());
    }
    
    return time_string_buf;
}

