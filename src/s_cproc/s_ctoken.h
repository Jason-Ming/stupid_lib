#ifndef __S_C_TOKEN_H__
#define __S_C_TOKEN_H__
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"
#include "s_list.h"
#include "s_cproc.h"

#define MAX_TOKEN_LEN 1024

typedef enum TAG_ENUM_C_TOKEN
{
    C_TOKEN_NORMAL = 0,                                           /* normal */
    C_TOKEN_BLANK,                                            /* continued spaces */
    C_TOKEN_CONTINUED_NEWLINE,                                /* continued newline */
    C_TOKEN_NEWLINE_LINUX,                                    /* \n */
    C_TOKEN_NEWLINE_WINDOWS,                                  /* \r\n */
    C_TOKEN_NEWLINE_MAC,                                      /* \r */
    C_TOKEN_LINE_COMMENT,                                     /* line comment */
    C_TOKEN_PAIR_COMMENT,                                     /* pair comment */

    C_TOKEN_PPD,                                              /* Preprocessing directive: # */
    C_TOKEN_PPD_STRINGIFICATION,                              /* Preprocessing directive: stringification # */
    C_TOKEN_PPD_CONCATENATE,                                  /* Preprocessing directive: concatenate ## */
    C_TOKEN_PPD_DEFINED,                                      /* Preprocessing directive: defined */

    /* group:
        group-part
        group group-part */

    /* group-part:
        if-section
        control-line
        text-line
        # non-directive */
    
    /* if-section:
        if-group elif-groups_opt else-group_opt endif-line */

    /* if-group:
        # if constant-expression new-line group_opt
        # ifdef identifier new-line group_opt
        # ifndef identifier new-line group_opt */
    C_TOKEN_PPD_IF,                                           /* Preprocessing directive: if */
    C_TOKEN_PPD_IFDEF,                                        /* Preprocessing directive: ifdef */
    C_TOKEN_PPD_IFNDEF,                                       /* Preprocessing directive: ifndef */

    
    /* elif-groups:
        elif-group
        elif-groups elif-group */

    /* elif-group:
        # elif constant-expression new-line group_opt */
    C_TOKEN_PPD_ELIF,                                         /* Preprocessing directive: elif */

    /* else-group:
        # else new-line group_opt */
    C_TOKEN_PPD_ELSE,                                         /* Preprocessing directive: else */

    /* endif-line:
        # endif new-line */
    C_TOKEN_PPD_ENDIF,                                        /* Preprocessing directive: endif */
    C_TOKEN_PPD_UNKNOWN,                                      /* Preprocessing directive: unknown */

    /* control-line:
        # include pp-tokens new-line
        # define identifier replacement-list new-line
        # define identifier lparen identifier–list_opt ) replacement-list new-line
        # define identifier lparen ... ) replacement-list new-line
        # define identifier lparen identifier–list_opt , ... ) replacement-list new-line
        # undef identifier new-line
        # line pp-tokens new-line
        # error pp-tokens_opt new-line
        # pragma pp-tokens_opt new-line
        # new-line */
    C_TOKEN_PP_CONTROL_INCLUDE,                               /* Preprocessing directive: include */
    C_TOKEN_PP_CONTROL_DEFINE,                                /* Preprocessing directive: define */
    C_TOKEN_PP_CONTROL_UNDEF,                                 /* Preprocessing directive: undef */
    C_TOKEN_PP_CONTROL_LINE,                                  /* Preprocessing directive: line */
    C_TOKEN_PP_CONTROL_ERROR,                                 /* Preprocessing directive: error */
    C_TOKEN_PP_CONTROL_PRAGMA,                                /* Preprocessing directive: pragma */
    
    /* text-line:
        pp-tokens_opt new-line */

    /* non-directive:
        pp-tokens new-line */

    /* replacement-list:
        pp-tokens_opt */

    /* pp-tokens:
        preprocessing-token
        pp-tokens preprocessing-token */
    
    /* preprocessing-token: 
        头文件名,
        标识符, 
        预处理数字,
        字符常量,
        字符串文字量,
        标点符号,
        以及在词法上不匹配其它预处理记号类别的非空白符 */
    C_TOKEN_PP_HEADER_NAME,                                   /* Preprocessing: header name */
    C_TOKEN_PP_HEADER_H_START,                                /* Preprocessing: header < */
    C_TOKEN_PP_HEADER_H_FINISH,                               /* Preprocessing: header > */
    C_TOKEN_PP_HEADER_Q_START,                                /* Preprocessing: header " */
    C_TOKEN_PP_HEADER_Q_FINISH,                               /* Preprocessing: header " */
    C_TOKEN_PP_IDENTIFIER,                                    /* Preprocessing: identifier */
    C_TOKEN_PP_NUMBER,                                        /* Preprocessing: number */
    C_TOKEN_PP_CHARACTER_CONSTANT,                            /* Preprocessing: character constant */
    C_TOKEN_PP_STRING_LITERAL,                                /* Preprocessing: string literal */
    C_TOKEN_PP_PUNCTUATOR,                                    /* Preprocessing: punctuator */
    C_TOKEN_PP_PLACEHOLDER,                                   /* Preprocessing: placeholder */
    C_TOKEN_PP_IGNORE,                                        /* Preprocessing: ignore */

    C_TOKEN_PARENTHESIS_LEFT,                                 /* ( */
    C_TOKEN_PARENTHESIS_RIGHT,                                /* ) */
    C_TOKEN_BRACKET_LEFT,                                     /* [ */
    C_TOKEN_BRACKET_RIGHT,                                    /* ] */
    C_TOKEN_BRACE_LEFT,                                       /* { */
    C_TOKEN_BRACE_RIGHT,                                      /* } */
    C_TOKEN_COMMA,                                            /* , */
    C_TOKEN_SEMICOLON,                                        /* ; */
    C_TOKEN_VA_ARGS,                                          /* ... */

    C_TOKEN_OPERATOR_MEMBER,                                  /* . */
    C_TOKEN_OPERATOR_MEMBER_POINTER,                          /* -> */
    C_TOKEN_OPERATOR_ARITHMETIC_ADD,                          /* + */
    C_TOKEN_OPERATOR_ARITHMETIC_INCREASE,                     /* ++ */
    C_TOKEN_OPERATOR_ARITHMETIC_ADD_ASSIGN,                   /* += */
	C_TOKEN_OPERATOR_ARITHMETIC_SUBTRACT,                     /* - */
	C_TOKEN_OPERATOR_ARITHMETIC_DECREASE,                     /* -- */
	C_TOKEN_OPERATOR_ARITHMETIC_SUBTRACT_ASSIGN,              /* -= */
    C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY,                     /* * */
	C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY_ASSIGN,              /* *= */
	C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE,                       /* / */
	C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE_ASSIGN,                /* /= */
	C_TOKEN_OPERATOR_ARITHMETIC_MOD,                          /* % */
	C_TOKEN_OPERATOR_ARITHMETIC_MOD_ASSIGN,                   /* %= */
	C_TOKEN_OPERATOR_LOGICAL_GREATER,                         /* > */
	C_TOKEN_OPERATOR_LOGICAL_GREATER_EQUAL,                   /* >= */
	C_TOKEN_OPERATOR_BIT_RIGHT_SHIFT,                         /* >> */
	C_TOKEN_OPERATOR_BIT_RIGHT_SHIFT_ASSIGN,                  /* >>= */
	C_TOKEN_OPERATOR_LOGICAL_LESS,                            /* < */
	C_TOKEN_OPERATOR_LOGICAL_LESS_EQUAL,                      /* <= */
	C_TOKEN_OPERATOR_BIT_LEFT_SHIFT,                          /* << */
	C_TOKEN_OPERATOR_BIT_LEFT_SHIFT_ASSIGN,                   /* <<= */
	C_TOKEN_OPERATOR_ASSIGN,                                  /* = */
	C_TOKEN_OPERATOR_LOGICAL_EQUAL,                           /* == */
	C_TOKEN_OPERATOR_LOGICAL_NOT_EQUAL,                       /* != */
	C_TOKEN_OPERATOR_BIT_AND,                                 /* & */
	C_TOKEN_OPERATOR_BIT_AND_ASSIGN,                          /* &= */
	C_TOKEN_OPERATOR_LOGICAL_AND,                             /* && */
	C_TOKEN_OPERATOR_BIT_OR,                                  /* | */
	C_TOKEN_OPERATOR_BIT_OR_ASSIGN,                           /* |= */
	C_TOKEN_OPERATOR_LOGICAL_OR,                              /* || */
	C_TOKEN_OPERATOR_BIT_XOR,                                 /* ^ */
	C_TOKEN_OPERATOR_BIT_XOR_ASSIGN,                          /* ^= */
	C_TOKEN_OPERATOR_BIT_NOT,                                 /* ~ */
	C_TOKEN_OPERATOR_LOGICAL_NOT,                             /* ! */
	C_TOKEN_OPERATOR_LOGICAL_QUESTION,                        /* ? */
	C_TOKEN_OPERATOR_LOGICAL_COLON,                           /* : */
    
    C_TOKEN_KEYWORD_TYPE,                                     /* c keyword: type */
    C_TOKEN_KEYWORD_TYPE_QUALIFIER,                           /* c keyword: qualifier */
    C_TOKEN_KEYWORD_CONTROL,                                  /* c keyword: control */
    C_TOKEN_IDENTIFIER,                                       /* c identifier */
    C_TOKEN_CONSTANT_INTEGER,                                 /* c constant: integer */
    C_TOKEN_CONSTANT_FLOAT,                                   /* c constant: float */
    C_TOKEN_STRING,                                           /* c constant: string */
    C_TOKEN_CHAR,                                             /* c constant: character */

    C_TOKEN_END,                                              /* END */
    C_TOKEN_UNKNOWN,                                          /* UNKNOWN */
    C_TOKEN_INVALID,                                          /* INVALID */
    C_TOKEN_MAX,
}ENUM_C_TOKEN;

