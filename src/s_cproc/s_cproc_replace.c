#include "s_text.h"
//#include <stdio.h/*sadf*/>
/*
1. 替换字符集
2. 处理连续断行
3. 处理注释
4. 输出token: normal string, blank, newline
*/
//#include <stdio.h>
/*
#define FILE_B() "s_cproc_include_file.h"
#include FILE_B()FILE_B()

int fff()
{
    printf("%s", FILE_B");
}
*/




/*
Formally, preprocessing numbers begin with an optional period, a required decimal digit, and then continue with any sequence of letters, 
digits, underscores, periods, and exponents. Exponents are the two-character sequences ‘e+’, ‘e-’, ‘E+’, ‘E-’, ‘p+’, 
‘p-’, ‘P+’, and ‘P-’. (The exponents that begin with ‘p’ or ‘P’ are used for hexadecimal floating-point constants.)
*/
/*
#define E 234
#define NUMBER111 5.50xer__99ME+P-12.4_323
int x = 5aa.50xer__99ME+P-12.4_323S-10 + NUMBER111;
int x = 5_.50xer__99ME+P-12.4_323S-10 + NUMBER111;
*/

#define MACRO(abc__VA_ARGS__) __VA_ARGS__





#define ADD_PARAMETER(x)  5 + x##1
void foo(int x)
{
}
int example_test()
{
    return 0;
}
/*
a+++++b
tokens: a ++ ++ + b
*/
