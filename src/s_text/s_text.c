#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <ctype.h>

#include "s_defines.h"
#include "s_log.h"
#include "s_text.h"
#include "s_mem.h"
#include "s_alg.h"
#include "s_stm.h"

#define MAX_CHAR 256

#define MAX_LINE_LEN			1000
PRIVATE ENUM_BOOLEAN word_separators[MAX_CHAR] = {BOOLEAN_FALSE};
PRIVATE ENUM_BOOLEAN whether_word_separators_have_been_set = BOOLEAN_FALSE;

void s_set_separators(_S8 *separators)
{
    whether_word_separators_have_been_set = BOOLEAN_TRUE;
    for(_S32 i = 0; i < MAX_CHAR; i++)
    {
        word_separators[i] = BOOLEAN_FALSE;
    }
    
    if(separators == NULL || strlen(separators) == 0)
    {
        word_separators['\n'] = BOOLEAN_TRUE;
        word_separators['\t'] = BOOLEAN_TRUE;
        word_separators[' '] = BOOLEAN_TRUE;
        word_separators['\v'] = BOOLEAN_TRUE;
        word_separators['\f'] = BOOLEAN_TRUE;

        return;
    }
    
    while(*separators != '\0')
    {
        word_separators[(_U8)(*separators++)] = BOOLEAN_TRUE;
    }
    
}

PRIVATE ENUM_BOOLEAN is_separator(_S8 c)
{
    return (whether_word_separators_have_been_set?
        word_separators[(_U8)c]:
        (' ' == c || '\n' == c || '\t' == c || '\v' == c || '\f' == c));
}

ENUM_RETURN s_count_word_f(FILE *pfr, size_t *word_num)
{
    R_ASSERT(pfr != NULL, RETURN_FAILURE);
    R_ASSERT(word_num != NULL, RETURN_FAILURE);
    *word_num = 0;
    _S8 c;
    ENUM_BOOLEAN in_word = BOOLEAN_FALSE;

    while((c = fgetc(pfr)) != EOF)
    {
        if(is_separator(c))
        {
            in_word = BOOLEAN_FALSE;
            continue;
        }

        if(in_word == BOOLEAN_FALSE)
        {
            (*word_num)++;
            in_word = BOOLEAN_TRUE;
        }
    }

    return RETURN_SUCCESS;
}

ENUM_RETURN s_count_word_s(const _S8 *source, size_t *word_num)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(word_num != NULL, RETURN_FAILURE);
    *word_num = 0;
    _S8 c;
    ENUM_BOOLEAN in_word = BOOLEAN_FALSE;
    
    while((c = *source++) != '\0')
    {
        if(is_separator(c))
        {
            in_word = BOOLEAN_FALSE;
            continue;
        }

        if(in_word == BOOLEAN_FALSE)
        {
            (*word_num)++;
            in_word = BOOLEAN_TRUE;
        }
    }

    return RETURN_SUCCESS;
}

ENUM_RETURN s_get_word_f(FILE *pfr, _S8 *word_buf, size_t buf_size, size_t *word_len)
{
    R_ASSERT(pfr != NULL, RETURN_FAILURE);
    R_ASSERT(word_buf != NULL, RETURN_FAILURE);
    R_ASSERT(buf_size > 0, RETURN_FAILURE);
    R_ASSERT(word_len != NULL, RETURN_FAILURE);

    size_t len = buf_size;
    _S8 c;
    ENUM_BOOLEAN in_word = BOOLEAN_FALSE;
    
    /* skip white space */
    while((c = fgetc(pfr)) != EOF)
    {
        if(is_separator(c))
        {
            if(in_word == BOOLEAN_FALSE)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            in_word = BOOLEAN_TRUE;
            OUTPUT_STR(c, word_buf, buf_size);
        }
    } 

    OUTPUT_END(word_buf, buf_size);
    *word_len = len - buf_size - 1;
    
    while((c = fgetc(pfr)) != EOF)
    {
        if(is_separator(c))
        {
            continue;
        }
        else
        {
            R_ASSERT(0 == fseek(pfr, -1, SEEK_CUR), RETURN_FAILURE);
            break;
        }
    }
    
    return RETURN_SUCCESS;
}