typedef struct TAG_STRU_C_TOKEN_POSITION
{
	size_t offset;
    size_t line;
    size_t column;
}STRU_C_TOKEN_POSITION;

typedef struct TAG_STRU_C_TYPE_QUALIFIER
{
    ENUM_BOOLEAN is_const;
    ENUM_BOOLEAN is_auto;
    ENUM_BOOLEAN is_extern;
    ENUM_BOOLEAN is_register;
    ENUM_BOOLEAN is_static;
    ENUM_BOOLEAN is_volatile;
}STRU_C_TYPE_QUALIFIER;

typedef struct TAG_STRU_C_TOKEN
{
    _S8 *p_string;
    _SL inode;
    STRU_C_TOKEN_POSITION line_position;
    ENUM_C_TOKEN token_type;
    STRU_C_TYPE_QUALIFIER qualifier;
}STRU_C_TOKEN;

typedef struct TAG_STRU_C_TOKEN_NODE
{
	STRU_C_TOKEN info;
    struct list_head list;
}STRU_C_TOKEN_NODE;

__BEGIN_C_DECLS

ENUM_C_TOKEN s_ctoken_parse_identifier(_S8 *string);

const _S8 * s_ctoken_get_str(ENUM_C_TOKEN token);
ENUM_RETURN s_ctoken_copy(
    const STRU_C_TOKEN_NODE *p_token_node_source, 
    STRU_C_TOKEN_NODE **pp_token_node_dest);

