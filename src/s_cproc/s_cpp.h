#ifndef __S_CPP_H__
#define __S_CPP_H__
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"
#include "s_stack.h"
#include "s_ctoken.h"

#define CPP_PRINT(fpw, fmt, args...)\
    do\
    {\
        if(fpw != NULL)\
        {\
            fprintf(fpw, fmt, ##args);\
        }\
        printf(LIGHT_BLUE""fmt""NONE, ##args);\
    }while(0);


__BEGIN_C_DECLS
/*
The preprocessor performs a series of textual transformations on its input. 

1.The input file is read into memory and broken into lines.
2.If trigraphs are enabled, they are replaced by their corresponding single characters.
3.Continued lines are merged into one long line.
4.All comments are replaced with single spaces.
5.preprocessing:
    1.expand marcos
    2.include files and recursively process preprocessing
    3.delete preprocessing directives
6.process escape character
7.combine strings

预处理记号(preprocessing-token)定义为:
    头文件名(header-name)
    标识符(identifier)
    预处理数字(pp-number)
    字符常量(character-constant)
    字符串文字量(string-literal)
    标点符号(punctuator)
    每个不属于上述各项的非空白符

8.lexical analysis, grammatical analysis, semantic analysis, intermediate code generating
记号(token)定义为:
        关键字(keyword)
        标识符(identifier)
        常量(constant)
        字符串文字量(string-literal)
        标点符号(punctuator)

9.linking, excutive code generating

the text after transform is output to a file.
if the file pointer is NULL, it will be output to a buffer which is allocated by this function. 
if the buffer is NULL, it will be output to the standard IO.
*/
/* make sure the file_name is the realpath */
ENUM_RETURN s_cpp(const _S8 * file_name, ENUM_RETURN *check_result);

__END_C_DECLS
#endif