ENUM_RETURN s_get_word_s(const _S8 **source, _S8 *word_buf, size_t buf_size, size_t *word_len)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(*source != NULL, RETURN_FAILURE);
    R_ASSERT(word_buf != NULL, RETURN_FAILURE);
    R_ASSERT(buf_size > 0, RETURN_FAILURE);
    R_ASSERT(word_len != NULL, RETURN_FAILURE);

    size_t len = 0;

    /* skip white space */
    while(is_separator(**source)) (*source)++;
    
    while(**source != '\0')
    {
        if(is_separator(**source))
        {
            break;
        }

        (*source)++;
        len++;
    };

    strncpy(word_buf, *source - len, MIN(len, buf_size - 1));
    word_buf[len] = '\0';
    *word_len = len;
    
    while(is_separator(**source)) (*source)++;

    return RETURN_SUCCESS;
}

ENUM_RETURN s_getlines_f(FILE *pfr, _S8 *line_ptr[], size_t line_ptr_num, size_t *line_num)
{
    R_ASSERT(pfr != NULL, RETURN_FAILURE);
    R_ASSERT(line_ptr != NULL, RETURN_FAILURE);
    R_ASSERT(line_ptr_num > 0, RETURN_FAILURE);
    R_ASSERT(line_num != NULL, RETURN_FAILURE);
    
    _S8 line_buffer[MAX_LINE_LEN];
    size_t line_len = 0;
    *line_num = 0;
    
    while(s_getline_f(pfr, line_buffer, MAX_LINE_LEN, &line_len) == RETURN_SUCCESS && line_len > 0 && *line_num < line_ptr_num)
    {
        line_ptr[*line_num] = (_S8*)malloc(line_len + 1);
        R_ASSERT(line_ptr[*line_num] != NULL, RETURN_FAILURE);

        strcpy(line_ptr[(*line_num)++], line_buffer);
    };

    /* still have lines to be read, the number of line_ptr is not enough */
    R_ASSERT(feof(pfr), RETURN_FAILURE);
    
    return RETURN_SUCCESS;
}

ENUM_RETURN s_getlines_s(const _S8 **source, _S8 *line_ptr[], size_t line_ptr_num, size_t *line_num)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(*source != NULL, RETURN_FAILURE);
    R_ASSERT(line_ptr != NULL, RETURN_FAILURE);
    R_ASSERT(line_ptr_num > 0, RETURN_FAILURE);
    R_ASSERT(line_num != NULL, RETURN_FAILURE);
    
    _S8 line_buffer[MAX_LINE_LEN];
    size_t line_len = 0;
    *line_num = 0;
    
    while(s_getline_s(source, line_buffer, MAX_LINE_LEN, &line_len) == RETURN_SUCCESS && line_len > 0 && *line_num < line_ptr_num)
    {
        line_ptr[*line_num] = (_S8*)malloc(line_len + 1);
        R_ASSERT(line_ptr[*line_num] != NULL, RETURN_FAILURE);

        strcpy(line_ptr[(*line_num)++], line_buffer);
    };

    /* still have lines to be read, the number of line_ptr is not enough */
    R_ASSERT(*source == '\0', RETURN_FAILURE);
    
    return RETURN_SUCCESS;
}

ENUM_RETURN s_getline_f(FILE *fp, _S8 buffer[], size_t buffer_size, size_t *length)
{
    R_ASSERT(fp != NULL, RETURN_FAILURE);
    R_ASSERT(buffer != NULL, RETURN_FAILURE);
    R_ASSERT(length != NULL, RETURN_FAILURE);

    _S8 c;
    size_t len_temp = buffer_size;

    while((c = fgetc(fp)) != EOF && c != '\n')
    {
        OUTPUT_STR(c, buffer, buffer_size);
    }

    if(c == '\n')
    {
        OUTPUT_STR(c, buffer, buffer_size);
    }

    OUTPUT_END(buffer, buffer_size);

    *length = len_temp - buffer_size - 1;
    
    return RETURN_SUCCESS;
}


ENUM_RETURN s_getline_s(const _S8 **source, _S8 buffer[], size_t buffer_size, size_t *length)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(*source != NULL, RETURN_FAILURE);
    R_ASSERT(buffer != NULL, RETURN_FAILURE);
    R_ASSERT(buffer_size > 0, RETURN_FAILURE);
    R_ASSERT(length != NULL, RETURN_FAILURE);

    _S8 c;
    size_t len_temp = buffer_size;

    while((c = **source) != '\0' && c != '\n')
    {
        OUTPUT_STR(c, buffer, buffer_size);
        (*source)++;
    }

    if(c == '\n')
    {
        OUTPUT_STR(c, buffer, buffer_size);
        (*source)++;
    }

    OUTPUT_END(buffer, buffer_size);

    *length = len_temp - buffer_size - 1;
    
    return RETURN_SUCCESS;
}

