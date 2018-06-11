#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "s_defines.h"
#include "s_log.h"
#include "s_text.h"
#include "s_mem.h"

#define IN 1 /* inside a word */
#define OUT 0 /* outside a word */

#define SWITCH_ENABLE 1
#define SWITCH_DISABLE 0

int output_switch = SWITCH_DISABLE;

void enable_output(void)
{
    output_switch = SWITCH_ENABLE;
}

void disable_output(void)
{
    output_switch = SWITCH_DISABLE;
}

/* count lines, words, and charactors in input */
int process_words(const char* filename, const char *separator)
{
    assert(filename != NULL);
    assert((separator != NULL && output_switch == SWITCH_ENABLE) 
        || (separator == NULL && output_switch == SWITCH_DISABLE));
    
    FILE* fpr = NULL;
    fpr = fopen(filename, "r");
	assert(fpr != NULL);
    FILE *fpw = NULL;
    char *filename_ouput = NULL;


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
    
    int c, nl, nw, nc, state;

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

    //printf("%d, %d, %d\n", nl, nw, nc);
	fclose(fpr);
    if(output_switch == SWITCH_ENABLE)
    {
        fclose(fpw);
    }

    return nw;
}
int count_words(const char* filename)
{
    int retval;
    assert(filename != NULL);
    disable_output();

    retval = process_words(filename, NULL);

    return retval;
}

int format_words(const char* filename, const char *separator)
{
    int retval;
    assert(filename != NULL);
    assert(separator != NULL);

    enable_output();
    
    retval = process_words(filename, separator);

    disable_output();
    return retval;
}

/* read a line into line, return length */
int get_line(FILE *fp, char line[], int maxline)
{
    int c;
    int i = 0;

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

ENUM_RETURN reverse(char *pstr_buf)
{
    R_ASSERT(pstr_buf != NULL, RETURN_FAILURE);
    int start = 0;
    int end = strlen(pstr_buf) - 1;
    char temp;
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

ENUM_RETURN fold(char *pstr_buf_source, char *pstr_buf_temp, int buf_temp_len, int fold_num)
{
    R_ASSERT(pstr_buf_source != NULL, RETURN_FAILURE);
    R_ASSERT(pstr_buf_temp != NULL, RETURN_FAILURE);
    R_ASSERT(fold_num > 0, RETURN_FAILURE);
    
    int len = strlen(pstr_buf_source);
    //if(len >= fold_num, RETURN_SUCCESS);

    memset(pstr_buf_temp, '\0', buf_temp_len);

    printf("source: %s\nsource len: %d\ndest: %s\ndest len: %d\n",
        pstr_buf_source, len, pstr_buf_temp, buf_temp_len);
    
    int pos = 0;
    int pos_blank = -1;
    int fold_len = 0;
    int start_pos = pos;
    
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


unsigned long long htou(const char *str)
{
    unsigned long long sum = 0;
    size_t len = strlen(str);

    if(len > 2)
    {
        if(str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
        {
            str = str + 2;
            len = len - 2;
        }
    }

    if(len > sizeof(long long int)*2)
    {
        perror("the string length is too large!\n");
        return 0;
    }
    
    for(size_t i = 0; i < len; i++)
    {
        size_t index =i;
        char c = str[index];
        int temp = 0;

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

long long htoi(const char *str)
{
    unsigned long long temp = htou(str);
    return VALUE_S64_OF_ADDR(&temp);
}

void squeeze(char s1[], const char s2[])
{
    char s[256] = {0};
    int c;
    while((c = *s2) != '\0')
    {
        s[c] = 1;
        s2++;
    }
    char *p = s1;
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

char* any(char s1[], const char s2[])
{
    char s[256] = {0};
    int c;
    while((c = *s2) != '\0')
    {
        s[c] = 1;
        s2++;
    }
    
    char *p = NULL;
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

