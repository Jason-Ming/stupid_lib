#ifndef __S_DRAW_H__
#define __S_DRAW_H__

#include <stdio.h>
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"

typedef struct TAG_STRU_POINT
{
    _S32 x;
    _S32 y;
}STRU_POINT;

typedef struct TAG_STRU_RECTANGLE
{
    STRU_POINT p1;
    STRU_POINT p2;
}STRU_RECTANGLE;


__BEGIN_C_DECLS

/* s_make_point: make a point from x and y components */
STRU_POINT s_make_point(_S32 x, _S32 y);

/* s_add_point: add two points */
STRU_POINT s_add_point(STRU_POINT p1, STRU_POINT p2);

/* s_point_in_rectangle: return 1 if p in rect, 0 if not */
ENUM_BOOLEAN s_point_in_rectangle(STRU_POINT p, STRU_RECTANGLE rect);

/* s_canonicalize_rectangle: canonicalize coordinates of rectangle */
STRU_RECTANGLE s_canonicalize_rectangle(STRU_RECTANGLE rect);


__END_C_DECLS

#endif