ENUM_RETURN s_reverse(_S8 *pstr_buf)
{
    R_ASSERT(pstr_buf != NULL, RETURN_FAILURE);
    _S32 start = 0;
    _S32 end = strlen(pstr_buf) - 1;
    _S8 temp;
    while(start < end)
    {
        temp = pstr_buf[start];
        pstr_buf[start] = pstr_buf[end];
        pstr_buf[end] = temp;
        start++;
        end--;
    }

    return RETURN_SUCCESS;
}

ENUM_RETURN s_fold(_S8 *source, _S8 *dest, _S32 size, _S32 fold_num)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(dest != NULL, RETURN_FAILURE);
    R_ASSERT(size > 0, RETURN_FAILURE);
    R_ASSERT(fold_num >= 5, RETURN_FAILURE);
    
    _S32 len = strlen(source);
    //if(len >= fold_num, RETURN_SUCCESS);

    memset(dest, '\0', size);

    DEBUG_PRINT("source: %s\nsource len: %d\ndest: %s\ndest len: %d",
        source, len, dest, size);
    
    _S32 pos = 0;
    _S32 pos_blank = -1;
    _S32 fold_len = 0;
    _S32 start_pos = pos;
    
    while(pos < len)
    {
        if(fold_len >= fold_num)
        {
            if(pos_blank != -1)
            {
                source[pos_blank] = '\0';
                
                R_ASSERT_LOG(
                    size - strlen(dest) >= pos - start_pos + 1, 
                    RETURN_FAILURE,
                    "size = %d, strlen(dest) = %zu, "
                    "pos = %d, start_pos = %d",
                    size, strlen(dest), pos, start_pos);

                DEBUG_PRINT("size = %d, strlen(dest) = %zu, "
                    "pos = %d, start_pos = %d",
                    size, strlen(dest), pos, start_pos);
                
                strcat(dest, &source[start_pos]);

                R_ASSERT_LOG(
                    size - strlen(dest) >= 2, 
                    RETURN_FAILURE,
                    "size = %d, strlen(dest) = %zu\n",
                    size, strlen(dest));

                strcat(dest, "\n");
                
                pos = pos_blank + 1;
                fold_len = 0;
                pos_blank = -1;
                start_pos = pos;
                continue;
            }
            else
            {
                
            }
        }
        
        if(source[pos] == ' ' || source[pos] == '\n')
        {
            pos_blank = pos;
        }

        pos++;
        fold_len++;
    }

    R_ASSERT_LOG(
        size - strlen(dest) >= pos - start_pos + 1, 
        RETURN_FAILURE,
        "size = %d, strlen(dest) = %zu, "
        "pos = %d, start_pos = %d",
        size, strlen(dest), pos, start_pos);

    DEBUG_PRINT("size = %d, strlen(dest) = %zu, "
        "pos = %d, start_pos = %d",
        size, strlen(dest), pos, start_pos);
    strcat(dest, &source[start_pos]);

    return RETURN_SUCCESS;
}


/* word in this function has 3 type: '\n', continuing blanks, continuing non-blanks */
PRIVATE _S32 find_next_word_position(const _S8 *source)
{
    _S8 c = *source;
    _S32 len = 0;
    if(' ' == c || '\n' == c || '\t' == c || '\v' == c || '\f' == c)
    {
        source++;
        len++;
        
        while(*source != '\0')
        {
            c = *source;
            if(' ' == c || '\t' == c || '\v' == c || '\f' == c)
            {
                ++(source);
                len++;
                continue;
            }
            else
            {
                return len;
            }
        }
    }
    else
    {
        source++;
        len++;
        
        while(*source != '\0')
        {
            c = *source;
            if(' ' == c || '\n' == c || '\t' == c || '\v' == c || '\f' == c)
            {
                return len;
            }
            else
            {
                ++(source);
                len++;
                continue;
            }
        }
    }

    return len;
}

ENUM_RETURN s_fold_s(const _S8 *source, _S8 *dest, size_t size, size_t fold_len)

