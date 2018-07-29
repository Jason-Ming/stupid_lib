#include <stdio.h>
#include <string.h>
#include "s_log.h"
#include "s_mem.h"
#include "s_text.h"
#include "s_cproc.h"

/* not include C99 and C11 */
PRIVATE _S8 *keyword_type[] = {
    "char", "double", "enum", "float", "int", "long", "short", "signed", 
    "struct", "union", "unsigned", "void", 
    "ENUM_RETURN", "FILE", "_S8", "size_t", "ENUM_DCL_TOKEN", "_S32" };

PRIVATE _S8 *keyword_type_qualifier[] = {
    "auto", "const", "extern", "register", 
    "static", "volatile" };

PRIVATE _S8 *keyword_control[] = {
    "break", "case", "continue", "default", "do", "else", "for" "goto", "if", "return", "sizeof", "switch", "typedef", "while"};

PRIVATE _S8 separators[] = {'(', ')', '[', ']', '{', '}', '*', ' ', '\t', ';', ',', '\n'};

PRIVATE _S8 * g_dcl_token_str[] = {
    "(", ")", "*", "[", "]", "{", "}", ",", ";", "type", "qualifier", "identifier", "number", "control", "end", "invalid token"};

PRIVATE _S8 * g_dcl_type_str[ ] = {
    "array", "pointer", "function", "struct", "enum", "union", "invalid type"};

_S8 * get_dcl_token_str(ENUM_DCL_TOKEN token)
{
    R_ASSERT(token >= DCL_TOKEN_PARENTHESIS_LEFT && token < DCL_TOKEN_MAX, "token out of range");
    R_ASSERT(DCL_TOKEN_MAX == SIZE_OF_ARRAY(g_dcl_token_str), "g_dcl_token_str missing elements");

    return g_dcl_token_str[token];
}

