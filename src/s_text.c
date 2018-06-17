#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "s_defines.h"
#include "s_log.h"
#include "s_text.h"
#include "s_mem.h"

#define IN 1 /* inside a word */
#define OUT 0 /* outside a word */

#define SWITCH_ENABLE 1
#define SWITCH_DISABLE 0

_S32 output_switch = SWITCH_DISABLE;

PRIVATE void enable_output(void)
{
    output_switch = SWITCH_ENABLE;
}

PRIVATE void disable_output(void)
{
    output_switch = SWITCH_DISABLE;
}

/* count lines, words, and _S8actors in input */
PRIVATE _S32 process_words(const _S8* filename, const _S8 *separator)
{
    assert(filename != NULL);
    assert((separator != NULL && output_switch == SWITCH_ENABLE) 
        || (separator == NULL && output_switch == SWITCH_DISABLE));
    
    FILE* fpr = NULL;
    fpr = fopen(filename, "r");
	assert(fpr != NULL);
    FILE *fpw = NULL;
    _S8 *filename_ouput = NULL;


    if(output_switch == SWITCH_ENABLE)
    {

        filename_ouput = malloc(strlen(filename) + strlen(".output") + 1);
        assert(filename_ouput != NULL);
        filename_ouput[0] = '\0';
        strcat(filename_ouput, filename);
        strcat(filename_ouput, ".output");
        fpw = fopen(filename_ouput, "w");
        assert(fpw != NULL);
    }
    
    _S32 c, nl, nw, nc, state;

    state = OUT;
    nl = nw = nc = 0;

    while((c = fgetc(fpr)) != EOF)
    {
        ++nc;

        if(c == '\n')
        {
            ++nl;
        }

        if(c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f')
        {
            if(state == IN)
            {
                if(output_switch == SWITCH_ENABLE)
                {
                    fputs(separator, fpw);
                }
            }
            
            state = OUT;
        }
        else 
        {
            if(state == OUT)
            {
                state = IN;
                ++nw;
            }
            if(output_switch == SWITCH_ENABLE)
            {
                fputc(c, fpw);
            }
        }
    }

    //pr_S32f("%d, %d, %d\n", nl, nw, nc);
	fclose(fpr);
    if(output_switch == SWITCH_ENABLE)
    {
        fclose(fpw);
    }

    return nw;
}
_S32 count_words(const _S8* filename)
{
    _S32 retval;
    assert(filename != NULL);
    disable_output();

    retval = process_words(filename, NULL);

    return retval;
}

_S32 format_words(const _S8* filename, const _S8 *separator)
{
    _S32 retval;
    assert(filename != NULL);
    assert(separator != NULL);

    enable_output();
    
    retval = process_words(filename, separator);

    disable_output();
    return retval;
}

/* read a line _S32o line, return length */
_S32 get_line(FILE *fp, _S8 line[], _S32 maxline)
{
    _S32 c;
    _S32 i = 0;

    while((c = fgetc(fp)) != EOF && c != '\n')
    {
        if(i < maxline-1)
        {
            line[i] = c;
        }
        ++i;
    }

    if(c == '\n')
    {
        if(i < (maxline-1))
        {
            line[i] = c;
        }
        ++i;
    }

    line[(maxline - 1) > i ? i : (maxline - 1)] = '\0';

    return i;
}

ENUM_RETURN reverse(_S8 *pstr_buf)
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

ENUM_RETURN fold(_S8 *pstr_buf_source, _S8 *pstr_buf_temp, _S32 buf_temp_len, _S32 fold_num)
{
    R_ASSERT(pstr_buf_source != NULL, RETURN_FAILURE);
    R_ASSERT(pstr_buf_temp != NULL, RETURN_FAILURE);
    R_ASSERT(fold_num > 0, RETURN_FAILURE);
    
    _S32 len = strlen(pstr_buf_source);
    //if(len >= fold_num, RETURN_SUCCESS);

    memset(pstr_buf_temp, '\0', buf_temp_len);

    printf("source: %s\nsource len: %d\ndest: %s\ndest len: %d\n",
        pstr_buf_source, len, pstr_buf_temp, buf_temp_len);
    
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
                pstr_buf_source[pos_blank] = '\0';
                
                R_ASSERT_LOG(
                    buf_temp_len - strlen(pstr_buf_temp) >= pos - start_pos + 1, 
                    RETURN_FAILURE,
                    "buf_temp_len = %d, strlen(pstr_buf_temp) = %zu, "
                    "pos = %d, start_pos = %d",
                    buf_temp_len, strlen(pstr_buf_temp), pos, start_pos);

                printf("buf_temp_len = %d, strlen(pstr_buf_temp) = %zu, "
                    "pos = %d, start_pos = %d\n",
                    buf_temp_len, strlen(pstr_buf_temp), pos, start_pos);
                
                strcat(pstr_buf_temp, &pstr_buf_source[start_pos]);

                R_ASSERT_LOG(
                    buf_temp_len - strlen(pstr_buf_temp) >= 2, 
                    RETURN_FAILURE,
                    "buf_temp_len = %d, strlen(pstr_buf_temp) = %zu\n",
                    buf_temp_len, strlen(pstr_buf_temp));

                strcat(pstr_buf_temp, "\n");
                
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
        
        if(pstr_buf_source[pos] == ' ' || pstr_buf_source[pos] == '\n')
        {
            pos_blank = pos;
        }

        pos++;
        fold_len++;
    }

    R_ASSERT_LOG(
        buf_temp_len - strlen(pstr_buf_temp) >= pos - start_pos + 1, 
        RETURN_FAILURE,
        "buf_temp_len = %d, strlen(pstr_buf_temp) = %zu, "
        "pos = %d, start_pos = %d",
        buf_temp_len, strlen(pstr_buf_temp), pos, start_pos);

    printf("buf_temp_len = %d, strlen(pstr_buf_temp) = %zu, "
        "pos = %d, start_pos = %d\n",
        buf_temp_len, strlen(pstr_buf_temp), pos, start_pos);
    strcat(pstr_buf_temp, &pstr_buf_source[start_pos]);

    return RETURN_SUCCESS;
}