{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(dest != NULL, RETURN_FAILURE);
    R_ASSERT(size > 0, RETURN_FAILURE);
    R_ASSERT(fold_len >= 5, RETURN_FAILURE);

    _S32 next_word_len = 0;
    _S32 left_space = fold_len;
    ENUM_BOOLEAN newline = BOOLEAN_TRUE;
    
    while(*source != '\0')
    {
        /* find next word position: word means 
        1.first ch is space, the word length is the longest space ch;
        2.first ch is not space, the word length is the longest non-space ch; */
        const _S8 *source_temp = source;
        
        next_word_len = find_next_word_position(source);

        if(left_space >= next_word_len)
        {
            OUTPUT_STRN(dest, size, source_temp, next_word_len);
            source = source_temp + next_word_len;
            left_space -= next_word_len;
            newline = BOOLEAN_FALSE;
        }
        else
        {
            if(newline)
            {
                OUTPUT_STRN(dest, size, source_temp, left_space);
                OUTPUT_STR('\n', dest, size);

                source = source_temp + left_space;
                left_space = fold_len;
                newline = BOOLEAN_TRUE;
            }
            else
            {
                OUTPUT_STR('\n', dest, size);
                source = source_temp;
                left_space = fold_len;
                newline = BOOLEAN_TRUE;
            }
        }
    }

    OUTPUT_END(dest, size);

    return RETURN_SUCCESS;
}

