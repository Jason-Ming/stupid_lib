#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "s_log.h"
#include "s_mem.h"
#include "s_text.h"
#include "s_stack.h"
#include "s_stm.h"
#include "s_diagram.h"
#include "s_cproc.h"
#include "s_cproc_parse.h"

typedef struct  TAG_STRU_C_TOKEN_PP_KEYWORD_INFO
{
    ENUM_C_TOKEN token;
    _S8 * p_keyword;
}STRU_C_TOKEN_PP_KEYWORD_INFO;

STRU_C_TOKEN_PP_KEYWORD_INFO g_c_token_pp_keyword_info[] =
{
    {C_TOKEN_PP_CONTROL_DEFINE,                                "define"},
    {C_TOKEN_PPD_DEFINED,                                      "defined"},
    {C_TOKEN_PPD_ELIF,                                         "elif"},
    {C_TOKEN_PPD_ENDIF,                                        "endif"},
    {C_TOKEN_PP_CONTROL_ERROR,                                 "error"},
    {C_TOKEN_PPD_IFDEF,                                        "ifdef"},
    {C_TOKEN_PPD_IFNDEF,                                       "ifndef"},
    {C_TOKEN_PP_CONTROL_INCLUDE,                               "include"},
    {C_TOKEN_PP_CONTROL_LINE,                                  "line"},
    {C_TOKEN_PP_CONTROL_PRAGMA,                                "pragma"},
    {C_TOKEN_PP_CONTROL_UNDEF,                                 "undef"},
};
/* not include C99 and C11 */
typedef enum TAG_ENUM_DCL_KEYWORD
{
    DCL_KEYWORD_TYPE = 0,
    DCL_KEYWORD_TYPE_QUALIFIER,
    DCL_KEYWORD_CONTROL,
    DCL_KEYWORD_INVALID,
}ENUM_DCL_KEYWORD;

typedef struct TAG_STRU_DCL_KEYWORD
{
    const _S8 *keyword;
    ENUM_DCL_KEYWORD type;
    _S64 count;
}STRU_DCL_KEYWORD;

PRIVATE STRU_DCL_KEYWORD g_ckeyword[] = {
    {"char", DCL_KEYWORD_TYPE, 0}, 
    {"double", DCL_KEYWORD_TYPE, 0},
    {"enum", DCL_KEYWORD_TYPE, 0}, 
    {"float", DCL_KEYWORD_TYPE, 0}, 
    {"int", DCL_KEYWORD_TYPE, 0}, 
    {"long", DCL_KEYWORD_TYPE, 0}, 
    {"short", DCL_KEYWORD_TYPE, 0}, 
    {"signed", DCL_KEYWORD_TYPE, 0}, 
    {"struct", DCL_KEYWORD_TYPE, 0}, 
    {"union", DCL_KEYWORD_TYPE, 0}, 
    {"unsigned", DCL_KEYWORD_TYPE, 0}, 
    {"void", DCL_KEYWORD_TYPE, 0}, 
    {"auto", DCL_KEYWORD_TYPE_QUALIFIER, 0}, 
    {"const", DCL_KEYWORD_TYPE_QUALIFIER, 0}, 
    {"extern", DCL_KEYWORD_TYPE_QUALIFIER, 0}, 
    {"register", DCL_KEYWORD_TYPE_QUALIFIER, 0}, 
    {"static", DCL_KEYWORD_TYPE_QUALIFIER, 0}, 
    {"volatile", DCL_KEYWORD_TYPE_QUALIFIER, 0},
    {"break", DCL_KEYWORD_CONTROL, 0}, 
    {"case", DCL_KEYWORD_CONTROL, 0}, 
    {"continue", DCL_KEYWORD_CONTROL, 0}, 
    {"default", DCL_KEYWORD_CONTROL, 0}, 
    {"do", DCL_KEYWORD_CONTROL, 0}, 
    {"else", DCL_KEYWORD_CONTROL, 0}, 
    {"for", DCL_KEYWORD_CONTROL, 0}, 
    {"goto", DCL_KEYWORD_CONTROL, 0}, 
    {"if", DCL_KEYWORD_CONTROL, 0}, 
    {"return", DCL_KEYWORD_CONTROL, 0}, 
    {"sizeof", DCL_KEYWORD_CONTROL, 0}, 
    {"switch", DCL_KEYWORD_CONTROL, 0}, 
    {"typedef", DCL_KEYWORD_CONTROL, 0}, 
    {"while", DCL_KEYWORD_CONTROL, 0},
}; 




