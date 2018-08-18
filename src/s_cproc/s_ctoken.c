#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "s_log.h"
#include "s_mem.h"
#include "s_text.h"
#include "s_stack.h"
#include "s_stm.h"
#include "s_cproc.h"
# include "s_ctoken.h"


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
}STRU_DCL_KEYWORD;

PRIVATE STRU_DCL_KEYWORD g_ckeyword[] = {
    {"char", DCL_KEYWORD_TYPE}, 
    {"double", DCL_KEYWORD_TYPE},
    {"enum", DCL_KEYWORD_TYPE}, 
    {"float", DCL_KEYWORD_TYPE}, 
    {"int", DCL_KEYWORD_TYPE}, 
    {"long", DCL_KEYWORD_TYPE}, 
    {"short", DCL_KEYWORD_TYPE}, 
    {"signed", DCL_KEYWORD_TYPE}, 
    {"struct", DCL_KEYWORD_TYPE}, 
    {"union", DCL_KEYWORD_TYPE}, 
    {"unsigned", DCL_KEYWORD_TYPE}, 
    {"void", DCL_KEYWORD_TYPE}, 
    {"auto", DCL_KEYWORD_TYPE_QUALIFIER}, 
    {"const", DCL_KEYWORD_TYPE_QUALIFIER}, 
    {"extern", DCL_KEYWORD_TYPE_QUALIFIER}, 
    {"register", DCL_KEYWORD_TYPE_QUALIFIER}, 
    {"static", DCL_KEYWORD_TYPE_QUALIFIER}, 
    {"volatile", DCL_KEYWORD_TYPE_QUALIFIER},
    {"break", DCL_KEYWORD_CONTROL}, 
    {"case", DCL_KEYWORD_CONTROL}, 
    {"continue", DCL_KEYWORD_CONTROL}, 
    {"default", DCL_KEYWORD_CONTROL}, 
    {"do", DCL_KEYWORD_CONTROL}, 
    {"else", DCL_KEYWORD_CONTROL}, 
    {"for", DCL_KEYWORD_CONTROL}, 
    {"goto", DCL_KEYWORD_CONTROL}, 
    {"if", DCL_KEYWORD_CONTROL}, 
    {"return", DCL_KEYWORD_CONTROL}, 
    {"sizeof", DCL_KEYWORD_CONTROL}, 
    {"switch", DCL_KEYWORD_CONTROL}, 
    {"typedef", DCL_KEYWORD_CONTROL}, 
    {"while", DCL_KEYWORD_CONTROL},
}; 


PRIVATE _S8 separators[] = {'#', '(', ')', '[', ']', '{', '}', '*',  ':', ';', ',', '\\', ' ', '\t','\n'};


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
    {C_TOKEN_NEWLINE_LINUX,                                    LIGHT_GREEN"newline linux"NONE},
    {C_TOKEN_NEWLINE_WINDOWS,                                  LIGHT_GREEN"newline windows"NONE},
    {C_TOKEN_NEWLINE_MAC,                                      LIGHT_GREEN"newline mac"NONE},
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
	
    {C_TOKEN_PP_HEADER_NAME,                                   LIGHT_GREEN"Preprocessing: header name"NONE},
    {C_TOKEN_PP_HEADER_H_START,                                LIGHT_GREEN"Preprocessing: header <"NONE},
    {C_TOKEN_PP_HEADER_H_FINISH,                               LIGHT_GREEN"Preprocessing: header >"NONE},
    {C_TOKEN_PP_IDENTIFIER,                                    LIGHT_GREEN"Preprocessing: identifier"NONE},
	{C_TOKEN_PP_NUMBER,                                        LIGHT_GREEN"Preprocessing: number"NONE},
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
    {C_TOKEN_OPERATOR_LOGICAL_LESS_QEUAL,                      LIGHT_GREEN"<="NONE},
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

    {C_TOKEN_END,                                              LIGHT_GREEN"END"NONE},
    {C_TOKEN_UNKNOWN,                                          LIGHT_RED"UNKNOWN"NONE},
    {C_TOKEN_INVALID,                                          LIGHT_RED"INVALID"NONE},
};