ENUM_RETURN s_hstrtou64(const _S8 *str, _U64 *value)
{
    R_ASSERT(str != NULL, RETURN_FAILURE);
    R_ASSERT(value != NULL, RETURN_FAILURE);

    /* skip white space */
    while(isspace(*str)) str++;
    
    _U64 sum = 0;
    size_t len = strlen(str);
    R_ASSERT(len > 0, RETURN_FAILURE);

    if(len > 2)
    {
        if(str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
        {
            str = str + 2;
            len = len - 2;
        }
    }

    R_ASSERT_LOG(len <= sizeof(_U64)*2, RETURN_FAILURE, "len = %zd", len);
    
    for(size_t i = 0; i < len; i++)
    {
        size_t index =i;
        _S8 c = str[index];
        _S32 temp = 0;

        /* between 0 ~ 9, a ~ f, A ~ F*/
        if(c >= '0' && c <= '9')
        {
            temp = c - '0';
        }
        else if(c >= 'a' && c <= 'f')
        {
            temp = c - 'a' + 10;
        }
        else if(c >= 'A' && c <= 'F')
        {
            temp = c - 'A' + 10;
        }
        else
        {
            R_RET_LOG(RETURN_FAILURE, "invalid char: %c", c);
        }

        sum = sum * 16 + temp;
    }

    *value = sum;

    return RETURN_SUCCESS;
}

ENUM_RETURN s_hstrtos64(const _S8 *str, _S64 *value)
{
    R_ASSERT(str != NULL, RETURN_FAILURE);
    R_ASSERT(value != NULL, RETURN_FAILURE);

    _U64 temp;
    ENUM_RETURN retval = s_hstrtou64(str, &temp);
    R_ASSERT(retval == RETURN_SUCCESS, RETURN_FAILURE);

    *value = VALUE_S64_OF_ADDR(&temp);
     
    return RETURN_SUCCESS;
}

ENUM_RETURN s_strtos32(const _S8 *str, _S32 *value)
{
    R_ASSERT(str != NULL, RETURN_FAILURE);
    R_ASSERT(value != NULL, RETURN_FAILURE);

    /* skip white space */
    while(isspace(*str)) str++;

    _S32 sign = 1;
    _S32 temp = 0;
    size_t len = strlen(str);
    R_ASSERT(len > 0, RETURN_FAILURE);
    
    if(*str == '-')
    {
        sign = -1;
        str++;
        len--;
    }
    else if (*str == '+')
    {
        sign = 1;
        str++;
        len--;
    }

    R_ASSERT(len > 0, RETURN_FAILURE);

    while(*str != '\0')
    {
        R_ASSERT(isdigit(*str), RETURN_FAILURE);
        temp = 10 * temp + (*str - '0');
        str++;
    }

    temp *= sign;

    *value = temp;

    return RETURN_SUCCESS;
    
}

ENUM_RETURN s_strtosd(const _S8 *str, _SD *value)
{
    R_ASSERT(str != NULL, RETURN_FAILURE);
    R_ASSERT(value != NULL, RETURN_FAILURE);

    /* skip white space */
    while(isspace(*str)) str++;

    _S32 sign = 1, exponent_sign = 1, exponent = 0;
    _SD temp = 0.0, integer_part = 0.0, decimal_part = 0.0, power = 1.0;
    size_t len = strlen(str);
    ENUM_BOOLEAN has_dot = BOOLEAN_FALSE, 
        has_e = BOOLEAN_FALSE,
        has_integer_part = BOOLEAN_FALSE, 
        has_decimal_part = BOOLEAN_FALSE,
        has_exponent_part = BOOLEAN_FALSE,
        has_exponent_sign_part = BOOLEAN_FALSE; 
    
    R_ASSERT(len > 0, RETURN_FAILURE);
    
    if(*str == '-')
    {
        sign = -1;
        str++;
        len--;
    }
    else if (*str == '+')
    {
        sign = 1;
        str++;
        len--;
    }

    R_ASSERT(len > 0, RETURN_FAILURE);

    while(*str != '\0' && isdigit(*str))
    {
        integer_part = 10.0 * integer_part + (*str - '0');
        str++;
        has_integer_part = BOOLEAN_TRUE;
    }

    if(*str != '\0' && *str == '.')
    {
        str++;
        has_dot = BOOLEAN_TRUE;
    }

    while(*str != '\0' && isdigit(*str))
    {
        decimal_part = 10.0 * decimal_part + (*str - '0');
        power *= 10.0;
        str++;
        has_decimal_part = BOOLEAN_TRUE;
    }

    if(*str != '\0' && (*str == 'e' || *str == 'E'))
    {
        str++;
        has_e = BOOLEAN_TRUE;
    }

    if(*str != '\0' && *str == '-')
    {
        exponent_sign = -1;
        has_exponent_sign_part = BOOLEAN_TRUE;
        str++;
    }
    else if (*str != '\0' && *str == '+')
    {
        exponent_sign = 1;
        has_exponent_sign_part = BOOLEAN_TRUE;
        str++;
    }

    while(*str != '\0' && isdigit(*str))
    {
        exponent = 10 * exponent + (*str - '0');
        str++;
        has_exponent_part = BOOLEAN_TRUE;
    }
    
    if(*str != '\0')
    {
        return RETURN_FAILURE;
    }
        
    if(has_dot == BOOLEAN_TRUE 
        && has_decimal_part == BOOLEAN_FALSE 
        && has_integer_part == BOOLEAN_FALSE)
    {
        return RETURN_FAILURE;
    }

    if(has_e == BOOLEAN_TRUE 
    && has_exponent_part == BOOLEAN_FALSE)
    {
        return RETURN_FAILURE;
    }

    if(has_exponent_sign_part == BOOLEAN_TRUE && has_e == BOOLEAN_FALSE)
    {
        return RETURN_FAILURE;
    }
    
    temp = (integer_part + decimal_part/power)*sign;
    power = (exponent_sign == 1)?10.0:0.1;
    for(_S32 i = 0; i < exponent; i++)
    {
        temp = power*temp;
    }

    *value = temp;
    
    return RETURN_SUCCESS;
}

ENUM_RETURN s_s32tostr(_S32 value, _S8 *dest, size_t size)
{
    R_ASSERT(dest != NULL, RETURN_FAILURE);

    _S32 sign = 1;
    _S8 *temp = dest;

    /* record sign and make vlaue positive */
    if((value) < 0)
    {
        sign = -1;
    }

    do
    {
        OUTPUT_STR((value % 10)*sign + '0', dest, size);
    }while((value /= 10) != 0);

    if(sign < 0)
    {
        OUTPUT_STR('-', dest, size);
    }

    *dest = '\0';

    ENUM_RETURN ret_val = s_reverse(temp);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

ENUM_RETURN s_s32tostrbw(_S32 value, _S32 b, _S32 w, _S8 *dest, size_t size)
{
    R_ASSERT(dest != NULL, RETURN_FAILURE);
    R_ASSERT(b >= 2 && b <= 36, RETURN_FAILURE);
    PRIVATE _S8 digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    _S32 sign = 1;
    _S8 *temp = dest;

    /* record sign and make vlaue positive */
    if((value) < 0)
    {
        sign = -1;
    }

    _S32 index = 0;    
    _S32 len = 0;
    do
    {
        index = (value % b)*sign;
        OUTPUT_STR( digits[index], dest, size);
        len++;
    }while((value /= b) != 0);

    if(sign < 0)
    {
        OUTPUT_STR('-', dest, size);
        len++;
    }

    while(len < w)
    {
        OUTPUT_STR(' ', dest, size);
        len++;
    }
    
    *dest = '\0';

    ENUM_RETURN ret_val = s_reverse(temp);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

ENUM_RETURN s_s32tostrb(_S32 value, _S32 b, _S8 *dest, size_t size)
{
    return s_s32tostrbw(value, b, 0, dest, size);
}

ENUM_RETURN s_squeeze(_S8 *source, const _S8 *target)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(target != NULL, RETURN_FAILURE);

    R_FALSE_RET(strlen(target) > 0, RETURN_SUCCESS);
    
    _S8 s[MAX_CHAR] = {0};
    _S8 c;
    while((c = *target) != '\0')
    {
        s[(_U8)c] = 1;
        target++;
    }
    _S8 *p = source;
    while((c = *source) != '\0')
    {
        if(s[(_U8)c] == 0)
        {
            *p = *source;
            p++;
        }
        source++;
    }
    *p = '\0';

    return RETURN_SUCCESS;
}

ENUM_RETURN s_any(const _S8 *source, const _S8 *target, const _S8**pp_occur)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(target != NULL, RETURN_FAILURE);
    R_ASSERT(pp_occur != NULL, RETURN_FAILURE);
    *pp_occur = NULL;

    R_FALSE_RET(strlen(target) > 0, RETURN_SUCCESS);
    
    _S8 s[MAX_CHAR] = {0};
    _S8 c;
    while((c = *target) != '\0')
    {
        s[(_U8)c] = 1;
        target++;
    }
    
    while((c = *source) != '\0')
    {
        if(s[(_U8)c] == 1)
        {
            *pp_occur = source;
            break;
        }
        source++;
    }

    return RETURN_SUCCESS;
}

ENUM_RETURN s_escape(const _S8* source, _S8* dest, size_t size)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(dest != NULL, RETURN_FAILURE);
    R_ASSERT(size > 0, RETURN_FAILURE);
    
    PRIVATE _S8 escape[MAX_CHAR] = {0};
    escape['\\'] = '\\';
    escape['\n'] = 'n';
    escape['\t'] = 't';
    escape['\a'] = 'a';
    escape['\b'] = 'b';
    escape['\f'] = 'f';
    escape['\r'] = 'r';
    escape['\v'] = 'v';
    escape['\"'] = '"';
    escape['\''] = '\'';
    escape['\?'] = '?';

    while(*source != '\0' && size > 2)
    {
        _S8 c = *source;
        switch(c)
        {
            case '\\':
            case '\a':
            case '\b':
            case '\n':
            case '\t':
            case '\f':
            case '\r':
            case '\v':
            case '\"':
            case '\'':
            case '\?':
            {
                OUTPUT_STR('\\', dest, size);
                OUTPUT_STR(escape[(_U8)c], dest, size);
                break;
            }
            default:
            {
                OUTPUT_STR(c, dest, size);
                break;
            }
        }
        source++;
    }

    /* append \0 */
    OUTPUT_END(dest, size);

    return RETURN_SUCCESS;
}