ENUM_RETURN htou(const _S8 *str, _U64 *value)
{
    R_ASSERT(str != NULL, RETURN_FAILURE);
    R_ASSERT(value != NULL, RETURN_FAILURE);

    /* skip white space */
    while(isspace(*str++));
    
    _U64 sum = 0;
    size_t len = strlen(str);

    if(len > 2)
    {
        if(str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
        {
            str = str + 2;
            len = len - 2;
        }
    }

    if(len > sizeof(_U64)*2)
    {
        perror("the string length is too large!\n");
        return 0;
    }
    
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
            perror("invalid input");
            return 0;
        }

        sum = sum * 16 + temp;
    }

    return sum;
}

ENUM_RETURN htoi(const _S8 *str, _S64 *value)
{
    R_ASSERT(str != NULL, RETURN_FAILURE);
    R_ASSERT(value != NULL, RETURN_FAILURE);

    _U64 temp;
    ENUM_RETURN retval = htou(str, &temp);
    R_ASSERT(retval == RETURN_SUCCESS, RETURN_FAILURE);

    *value = VALUE_S64_OF_ADDR(&temp);
     
    return RETURN_SUCCESS;
}

_VOID squeeze(_S8 s1[], const _S8 s2[])
{
    _S8 s[256] = {0};
    _S32 c;
    while((c = *s2) != '\0')
    {
        s[c] = 1;
        s2++;
    }
    _S8 *p = s1;
    while((c = *s1) != '\0')
    {
        if(s[c] == 0)
        {
            *p = *s1;
            p++;
        }
        s1++;
    }
    *p = '\0';
}

_S8* any(_S8 s1[], const _S8 s2[])
{
    _S8 s[256] = {0};
    _S32 c;
    while((c = *s2) != '\0')
    {
        s[c] = 1;
        s2++;
    }
    
    _S8 *p = NULL;
    while((c = *s1) != '\0')
    {
        if(s[c] == 1)
        {
            p = s1;
            break;
        }
        s1++;
    }

    return p;
}