_S8 * get_dcl_type_str(ENUM_DCL_TYPE type)
{
    R_ASSERT(type >= DCL_TYPE_ARRAY && type < DCL_TYPE_MAX, "type out of range");
    R_ASSERT(DCL_TYPE_MAX == SIZE_OF_ARRAY(g_dcl_type_str), "g_dcl_type_str missing elements");

    return g_dcl_type_str[type];
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

ENUM_BOOLEAN is_keyword_type(_S8 *string)
{
    R_ASSERT(string != NULL, BOOLEAN_FALSE);
    _S32 i = 0;

    //DEBUG_PRINT("%zd\n", SIZE_OF_ARRAY(keyword_type));
    
    while(i < SIZE_OF_ARRAY(keyword_type))
    {
        //DEBUG_PRINT("compare: %zd %s, %zd %s\n", strlen(keyword_type[i]), keyword_type[i], strlen(string), string);
        R_FALSE_RET(strcmp(keyword_type[i], string) != 0, BOOLEAN_TRUE);
        i++;
    };

    //DEBUG_PRINT("return!\n");
    return BOOLEAN_FALSE;
}

ENUM_BOOLEAN is_keyword_type_qualifier(_S8 *string)
{
    R_ASSERT(string != NULL, BOOLEAN_FALSE);
    _S32 i = 0;

    while(i < SIZE_OF_ARRAY(keyword_type_qualifier))
    {
        R_FALSE_RET(strcmp(keyword_type_qualifier[i], string) != 0, BOOLEAN_TRUE);
        i++;
    };

    return BOOLEAN_FALSE;
}

ENUM_BOOLEAN is_keyword_control(_S8 *string)
{
    R_ASSERT(string != NULL, BOOLEAN_FALSE);
    _S32 i = 0;
    //DEBUG_PRINT("%zd\n", SIZE_OF_ARRAY(keyword_control));
    while(i < SIZE_OF_ARRAY(keyword_control))
    {
//        DEBUG_PRINT("compare: %zd %s, %zd %s\n", strlen(keyword_control[i]), keyword_control[i], strlen(string), string);
        R_FALSE_RET(strcmp(keyword_control[i], string) != 0, BOOLEAN_TRUE);
        i++;
    };

   // DEBUG_PRINT("return!\n");
    return BOOLEAN_FALSE;
}

ENUM_BOOLEAN is_keyword(_S8 *string)
{
    R_ASSERT(string != NULL, BOOLEAN_FALSE);
    
    return (is_keyword_control(string) || is_keyword_type(string) || is_keyword_type_qualifier(string));
}

ENUM_BOOLEAN is_identifier(_S8 *string)
{
    R_ASSERT(string != NULL, BOOLEAN_FALSE);

    R_FALSE_RET(string[0] == '_' || IS_ALPHABET(string[0]), BOOLEAN_FALSE);

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

PRIVATE ENUM_DCL_TOKEN parse_symbol(_S8 c)
{
    ENUM_DCL_TOKEN token_temp = DCL_TOKEN_INVALID;
    
    switch(c)
    {
        case '(':
        {
            token_temp = DCL_TOKEN_PARENTHESIS_LEFT;
            break;
        }
        case ')':
        {
            token_temp = DCL_TOKEN_PARENTHESIS_RIGHT;
            break;
        }
        case '*':
        {
            token_temp = DCL_TOKEN_STAR;
            break;
        }
        case '[':
        {
            token_temp = DCL_TOKEN_BRACKET_LEFT;
            break;
        }
        case ']':
        {
            token_temp = DCL_TOKEN_BRACKET_RIGHT;
            break;
        }
        case '{':
        {
            token_temp = DCL_TOKEN_BRACE_LEFT;
            break;
        }
        case '}':
        {
            token_temp = DCL_TOKEN_BRACE_RIGHT;
            break;
        }
        case ',':
        {
            token_temp = DCL_TOKEN_COMMA;
            break;
        }
        case ';':
        {
            token_temp = DCL_TOKEN_SEMICOLON;
            break;
        }
        default:
        {
            token_temp = DCL_TOKEN_INVALID;
            break;
        }
    }

    return token_temp;
}

PRIVATE ENUM_DCL_TOKEN parse_word(_S8 *string)
{
    R_ASSERT(string != NULL, DCL_TOKEN_INVALID);
    ENUM_DCL_TOKEN token_temp = DCL_TOKEN_INVALID;

    if(strlen(string) == 0)
    {
        return DCL_TOKEN_END;
    }
    
    if(is_number(string))
    {
        token_temp = DCL_TOKEN_NUMBER;
    }
    else if(is_keyword_type(string))
    {
        token_temp = DCL_TOKEN_TYPE;
    }
    else if(is_keyword_type_qualifier(string))
    {
        token_temp = DCL_TOKEN_TYPE_QUALIFIER;
    }
    else if(is_keyword_control(string))
    {
        token_temp = DCL_TOKEN_CONTROL;
    }
    else if(is_identifier(string))
    {
        token_temp = DCL_TOKEN_IDENTIFIER;
    }
    else
    {
        token_temp = DCL_TOKEN_INVALID;
    }

    return token_temp;
}

ENUM_RETURN s_cget_token(const _S8 * statement, _S8 token_buffer[], size_t buffer_size, size_t *len, ENUM_DCL_TOKEN *token, const _S8 **next_token)
{
    R_ASSERT(statement != NULL, RETURN_FAILURE);
    R_ASSERT(token_buffer != NULL, RETURN_FAILURE);
    R_ASSERT(buffer_size > 0, RETURN_FAILURE);
    R_ASSERT(len != NULL, RETURN_FAILURE);
    R_ASSERT(token != NULL, RETURN_FAILURE);
    R_ASSERT(next_token != NULL, RETURN_FAILURE);
    
    _S8 *temp = token_buffer;
    *token = DCL_TOKEN_INVALID;
    *len = 0;
    ENUM_DCL_TOKEN token_temp = DCL_TOKEN_INVALID;

    /* skip white space */
    while(isspace(*statement)) statement++;

    if(*statement != '\0')
    {
        token_temp = parse_symbol(*statement);
        OUTPUT_STR(*statement++, token_buffer, buffer_size);
    }

    if(token_temp != DCL_TOKEN_INVALID)
    {
        OUTPUT_END(token_buffer, buffer_size);
        *token = token_temp;
        *len = 1;
        *next_token = statement;
        return RETURN_SUCCESS;
    }

    while(*statement != '\0' && !is_separator(*statement))
    {
        OUTPUT_STR(*statement++, token_buffer, buffer_size);
    }
    
    OUTPUT_END(token_buffer, buffer_size);

    token_temp = parse_word(temp);

    *token = token_temp;
    *len = strlen(temp);
    *next_token = statement;
    return RETURN_SUCCESS;
}

ENUM_RETURN s_cget_statement(FILE * pfr, _S8 statement_buffer[], size_t buffer_size, size_t *len)
{
    R_ASSERT(pfr != NULL, RETURN_FAILURE);
    R_ASSERT(statement_buffer != NULL, RETURN_FAILURE);
    R_ASSERT(buffer_size > 0, RETURN_FAILURE);
    R_ASSERT(len != NULL, RETURN_FAILURE);
    _S32 brace_num = 0;
    ENUM_BOOLEAN whether_c_can_be_ignored = BOOLEAN_TRUE;
    
    _S8 *temp = statement_buffer;
    *len = 0;
    _S32 c;
    
    /* skip white space */
    while((c = fgetc(pfr)) != EOF)
    {
        if(isspace(c) && whether_c_can_be_ignored)
        {
            continue;
        }

        if(c == ';')
        {
            if(whether_c_can_be_ignored)
            {
                continue;
            }
            else if(brace_num == 0)
            {
                break;
            }
        }

        whether_c_can_be_ignored = BOOLEAN_FALSE;

        if(c == '{')
        {
            brace_num++;
        }

        if(c == '}')
        {
            brace_num--;
        }

        OUTPUT_STR(c, statement_buffer, buffer_size);

        
    }

    OUTPUT_END(statement_buffer, buffer_size);

    *len = strlen(temp);
    return RETURN_SUCCESS;
}

ENUM_RETURN s_cparse_statement(_S8 *statement)
{
    return RETURN_SUCCESS;
}