typedef struct TAG_STRU_C_TOKEN_ENUM_INFO
{
    ENUM_C_TOKEN token;
    const _S8 *info;
}STRU_C_TOKEN_ENUM_INFO;

PRIVATE STRU_C_TOKEN_ENUM_INFO g_c_token_enum_info[C_TOKEN_MAX] = 
{
    {C_TOKEN_NORMAL,                                           LIGHT_GREEN"normal"NONE},
    {C_TOKEN_BLANK,                                            LIGHT_GREEN"continued spaces"NONE},
    {C_TOKEN_CONTINUED_NEWLINE,                                LIGHT_GREEN"continued newline"NONE},
    {C_TOKEN_NEWLINE,                                          LIGHT_GREEN"newline"NONE},
    //{C_TOKEN_NEWLINE_LINUX,                                    LIGHT_GREEN"newline linux"NONE},
    //{C_TOKEN_NEWLINE_WINDOWS,                                  LIGHT_GREEN"newline windows"NONE},
    //{C_TOKEN_NEWLINE_MAC,                                      LIGHT_GREEN"newline mac"NONE},
    {C_TOKEN_LINE_COMMENT,                                     LIGHT_GREEN"one line comment"NONE},
    {C_TOKEN_PAIR_COMMENT,                                     LIGHT_GREEN"pairing comment"NONE},

    /* #, ##,define,defined,elif,endif,error,ifdef,ifndef,include,line,pragma,undef */
    {C_TOKEN_PPD,                                              LIGHT_GREEN"preprocessing directive: #"NONE},
    {C_TOKEN_PPD_STRINGIFICATION,                              LIGHT_GREEN"preprocessing directive: stringification #"NONE},
	{C_TOKEN_PPD_CONCATENATE,                                  LIGHT_GREEN"preprocessing directive: concatenate ##"NONE},
	{C_TOKEN_PPD_DEFINED,                                      LIGHT_GREEN"preprocessing directive: defined"NONE},
    
    {C_TOKEN_PPD_IF,                                           LIGHT_GREEN"preprocessing directive: if"NONE},
    {C_TOKEN_PPD_IFDEF,                                        LIGHT_GREEN"preprocessing directive: ifdef"NONE},
    {C_TOKEN_PPD_IFNDEF,                                       LIGHT_GREEN"preprocessing directive: ifndef"NONE},
        
    {C_TOKEN_PPD_ELIF,                                         LIGHT_GREEN"preprocessing directive: elif"NONE},
    {C_TOKEN_PPD_ELSE,                                         LIGHT_GREEN"preprocessing directive: else"NONE},
    {C_TOKEN_PPD_ENDIF,                                        LIGHT_GREEN"preprocessing directive: endif"NONE},
    {C_TOKEN_PPD_UNKNOWN,                                      LIGHT_GREEN"preprocessing directive: unknown"NONE},

    {C_TOKEN_PP_CONTROL_INCLUDE,                               LIGHT_GREEN"preprocessing control: include"NONE},
    {C_TOKEN_PP_CONTROL_DEFINE,                                LIGHT_GREEN"preprocessing control: define"NONE},
	{C_TOKEN_PP_CONTROL_UNDEF,                                 LIGHT_GREEN"preprocessing control: undef"NONE},
    {C_TOKEN_PP_CONTROL_LINE,                                  LIGHT_GREEN"preprocessing control: line"NONE},
    {C_TOKEN_PP_CONTROL_ERROR,                                 LIGHT_GREEN"preprocessing control: error"NONE},
    {C_TOKEN_PP_CONTROL_PRAGMA,                                LIGHT_GREEN"preprocessing control: pragma"NONE},
	
    {C_TOKEN_PP_HEADER_H_NAME,                                 LIGHT_GREEN"Preprocessing: header <name>"NONE},
    {C_TOKEN_PP_HEADER_Q_NAME,                                 LIGHT_GREEN"Preprocessing: header \"name\""NONE},
    {C_TOKEN_PP_HEADER_H_START,                                LIGHT_GREEN"Preprocessing: header <-START"NONE},
    {C_TOKEN_PP_HEADER_H_FINISH,                               LIGHT_GREEN"Preprocessing: header >-FINISH"NONE},
    {C_TOKEN_PP_HEADER_Q_START,                                LIGHT_GREEN"Preprocessing: header \"-START"NONE},
    {C_TOKEN_PP_HEADER_Q_FINISH,                               LIGHT_GREEN"Preprocessing: header \"-FINISH"NONE},
    {C_TOKEN_PP_MACRO,                                         LIGHT_GREEN"Preprocessing: macro"NONE},
    {C_TOKEN_PP_IDENTIFIER,                                    LIGHT_GREEN"Preprocessing: identifier"NONE},
    {C_TOKEN_PP_IDENTIFIER_VA,                                 LIGHT_GREEN"Preprocessing: identifier __VA_ARGS__"NONE},
    {C_TOKEN_PP_PARAMETER_START,                               LIGHT_GREEN"Preprocessing: parameter: start"NONE},
    {C_TOKEN_PP_PARAMETER_FINISH,                              LIGHT_GREEN"Preprocessing: parameter: finish"NONE},
    {C_TOKEN_PP_PARAMETER_ID,                                  LIGHT_GREEN"Preprocessing: parameter: identifier"NONE},
    {C_TOKEN_PP_PARAMETER_ID_VA,                               LIGHT_GREEN"Preprocessing: parameter: identifier..."NONE},
    {C_TOKEN_PP_PARAMETER_VA,                                  LIGHT_GREEN"Preprocessing: parameter: ..."NONE},
    {C_TOKEN_PP_INTEGER_CONSTANT,                              LIGHT_GREEN"Preprocessing: integer constant"NONE},
	{C_TOKEN_PP_FLOATING_CONSTANT,                             LIGHT_GREEN"Preprocessing: floating constant"NONE},
	{C_TOKEN_PP_CHARACTER_CONSTANT,                            LIGHT_GREEN"Preprocessing: character constant"NONE},
	{C_TOKEN_PP_STRING_LITERAL,                                LIGHT_GREEN"Preprocessing: string literal"NONE},
	{C_TOKEN_PP_PUNCTUATOR,                                    LIGHT_GREEN"Preprocessing: punctuator"NONE},
	{C_TOKEN_PP_PLACEHOLDER,                                   LIGHT_GREEN"Preprocessing: placeholder"NONE},
    {C_TOKEN_PP_IGNORE,                                        LIGHT_GREEN"Preprocessing: ignore"NONE},

    {C_TOKEN_PARENTHESIS_LEFT,                                 LIGHT_GREEN"("NONE},
    {C_TOKEN_PARENTHESIS_RIGHT,                                LIGHT_GREEN")"NONE},
    {C_TOKEN_BRACKET_LEFT,                                     LIGHT_GREEN"["NONE},
    {C_TOKEN_BRACKET_RIGHT,                                    LIGHT_GREEN"]"NONE},
    {C_TOKEN_BRACE_LEFT,                                       LIGHT_GREEN"{"NONE},
    {C_TOKEN_BRACE_RIGHT,                                      LIGHT_GREEN"}"NONE},

    {C_TOKEN_COMMA,                                            LIGHT_GREEN","NONE},
    {C_TOKEN_SEMICOLON,                                        LIGHT_GREEN";"NONE},
    {C_TOKEN_VA_ARGS,                                          LIGHT_GREEN"..."NONE},

    {C_TOKEN_OPERATOR_MEMBER,                                  LIGHT_GREEN"."NONE},
    {C_TOKEN_OPERATOR_MEMBER_POINTER,                          LIGHT_GREEN"->"NONE},

    {C_TOKEN_OPERATOR_ARITHMETIC_ADD,                          LIGHT_GREEN"+"NONE},
    {C_TOKEN_OPERATOR_ARITHMETIC_INCREASE,                     LIGHT_GREEN"++"NONE},
    {C_TOKEN_OPERATOR_ARITHMETIC_ADD_ASSIGN,                   LIGHT_GREEN"+="NONE},

    {C_TOKEN_OPERATOR_ARITHMETIC_SUBTRACT,                     LIGHT_GREEN"-"NONE},
    {C_TOKEN_OPERATOR_ARITHMETIC_DECREASE,                     LIGHT_GREEN"--"NONE},
    {C_TOKEN_OPERATOR_ARITHMETIC_SUBTRACT_ASSIGN,              LIGHT_GREEN"-="NONE},

    {C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY,                     LIGHT_GREEN"*"NONE},
    {C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY_ASSIGN,              LIGHT_GREEN"*="NONE},

	{C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE,                       LIGHT_GREEN"/"NONE},
	{C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE_ASSIGN,                LIGHT_GREEN"/="NONE},

	{C_TOKEN_OPERATOR_ARITHMETIC_MOD,                          LIGHT_GREEN"%"NONE},
	{C_TOKEN_OPERATOR_ARITHMETIC_MOD_ASSIGN,                   LIGHT_GREEN"%="NONE},

    {C_TOKEN_OPERATOR_LOGICAL_GREATER,                         LIGHT_GREEN">"NONE},
    {C_TOKEN_OPERATOR_LOGICAL_GREATER_EQUAL,                   LIGHT_GREEN">="NONE},
    {C_TOKEN_OPERATOR_BIT_RIGHT_SHIFT,                         LIGHT_GREEN">>"NONE},
    {C_TOKEN_OPERATOR_BIT_RIGHT_SHIFT_ASSIGN,                  LIGHT_GREEN">>="NONE},

    {C_TOKEN_OPERATOR_LOGICAL_LESS,                            LIGHT_GREEN"<"NONE},
    {C_TOKEN_OPERATOR_LOGICAL_LESS_EQUAL,                      LIGHT_GREEN"<="NONE},
    {C_TOKEN_OPERATOR_BIT_LEFT_SHIFT,                          LIGHT_GREEN"<<"NONE},
    {C_TOKEN_OPERATOR_BIT_LEFT_SHIFT_ASSIGN,                   LIGHT_GREEN"<<="NONE},

    {C_TOKEN_OPERATOR_ASSIGN,                                  LIGHT_GREEN"="NONE},
    {C_TOKEN_OPERATOR_LOGICAL_EQUAL,                           LIGHT_GREEN"=="NONE},
    {C_TOKEN_OPERATOR_LOGICAL_NOT_EQUAL,                       LIGHT_GREEN"!="NONE},

    {C_TOKEN_OPERATOR_BIT_AND,                                 LIGHT_GREEN"&"NONE},
    {C_TOKEN_OPERATOR_BIT_AND_ASSIGN,                          LIGHT_GREEN"&="NONE},
    {C_TOKEN_OPERATOR_LOGICAL_AND,                             LIGHT_GREEN"&&"NONE},
    {C_TOKEN_OPERATOR_BIT_OR,                                  LIGHT_GREEN"|"NONE},
    {C_TOKEN_OPERATOR_BIT_OR_ASSIGN,                           LIGHT_GREEN"|="NONE},
    {C_TOKEN_OPERATOR_LOGICAL_OR,                              LIGHT_GREEN"||"NONE},
    {C_TOKEN_OPERATOR_BIT_XOR,                                 LIGHT_GREEN"^"NONE},
    {C_TOKEN_OPERATOR_BIT_XOR_ASSIGN,                          LIGHT_GREEN"^="NONE},
    {C_TOKEN_OPERATOR_BIT_NOT,                                 LIGHT_GREEN"~"NONE},
    {C_TOKEN_OPERATOR_LOGICAL_NOT,                             LIGHT_GREEN"!"NONE},
        
	{C_TOKEN_OPERATOR_LOGICAL_QUESTION,                        LIGHT_GREEN"?"NONE},
    {C_TOKEN_OPERATOR_LOGICAL_COLON,                           LIGHT_GREEN":"NONE},

    {C_TOKEN_KEYWORD_TYPE,                                     LIGHT_GREEN"C keyword: type"NONE},
    {C_TOKEN_KEYWORD_TYPE_QUALIFIER,                           LIGHT_GREEN"C keyword: qualifier"NONE},
    {C_TOKEN_KEYWORD_CONTROL,                                  LIGHT_GREEN"C keyword: control"NONE},
    {C_TOKEN_IDENTIFIER,                                       LIGHT_GREEN"C identifier"NONE},
    {C_TOKEN_CONSTANT_INTEGER,                                 LIGHT_GREEN"C constant: integer"NONE},
    {C_TOKEN_CONSTANT_FLOAT,                                   LIGHT_GREEN"C constant: float"NONE},
 
    {C_TOKEN_STRING,                                           LIGHT_GREEN"C constant: string"NONE},
    {C_TOKEN_CHAR,                                             LIGHT_GREEN"C constant: character"NONE},
    {C_TOKEN_PUNCTUATOR,                                       LIGHT_GREEN"C punctuator"NONE},
    {C_TOKEN_OTHER,                                            LIGHT_GREEN"C other"NONE},
    {C_TOKEN_END,                                              LIGHT_GREEN"END"NONE},
    {C_TOKEN_UNKNOWN,                                          LIGHT_RED"UNKNOWN"NONE},
    {C_TOKEN_LIST_HEAD,                                        LIGHT_RED"LIST HEAD"NONE},
    {C_TOKEN_INVALID,                                          LIGHT_RED"INVALID"NONE},
};