ENUM_RETURN s_unescape(const _S8* source, _S8* dest, size_t size)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(dest != NULL, RETURN_FAILURE);
    R_ASSERT(size > 0, RETURN_FAILURE);
    
    PRIVATE _S8 unescape[MAX_CHAR] = {0};
    /* init all s_escape charactors */
    unescape['\\'] = '\\';
    unescape['b'] = '\b';
    unescape['n'] = '\n';
    unescape['t'] = '\t';
    unescape['a'] = '\a';
    unescape['b'] = '\b';
    unescape['f'] = '\f';
    unescape['r'] = '\r';
    unescape['v'] = '\v';
    unescape['"'] = '\"';
    unescape['\''] = '\'';
    unescape['?'] = '\?';

    ENUM_BOOLEAN in_escape = BOOLEAN_FALSE;
    
    while(*source != '\0')
    {
        _S8 c = *source;
        DEBUG_PRINT("c = %c, in_escape = %d", c, in_escape);
        
        if(in_escape == BOOLEAN_TRUE)
        {
            switch(c)
            {
                case '\\':
                case 'a':
                case 'b':
                case 'n':
                case 't':
                case 'f':
                case 'r':
                case 'v':
                case '\"':
                case '\'':
                case '?':
                {
                    OUTPUT_STR(unescape[(_U8)c], dest, size);
                    break;
                }
                default:
                {
                    OUTPUT_STR('\\', dest, size);
                    OUTPUT_STR(c, dest, size);
                    break;
                }
            }
            in_escape = BOOLEAN_FALSE;
        }
        else
        {
            if(c == '\\')
            {
                in_escape = BOOLEAN_TRUE;
               
            }
            else
            {
                OUTPUT_STR(c, dest, size);
            }
        }
        source++;
    }

    /* append \0 */
    OUTPUT_END(dest, size);

    return RETURN_SUCCESS; 
}

