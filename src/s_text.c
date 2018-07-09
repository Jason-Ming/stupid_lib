#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <ctype.h>

#include "s_defines.h"
#include "s_log.h"
#include "s_text.h"
#include "s_mem.h"
#include "s_stm.h"

#define IN 1 /* inside a word */
#define OUT 0 /* outside a word */

#define SWITCH_ENABLE 1
#define SWITCH_DISABLE 0

#define OUTPUT_STR(c, dest, size)\
    do{\
        DEBUG_PRINT("OUTPUT_STR: %c", c);\
        if(size > 1)\
        {\
            *dest++ = c;\
            size--;\
        }\
    }while(0);

#define OUTPUT_STR_RANGE(begin, end, dest, size) \
    do{\
        if(begin != '\0' && end != '\0')\
        {\
            DEBUG_PRINT("OUTPUT_STR_RANGE: %c~%c", begin, end);\
            for(_S8 c = begin; c <= end; c++)\
            {\
                OUTPUT_STR(c, dest, size);\
            }\
            begin = end = '\0';\
        }\
    }while(0);

#define OUTPUT_STR_MULTI(c, num, dest, size) \
    do{\
        DEBUG_PRINT("OUTPUT_STR_MULTI: %c, %zd", c, num);\
        for(_S32 i = 0; i < num; i++)\
        {\
            OUTPUT_STR(c, dest, size);\
        }\
    }while(0);

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

ENUM_RETURN s_getline(FILE *fp, _S8 buffer[], _S32 buffer_size, _S32 *length)
{
    R_ASSERT(fp != NULL, RETURN_FAILURE);
    R_ASSERT(buffer != NULL, RETURN_FAILURE);
    R_ASSERT(length != NULL, RETURN_FAILURE);

    _S32 c;
    _S32 len_temp = 0;

    while((c = fgetc(fp)) != EOF && c != '\n')
    {
        if(len_temp < buffer_size-1)
        {
            buffer[len_temp] = c;
        }
        ++len_temp;
    }

    if(c == '\n')
    {
        if(len_temp < (buffer_size-1))
        {
            buffer[len_temp] = c;
        }
        ++len_temp;
    }

    buffer[(buffer_size - 1) > len_temp ? len_temp : (buffer_size - 1)] = '\0';

    *length = len_temp;

    return RETURN_SUCCESS;
}

ENUM_RETURN s_get_word(const _S8 *source, _S8 *word_buf, size_t buf_size, size_t *word_len, const _S8 **next)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(word_buf != NULL, RETURN_FAILURE);
    R_ASSERT(word_len != NULL, RETURN_FAILURE);
    R_ASSERT(next != NULL, RETURN_FAILURE);

    size_t len = 0;
    
    /* skip white space */
    while(isspace(*source)) source++;
    
    while(*source != '\0')
    {
        if(isspace(*source))
        {
            break;
        }

        source++;
        len++;
    };

    strncpy(word_buf, source - len, MIN(len, buf_size - 1));
    word_buf[len] = '\0';
    *word_len = len;
    
    while(isspace(*source)) source++;

    *next = source;

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

