#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <ctype.h>

#include "s_defines.h"
#include "s_limits.h"
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

PRIVATE ENUM_BOOLEAN is_separator(_S32 c)
{
    R_ASSERT(c >= 0 && c < MAX_CHAR, BOOLEAN_FALSE);
    
    return (whether_word_separators_have_been_set?
        word_separators[c]:
        (' ' == c || '\n' == c || '\t' == c || '\v' == c || '\f' == c));
}

ENUM_RETURN s_count_word_f(FILE *pfr, size_t *word_num)
{
    R_ASSERT(pfr != NULL, RETURN_FAILURE);
    R_ASSERT(word_num != NULL, RETURN_FAILURE);
    *word_num = 0;
    _S32 c;
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
    _S32 c;
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
            OUTPUT_C(c, word_buf, buf_size);
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
    if(!feof(pfr))
    {
        DEBUG_PRINT("still have lines to be read, the number of line_ptr is not enough!\n");
    }
    
    return RETURN_SUCCESS;
}

ENUM_RETURN s_getlines_f_r(FILE *pfr, _S8 *line_ptr[], size_t line_ptr_num, size_t *line_num)
{
    R_ASSERT(pfr != NULL, RETURN_FAILURE);
    R_ASSERT(line_ptr != NULL, RETURN_FAILURE);
    R_ASSERT(line_ptr_num > 0, RETURN_FAILURE);
    R_ASSERT(line_num != NULL, RETURN_FAILURE);
    
    _S8 line_buffer[MAX_LINE_LEN];
    size_t line_len = 0;
    *line_num = 0;

    ENUM_RETURN ret_val = fseek(pfr, 0, SEEK_END);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    while(s_getline_f_r(pfr, line_buffer, MAX_LINE_LEN, &line_len) == RETURN_SUCCESS && line_len > 0 && *line_num < line_ptr_num)
    {
        line_ptr[*line_num] = (_S8*)malloc(line_len + 1);
        R_ASSERT(line_ptr[*line_num] != NULL, RETURN_FAILURE);

        strcpy(line_ptr[(*line_num)++], line_buffer);
    };

    /* still have lines to be read, the number of line_ptr is not enough */
    if(fseek(pfr, -1, SEEK_CUR) == 0)
    {
        DEBUG_PRINT("still have lines to be read, the number of line_ptr is not enough!\n");
    }
    
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
    R_ASSERT(**source == '\0', RETURN_FAILURE);
    
    return RETURN_SUCCESS;
}

ENUM_RETURN s_getline_f(FILE *pfr, _S8 buffer[], size_t buffer_size, size_t *length)
{
    R_ASSERT(pfr != NULL, RETURN_FAILURE);
    R_ASSERT(buffer != NULL, RETURN_FAILURE);
    R_ASSERT(length != NULL, RETURN_FAILURE);
    
    *length = 0;

    _S32 c;
    size_t len_temp = buffer_size;

    while((c = fgetc(pfr)) != EOF && c != '\n')
    {
        OUTPUT_C(c, buffer, buffer_size);
    }

    if(c == '\n')
    {
        OUTPUT_C(c, buffer, buffer_size);
    }

    OUTPUT_END(buffer, buffer_size);

    *length = (len_temp - buffer_size) - 1;
    
    return RETURN_SUCCESS;
}

ENUM_RETURN s_getline_f_r(FILE *pfr, _S8 buffer[], size_t buffer_size, size_t *length)
{
    R_ASSERT(pfr != NULL, RETURN_FAILURE);
    R_ASSERT(buffer != NULL, RETURN_FAILURE);
    R_ASSERT(length != NULL, RETURN_FAILURE);
    
    *length = 0;

    ENUM_RETURN ret_val;

    _S32 c;
    _S8 *temp_buffer = buffer;
    size_t len_temp = buffer_size;
    ENUM_BOOLEAN whether_last_char_has_been_read = BOOLEAN_FALSE;

    //前提条件是调用者要把指针移到文件末尾，且每次调用假设指向的都是上一行的最后一个字符（有可能是换行）。
    while(fseek(pfr, -1, SEEK_CUR) == 0)
    {
        if((c = fgetc(pfr)) == '\n')
        {
            if(whether_last_char_has_been_read == BOOLEAN_TRUE)
            {
                break;
            }
        }

        whether_last_char_has_been_read = BOOLEAN_TRUE;

        OUTPUT_C(c, buffer, buffer_size);
        
        R_ASSERT(fseek(pfr, -1, SEEK_CUR) == 0, RETURN_FAILURE);
    }

    OUTPUT_END(buffer, buffer_size);

    *length = (len_temp - buffer_size) - 1;

    ret_val = s_reverse(temp_buffer);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    return RETURN_SUCCESS;
}

