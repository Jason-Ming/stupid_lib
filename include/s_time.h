#ifndef __S_TIME_H__
#define __S_TIME_H__

#include "s_clinkage.h"
#include "s_type.h"

__BEGIN_C_DECLS

char* get_date_string(void);
char* get_time_string(void);
char* get_time_stamp(void);
ENUM_BOOLEAN is_leapyear(_U32 year);

__END_C_DECLS

#endif


