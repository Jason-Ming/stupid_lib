#include <time.h>
#include <sys/time.h>
#include <string.h>

#include "s_defines.h"
#include "s_time.h"

#define TIME_STRING_BUFFER_SIZE 128
#define LENGTH_OF_USECS 24

char* get_usec_string(void)
{
    PRIVATE struct timeval tv;
    PRIVATE char time_string_buf[LENGTH_OF_USECS];
    gettimeofday(&tv, NULL);
    memset(time_string_buf, '\0', LENGTH_OF_USECS);
    sprintf(time_string_buf, "%lu", tv.tv_usec);
    return time_string_buf;
}

char* get_time_string(void)
{
    PRIVATE char time_string_buf[TIME_STRING_BUFFER_SIZE];
    time_t current_time = time(&current_time);
    
    struct tm *ptm = localtime(&current_time);

    strftime(time_string_buf, TIME_STRING_BUFFER_SIZE, "%T", ptm);
    
    return time_string_buf;
}

char* get_date_string(void)
{
    PRIVATE char date_string_buf[TIME_STRING_BUFFER_SIZE];
    time_t current_time = time(&current_time);
    
    struct tm *ptm = localtime(&current_time);

    strftime(date_string_buf, TIME_STRING_BUFFER_SIZE, "%F", ptm);
    
    return date_string_buf;
}

char* get_time_stamp(void)
{
    PRIVATE char time_string_buf[TIME_STRING_BUFFER_SIZE*2 + LENGTH_OF_USECS +2];

    sprintf(time_string_buf, "%s %s %s", get_date_string(), get_time_string(), get_usec_string());
    
    return time_string_buf;
}