ENUM_RETURN s_trim(_S8 *source)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);

    _S32 n; 
    for (n = strlen(source)-1; n >= 0; n--)
    {
        if (source[n] != ' ' && source[n] != '\t' && source[n] != '\n') 
        {
            break; 
        }
    }
    
    source[n+1] = '\0'; 
    return RETURN_SUCCESS;
}

ENUM_RETURN s_trim_nl(_S8 *source)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    ENUM_BOOLEAN has_nl = BOOLEAN_FALSE;
    _S32 n; 
    for (n = strlen(source)-1; n >= 0; n--)
    {
        if(source[n] == '\n')
        {
            has_nl = BOOLEAN_TRUE;
        }
        
        if (source[n] != ' ' && source[n] != '\t' && source[n] != '\n') 
        {
            break; 
        }
    }
    if(has_nl && n >= 0)
    {
        source[++n] = '\n';
    }
    
    source[n+1] = '\0'; 
    return RETURN_SUCCESS;
}

ENUM_RETURN s_strindex(const _S8 *source, const _S8 *target, _S32 *index)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(target != NULL, RETURN_FAILURE);
    R_ASSERT(index != NULL, RETURN_FAILURE);

    *index = -1;
    size_t len = strlen(target);
    R_FALSE_RET(len > 0, RETURN_SUCCESS);

    /*  source*  len = 6 */
    /*  0123456  */
    const _S8 *source_end = source + strlen(source);
    const _S8 *source_start = source;
    
    while(source <= source_end - len)
    {
        if(memcmp(source, target, len) == 0)
        {
            *index = source - source_start;
            break;
        }
        source++;
    }

    return RETURN_SUCCESS;
}

ENUM_RETURN s_strrindex(const _S8 *source, const _S8 *target, _S32 *index)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(target != NULL, RETURN_FAILURE);
    R_ASSERT(index != NULL, RETURN_FAILURE);

    *index = -1;
    size_t len = strlen(target);
    R_FALSE_RET(len > 0, RETURN_SUCCESS);
    
    const _S8 *temp = source + strlen(source) - len;
    
    while(temp >= source)
    {
        if(memcmp(temp, target, len) == 0)
        {
            *index = temp - source;
            break;
        }
        temp--;
    }

    return RETURN_SUCCESS;
}

ENUM_RETURN s_strend(const _S8 *source, const _S8 *target, ENUM_BOOLEAN *whether_target_occur)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(target != NULL, RETURN_FAILURE);
    R_ASSERT(whether_target_occur != NULL, RETURN_FAILURE);

    size_t len_t = strlen(target);
    *whether_target_occur = BOOLEAN_FALSE;
    R_FALSE_RET(len_t > 0, RETURN_SUCCESS);

    size_t len_s = strlen(source);
    

    if(len_s < len_t)
    {
        return RETURN_SUCCESS;
    }

    *whether_target_occur = (strcmp(source + len_s - len_t, target) == 0?BOOLEAN_TRUE:BOOLEAN_FALSE);
    return RETURN_SUCCESS;
}


PRIVATE _S32 cal_space_number(_S32 offset, _S32 tab_stop)
{
    return tab_stop - offset % tab_stop;
}

ENUM_RETURN s_entab(const _S8 *source, _S8 *dest, size_t len, _S32 tab_stop)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(dest != NULL, RETURN_FAILURE);
    R_ASSERT(len > 0, RETURN_FAILURE);
    R_ASSERT(tab_stop > 0, RETURN_FAILURE);
    
    _S32 offset = 0;
    _S32 j = 0;
    while(*source != '\0')
    {
        if(*source != ' ')
        {
            OUTPUT_STR(*source, dest, len);
            if(*source == '\n' || *source == '\t')
            {
                offset = 0;
            }
            else
            {
                offset++;
            }
            source++;
        }
        else
        {
            j = cal_space_number(offset, tab_stop);
            /* are there enough spaces to replace */
            _S32 k = 0;
            for(; *(source+k) == ' '; k++)
            {
            }

            if(j <= k)
            {
                OUTPUT_STR('\t', dest, len);
                offset = 0;
                source += j;
            }
            else if(*(source + k) == '\t')
            {
                OUTPUT_STR('\t', dest, len);
                offset = 0;
                source += (k + 1);
            }
            else
            {
                for(_S32 l = 0; l < k; l++)
                {
                    OUTPUT_STR(*source, dest, len);
                    offset++;
                    source++;
                }
            }
        }
    };

    OUTPUT_END(dest, len);

    return RETURN_SUCCESS;
}

