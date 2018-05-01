#ifndef __DRAW_H__
#define __DRAW_H__

#include <stdio.h>
#include "clinkage.h"
#include "defines.h"

#define CHART_DATA_INFO_STR_SIZE 6 //长度为5的字符串

typedef struct TAG_STRU_CHART_DATA  
{
    int val;//数据值
    char info[CHART_DATA_INFO_STR_SIZE];//数据标签
}STRU_CHART_DATA;

__BEGIN_C_DECLS

ENUM_RETURN draw_histogram(FILE *fpw, STRU_CHART_DATA array[], int array_size);

__END_C_DECLS

#endif