const _S8 * s_ctoken_get_str(ENUM_C_TOKEN token)
{
    R_ASSERT(token >= C_TOKEN_NORMAL && token < C_TOKEN_MAX, "token out of range");
    R_ASSERT_LOG(g_c_token_enum_info[token].token == token, "token value in private data error", "token = %d", token);
    R_ASSERT(g_c_token_enum_info[token].info != NULL, "token string in private data error");

    return g_c_token_enum_info[token].info;
}
#if 0
PRIVATE _S8 separators[] = {'#', '(', ')', '[', ']', '{', '}', '*',  ':', ';', ',', '\\', ' ', '\t','\n'};

PRIVATE ENUM_BOOLEAN is_separator(_S32 c)
{
    size_t size = SIZE_OF_ARRAY(separators);
    _S32 i = 0;
    for(; i < size; i++)
    {
        if(c == separators[i])
        {
            return BOOLEAN_TRUE;
        }
    }

    return BOOLEAN_FALSE;
}

PRIVATE ENUM_BOOLEAN is_number(const _S8 *string)
{
    R_ASSERT(string != NULL, BOOLEAN_FALSE);
    _S32 number = -1;
    return(RETURN_SUCCESS == s_strtos32(string, &number) && number >= 0)?BOOLEAN_TRUE:BOOLEAN_FALSE;
}
#endif
PRIVATE ENUM_DCL_KEYWORD get_ckeyword_type_and_count(const _S8 *string)
{
    R_ASSERT(string != NULL, DCL_KEYWORD_INVALID);
    
    _S32 i = 0;
    _S32 size = SIZE_OF_ARRAY(g_ckeyword);

    //need to be optimized, using the binsearch or bin tree
    while(i < size)
    {
        if(strcmp(g_ckeyword[i].keyword, string) == 0)
        {
            break;
        }
        
        i++;
    };

    S_R_FALSE(i < size, DCL_KEYWORD_INVALID);

    g_ckeyword[i].count++;
    return g_ckeyword[i].type;
}

