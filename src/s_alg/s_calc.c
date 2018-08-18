#include <string.h>
#include <ctype.h>
#include <math.h>

#include "s_stack.h"
#include "s_log.h"
#include "s_alg.h"
#include "s_text.h"
#include "s_mem.h"
#include "s_limits.h"

#define ERROR_PRINT(fmt, args...)\
    printf("Error!! "fmt, ##args);

#define MAX_LEN_OF_WORD 65

typedef enum TAG_ENUM_CALC_WORD_TYPE
{
    CALC_WORD_TYPE_NUMBER = 0,
    CALC_WORD_TYPE_VAR,
    CALC_WORD_TYPE_VAR_ASSIGN,
    CALC_WORD_TYPE_ADD,
    CALC_WORD_TYPE_SUBTRACT,
    CALC_WORD_TYPE_MULTIPLY,
    CALC_WORD_TYPE_DIVIDE,
    CALC_WORD_TYPE_MODULAR,
    CALC_WORD_TYPE_SIN,
    CALC_WORD_TYPE_COS,
    CALC_WORD_TYPE_POW,
    CALC_WORD_TYPE_EXP,
    CALC_WORD_TYPE_EMPTY,
    CALC_WORD_TYPE_UNKNOWN,
}ENUM_CALC_WORD_TYPE;

#define VAR_NAME_LEN 33
#define MAX_VAR 64
typedef struct TAG_STRU_CALC_VAR
{
    _S8 name[VAR_NAME_LEN];
    _SD value;
}STRU_CALC_VAR;

PRIVATE STACK s_calc_stack;
PRIVATE STRU_CALC_VAR vars[MAX_VAR];
PRIVATE _S32 current_idle_var_index = 0;
PRIVATE STRU_CALC_VAR last_var;

PRIVATE _S8 *s_calc_get_last_var(_VOID)
{
    return last_var.name;
}

PRIVATE _S32 s_calc_get_var_index(_S8 *var)
{
    _S32 i = 0;
    for(; i < MAX_VAR; i++)
    {
        if(strcmp(var, vars[i].name) == 0)
        {
            return i;
        }
    }

    return S32_INVALID;
}

PRIVATE ENUM_RETURN s_calc_save_var(_S8 *var, _SD value)
{
    if(current_idle_var_index >= MAX_VAR)
    {
        ERROR_PRINT("Too more variables! '%s'\n", var);
        return RETURN_FAILURE;
    }

    strncpy(vars[current_idle_var_index].name, var, VAR_NAME_LEN);
    vars[current_idle_var_index].name[VAR_NAME_LEN - 1] = '\0';
    vars[current_idle_var_index].value = value;
    current_idle_var_index++;

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_calc_set_var_value(_S8* var, _SD value)
{
    ENUM_RETURN ret_val;
    if(strlen(var) == 0)
    {
        ret_val = RETURN_FAILURE;
    }
    
    _S32 i = s_calc_get_var_index(var);
    if(i == S32_INVALID)
    {
        ret_val = RETURN_FAILURE;
    }
    else
    {
        vars[i].value = value;
        ret_val =  RETURN_SUCCESS;
    }

    if(ret_val == RETURN_FAILURE)
    {
        ERROR_PRINT("The variable should be operated was not existed!\n");
    }

    return ret_val;
}

PRIVATE ENUM_RETURN s_calc_get_var_value(_S8 *var, _SD *value)
{
    *value = 0.0;
    ENUM_RETURN ret_val;
    if(strlen(var) > VAR_NAME_LEN - 1)
    {
        ERROR_PRINT("The variable '%s''s length was exceeded!\n", var);
        return RETURN_FAILURE;
    }
    
    _S32 i = s_calc_get_var_index(var);
    if(i == S32_INVALID)
    {
        ret_val = s_calc_save_var(var, 0.0);
    }
    else
    {
        *value = vars[i].value;
        ret_val = RETURN_SUCCESS;
    }

    if(ret_val == RETURN_SUCCESS)
    {
        strncpy(last_var.name, var, VAR_NAME_LEN);
        last_var.name[VAR_NAME_LEN - 1] = '\0';
        last_var.value = *value;
    }

    return ret_val;
}

PRIVATE _VOID s_calc_set_var_value_to_default(_VOID)
{
    for(_S32 i = 0; i < MAX_VAR; i++)
    {
        vars[i].name[0] = '\0';
        vars[i].value = 0.0;
    }

    last_var.name[0] = '\0';
    last_var.value = 0.0;

    current_idle_var_index = 0;
}

PRIVATE ENUM_RETURN s_calc_rp_init(_VOID)
{
    ENUM_RETURN ret_val = stack_create(&s_calc_stack);
    R_ASSERT(ret_val != RETURN_FAILURE, RETURN_FAILURE);

    s_calc_set_var_value_to_default();
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_calc_rp_clear(_VOID)
{
    //stack_print(s_calc_stack);
    ENUM_RETURN ret_val = stack_delete(&s_calc_stack);
    R_ASSERT(ret_val != RETURN_FAILURE, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_calc_rp_get_word_type_and_value(_S8 *word, ENUM_CALC_WORD_TYPE *type, _SD *value)
{
    R_ASSERT(word != NULL, RETURN_FAILURE);
    R_ASSERT(value != NULL, RETURN_FAILURE);

    ENUM_RETURN ret_val = RETURN_SUCCESS;
    ENUM_CALC_WORD_TYPE temp_type = CALC_WORD_TYPE_UNKNOWN;
    _SD temp_value = 0.0;
    
    size_t word_len = strlen(word);
    if(word_len == 0)
    {
        temp_type = CALC_WORD_TYPE_EMPTY;
    }
    else if(strlen(word) == 1 && !isdigit(word[0]) && !isalpha(word[0]))
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
            case '=':
                temp_type = CALC_WORD_TYPE_VAR_ASSIGN;
                break;
            default:
                temp_type = CALC_WORD_TYPE_UNKNOWN;
                break;
        }
                
    }
    else if(isalpha(word[0]))
    {
        if(0 == strcmp(word, "sin"))
        {
            temp_type = CALC_WORD_TYPE_SIN;
        }
        else if(0 == strcmp(word, "cos"))
        {
            temp_type = CALC_WORD_TYPE_COS;
        }
        else if(0 == strcmp(word, "pow"))
        {
            temp_type = CALC_WORD_TYPE_POW;
        }
        else if(0 == strcmp(word, "exp"))
        {
            temp_type = CALC_WORD_TYPE_EXP;
        }
        else
        {
            temp_type = CALC_WORD_TYPE_VAR;
            ret_val = s_calc_get_var_value(word, &temp_value);
            
        }
    }
    else
    {
        ret_val = s_strtosd(word, &temp_value);
        if(ret_val == RETURN_SUCCESS)
        {
            temp_type = CALC_WORD_TYPE_NUMBER;
        }
        else
        {
            temp_type = CALC_WORD_TYPE_UNKNOWN;
        }
    }

    if(temp_type == CALC_WORD_TYPE_UNKNOWN)
    {
        ERROR_PRINT("'%s' couldn't be parsed!\n", word);
        ret_val = RETURN_FAILURE;
    }
    
    *type = temp_type;
    *value = temp_value;
    
    return ret_val;
}

PRIVATE ENUM_RETURN s_calc_rp_push(_SD value)
{
    DEBUG_PRINT("push: %g", value);
    return stack_push(s_calc_stack, (_VOID*)&value, sizeof(value));
}

PRIVATE ENUM_RETURN s_calc_rp_pop(_SD *value)
{
    size_t size_out;
    ENUM_RETURN ret_val = stack_pop(s_calc_stack, (_VOID *)value, &size_out,  sizeof(*value));
    DEBUG_PRINT("pop: %g", *value);

    return ret_val;
}

PRIVATE ENUM_RETURN s_calc_rp_do(const _S8 * str, _SD * result)
{
    PRIVATE _S8 word[MAX_LEN_OF_WORD];
    size_t word_len = 0;
    ENUM_CALC_WORD_TYPE word_type;
    _SD value, op1, op2;
    ENUM_RETURN ret_val;
    s_set_separators(NULL);
    
    for(;;)
    {
        //get a word
        ret_val = s_get_word_s(&str, word, MAX_LEN_OF_WORD, &word_len);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

        DEBUG_PRINT("word: %s", word);
        
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
                    ERROR_PRINT("Devide zero!\n");
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
                    ERROR_PRINT("Modular zero!\n");
                }
                
                value = fmod(op2, op1);
                ret_val = s_calc_rp_push(value);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                break;
                
            case CALC_WORD_TYPE_SIN:
                ret_val = s_calc_rp_pop(&op1);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                
                value = sin(op1);
                ret_val = s_calc_rp_push(value);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                break;
            
            case CALC_WORD_TYPE_COS:
                ret_val = s_calc_rp_pop(&op1);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                
                value = cos(op1);
                ret_val = s_calc_rp_push(value);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                break;
                
            case CALC_WORD_TYPE_EXP:
                ret_val = s_calc_rp_pop(&op1);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                
                value = exp(op1);
                ret_val = s_calc_rp_push(value);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                break;

            case CALC_WORD_TYPE_POW:
                ret_val = s_calc_rp_pop(&op1);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                ret_val = s_calc_rp_pop(&op2);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                value = pow(op2, op1);
                ret_val = s_calc_rp_push(value);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                break;
                
            case CALC_WORD_TYPE_VAR:
                ret_val = s_calc_rp_push(value);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                break;

            case CALC_WORD_TYPE_VAR_ASSIGN:
                ret_val = s_calc_rp_pop(&op1);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                ret_val = s_calc_rp_pop(&op2);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                
                ret_val = s_calc_set_var_value(s_calc_get_last_var(), op2);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

                ret_val = s_calc_rp_push(op2);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                break;

            case CALC_WORD_TYPE_EMPTY:
                ret_val = s_calc_rp_pop(&value);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

                /* this pop must be failed, or the expression is error */
                ret_val = s_calc_rp_pop(&value);
                R_ASSERT(ret_val == RETURN_FAILURE, RETURN_FAILURE);
                
                * result = value;
                return RETURN_SUCCESS;
                break;
                
            default:
                return RETURN_FAILURE;
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
        ERROR_PRINT("Expression error!\n"); 
        SET_VAR_VALUE(*result, DOUBLE_NAN_PLUS);
        S_LOG("s_calc_rp_do failed!");
    }

    ret_val = s_calc_rp_clear();
    R_ASSERT(ret_val != RETURN_FAILURE, RETURN_FAILURE);
    return calc_ret;
}