ENUM_RETURN s_getline_s(const _S8 **source, _S8 buffer[], size_t buffer_size, size_t *length)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(*source != NULL, RETURN_FAILURE);
    R_ASSERT(buffer != NULL, RETURN_FAILURE);
    R_ASSERT(buffer_size > 0, RETURN_FAILURE);
    R_ASSERT(length != NULL, RETURN_FAILURE);
    *length = 0;

    _S8 c;
    size_t len_temp = buffer_size;

    while((c = **source) != '\0' && c != '\n')
    {
        OUTPUT_C(c, buffer, buffer_size);
        (*source)++;
    }

    if(c == '\n')
    {
        OUTPUT_C(c, buffer, buffer_size);
        (*source)++;
    }
    
    *length = len_temp - buffer_size;

    OUTPUT_END(buffer, buffer_size);

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

/* word in this function has 3 type: '\n', continuing blanks, continuing non-blanks */
PRIVATE size_t find_next_word_position_f(FILE *pfr, ENUM_BOOLEAN *next_word_is_newline)
{
    _S32 c;
    size_t len = 0;
    *next_word_is_newline = BOOLEAN_FALSE;
    if((c = fgetc(pfr)) == EOF)
    {
        return 0;
    }
    
    if(' ' == c || '\t' == c || '\v' == c || '\f' == c)
    {
        len++;
        while((c = fgetc(pfr)) != EOF)
        {
            if(' ' == c || '\t' == c || '\v' == c || '\f' == c)
            {
                len++;
                continue;
            }
            else
            {
                break;
            }
        }
    }
    else if ('\n' == c)
    {
        len++;
        *next_word_is_newline = BOOLEAN_TRUE;
        return len;
    }
    else
    {
        len++;
        
        while((c = fgetc(pfr)) != EOF)
        {
            if(' ' == c || '\n' == c || '\t' == c || '\v' == c || '\f' == c)
            {
                break;
            }
            else
            {
                len++;
                continue;
            }
        }
    }

    fseek(pfr, -1, SEEK_CUR);
    return len;
}
PRIVATE _S32 find_next_word_position_s(const _S8 *source, ENUM_BOOLEAN *next_word_is_newline)
{
    _S8 c = *source;
    _S32 len = 0;
    *next_word_is_newline = BOOLEAN_FALSE;
    
    if(c == '\0')
    {
        return 0;
    }
    
    if(' ' == c || '\t' == c || '\v' == c || '\f' == c)
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
    else if('\n' == c)
    {
        source++;
        len++;
        *next_word_is_newline = BOOLEAN_TRUE;
        return len;
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

ENUM_RETURN s_fold_f(FILE *pfr, FILE *pfw, size_t fold_len)

{
    R_ASSERT(pfr != NULL, RETURN_FAILURE);
    R_ASSERT(pfw != NULL, RETURN_FAILURE);
    R_ASSERT(fold_len >= 5, RETURN_FAILURE);

    _S32 c;
    size_t next_word_len = 0;
    size_t left_space = fold_len;
    ENUM_BOOLEAN newline = BOOLEAN_TRUE;
    ENUM_BOOLEAN next_word_is_newline = BOOLEAN_FALSE;

    //feof must be triggered by fgetc, if pfr point to EOF, but has not been touched by fgetc, feof is false
    while((c = fgetc(pfr)) != EOF)
    {
        fseek(pfr, -1, SEEK_CUR);
        
        /* find next word position: word means 
        1.first ch is space, the word length is the longest space ch;
        2.first ch is not space, the word length is the longest non-space ch; */

        next_word_len = find_next_word_position_f(pfr, &next_word_is_newline);

        if(next_word_len == 0)
        {
            break;
        }
        
        fseek(pfr, -next_word_len, SEEK_CUR);

        if(next_word_is_newline)
        {
            OUTPUT_STR_F(pfw, pfr, next_word_len);
            left_space = fold_len;
            newline = BOOLEAN_TRUE;
            continue;
        }
        
        if(left_space >= next_word_len)
        {
            OUTPUT_STR_F(pfw, pfr, next_word_len);
            left_space -= next_word_len;
            newline = BOOLEAN_FALSE;
        }
        else
        {
            if(newline)
            {
                OUTPUT_STR_F(pfw, pfr, left_space);
                fputc('\n', pfw);

                left_space = fold_len;
                newline = BOOLEAN_TRUE;
            }
            else
            {
                fputc('\n', pfw);
                left_space = fold_len;
                newline = BOOLEAN_TRUE;
            }
        }
    }

    return RETURN_SUCCESS;
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
    ENUM_BOOLEAN next_word_is_newline = BOOLEAN_FALSE;
    
    while(*source != '\0')
    {
        /* find next word position: word means 
        1.first ch is space, the word length is the longest space ch;
        2.first ch is not space, the word length is the longest non-space ch; */

        next_word_len = find_next_word_position_s(source, &next_word_is_newline);
        if(next_word_len == 0)
        {
            break;
        }

        if(next_word_is_newline)
        {
            OUTPUT_STR(dest, size, source, next_word_len);
            source = source + next_word_len;
            left_space = fold_len;
            newline = BOOLEAN_TRUE;
            continue;
        }

        if(left_space >= next_word_len)
        {
            OUTPUT_STR(dest, size, source, next_word_len);
            source = source + next_word_len;
            left_space -= next_word_len;
            newline = BOOLEAN_FALSE;
        }
        else
        {
            if(newline)
            {
                OUTPUT_STR(dest, size, source, left_space);
                OUTPUT_C('\n', dest, size);

                source = source + left_space;
                left_space = fold_len;
                newline = BOOLEAN_TRUE;
            }
            else
            {
                OUTPUT_C('\n', dest, size);
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
            S_LOG("invalid char: %c", c);
			return RETURN_FAILURE;
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
    S_R_FALSE(len > 0, RETURN_FAILURE);
    
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

    S_R_FALSE(len > 0, RETURN_FAILURE);

    while(*str != '\0')
    {
        S_R_FALSE(isdigit(*str), RETURN_FAILURE);
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
        OUTPUT_C((value % 10)*sign + '0', dest, size);
    }while((value /= 10) != 0);

    if(sign < 0)
    {
        OUTPUT_C('-', dest, size);
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
        OUTPUT_C( digits[index], dest, size);
        len++;
    }while((value /= b) != 0);

    if(sign < 0)
    {
        OUTPUT_C('-', dest, size);
        len++;
    }

    while(len < w)
    {
        OUTPUT_C(' ', dest, size);
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

    S_R_FALSE(strlen(target) > 0, RETURN_SUCCESS);
    
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

    S_R_FALSE(strlen(target) > 0, RETURN_SUCCESS);
    
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
                OUTPUT_C('\\', dest, size);
                OUTPUT_C(escape[(_U8)c], dest, size);
                break;
            }
            default:
            {
                OUTPUT_C(c, dest, size);
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
                    OUTPUT_C(unescape[(_U8)c], dest, size);
                    break;
                }
                default:
                {
                    OUTPUT_C('\\', dest, size);
                    OUTPUT_C(c, dest, size);
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
                OUTPUT_C(c, dest, size);
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

ENUM_RETURN s_strnindex(const _S8 *source, const size_t source_len, const _S8 *target, _S32 *index)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(target != NULL, RETURN_FAILURE);
    R_ASSERT(index != NULL, RETURN_FAILURE);

    *index = -1;
    size_t len = strlen(target);
    S_R_FALSE(len > 0, RETURN_SUCCESS);

    /*  source*  len = 6 */
    /*  0123456  */
    const _S8 *source_end = source + source_len;
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

ENUM_RETURN s_strindex(const _S8 *source, const _S8 *target, _S32 *index)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);

    return s_strnindex(source, strlen(source), target, index);
}

ENUM_RETURN s_strrindex(const _S8 *source, const _S8 *target, _S32 *index)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(target != NULL, RETURN_FAILURE);
    R_ASSERT(index != NULL, RETURN_FAILURE);

    *index = -1;
    size_t len = strlen(target);
    S_R_FALSE(len > 0, RETURN_SUCCESS);
    
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
    S_R_FALSE(len_t > 0, RETURN_SUCCESS);

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
            OUTPUT_C(*source, dest, len);
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
                OUTPUT_C('\t', dest, len);
                offset = 0;
                source += j;
            }
            else if(*(source + k) == '\t')
            {
                OUTPUT_C('\t', dest, len);
                offset = 0;
                source += (k + 1);
            }
            else
            {
                for(_S32 l = 0; l < k; l++)
                {
                    OUTPUT_C(*source, dest, len);
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
            OUTPUT_C(*source, dest, len);
            (*source == '\n')?(offset = 0):(offset++);
        }
        else
        {
            j = cal_space_number(offset, tab_stop);
            for(_S32 k = 0; k < j; k++)
            {
                OUTPUT_C(' ', dest, len);
                
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

ENUM_RETURN s_file_compare(FILE *pfr1, FILE *pfr2, _S32 *result)
{
    S_R_ASSERT(pfr1 != NULL, RETURN_FAILURE);
    S_R_ASSERT(pfr2 != NULL, RETURN_FAILURE);
    S_R_ASSERT(result != NULL, RETURN_FAILURE);
    *result = 0;

    
    ENUM_RETURN ret_val;
    _S8 *p_text1 = NULL;
    size_t size_text1 = 0;
    _S8 *p_text2 = NULL;
    size_t size_text2 = 0;

    ret_val = s_save_file_to_text_buffer(pfr1, &p_text1, &size_text1);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    S_R_ASSERT(p_text1 != NULL, RETURN_FAILURE);

    //display_mem(p_text1, size_text1, BOOLEAN_TRUE);
    ret_val = s_save_file_to_text_buffer(pfr2, &p_text2, &size_text2);
    S_R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, S_FREE(p_text1));
    S_R_ASSERT_DO(p_text2 != NULL, RETURN_FAILURE, S_FREE(p_text1));

    //display_mem(p_text2, size_text2, BOOLEAN_TRUE);

    if(size_text1 != size_text2)
    {
        *result = size_text1 < size_text2 ? (-1):(1);
    }
    else
    {
        *result = strcmp(p_text1, p_text2);
    }
    
    S_FREE(p_text1);
    S_FREE(p_text2);

    return RETURN_SUCCESS;
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

#define MAX_TEXT_TABLE_ELEMENT_LINE 8
#define MAX_TEXT_TABLE_ELEMENT_LINE_LEN 256
#define MAX_TEXT_TABLE_ELEMENT_FORMAT_LEN 256
#define MAX_TEXT_TABLE_ELEMENT_FOLD_TEXT_LEN (MAX_TEXT_TABLE_ELEMENT_LINE_LEN*MAX_TEXT_TABLE_ELEMENT_LINE)

typedef struct TAG_STRU_TEXT_TABLE_ELEMENT
{
    _S8 format[MAX_TEXT_TABLE_ELEMENT_FORMAT_LEN];
    size_t lines;//line_max 8
    _S8 line_text[MAX_TEXT_TABLE_ELEMENT_LINE][MAX_TEXT_TABLE_ELEMENT_LINE_LEN];
}STRU_TEXT_TABLE_ELEMENT;

PRIVATE STRU_TEXT_TABLE_ELEMENT* get_new_text_table(size_t rows, size_t columns)
{
    size_t table_elements = rows * columns;
    STRU_TEXT_TABLE_ELEMENT* text_table_elements =
        (STRU_TEXT_TABLE_ELEMENT*)malloc(table_elements*sizeof(STRU_TEXT_TABLE_ELEMENT));
    R_ASSERT(text_table_elements != NULL, NULL);

    for(int i = 0; i < table_elements; i++)
    {
        memset(text_table_elements[i].format, '\0', MAX_TEXT_TABLE_ELEMENT_FORMAT_LEN);
        text_table_elements[i].lines = 0;
        memset(text_table_elements[i].line_text, '\0', MAX_TEXT_TABLE_ELEMENT_FOLD_TEXT_LEN);
    }

    return text_table_elements;
}

PRIVATE ENUM_RETURN build_text_table_format(STRU_TEXT_TABLE_ELEMENT* text_table_element,
    STRU_TABLE_TEXT_FORMAT format)
{
    PRIVATE _S8 format_string[MAX_TEXT_TABLE_ELEMENT_FORMAT_LEN];
    R_ASSERT(format.margin_left + format.margin_right + 5 
        < format.table_column_size, RETURN_FAILURE);
    
    size_t fold_len = format.table_column_size - format.margin_left - format.margin_right;
    _S8 *format_temp = text_table_element->format;
    size_t format_len = MAX_TEXT_TABLE_ELEMENT_FORMAT_LEN;

    OUTPUT_C_MULTI(' ', format.margin_left, format_temp, format_len);

    sprintf(format_string, "%%%s%zds", format.align_left?"-":"", fold_len);

    OUTPUT_STR(format_temp, format_len, format_string, strlen(format_string));

    OUTPUT_C_MULTI(' ', format.margin_right, format_temp, format_len);

    OUTPUT_END(format_temp, format_len);

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN build_text_table_element(STRU_TEXT_TABLE_ELEMENT* text_table_element,
    const _S8 *text, STRU_TABLE_TEXT_FORMAT format)
{
    
    PRIVATE _S8 text_fold_buffer[MAX_TEXT_TABLE_ELEMENT_FOLD_TEXT_LEN];
    ENUM_RETURN ret_val;
    size_t fold_len = format.table_column_size - format.margin_left - format.margin_right;

    R_ASSERT(text != NULL, RETURN_FAILURE);
    ret_val = s_fold_s(text, text_fold_buffer, MAX_TEXT_TABLE_ELEMENT_FOLD_TEXT_LEN, fold_len);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    _S32 lines = 0;
    size_t len = 0;
    const _S8 *text_temp = text_fold_buffer;

    while(lines < MAX_TEXT_TABLE_ELEMENT_LINE 
        && s_getline_s(&text_temp, 
            text_table_element->line_text[lines], 
            MAX_TEXT_TABLE_ELEMENT_LINE_LEN, &len) == RETURN_SUCCESS 
        && len > 0)
    {
        ret_val = s_trim(text_table_element->line_text[lines]);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        lines++;
    }
        
    text_table_element->lines = lines;
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN build_text_table(STRU_TEXT_TABLE_ELEMENT* text_table_elements, 
    const _S8 *text[], size_t rows, size_t columns, STRU_TABLE_TEXT_FORMAT format[])
{
    ENUM_RETURN ret_val;

    for(size_t row = 0; row < rows; row++)
    {
        size_t max_line = 0;
        
        for(size_t col = 0; col < columns; col++)
        {
            size_t index = row * columns + col;

            STRU_TEXT_TABLE_ELEMENT* text_table_element = text_table_elements + index;

            ret_val = build_text_table_format(text_table_element, format[index]);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

            ret_val = build_text_table_element(text_table_element, text[index], format[index]);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            
            if(max_line < text_table_element->lines)
            {
                max_line = text_table_element->lines;
            }
        }

        for(size_t col = 0; col < columns; col++)
        {
            size_t index = row * columns + col;
            STRU_TEXT_TABLE_ELEMENT* print_table_text_element = text_table_elements + index;
            print_table_text_element->lines = max_line;
        }
        
    }
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN print_text_table(STRU_TEXT_TABLE_ELEMENT* print_table_text_elements,
    size_t rows, size_t columns)
{
    for(size_t row = 0; row < rows; row++)
    {
        size_t max_line = print_table_text_elements[row*columns].lines;

        for(size_t line = 0; line < max_line; line++)
        {
            
            for(size_t col = 0; col < columns; col++)
            {
                size_t index = row * columns + col;
                
                STRU_TEXT_TABLE_ELEMENT* print_table_text_element = print_table_text_elements + index;
                printf(print_table_text_element->format, print_table_text_element->line_text[line]);
            }
            printf("\n");
        }
    }

    return RETURN_SUCCESS;
}
ENUM_RETURN s_print_text_table(const _S8 *text[], size_t rows, size_t columns, STRU_TABLE_TEXT_FORMAT format[])
{
    R_ASSERT(text != NULL, RETURN_FAILURE);
    R_ASSERT(format != NULL, RETURN_FAILURE);
    R_ASSERT(rows > 0, RETURN_FAILURE);
    R_ASSERT(columns > 0, RETURN_FAILURE);
    
    ENUM_RETURN ret_val;
    
    STRU_TEXT_TABLE_ELEMENT* print_table_text_elements = NULL;
    print_table_text_elements = get_new_text_table(rows, columns);
    R_ASSERT(print_table_text_elements != NULL, RETURN_FAILURE);

    ret_val = build_text_table(print_table_text_elements, text, rows, columns, format);
    R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, S_FREE(print_table_text_elements));

    ret_val = print_text_table(print_table_text_elements, rows, columns);
    R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, S_FREE(print_table_text_elements));

    S_FREE(print_table_text_elements);

    return RETURN_SUCCESS;
}

_S8* s_get_realdir(const _S8 *p_filename)
{
    S_R_FALSE(BOOLEAN_FALSE != s_file_exist(p_filename), NULL);

    _S8 * p_file_realpath = realpath(p_filename, NULL);
    S_R_ASSERT(p_file_realpath != NULL, NULL);
    _S32 index_slash;
    //_S32 index_backslash;
    S_R_ASSERT_DO(RETURN_SUCCESS == s_strrindex(p_file_realpath, "/", &index_slash), NULL, S_FREE(p_file_realpath));
    //S_R_ASSERT_DO(RETURN_SUCCESS == s_strrindex(p_file_realpath, "\\", &index_slash), NULL, S_FREE(p_file_realpath));
    S_R_ASSERT_DO(-1 != index_slash /*|| -1 != index_backslash*/, NULL, S_FREE(p_file_realpath));
    S_R_ASSERT_DO(index_slash < strlen(p_file_realpath), NULL, S_FREE(p_file_realpath));
    p_file_realpath[index_slash + 1] = '\0';

    return p_file_realpath;    
}

ENUM_BOOLEAN s_file_exist(const _S8 * p_filename)
{
    S_R_ASSERT(p_filename != NULL, BOOLEAN_FALSE);
    DEBUG_PRINT("file name: %s", p_filename);
    
    FILE *f = fopen(p_filename, "rt");
    S_R_FALSE(f != NULL, BOOLEAN_FALSE);
    S_FCLOSE(f);
    return BOOLEAN_TRUE;
}

_UL s_get_inode(const _S8 *p_filename)
{
    S_R_ASSERT(p_filename != NULL, SL_INVALID);

    FILE *f = fopen(p_filename, "rt");
    S_R_FALSE(f != NULL, SL_INVALID);
    S_FCLOSE(f);
    
    struct stat *buf=NULL;
    buf=(struct stat *)malloc(sizeof(struct stat));
    S_R_ASSERT(buf != NULL, SL_INVALID);
    
    stat(p_filename,buf);
    
    _UL inode = buf->st_ino;
    S_FREE(buf);
    //printf("file: %s, inode: %ld\n", p_filename, inode);
    return inode;
}

ENUM_RETURN s_save_file_to_text_buffer(
    FILE *pfr, 
    _S8 **pp_text_buffer, 
    size_t *p_buffer_size)
{
    R_ASSERT(pfr != NULL, RETURN_FAILURE);
    R_ASSERT(pp_text_buffer != NULL, RETURN_FAILURE);
    R_ASSERT(p_buffer_size != NULL, RETURN_FAILURE);
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    *pp_text_buffer = NULL;
    *p_buffer_size = 0;
        
    /* 获取文件大小 */  
    ret_val = fseek (pfr , 0 , SEEK_END);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    _SL file_size = ftell (pfr);
	R_ASSERT(file_size != -1, RETURN_FAILURE);

	DEBUG_PRINT("file size = %ld\n", file_size);
	
	*p_buffer_size = file_size + 3;

    *pp_text_buffer = (_S8 *)malloc(*p_buffer_size);
    R_ASSERT(*pp_text_buffer != NULL, RETURN_FAILURE);

    ret_val = fseek (pfr , 0 , SEEK_SET);
    R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, S_FREE(*pp_text_buffer));

    size_t len = fread (*pp_text_buffer,1,*p_buffer_size,pfr);
    R_ASSERT_DO_LOG(len + 3 <= *p_buffer_size, RETURN_FAILURE, S_FREE(*pp_text_buffer), "len = %zd", len);

	/* append newline at the end of file */
	(*pp_text_buffer)[len] = '\n';
	(*pp_text_buffer)[len + 1] = '\n';
    (*pp_text_buffer)[len + 2] = '\0';

    //display_mem(*pp_text_buffer, *p_buffer_size, BOOLEAN_TRUE);
    return RETURN_SUCCESS;
}

_S8 *s_duplicate_string(const _S8 *p_source)
{
    S_R_ASSERT(p_source != NULL, NULL);

    _S8*p_temp = (_S8*)malloc(strlen(p_source) + 1);
    S_R_ASSERT(p_temp != NULL, NULL);

    strcpy(p_temp, p_source);

    return p_temp;
}
_S8* s_concatenate_string(const _S8*p_string1, const _S8*p_string2)
{
    S_R_ASSERT(p_string1 != NULL, NULL);
    S_R_ASSERT(p_string2 != NULL, NULL);

    _S8*p_temp = (_S8*)malloc(strlen(p_string1) + strlen(p_string2) + 1);
    S_R_ASSERT(p_temp != NULL, NULL);

    strcpy(p_temp, p_string1);
    strcat(p_temp, p_string2);

    return p_temp;
}