ENUM_RETURN s_ctoken_make_new_node_and_add_to_list(
    STRU_C_TOKEN_NODE *p_token_list_head,
    const _S8 *token_string, 
    ENUM_C_TOKEN token_type, 
    size_t text_offset,
    size_t line_index,
    size_t line_column);

ENUM_RETURN s_ctoken_add_node_to_list(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_new_token_node);

ENUM_RETURN s_ctoken_delete_node_from_list(
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_to_be_deleted);

ENUM_RETURN s_ctoken_get_token_type(const _S8 *p_string, ENUM_C_TOKEN *p_type);
ENUM_RETURN s_ctoken_get_token(
	const _S8 *p_text_buffer, 
	size_t *len, 
	ENUM_C_TOKEN *token);
_VOID s_ctoken_delete_blanks_and_newline_from_list(STRU_C_TOKEN_NODE *p_token_list_head);

_VOID s_ctoken_release_list(STRU_C_TOKEN_NODE *p_token_list_head);
_VOID s_ctoken_free_node(STRU_C_TOKEN_NODE *p_token_to_be_deleted);

_VOID s_ctoken_print_list(
    FILE *fpw,
    STRU_C_TOKEN_NODE *p_token_list_head,
    STRU_C_TOKEN_NODE *p_token_list_start, 
    STRU_C_TOKEN_NODE *p_token_list_end);
_VOID s_ctoken_print_list_debug_info(STRU_C_TOKEN_NODE *p_token_list_head);

__END_C_DECLS
#endif

