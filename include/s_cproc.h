#ifndef __S_CPROC_H__
#define __S_CPROC_H__
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"


typedef enum TAG_ENUM_DCL_TOKEN
{
    DCL_TOKEN_PARENTHESIS_LEFT = 0,  /* ( */
    DCL_TOKEN_PARENTHESIS_RIGHT,     /* ) */
    DCL_TOKEN_STAR,                  /* * */
    DCL_TOKEN_BRACKET_LEFT,          /* [ */
    DCL_TOKEN_BRACKET_RIGHT,         /* ] */
    DCL_TOKEN_BRACE_LEFT,            /* { */
    DCL_TOKEN_BRACE_RIGHT,           /* } */
    DCL_TOKEN_COMMA,                 /* , */
    DCL_TOKEN_SEMICOLON,             /* ; */
    DCL_TOKEN_TYPE,                  /* int, char, short, long, float, double, unsigned, etc.. */
    DCL_TOKEN_IDENTIFIER,            /* x??, _?? */
    DCL_TOKEN_NUMBER,                /* 10 */
    DCL_TOKEN_CONTROL,               /* if, break, etc.. */
    DCL_TOKEN_INVALID,
    DCL_TOKEN_MAX,
}ENUM_DCL_TOKEN;

typedef enum TAG_ENUM_DCL_TYPE
{
    DCL_TYPE_ARRAY = 0,
    DCL_TYPE_POINTER,
    DCL_TYPE_FUNCTION,
    DCL_TYPE_STRUCT,
    DCL_TYPE_ENUM,
    DCL_TYPE_UNION,
    DCL_TYPE_NORMAL,/* system type like int, char... */
    DCL_TYPE_INVALID,
    DCL_TYPE_MAX,
}ENUM_DCL_TYPE;

__BEGIN_C_DECLS
_S8 * get_dcl_token_str(ENUM_DCL_TOKEN token);
_S8 * get_dcl_type_str(ENUM_DCL_TYPE type);
ENUM_BOOLEAN is_keyword_type(_S8 *string);
ENUM_BOOLEAN is_keyword_control(_S8 *string);
ENUM_BOOLEAN is_keyword(_S8 *string);
ENUM_BOOLEAN is_identifier(_S8 *string);

/* delete comment from source file, and store new content to another file */
ENUM_RETURN s_delcmnt(FILE *pfr, FILE *pfw);


/* be sure the content in file does not contain any comment��macro and had been checked pairing match */
ENUM_RETURN get_statement(FILE * pfr, _S8 statement_buffer[], size_t buffer_size, size_t *len);

/* make sure you get the statement from 'get_statement' */
ENUM_RETURN get_token(_S8 * statement, _S8 token_buffer[], size_t buffer_size, size_t *len, ENUM_DCL_TOKEN *token, _S8 **next_token);

/* make sure you get the statement from 'get_statement', and at present, this function could parse declaration only */
ENUM_RETURN parse_statement(_S8 *statement);
__END_C_DECLS
#endif
