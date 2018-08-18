#ifndef __S_CPROC_H__
#define __S_CPROC_H__
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"


__BEGIN_C_DECLS

/* check a C program for rudimentary syntax errors like unbalanced parentheses, brackets and braces. 
   If check is passed, check_result is 1 */
ENUM_RETURN s_cchk_pair(FILE *pfr, ENUM_RETURN *check_result);

/* statement means: code separated by semicolon.
   be sure the content in text_buffer does not contain any comment¡¢macro and had been checked pairing match
   the result does not include semicolon at the end 
   after processed, *pp_text_buffer is point to next statement
*/
ENUM_RETURN s_cget_statement(_S8 **pp_text_buffer, _S8 statement_buffer[], size_t buffer_size, size_t *len);

/*
The c compiler compile on its input. 

1.preprocess input file.
2.process declaration.
4.others.

the text after transform of 1 is output to a file.
if the file pointer is NULL, it will be output to a buffer which is allocated by this function. 
if the buffer is NULL, it will be output to the standard IO.
*/
ENUM_RETURN s_cc(FILE * pfr, FILE * pfw);

__END_C_DECLS
#endif