ENUM_RETURN s_fold(_S8 *pstr_buf_source, _S8 *pstr_buf_temp, _S32 buf_temp_len, _S32 fold_num)
{
    R_ASSERT(pstr_buf_source != NULL, RETURN_FAILURE);
    R_ASSERT(pstr_buf_temp != NULL, RETURN_FAILURE);
    R_ASSERT(fold_num > 0, RETURN_FAILURE);
    
    _S32 len = strlen(pstr_buf_source);
    //if(len >= fold_num, RETURN_SUCCESS);

    memset(pstr_buf_temp, '\0', buf_temp_len);

    DEBUG_PRINT("source: %s\nsource len: %d\ndest: %s\ndest len: %d\n",
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

                DEBUG_PRINT("buf_temp_len = %d, strlen(pstr_buf_temp) = %zu, "
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

    DEBUG_PRINT("buf_temp_len = %d, strlen(pstr_buf_temp) = %zu, "
        "pos = %d, start_pos = %d\n",
        buf_temp_len, strlen(pstr_buf_temp), pos, start_pos);
    strcat(pstr_buf_temp, &pstr_buf_source[start_pos]);

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
    size_t len = 0;
    
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

typedef enum TAG_ENUM_EXPAND_STATE
{
    EXPAND_STATE_CONC = 0,
    EXPAND_STATE_RANGE_BEGIN,
    EXPAND_STATE_RANGE_TO,
    EXPAND_STATE_RANGE_END,
    EXPAND_STATE_END,
    EXPAND_STATE_MAX
}ENUM_EXPAND_STATE;

typedef struct TAG_STRU_EXPAND_STM_RUN_DATA
{
    const _S8 *s1;
    _S8 *s2;
    size_t size;
    _S8 begin;
    _S8 end;
    _S8 c;
}STRU_EXPAND_STM_RUN_DATA;

PRIVATE STM s_expand_stm;
PRIVATE STRU_EXPAND_STM_RUN_DATA s_expand_run_data;

PRIVATE ENUM_RETURN s_expand_stm_prepare_proc()
{
    s_expand_run_data.begin = s_expand_run_data.end = s_expand_run_data.c = '\0';
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_expand_stm_clear_proc()
{

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_expand_stm_preproc()
{
    ENUM_RETURN ret_val;
    s_expand_run_data.c = *(s_expand_run_data.s1);

    if(s_expand_run_data.c == '\0')
    {
        ret_val = set_current_stm_state(s_expand_stm, EXPAND_STATE_END);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        
        return RETURN_SUCCESS;
    }
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_expand_stm_postproc()
{
    (s_expand_run_data.s1)++;
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_expand_stm_state_notifier()
{

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_expand_stm_state_proc_conc()
{
    _S8 c = s_expand_run_data.c;
    _S8 begin = s_expand_run_data.begin;
    _S8 end = s_expand_run_data.end;

    if(c == '-')
    {
        //原样输出
        OUTPUT_STR(c, s_expand_run_data.s2, s_expand_run_data.size);
    }
    else if(!isdigit(c) && !isupper(c) && !(islower(c))) /* -1-2a-z---B*/
    {
        set_current_stm_state(s_expand_stm, EXPAND_STATE_END);
        R_RET_LOG(RETURN_FAILURE, "unexpected char: %c", c);
    }
    else
    {
        begin = end = c;
        set_current_stm_state(s_expand_stm, EXPAND_STATE_RANGE_BEGIN);
    }
    
    s_expand_run_data.begin = begin;
    s_expand_run_data.end = end;

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_expand_stm_state_proc_range_begin()
{
    _S8 c = s_expand_run_data.c;
    _S8 begin = s_expand_run_data.begin;
    _S8 end = s_expand_run_data.end;

    if(c == '-')
    {
        set_current_stm_state(s_expand_stm, EXPAND_STATE_RANGE_TO);
    }
    else if(!isdigit(c) && !isupper(c) && !(islower(c))) /* -1-2a-z---B*/
    {
        set_current_stm_state(s_expand_stm, EXPAND_STATE_END);
        R_RET_LOG(RETURN_FAILURE, "unexpected char: %c", c);
    }
    else
    {
        OUTPUT_STR_RANGE(begin, end, s_expand_run_data.s2, s_expand_run_data.size);
        begin = end = c;
    }
    
    s_expand_run_data.begin = begin;
    s_expand_run_data.end = end;

    return RETURN_SUCCESS;
}
PRIVATE ENUM_BOOLEAN s_expand_in_same_range(_S8 begin, _S8 end)
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
PRIVATE ENUM_RETURN s_expand_stm_state_proc_range_to()
{
    _S8 c = s_expand_run_data.c;
    _S8 begin = s_expand_run_data.begin;
    _S8 end = s_expand_run_data.end;

    if(c == '-')
    {
        OUTPUT_STR_RANGE(begin, end, s_expand_run_data.s2, s_expand_run_data.size);
        
        //原样输出
        OUTPUT_STR(c, s_expand_run_data.s2, s_expand_run_data.size);
        OUTPUT_STR(c, s_expand_run_data.s2, s_expand_run_data.size);
        set_current_stm_state(s_expand_stm, EXPAND_STATE_CONC);
    }
    else if(!isdigit(c) && !isupper(c) && !(islower(c))) /* -1-2a-z---B*/
    {
        set_current_stm_state(s_expand_stm, EXPAND_STATE_END);
        R_RET_LOG(RETURN_FAILURE, "unexpected char: %c", c);
    }
    else
    {
        //与之前的begin不形成递增
        if(s_expand_in_same_range(end, c) == BOOLEAN_FALSE)
        {
            OUTPUT_STR_RANGE(begin, end, s_expand_run_data.s2, s_expand_run_data.size);
            OUTPUT_STR('-', s_expand_run_data.s2, s_expand_run_data.size);
            begin = end = c;
            set_current_stm_state(s_expand_stm, EXPAND_STATE_RANGE_BEGIN);
        }
        else
        {
            //与之前的begin形成递增
            end = c;
            set_current_stm_state(s_expand_stm, EXPAND_STATE_RANGE_END);
        }
    }
    
    s_expand_run_data.begin = begin;
    s_expand_run_data.end = end;

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_expand_stm_state_proc_range_end()
{
    _S8 c = s_expand_run_data.c;
    _S8 begin = s_expand_run_data.begin;
    _S8 end = s_expand_run_data.end;

    if(c == '-')
    {
        set_current_stm_state(s_expand_stm, EXPAND_STATE_RANGE_TO);
    }
    else if(!isdigit(c) && !isupper(c) && !(islower(c))) /* -1-2a-z---B*/
    {
        set_current_stm_state(s_expand_stm, EXPAND_STATE_END);
        R_RET_LOG(RETURN_FAILURE, "unexpected char: %c", c);
    }
    else
    {
        OUTPUT_STR_RANGE(begin, end, s_expand_run_data.s2, s_expand_run_data.size);
        begin = end = c;
        set_current_stm_state(s_expand_stm, EXPAND_STATE_RANGE_BEGIN);
    }
    
    s_expand_run_data.begin = begin;
    s_expand_run_data.end = end;

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_expand_stm_state_proc_end()
{
    _S8 begin = s_expand_run_data.begin;
    _S8 end = s_expand_run_data.end;
    ENUM_EXPAND_STATE state = get_last_stm_state(s_expand_stm);


    OUTPUT_STR_RANGE(begin, end, s_expand_run_data.s2, s_expand_run_data.size);
    
    if(state == EXPAND_STATE_RANGE_TO)
    {
        OUTPUT_STR('-', s_expand_run_data.s2, s_expand_run_data.size);
    }
    
    *(s_expand_run_data.s2)++ = '\0';
    (s_expand_run_data.size)--;

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_expand_init()
{
    ENUM_RETURN ret_val = RETURN_FAILURE;
    ret_val = stm_create(&s_expand_stm, EXPAND_STATE_MAX);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = set_stm_start_state(s_expand_stm, EXPAND_STATE_CONC);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = set_stm_end_state(s_expand_stm, EXPAND_STATE_END);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_prepare_handler(s_expand_stm, s_expand_stm_prepare_proc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_clear_handler(s_expand_stm, s_expand_stm_clear_proc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_preproc_handler(s_expand_stm, s_expand_stm_preproc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_postproc_handler(s_expand_stm, s_expand_stm_postproc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_state_notifier(s_expand_stm, s_expand_stm_state_notifier);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_state_handler(s_expand_stm, EXPAND_STATE_CONC, s_expand_stm_state_proc_conc, "- state");
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_state_handler(s_expand_stm, EXPAND_STATE_RANGE_BEGIN, s_expand_stm_state_proc_range_begin, "range begin state");
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_state_handler(s_expand_stm, EXPAND_STATE_RANGE_TO, s_expand_stm_state_proc_range_to, "range to state");
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_state_handler(s_expand_stm, EXPAND_STATE_RANGE_END, s_expand_stm_state_proc_range_end, "range end state");
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_state_handler(s_expand_stm, EXPAND_STATE_END, s_expand_stm_state_proc_end, "end state");
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}
PRIVATE ENUM_RETURN s_expand_clear()
{
    ENUM_RETURN ret_val = stm_delete(&s_expand_stm);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    return RETURN_SUCCESS;
}

ENUM_RETURN s_expand(const _S8 *s1, _S8 *s2, size_t size)
{
    R_ASSERT(s1 != NULL, RETURN_FAILURE);
    R_ASSERT(s2 != NULL, RETURN_FAILURE);

    ENUM_RETURN ret_val = s_expand_init();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    s_expand_run_data.s1 = s1;
    s_expand_run_data.s2 = s2;
    s_expand_run_data.size = size;
    
    ret_val = stm_run(s_expand_stm);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = s_expand_clear();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    

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

#define MAX_CHAR 256

_VOID s_escape(_S8* source, _S8* dest, size_t size)
{
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
        _S32 c = *source;
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
                *dest++ = '\\';
                *dest++ = escape[c];
                size -= 2;
                break;
            }
            default:
            {
                *dest++ = c;
                size --;
                break;
            }
        }
        source++;
    }

    /* append \0 */
    *dest = *source;
}

_VOID s_unescape(_S8* source, _S8* dest, size_t size)
{
    
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
    
    while(*source != '\0' && size > 2)
    {
        _S32 c = *source;
        printf("c = %c, in_escape = %d\n", c, in_escape);
        
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
                    *dest++ = unescape[c];
                    size--;
                    break;
                }
                default:
                {
                    *dest++ = '\\';
                    *dest++ = c;
                    size -= 2;
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
                *dest++ = c;
                size--;
            }
        }
        source++;
    }

    /* append \0 */
    *dest = *source;
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

ENUM_RETURN s_strindex(const _S8 *source, const _S8 *target, _S32 *index)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(target != NULL, RETURN_FAILURE);
    R_ASSERT(index != NULL, RETURN_FAILURE);

    _S32 temp = 0;
    *index = -1;
    size_t len = strlen(target);
    
    while(*source != '\0')
    {
        if(memcmp(source, target, len) == 0)
        {
            *index = temp;
            break;
        }
        source++;
        temp++;
    }

    return RETURN_SUCCESS;
}

ENUM_RETURN s_strrindex(const _S8 *source, const _S8 *target, _S32 *index)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(target != NULL, RETURN_FAILURE);
    R_ASSERT(index != NULL, RETURN_FAILURE);

    const _S8 *temp = source + strlen(source);
    *index = -1;
    size_t len = strlen(target);
    
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

ENUM_RETURN s_strend(const _S8 *source, const _S8 *target, ENUM_BOOLEAN *occur)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(target != NULL, RETURN_FAILURE);
    R_ASSERT(occur != NULL, RETURN_FAILURE);

    *occur = BOOLEAN_FALSE;
    size_t len_s = strlen(source);
    size_t len_t = strlen(target);

    if(len_s < len_t)
    {
        return RETURN_SUCCESS;
    }

    *occur = (strcmp(source + len_s - len_t, target) == 0?BOOLEAN_TRUE:BOOLEAN_FALSE);
    return RETURN_SUCCESS;
}


