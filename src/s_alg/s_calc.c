#include <string.h>
#include <ctype.h>
#include <math.h>

#include "s_stack.h"
#include "s_log.h"
#include "s_alg.h"
#include "s_text.h"
#include "s_mem.h"
#include "s_limits.h"

#define MAX_LEN_OF_WORD 32

typedef enum TAG_ENUM_CALC_WORD_TYPE
{
    CALC_WORD_TYPE_NUMBER = 0,
    CALC_WORD_TYPE_ADD,
    CALC_WORD_TYPE_SUBTRACT,
    CALC_WORD_TYPE_MULTIPLY,
    CALC_WORD_TYPE_DIVIDE,
    CALC_WORD_TYPE_MODULAR,
    CALC_WORD_TYPE_EMPTY,
    CALC_WORD_TYPE_UNKNOWN,
}ENUM_CALC_WORD_TYPE;

PRIVATE STACK s_calc_stack;

PRIVATE ENUM_RETURN s_calc_rp_init(_VOID)
{
    ENUM_RETURN ret_val = stack_create(&s_calc_stack);
    R_ASSERT(ret_val != RETURN_FAILURE, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_calc_rp_clear(_VOID)
{
    ENUM_RETURN ret_val = stack_delete(&s_calc_stack);
    R_ASSERT(ret_val != RETURN_FAILURE, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_calc_rp_get_word_type_and_value(_S8 *word, ENUM_CALC_WORD_TYPE *type, _SD *value)
{
    R_ASSERT(word != NULL, RETURN_FAILURE);
    R_ASSERT(value != NULL, RETURN_FAILURE);

    ENUM_RETURN ret_val = RETURN_FAILURE;
    ENUM_CALC_WORD_TYPE temp_type = CALC_WORD_TYPE_UNKNOWN;
    _SD temp_value = 0.0;
    
    size_t word_len = strlen(word);
    if(word_len == 0)
    {
        temp_type = CALC_WORD_TYPE_EMPTY;
    }
    else if(strlen(word) == 1 && !isdigit(word[0]))
    {
        switch(word[0])
        {
            case '+':
                temp_type = CALC_WORD_TYPE_ADD;
                break;
            case '-':
                temp_type = CALC_WORD_TYPE_SUBTRACT;
                break;
            case '*':
                temp_type = CALC_WORD_TYPE_MULTIPLY;
                break;
            case '/':
                temp_type = CALC_WORD_TYPE_DIVIDE;
                break;
            case '%':
                temp_type = CALC_WORD_TYPE_MODULAR;
                break;
            default:
                temp_type = CALC_WORD_TYPE_UNKNOWN;
                break;
        }
                
    }
    else
    {
        ret_val = s_strtosd(word, &temp_value);
        if(ret_val == RETURN_SUCCESS)
        {
            temp_type = CALC_WORD_TYPE_NUMBER;
        }
    }

    *type = temp_type;
    *value = temp_value;
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_calc_rp_push(_SD value)
{
    //printf("push: %g\n", value);
    return stack_push(s_calc_stack, (_VOID*)&value, sizeof(value));
}

PRIVATE ENUM_RETURN s_calc_rp_pop(_SD *value)
{
    size_t size_out;
    ENUM_RETURN ret_val = stack_pop(s_calc_stack, (_VOID *)value, &size_out,  sizeof(*value));
    //printf("pop: %g\n", *value);

    return ret_val;
}

PRIVATE ENUM_RETURN s_calc_rp_do(const _S8 * str, _SD * result)
{
    PRIVATE _S8 word[MAX_LEN_OF_WORD];
    size_t word_len = 0;
    ENUM_CALC_WORD_TYPE word_type;
    _SD value, op1, op2;
    ENUM_RETURN ret_val;
    
    for(;;)
    {
        //get a word
        ret_val = s_get_word(str, word, MAX_LEN_OF_WORD, &word_len, &str);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

        //printf("word: %s\n", word);
        
        //analyze word type: op, number, empty, other
        ret_val = s_calc_rp_get_word_type_and_value(word, &word_type, &value);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

        switch(word_type)
        {
            case CALC_WORD_TYPE_NUMBER:
                ret_val = s_calc_rp_push(value);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                break;
                
            case CALC_WORD_TYPE_ADD:
                ret_val = s_calc_rp_pop(&op1);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                ret_val = s_calc_rp_pop(&op2);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                value = op1 + op2;
                ret_val = s_calc_rp_push(value);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                break;
                
            case CALC_WORD_TYPE_MULTIPLY:
                ret_val = s_calc_rp_pop(&op1);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                ret_val = s_calc_rp_pop(&op2);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                value = op1 * op2;
                ret_val = s_calc_rp_push(value);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                break;
                
            case CALC_WORD_TYPE_SUBTRACT:
                ret_val = s_calc_rp_pop(&op1);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                ret_val = s_calc_rp_pop(&op2);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                value = op2 - op1;
                ret_val = s_calc_rp_push(value);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                break;
                
            case CALC_WORD_TYPE_DIVIDE:
                ret_val = s_calc_rp_pop(&op1);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                ret_val = s_calc_rp_pop(&op2);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                if(op1 != 0.0)
                {
                    value = op2 / op1;
                    ret_val = s_calc_rp_push(value);
                    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                }
                else
                {
                    printf("devide zero!\n");
                    SET_VAR_VALUE(value, DOUBLE_INFINITY_PLUS);
                    ret_val = s_calc_rp_push(value);
                    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                }

                break;
                
            case CALC_WORD_TYPE_MODULAR:
                ret_val = s_calc_rp_pop(&op1);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                ret_val = s_calc_rp_pop(&op2);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                if(op1 == 0.0)
                {
                    printf("modular zero!\n");
                }
                
                value = fmod(op2, op1);
                ret_val = s_calc_rp_push(value);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                break;
                
            case CALC_WORD_TYPE_EMPTY:
                ret_val = s_calc_rp_pop(&value);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                ret_val = s_calc_rp_pop(&value);
                R_ASSERT(ret_val == RETURN_FAILURE, RETURN_FAILURE);
                
                * result = value;
                return RETURN_SUCCESS;
                break;
                
            default:
                printf("unknown type: %d\n", word_type);
                break;
        }
    }

    return RETURN_SUCCESS;
}

ENUM_RETURN s_calc_rp(const _S8 *str, _SD *result)
{
    R_ASSERT(str != NULL, RETURN_FAILURE);
    R_ASSERT(result != NULL, RETURN_FAILURE);

    ENUM_RETURN calc_ret, ret_val = s_calc_rp_init();
    R_ASSERT(ret_val != RETURN_FAILURE, RETURN_FAILURE);

    calc_ret = s_calc_rp_do(str, result);
    if(calc_ret == RETURN_FAILURE)
    {
        printf("expression error!\n"); 
        SET_VAR_VALUE(*result, DOUBLE_NAN_PLUS);
        R_LOG("s_calc_rp_do failed!");
    }

    ret_val = s_calc_rp_clear();
    R_ASSERT(ret_val != RETURN_FAILURE, RETURN_FAILURE);
    return calc_ret;
}

