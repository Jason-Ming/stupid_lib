#ifndef __S_CPP_H__
#define __S_CPP_H__
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"


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
ENUM_RETURN s_cpp(
    FILE *pfr,
    _S8 **pp_text_buffer, 
    size_t *p_text_buffer_size, 
    STRU_C_TOKEN_NODE **pp_token_list_head,
    STRU_C_TOKEN_NODE **pp_token_list_tail);

_VOID s_cpp_print(
    FILE *pfw,
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_tail);

__END_C_DECLS
#endif