const _S8 * get_dcl_token_str(ENUM_C_TOKEN token)
{
    R_ASSERT(token >= C_TOKEN_NORMAL && token < C_TOKEN_MAX, "token out of range");
    R_ASSERT_LOG(g_c_token_enum_info[token].token == token, "token value in private data error", "token = %d", token);
    R_ASSERT(g_c_token_enum_info[token].info != NULL, "token string in private data error");

    return g_c_token_enum_info[token].info;
}

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

PRIVATE ENUM_BOOLEAN is_number(_S8 *string)
{
    R_ASSERT(string != NULL, BOOLEAN_FALSE);
    _S32 number = -1;
    return(RETURN_SUCCESS == s_strtos32(string, &number) && number >= 0)?BOOLEAN_TRUE:BOOLEAN_FALSE;
}

PRIVATE ENUM_DCL_KEYWORD get_ckeyword_type(const _S8 *string)
{
    R_ASSERT(string != NULL, DCL_KEYWORD_INVALID);
    
    _S32 i = 0;
    _S32 size = SIZE_OF_ARRAY(g_ckeyword);
    while(i < size)
    {
        if(strcmp(g_ckeyword[i].keyword, string) == 0)
        {
            break;
        }
        
        i++;
    };

    S_R_FALSE(i < size, DCL_KEYWORD_INVALID);

    return g_ckeyword[i].type;
}

ENUM_BOOLEAN is_keyword_type(_S8 *string)
{
    return get_ckeyword_type(string) == DCL_KEYWORD_TYPE;
}

ENUM_BOOLEAN is_keyword_type_qualifier(_S8 *string)
{
    return get_ckeyword_type(string) == DCL_KEYWORD_TYPE_QUALIFIER;
}

ENUM_BOOLEAN is_keyword_control(_S8 *string)
{
    return get_ckeyword_type(string) == DCL_KEYWORD_CONTROL;
}

ENUM_BOOLEAN is_keyword(_S8 *string)
{
    return get_ckeyword_type(string) != DCL_KEYWORD_INVALID;
}

ENUM_BOOLEAN is_identifier(_S8 *string)
{
    R_ASSERT(string != NULL, BOOLEAN_FALSE);

    S_R_FALSE(string[0] == '_' || IS_ALPHABET(string[0]), BOOLEAN_FALSE);

    string++;
    while(*string != '\0')
    {
        if(!IS_DEC(*string) && !IS_ALPHABET(*string) && *string != '_')
        {
            return BOOLEAN_FALSE;
        }
        string++;
    }

    return BOOLEAN_TRUE;
}

PRIVATE ENUM_C_TOKEN parse_symbol(_S8 c)
{
    ENUM_C_TOKEN token_temp = C_TOKEN_INVALID;
    
    switch(c)
    {
        
    }

    return token_temp;
}

PRIVATE ENUM_C_TOKEN parse_word(_S8 *string)
{
    R_ASSERT(string != NULL, C_TOKEN_INVALID);
    ENUM_C_TOKEN token_temp = C_TOKEN_INVALID;

    if(strlen(string) == 0)
    {
        return C_TOKEN_END;
    }
    
    if(is_number(string))
    {
        token_temp = C_TOKEN_CONSTANT_INTEGER;
    }
    else if(is_keyword_type(string))
    {
        token_temp = C_TOKEN_KEYWORD_TYPE;
    }
    else if(is_keyword_type_qualifier(string))
    {
        token_temp = C_TOKEN_KEYWORD_TYPE_QUALIFIER;
    }
    else if(is_keyword_control(string))
    {
        token_temp = C_TOKEN_KEYWORD_CONTROL;
    }
    else if(is_identifier(string))
    {
        token_temp = C_TOKEN_IDENTIFIER;
    }
    else
    {
        token_temp = C_TOKEN_INVALID;
    }

    return token_temp;
}

/* get token from cpp lines */


