#ifndef __S_ALG_H__
#define __S_ALG_H__

#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"


__BEGIN_C_DECLS

/* binsearch: find x in v[0] <= v[1] <= ... <= v[n-1] */ 
_S32 binsearch(_S32 x, _S32 v[], _S32 n);
_S32 binsearch2(_S32 x, _S32 v[], _S32 n);

/* O(n^2) */
_VOID insert_sort(_S32 a[], _S32 n);

/* shell sort O(n^2) */
_VOID shell_sort(_S32 a[], _S32 n);

/**
 * @author: Jason Ming
 * @description: calculate the reverse polish expression
 * @param str: the expression, see expatiation followed function declaration
 * @param result: store the calculated result 
 * @return: RETURN_SUCCESS£¬RETURN_FAILUE
 */
ENUM_RETURN s_calc_rp(const _S8 *str, _SD *result);
/* expression includes numbers, operators and vars, they must be separated with space char */
/* number: Floating-point constants, push it to the stack */
/* operator: pop the numbers from stack, calculate and push the result to the stack, operator includes: */
/*      +, -, *, /, %, sin, cos, exp, pow, clr, swp, top, dup, = */
/* var: start with english letters, length less than 32, if exceed, the var'value always equal to 0.0 */
/*      this lib can store up to 64 variables, if exceed, the var'value always equal to 0.0 */
/*      when the var appears, it will get the var's value, and push the value to the stack */
/*      if the var'value is not exist, assign 0 to it. */
/*      The '=' operator will assign the value in the top of stack to it */

/* the following expressions are examples: v1 and v2 are numbers, x1 is variables */
/*      +: [v1 v2 +] push(v1), push(v2), pop(v2), pop(v1), push(v1+v2) */
/*      -: [v1 v2 -] push(v1), push(v2), pop(v2), pop(v1), push(v1-v2) */
/*      *: [v1 v2 *] push(v1), push(v2), pop(v2), pop(v1), push(v1*v2) */
/*      /: [v1 v2 /] push(v1), push(v2), pop(v2), pop(v1), push(v1/v2) ,if v2 equals 0, result is inf */
/*      %: [v1 v2 %] push(v1), push(v2), pop(v2), pop(v1), push(v1%v2) ,if v2 equals 0, result is nan */
/*    sin: [v1 sin] push(v1), pop(v1), push(sin(v1)) , v1 is radian */
/*    cos: [v1 cos] push(v1), pop(v1), push(cos(v1)) , v1 is radian */
/*    exp: [v1 exp] push(v1), pop(v1), push(exp(v1)) */
/*    pow: [v1 v2 pow] push(v1), push(v2), pop(v2), pop(v1), push(pow(v1,v2)) */
/*    clr: clear all numbers in the stack */
/*    swp: [v1 v2 swp] push(v1), push(v2), pop(v2), pop(v1), push(v2), push(v1) */
/*    top: [v1 top] push(v1), pop(v1), print(v1) push(v1) */
/*    dup: [v1 dup] push(v1), pop(v1), push(v1), push(v1) */
/*      =: [v1 x1 =] push(v1) get_value(x1), push(x1), pop(x1), pop(v1), set_value(x1, v1) push(v1) */


__END_C_DECLS

#endif

