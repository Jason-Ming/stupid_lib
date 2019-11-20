#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

#include "s_defines.h"
#include "s_log.h"
#include "s_mem.h"
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

ENUM_BOOLEAN whether_year_is_leapyear(_U32 year)
{
    return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)? BOOLEAN_TRUE:BOOLEAN_FALSE;
}


PRIVATE _S8 	daytab[2][13] =
{
	{
		0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	},
	{
		0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	}
};


/* day_of_year: set day of year from month & day */
ENUM_RETURN day_of_year(_S32 year, _S32 month, _S32 day, _S32 *yearday)
{
	_S32 i, leap;

    R_ASSERT(yearday != NULL, RETURN_FAILURE);
    R_ASSERT(year >= 1752 && month >= 1 && month <= 12 && day >= 1, RETURN_FAILURE);
    
	leap = whether_year_is_leapyear(year);

    R_ASSERT(day <= daytab[leap][month], RETURN_FAILURE);
    
	for (i = 1; i < month; i++)
	{
		day += daytab[leap][i];
	}

    *yearday = day;
    
	return RETURN_SUCCESS;
}


/* month_day: set month, day from day of year */
ENUM_RETURN month_day(_S32 year, _S32 yearday, _S32 * pmonth, _S32 * pday)
{
    R_ASSERT(pmonth != NULL, RETURN_FAILURE);
	R_ASSERT(pday != NULL, RETURN_FAILURE);
    R_ASSERT(year >= 1752 && yearday >= 1, RETURN_FAILURE);

    _S32 i, leap;
	leap = whether_year_is_leapyear(year);

    R_ASSERT((leap == BOOLEAN_FALSE && yearday <= 365) || (leap == BOOLEAN_TRUE && yearday <= 366), RETURN_FAILURE);
    
	for (i = 1; yearday > daytab[leap][i]; i++)
	{
		yearday -= daytab[leap][i];
	}

	*pmonth = i;
	*pday = yearday;

    return RETURN_SUCCESS;
}

/* month_name: return name of n-th month */ 
_S8 *month_name(_S32 moth)
{ 
    PRIVATE _S8 *	name[] =
        {
            "Illegal month", 
            "January", "February", "March", 
            "April", "May", "June", 
            "July", "August", "September", 
            "October", "November", "December"
        };

    R_ASSERT(moth >= 1 && moth <= SIZE_OF_ARRAY(name), NULL);

    return name[moth - 1];
}
 


