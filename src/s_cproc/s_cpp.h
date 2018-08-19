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
        else\
        {\
            printf(fmt, ##args);\
        }\
    }while(0);


__BEGIN_C_DECLS
/*
The preprocessor performs a series of textual transformations on its input. 

1.The input file is read into memory and broken into lines.
2.If trigraphs are enabled, they are replaced by their corresponding single characters.
3.Continued lines are merged into one long line.
4.All comments are replaced with single spaces.

the text after transform is output to a file.
if the file pointer is NULL, it will be output to a buffer which is allocated by this function. 
if the buffer is NULL, it will be output to the standard IO.
*/
ENUM_RETURN s_cpp(  const _S8 * file_name, STACK stack);

__END_C_DECLS
#endif

