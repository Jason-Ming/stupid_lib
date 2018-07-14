#ifndef __S_TIME_H__
#define __S_TIME_H__

#include "s_clinkage.h"
#include "s_type.h"

__BEGIN_C_DECLS

_S8* get_date_string(_VOID);
_S8* get_time_string(_VOID);
_S8* get_time_stamp(_VOID);
ENUM_BOOLEAN whether_year_is_leapyear(_U32 year);

/* day_of_year: set day of year from month & day */
ENUM_RETURN day_of_year(_S32 year, _S32 month, _S32 day, _S32 *yearday);


/* month_day: set month, day from day of year */
ENUM_RETURN month_day(_S32 year, _S32 yearday, _S32 * pmonth, _S32 * pday);

/* month_name: return name of n-th month */ 
_S8 *month_name(_S32 moth);

__END_C_DECLS

#endif