ENUM_BOOLEAN s_cproc_has_dot_before_number_or_alphabet(const _S8*p_text)
{
    S_R_ASSERT(p_text != NULL, BOOLEAN_FALSE);
    ENUM_BOOLEAN has_dot_before_number_or_alphabet = BOOLEAN_FALSE;
    while(*p_text != '\0' && !(*p_text == '_' || IS_ALPHABET(*p_text)))
    {
        if(*p_text == '.')
        {
            has_dot_before_number_or_alphabet = BOOLEAN_TRUE;
            break;
        }
        p_text++;
    }

    return has_dot_before_number_or_alphabet;
}

ENUM_RETURN s_cproc_parse_pp_directive(const _S8 *p_text_buffer, ENUM_C_TOKEN *token_type)
{
    R_ASSERT(p_text_buffer != NULL, RETURN_FAILURE);
    R_ASSERT(token_type != NULL, RETURN_FAILURE);

    *token_type = C_TOKEN_PP_IDENTIFIER;

    S_R_ASSERT(*p_text_buffer != '\0', RETURN_FAILURE);

    size_t size = SIZE_OF_ARRAY(g_c_token_pp_keyword_info);
    
    for(size_t i = 0; i < size; i++)
    {
        if(strcmp(g_c_token_pp_keyword_info[i].p_keyword, p_text_buffer) == 0)
        {
            *token_type = g_c_token_pp_keyword_info[i].token;
            return RETURN_SUCCESS;
        }
    }

    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_parse_pp_is_va_suffix(const _S8* p_text_buffer, ENUM_BOOLEAN *is_va_suffix)
{
    S_R_ASSERT(p_text_buffer != NULL, RETURN_FAILURE);
    S_R_ASSERT(is_va_suffix != NULL, RETURN_FAILURE);
    *is_va_suffix = BOOLEAN_FALSE;
    
    //get parameter type: identifier, identifier va, va
    S_R_ASSERT(*p_text_buffer != '\0', RETURN_FAILURE);

    if(strcmp(p_text_buffer, "...") == 0)
    {
        *is_va_suffix = BOOLEAN_TRUE;
    }

    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_parse_keyword(const _S8* p_text_buffer, ENUM_C_TOKEN *token_type)
{
    S_R_ASSERT(p_text_buffer != NULL, RETURN_FAILURE);
    S_R_ASSERT(token_type != NULL, RETURN_FAILURE);
    *token_type = C_TOKEN_IDENTIFIER;
    ENUM_DCL_KEYWORD dcl_keyword = get_ckeyword_type_and_count(p_text_buffer);

    switch(dcl_keyword)
    {
        case DCL_KEYWORD_TYPE:
            *token_type = C_TOKEN_KEYWORD_TYPE;
            break;
        case DCL_KEYWORD_CONTROL:
            *token_type = C_TOKEN_KEYWORD_CONTROL;
            break;
        case DCL_KEYWORD_TYPE_QUALIFIER:
            *token_type = C_TOKEN_KEYWORD_TYPE_QUALIFIER;
            break;
        default:
            break;
    }

    return RETURN_SUCCESS;
}

_VOID s_cproc_draw_keyword_count(_VOID)
{
    STRU_CHART_DATA *chart_data = NULL;
    _S32 i = 0;
    _S32 size = SIZE_OF_ARRAY(g_ckeyword);

    chart_data = (STRU_CHART_DATA * )malloc(sizeof(STRU_CHART_DATA) * size);
    S_V_ASSERT(chart_data != NULL);

     while(i < size)
    {
        strncpy(chart_data[i].info, g_ckeyword[i].keyword, CHART_DATA_INFO_STR_SIZE - 1);
        chart_data[i].info[CHART_DATA_INFO_STR_SIZE - 1] = '\0';
        chart_data[i].val = g_ckeyword[i].count;
        
        i++;
    };

    FILE *fpw = fopen("keyword_count.txt", "w");
    S_V_ASSERT_DO(fpw != NULL, S_FREE(chart_data));
    
    ENUM_RETURN ret_val = draw_histogram(fpw, chart_data, size);
    fclose(fpw);
    S_FREE(chart_data);
    
    S_V_ASSERT(ret_val == RETURN_SUCCESS);

}