ENUM_RETURN s_cget_token(
	const _S8 *p_text_buffer, 
	size_t *len, 
	ENUM_C_TOKEN *token)
{
    R_ASSERT(p_text_buffer != NULL, RETURN_FAILURE);
    R_ASSERT(len != NULL, RETURN_FAILURE);
    R_ASSERT(token != NULL, RETURN_FAILURE);

    PRIVATE _S8 token_buffer[MAX_TOKEN_LEN];
    _S8 *p_token_buffer = token_buffer;
    size_t buffer_size = MAX_TOKEN_LEN;
    _S8 c = '\0';
    *token = C_TOKEN_INVALID;
    *len = 0;
    ENUM_C_TOKEN token_temp = C_TOKEN_INVALID;

    if((c = *p_text_buffer) == '\0')
    {
        token_temp = C_TOKEN_END;
        (*len) = 0;
        return RETURN_SUCCESS;
    }
    
    /* skip white space */
    while((c = *p_text_buffer) != '\0' && isspace(c))
    {
        token_temp = C_TOKEN_BLANK;
        OUTPUT_C(c, p_token_buffer, buffer_size);
        (*len)++;
		p_text_buffer++;
    }

    if(*len != 0)
    {
        OUTPUT_END(p_token_buffer, buffer_size);
        *token = token_temp;
        return RETURN_SUCCESS;
    }

    if((c = *p_text_buffer) == '_' || IS_ALPHABET(c) || c == '$')
    {
        token_temp = C_TOKEN_IDENTIFIER;
        (*len)++;
        p_text_buffer++;
        while((c = *p_text_buffer) != '\0' && (IS_ALPHABET(c) || IS_DEC(c) || c == '$' || c == '_'))
        {
            (*len)++;
            p_text_buffer++;
        };
        
        return RETURN_SUCCESS;
    }
    
    switch(c)
    {
        case '\0':
        {
            token_temp = C_TOKEN_END;
            *len = 0;
            break;
        }
        case '(':
        {
            token_temp = C_TOKEN_PARENTHESIS_LEFT;
            (*len)++;
            p_text_buffer++;
            break;
        }
        case ')':
        {
            token_temp = C_TOKEN_PARENTHESIS_RIGHT;
            (*len)++;
            p_text_buffer++;
            break;
        }
        case '[':
        {
            token_temp = C_TOKEN_BRACKET_LEFT;
            (*len)++;
            p_text_buffer++;
            break;
        }
        case ']':
        {
            token_temp = C_TOKEN_BRACKET_RIGHT;
            (*len)++;
            p_text_buffer++;
            break;
        }
        case '{':
        {
            token_temp = C_TOKEN_BRACE_LEFT;
            (*len)++;
            p_text_buffer++;
            break;
        }
        case '}':
        {
            token_temp = C_TOKEN_BRACE_RIGHT;
            (*len)++;
            p_text_buffer++;
            break;
        }
		case '.':
        {
            token_temp = C_TOKEN_OPERATOR_MEMBER;
            (*len)++;
            p_text_buffer++;
            if(*p_text_buffer == '.' && *(p_text_buffer + 1) == '.')
            {
                token_temp = C_TOKEN_VA_ARGS;
                (*len)+= 2;
                p_text_buffer+=2;
            }
            break;
        }
        case ',':
        {
            token_temp = C_TOKEN_COMMA;
            (*len)++;
            p_text_buffer++;
            break;
        }
        case ';':
        {
            token_temp = C_TOKEN_SEMICOLON;
            (*len)++;
            p_text_buffer++;
            break;
        }
		case '+':
		{
			token_temp = C_TOKEN_OPERATOR_ARITHMETIC_ADD;
            (*len)++;
            p_text_buffer++;
            if(*p_text_buffer == '=')
            {
                token_temp = C_TOKEN_OPERATOR_ARITHMETIC_ADD_ASSIGN;
                (*len)++;
                p_text_buffer++;
            }
            else if(*p_text_buffer == '+')
            {
                token_temp = C_TOKEN_OPERATOR_ARITHMETIC_INCREASE;
                (*len)++;
                p_text_buffer++;
            }
			break;
		}
		case '-':
		{
			token_temp = C_TOKEN_OPERATOR_ARITHMETIC_SUBTRACT;
            (*len)++;
            p_text_buffer++;
            if(*p_text_buffer == '=')
            {
                token_temp = C_TOKEN_OPERATOR_ARITHMETIC_SUBTRACT_ASSIGN;
                (*len)++;
                p_text_buffer++;
            }
            else if(*p_text_buffer == '-')
            {
                token_temp = C_TOKEN_OPERATOR_ARITHMETIC_DECREASE;
                (*len)++;
                p_text_buffer++;
            }
            else if(*p_text_buffer == '>')
            {
                token_temp = C_TOKEN_OPERATOR_ARITHMETIC_DECREASE;
                (*len)++;
                p_text_buffer++;
            }
            break;
		}
        case '*':
        {
            token_temp = C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY;
            (*len)++;
            p_text_buffer++;
            if(*p_text_buffer == '=')
            {
                token_temp = C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY_ASSIGN;
                (*len)++;
                p_text_buffer++;
            }
            break;
        }
		case '/':
		{
			token_temp = C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE;
            (*len)++;
            p_text_buffer++;
            if(*p_text_buffer == '=')
            {
                token_temp = C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE_ASSIGN;
                (*len)++;
                p_text_buffer++;
            }
			break;
		}
        case '%':
		{
			token_temp = C_TOKEN_OPERATOR_ARITHMETIC_MOD;
            (*len)++;
            p_text_buffer++;
            if(*p_text_buffer == '=')
            {
                token_temp = C_TOKEN_OPERATOR_ARITHMETIC_MOD_ASSIGN;
                (*len)++;
                p_text_buffer++;
            }
			break;
		}
		case '>':
		{
			token_temp = C_TOKEN_OPERATOR_LOGICAL_GREATER;
            (*len)++;
            p_text_buffer++;
            if(*p_text_buffer == '=')
            {
                token_temp = C_TOKEN_OPERATOR_LOGICAL_GREATER_EQUAL;
                (*len)++;
                p_text_buffer++;
            }
            else if(*p_text_buffer == '>')
            {
                token_temp = C_TOKEN_OPERATOR_BIT_RIGHT_SHIFT;
                (*len)++;
                p_text_buffer++;
                if(*p_text_buffer == '=')
                {
                    token_temp = C_TOKEN_OPERATOR_BIT_RIGHT_SHIFT_ASSIGN;
                    (*len)++;
                    p_text_buffer++;
                }
            }
			break;
		}
		case '<':
		{
			token_temp = C_TOKEN_OPERATOR_LOGICAL_LESS;
            (*len)++;
            p_text_buffer++;
            if(*p_text_buffer == '=')
            {
                token_temp = C_TOKEN_OPERATOR_LOGICAL_LESS_QEUAL;
                (*len)++;
                p_text_buffer++;
            }
            else if(*p_text_buffer == '<')
            {
                token_temp = C_TOKEN_OPERATOR_BIT_LEFT_SHIFT;
                (*len)++;
                p_text_buffer++;
                if(*p_text_buffer == '=')
                {
                    token_temp = C_TOKEN_OPERATOR_BIT_LEFT_SHIFT_ASSIGN;
                    (*len)++;
                    p_text_buffer++;
                }
            }
			break;
		}
		case '=':
		{
			token_temp = C_TOKEN_OPERATOR_ASSIGN;
            (*len)++;
            p_text_buffer++;
            if(*p_text_buffer == '=')
            {
                token_temp = C_TOKEN_OPERATOR_LOGICAL_LESS_QEUAL;
                (*len)++;
                p_text_buffer++;
            }
			break;
		}
		case '&':
		{
			token_temp = C_TOKEN_OPERATOR_BIT_AND;
            (*len)++;
            p_text_buffer++;
            if(*p_text_buffer == '=')
            {
                token_temp = C_TOKEN_OPERATOR_BIT_AND_ASSIGN;
                (*len)++;
                p_text_buffer++;
            }
            else if(*p_text_buffer == '&')
            {
                token_temp = C_TOKEN_OPERATOR_LOGICAL_AND;
                (*len)++;
                p_text_buffer++;
            }
			break;
		}
		case '|':
		{
			token_temp = C_TOKEN_OPERATOR_BIT_OR;
            (*len)++;
            p_text_buffer++;
            if(*p_text_buffer == '=')
            {
                token_temp = C_TOKEN_OPERATOR_BIT_AND_ASSIGN;
                (*len)++;
                p_text_buffer++;
            }
            else if(*p_text_buffer == '&')
            {
                token_temp = C_TOKEN_OPERATOR_LOGICAL_AND;
                (*len)++;
                p_text_buffer++;
            }
			break;
		}
        case '^':
		{
			token_temp = C_TOKEN_OPERATOR_BIT_XOR;
            (*len)++;
            p_text_buffer++;
            if(*p_text_buffer == '=')
            {
                token_temp = C_TOKEN_OPERATOR_BIT_XOR_ASSIGN;
                (*len)++;
                p_text_buffer++;
            }
			break;
		}
		case '~':
		{
			token_temp = C_TOKEN_OPERATOR_BIT_NOT;
            (*len)++;
            p_text_buffer++;
			break;
		}
		case '!':
		{
			token_temp = C_TOKEN_OPERATOR_LOGICAL_NOT;
            (*len)++;
            p_text_buffer++;
            if(*p_text_buffer == '=')
            {
                token_temp = C_TOKEN_OPERATOR_LOGICAL_NOT_EQUAL;
                (*len)++;
                p_text_buffer++;
            }
			break;
		}
        default:
        {
            token_temp = C_TOKEN_INVALID;
            break;
        }
    }
    
    if(c != '\0')
    {
		//printf("----------->%c\n", *c);
        token_temp = parse_symbol(c);
        OUTPUT_C(c, p_token_buffer, buffer_size);
		(*len)++;
		p_text_buffer++;
    }

    if(token_temp != C_TOKEN_INVALID)
    {
        OUTPUT_END(p_token_buffer, buffer_size);
        *token = token_temp;
        return RETURN_SUCCESS;
    }

    while((c = *p_text_buffer) != '\0' && !is_separator(c))
    {
		//printf("----------->%c\n", *c);
        OUTPUT_C(c, p_token_buffer, buffer_size);
		(*len)++;
		p_text_buffer++;
    }
    
    OUTPUT_END(p_token_buffer, buffer_size);

    token_temp = parse_word(token_buffer);

    *token = token_temp;
    return RETURN_SUCCESS;
}

