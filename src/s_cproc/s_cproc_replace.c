#include "s_text.h"
//#include <stdio.h/*sadf*/>
/*
1. 替换字符集
2. 处理连续断行
3. 处理注释
4. 输出token: normal string, blank, newline
*/
#include "s_alg.h"
#include <stdio.h>


#define ADD_PARAMETER(x)  5 + x##1

void foo(int x)
{
    int x1 = 0;
    x1 = MAX(MIN(4,5), 6) + x1;
}
int example_test()
{
    return 0;
}
/*
a+++++b
tokens: a ++ ++ + b
*/
#define NUMBERS(x) example_test()
#define NUMBERS1 NUMBERS

void fff()
{
    int ai = NUMBERS1(1);
}