ENUM_RETURN s_detab(const _S8 *source, _S8 *dest, size_t len, _S32 tab_stop)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(dest != NULL, RETURN_FAILURE);
    R_ASSERT(len > 0, RETURN_FAILURE);
    R_ASSERT(tab_stop > 0, RETURN_FAILURE);

    _S32 offset = 0;
    _S32 j = 0;
    while(*source != '\0')
    {
        if(*source != '\t')
        {
            OUTPUT_STR(*source, dest, len);
            (*source == '\n')?(offset = 0):(offset++);
        }
        else
        {
            j = cal_space_number(offset, tab_stop);
            for(_S32 k = 0; k < j; k++)
            {
                OUTPUT_STR(' ', dest, len);
                
            }
            offset = 0;
        }

        source++;
    };
    OUTPUT_END(dest, len);

    return RETURN_SUCCESS;
}

ENUM_BOOLEAN s_range(_S8 begin, _S8 end)
{
    if(isdigit(begin))
    {
        if(isdigit(end) && begin <= end)
        {
            return BOOLEAN_TRUE;
        }
    }
    else if(isupper(begin))
    {
        if(isupper(end) && begin <= end)
        {
            return BOOLEAN_TRUE;
        }
    }
    else if(islower(begin))
    {
        if(islower(end) && begin <= end)
        {
            return BOOLEAN_TRUE;
        }
    }

    return BOOLEAN_FALSE;
}

PRIVATE _S32	table_case[MAX_CHAR] =
{
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
     31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
     58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
     85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
     80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
     136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157,
     158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
     180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201,
     202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
     224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245,
     246, 247, 248, 249, 250, 251, 252, 253, 254, 255
};

_S32 strcasecmp(const _S8*s1, const _S8*s2)
{
    for(; table_case[(_U8)*s1] == table_case[(_U8)*s2]; ++s1, ++s2)
    {
        if(*s1 == '\0')
        {
            return (0);
        }
    }

    return ((table_case[(_U8)*s1] <table_case[(_U8)*s2])? (-1): (+1));
}

_S32 strcasecmp_r(const _S8 * s1, const _S8 * s2)
{
    return strcasecmp(s2, s1);
}


PRIVATE _S32 table_dir[MAX_CHAR] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0
};

_S32 dircmp(const _S8 * s1, const _S8 * s2)
{
    for(; ; ++s1, ++s2)
    {
        if(!table_dir[(_U8)*s1])
        {
            ++s1;
            continue;
        }

        if(!table_dir[(_U8)*s2])
        {
            ++s2;
            continue;
        }
        
        if(*s1 != *s2)
        {
            break;
        }
        
        if(*s1 == '\0')
        {
            return (0);
        }
        
    }

    return ((*(_U8*)s1 <*(_U8*)s2)? (-1): (+1));
}

_S32 dircmp_r(const _S8 * s1, const _S8 * s2)
{
    return dircmp(s2, s1);
}

_S32 dircasecmp(const _S8 * s1, const _S8 * s2)
{
    for(; ; ++s1, ++s2)
    {
        if(!table_dir[(_U8)*s1])
        {
            ++s1;
            continue;
        }

        if(!table_dir[(_U8)*s2])
        {
            ++s2;
            continue;
        }
        
        if(table_case[(_U8)*s1] != table_case[(_U8)*s2])
        {
            break;
        }
        
        if(*s1 == '\0')
        {
            return (0);
        }
    }

    return ((table_case[(_U8)*s1] <table_case[(_U8)*s2])? (-1): (+1));
}

_S32 dircasecmp_r(const _S8 * s1, const _S8 * s2)
{
    return dircasecmp(s2, s1);
}

_S32 strcmp_r(const _S8 * s1, const _S8 * s2)
{
	return strcmp(s2, s1);
}

/* numcmp: compare s1 and s2 numerically */
_S32 numcmp(const _S8 * s1, const _S8 * s2)
{
	double	v1, v2;

	v1		= atof(s1);
	v2		= atof(s2);

	if (v1 < v2)
	{
		return - 1;

	}
	else if (v1 > v2)
	{
		return 1;

	}
	else 
	{
		return 0;
	}
}

_S32 numcmp_r(const _S8 * s1, const _S8 * s2)
{
	return numcmp(s2, s1);
}

