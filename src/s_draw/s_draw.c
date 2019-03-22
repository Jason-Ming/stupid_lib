#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "s_defines.h"
#include "s_limits.h"
#include "s_log.h"
#include "s_alg.h"

#include "s_draw.h"

STRU_POINT s_make_point(_S32 x, _S32 y)
{
    STRU_POINT temp;
    temp.x = x;
    temp.y = y;

    return temp;
}

STRU_POINT s_add_point(STRU_POINT p1, STRU_POINT p2)
{
    p1.x += p2.x;
    p1.y += p2.y;

    return p1;
}

_SD s_dist_of_points(STRU_POINT p1, STRU_POINT p2)
{
    return sqrt((_SD)(p1.x - p2.x) * (p1.x - p2.x) + (_SD)(p1.y - p2.y) * (p1.y - p2.y));
}

ENUM_BOOLEAN s_point_in_rectangle(STRU_POINT p, STRU_RECTANGLE rect)
{
    return p.x >= rect.p1.x && p.x <= rect.p2.x && p.y >= rect.p1.y && p.y <= rect.p2.y;
}

STRU_RECTANGLE s_canonicalize_rectangle(STRU_RECTANGLE rect)
{
    STRU_RECTANGLE temp;
    temp.p1.x = MIN(rect.p1.x, rect.p2.x);
    temp.p1.y = MIN(rect.p1.y, rect.p2.y);
    temp.p2.x = MAX(rect.p1.x, rect.p2.x);
    temp.p2.y = MAX(rect.p1.y, rect.p2.y);

    return temp;
}