ENUM_RETURN get_token_type(const _S8 *p_string, ENUM_C_TOKEN *p_type)
{
	return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_token_make_new(
    const _S8 *token_string, 
    ENUM_C_TOKEN token_type, 
    size_t text_offset,
    size_t line_index,
    size_t line_column,
    STRU_C_TOKEN_NODE **pp_new_token_node)
{
    R_ASSERT(token_string != NULL, RETURN_FAILURE);
    R_ASSERT(pp_new_token_node != NULL, RETURN_FAILURE);
    *pp_new_token_node = NULL;

    STRU_C_TOKEN_NODE *p_new_token_temp = (STRU_C_TOKEN_NODE*)malloc(sizeof(STRU_C_TOKEN_NODE));
    R_ASSERT(p_new_token_temp != NULL, RETURN_FAILURE);

    p_new_token_temp->info.p_string = (_S8*)malloc(strlen(token_string) + 1);
    R_ASSERT_DO(p_new_token_temp->info.p_string != NULL, RETURN_FAILURE, FREE(p_new_token_temp));

    strcpy(p_new_token_temp->info.p_string, token_string);
    p_new_token_temp->info.token_type = token_type;
    p_new_token_temp->info.qualifier.is_auto = BOOLEAN_FALSE;
    p_new_token_temp->info.qualifier.is_const = BOOLEAN_FALSE;
    p_new_token_temp->info.qualifier.is_extern = BOOLEAN_FALSE;
    p_new_token_temp->info.qualifier.is_register = BOOLEAN_FALSE;
    p_new_token_temp->info.qualifier.is_static = BOOLEAN_FALSE;
    p_new_token_temp->info.qualifier.is_volatile = BOOLEAN_FALSE;
    p_new_token_temp->info.line_position.offset = text_offset;
    p_new_token_temp->info.line_position.line = line_index;
    p_new_token_temp->info.line_position.column = line_column;
    p_new_token_temp->next = NULL;
    p_new_token_temp->previous = NULL;
    *pp_new_token_node = p_new_token_temp;

    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_token_copy(const STRU_C_TOKEN_NODE *p_token_node_source, STRU_C_TOKEN_NODE **pp_token_node_dest)
{
    R_ASSERT(p_token_node_source != NULL, RETURN_FAILURE);
    R_ASSERT(pp_token_node_dest != NULL, RETURN_FAILURE);

    *pp_token_node_dest = NULL;

    STRU_C_TOKEN_NODE *p_token_node_temp = NULL;

    p_token_node_temp = (STRU_C_TOKEN_NODE*)malloc(sizeof(STRU_C_TOKEN_NODE));
    S_R_ASSERT(p_token_node_temp != NULL, RETURN_FAILURE);

    p_token_node_temp->previous = NULL;
    p_token_node_temp->next = NULL;
    p_token_node_temp->info = p_token_node_source->info;
    p_token_node_temp->info.p_string = NULL;
    p_token_node_temp->info.p_string = (_S8*)malloc(strlen(p_token_node_source->info.p_string) + 1);
    S_R_ASSERT_DO(p_token_node_temp->info.p_string != NULL, RETURN_FAILURE, FREE(p_token_node_temp));

    strcpy(p_token_node_temp->info.p_string, p_token_node_source->info.p_string);
    *pp_token_node_dest = p_token_node_temp;

    return RETURN_SUCCESS;
}
ENUM_RETURN s_cproc_token_add_node_to_list(
    STRU_C_TOKEN_NODE *p_new_token_node,
    STRU_C_TOKEN_NODE **pp_token_list_head, 
    STRU_C_TOKEN_NODE **pp_token_list_tail)
{
    R_ASSERT(p_new_token_node != NULL, RETURN_FAILURE);
    R_ASSERT(pp_token_list_head != NULL, RETURN_FAILURE);
    R_ASSERT(pp_token_list_tail != NULL, RETURN_FAILURE);

    if(*pp_token_list_head == NULL)
    {
        R_ASSERT(*pp_token_list_tail == NULL, RETURN_FAILURE);
        
        *pp_token_list_head = p_new_token_node;
        *pp_token_list_tail = p_new_token_node;
    }
    else
    {
        R_ASSERT(*pp_token_list_tail != NULL, RETURN_FAILURE);

        p_new_token_node->previous = *pp_token_list_tail;
        (*pp_token_list_tail)->next = p_new_token_node;
        *pp_token_list_tail = p_new_token_node;
    }

    return RETURN_SUCCESS;
}

ENUM_RETURN add_token_to_list(
    const _S8 *token_string, 
    ENUM_C_TOKEN token_type, 
    size_t text_offset,
    size_t line_index,
    size_t line_column,
    STRU_C_TOKEN_NODE **pp_token_list_head, 
    STRU_C_TOKEN_NODE **pp_token_list_tail)
{
    R_ASSERT(token_string != NULL, RETURN_FAILURE);
    R_ASSERT(pp_token_list_head != NULL, RETURN_FAILURE);
    R_ASSERT(pp_token_list_tail != NULL, RETURN_FAILURE);
    
    STRU_C_TOKEN_NODE *p_new_token_node = NULL;
    ENUM_RETURN ret_val;
    ret_val = s_cproc_token_make_new(
        token_string,
        token_type, 
        text_offset, 
        line_index, 
        line_column, 
        &p_new_token_node);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    S_R_ASSERT(p_new_token_node != NULL, RETURN_FAILURE);
    
    ret_val = s_cproc_token_add_node_to_list(
        p_new_token_node,
        pp_token_list_head,
        pp_token_list_tail);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    return RETURN_SUCCESS;
};

ENUM_RETURN delete_token_from_list(
    STRU_C_TOKEN_NODE *p_token_to_be_deleted,
    STRU_C_TOKEN_NODE **pp_token_list_head, 
    STRU_C_TOKEN_NODE **pp_token_list_tail)
{
    R_ASSERT(p_token_to_be_deleted != NULL, RETURN_FAILURE);
    R_ASSERT(pp_token_list_head != NULL, RETURN_FAILURE);
    R_ASSERT(pp_token_list_tail != NULL, RETURN_FAILURE);

    R_ASSERT(*pp_token_list_head != NULL, RETURN_FAILURE);
    R_ASSERT(*pp_token_list_tail != NULL, RETURN_FAILURE);

    /* the token to be deleted is the list head and list tail */
    if(*pp_token_list_head == p_token_to_be_deleted 
        && *pp_token_list_tail == p_token_to_be_deleted)
    {
        *pp_token_list_head = NULL;
        *pp_token_list_tail = NULL;
    }
    /* the token to be deleted is the list head */
    else if(*pp_token_list_head == p_token_to_be_deleted)
    {
        *pp_token_list_head = (*pp_token_list_head)->next;
        (*pp_token_list_head)->previous = NULL;
    }
    /* the token to be deleted is the list tail */
    else if(*pp_token_list_tail == p_token_to_be_deleted)
    {
        *pp_token_list_tail = (*pp_token_list_tail)->previous;
        (*pp_token_list_tail)->next = NULL;
    }
    else
    {
        p_token_to_be_deleted->previous->next = p_token_to_be_deleted->next;
        p_token_to_be_deleted->next->previous = p_token_to_be_deleted->previous;
    }
    
    FREE(p_token_to_be_deleted->info.p_string);
    FREE(p_token_to_be_deleted);

    return RETURN_SUCCESS;
};

ENUM_RETURN release_token_list(
    STRU_C_TOKEN_NODE **pp_token_list_head, 
    STRU_C_TOKEN_NODE **pp_token_list_tail)
{
    R_ASSERT(pp_token_list_head != NULL, RETURN_FAILURE);
    R_ASSERT(pp_token_list_tail != NULL, RETURN_FAILURE);
    STRU_C_TOKEN_NODE *p_token_list_head = *pp_token_list_head;
    STRU_C_TOKEN_NODE *p_token_temp;
    
    while(p_token_list_head != NULL)
    {
        p_token_temp = p_token_list_head;
        FREE(p_token_temp->info.p_string);
        p_token_list_head = p_token_list_head->next;
        FREE(p_token_temp);
    }

    *pp_token_list_head = NULL;
    *pp_token_list_tail = NULL;
    
    return RETURN_SUCCESS;
}

_VOID print_token_list(
    STRU_C_TOKEN_NODE *p_token_list_head, 
    STRU_C_TOKEN_NODE *p_token_list_tail)
{
	S_V_ASSERT(p_token_list_head != NULL);
	S_V_ASSERT(p_token_list_tail != NULL);

	printf("token list:\n");
	while(p_token_list_head != NULL)
	{
		printf("%s ", p_token_list_head->info.p_string);
		if(p_token_list_head == p_token_list_tail)
		{
			break;
		}
		
		p_token_list_head = p_token_list_head->next;
	}

	printf("\n");

}

_VOID debug_print_token_list(
    STRU_C_TOKEN_NODE *p_token_list_head, 
    STRU_C_TOKEN_NODE *p_token_list_tail)
{
	S_V_ASSERT(p_token_list_head != NULL);
	S_V_ASSERT(p_token_list_tail != NULL);

	printf("token list:\n");
	while(p_token_list_head != NULL)
	{
		printf("\033[7m%s"NONE, p_token_list_head->info.p_string);
		size_t len = strlen(p_token_list_head->info.p_string);
		_S32 print_space_num = (len > 20)?0:(20 - len);
		for(_S32 i = 0; i < print_space_num; i++)
		{
			printf(" ");
		}

		printf(" %s\n", get_dcl_token_str(p_token_list_head->info.token_type));
		
		if(p_token_list_head == p_token_list_tail)
		{
			break;
		}
		
		p_token_list_head = p_token_list_head->next;
	}

	printf("\n");

}

