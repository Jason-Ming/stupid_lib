//#include <stdio.h/*sadf*/>
/*
1. 替换字符集
2. 处理连续断行
3. 处理注释
4. 输出token: normal string, blank, newline
*/
#include <stdio.h>
/*
#define FILE_B() "s_cproc_include_file.h"
#include FILE_B()FILE_B()

#define VA1(arg...,.) (##arg)
#define MACRO(A*&&& ... 123)
#define MACRO1(A, *&&& ... 123)
#define MACRO2(,A, *&&& ... 123)
#define MACRO2(A, a123,)
int fff()
{
    printf("%s", FILE_B");
}
*/
#define FILE_B "b.input\"
#define VA(...) __VA_ARGS__
#define aaaa "adsf
#define a\ \  
bc "abc /*\  
*/abc"
\

int asdf;
