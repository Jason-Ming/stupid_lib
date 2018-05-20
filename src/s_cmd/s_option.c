#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "s_log.h"
#include "s_option.h"
#include "s_subcmd.h"
#include "s_input_file.h"
#include "s_error.h"

#define FREE(p) do{ if(p != NULL){free(p);}}while(0);

PRIVATE void debug_print_option_cb(STRU_OPTION_CONTROL_BLOCK *p)
{
    R_LOG("%s = %s, %s = %s, %s = %d, %s = %d, %s = %p, %s = %s\n", 
        "subcmd", p->subcmd,
        "option", p->option,
        "option type", p->option_type,
        "arg type", p->arg_type,
        "handler", p->handler,
        "help_info", p->help_info);
}

void debug_print_option_cb_list(STRU_OPTION_CONTROL_BLOCK *p)
{
    while(p != NULL)
    {
        debug_print_option_cb(p);

        p = p->next;
    }
}

PRIVATE ENUM_RETURN get_a_new_option_cb_do(STRU_OPTION_CONTROL_BLOCK **pp_new, 
    const char* subcmd_name,
    const char* option_name, 
    ENUM_OPTION_TYPE option_type,
    ENUM_ARG_TYPE arg_type,
    FUNC_OPTION_PROC handler, 
    const char* help_info)
{
    R_ASSERT(pp_new != NULL, RETURN_FAILURE);
    
    *pp_new = (STRU_OPTION_CONTROL_BLOCK*)malloc(sizeof(STRU_OPTION_CONTROL_BLOCK));
    R_ASSERT(*pp_new != NULL, RETURN_FAILURE);

    (*pp_new)->subcmd = NULL;
    (*pp_new)->option = NULL;
    (*pp_new)->option_type = option_type;
    (*pp_new)->arg_type = arg_type;
    (*pp_new)->handler = handler;
    (*pp_new)->help_info = NULL;
    (*pp_new)->next = NULL;

    (*pp_new)->subcmd = (char*)malloc(strlen(subcmd_name) + 1);
    R_ASSERT((*pp_new)->subcmd != NULL, RETURN_FAILURE);
    strcpy((*pp_new)->subcmd, subcmd_name);

    (*pp_new)->option = (char*)malloc(strlen(option_name) + 1);
    R_ASSERT((*pp_new)->option != NULL, RETURN_FAILURE);
    strcpy((*pp_new)->option, option_name);
    
    (*pp_new)->help_info = (char*)malloc(strlen(help_info) + 1);
    R_ASSERT((*pp_new)->help_info != NULL, RETURN_FAILURE);
    strcpy((*pp_new)->help_info, help_info);

    return RETURN_SUCCESS;
}

PRIVATE void get_a_new_option_cb_do_error(STRU_OPTION_CONTROL_BLOCK *p_new)
{
    if(p_new == NULL)
    {
        return;
    }
    FREE(p_new->subcmd);
    FREE(p_new->option);
    FREE(p_new->help_info);
    free(p_new);
}

PRIVATE STRU_OPTION_CONTROL_BLOCK *get_a_new_option_cb(
    const char* subcmd_name,
    const char* option_name, 
    ENUM_OPTION_TYPE option_type,
    ENUM_ARG_TYPE arg_type,
    FUNC_OPTION_PROC handler, 
    const char* help_info)
{
    ENUM_RETURN ret_val;
    STRU_OPTION_CONTROL_BLOCK *p_new = NULL;
    ret_val = get_a_new_option_cb_do(&p_new, subcmd_name, option_name, option_type, arg_type, handler, help_info);
    R_ASSERT_DO(ret_val == RETURN_SUCCESS, NULL, get_a_new_option_cb_do_error(p_new));

    return p_new;
}

ENUM_BOOLEAN is_option_name_valid(const char* option_name)
{
    int position = 0;

    R_ASSERT(option_name[position] == '-', BOOLEAN_FALSE);
    position++;
    
    while(option_name[position] != '\0')
    {
        R_ASSERT(IS_ALPHABET(option_name[position]) == BOOLEAN_TRUE, BOOLEAN_FALSE);
        position++;
    }

    R_ASSERT(position > 1, BOOLEAN_FALSE);
    
    return BOOLEAN_TRUE;
}

ENUM_BOOLEAN is_option_type_valid(ENUM_OPTION_TYPE type)
{
    return (0 <= type && type < OPTION_TYPE_MAX)?BOOLEAN_TRUE:BOOLEAN_FALSE;
}

ENUM_BOOLEAN is_arg_type_valid(ENUM_ARG_TYPE type)
{
    return (0 <= type && type < ARG_TYPE_MAX)?BOOLEAN_TRUE:BOOLEAN_FALSE;
}

ENUM_RETURN register_option(
    const char* subcmd_name,
    const char* option_name,
    ENUM_OPTION_TYPE option_type,
    ENUM_ARG_TYPE arg_type,
    FUNC_OPTION_PROC handler, 
    const char* help_info)
{
    R_ASSERT(option_name != NULL, RETURN_FAILURE);
    R_ASSERT_LOG(BOOLEAN_TRUE == is_subcmd_registered(subcmd_name), RETURN_FAILURE, "subcmd: %s", subcmd_name);
    R_ASSERT_LOG(BOOLEAN_TRUE == is_option_name_valid(option_name), RETURN_FAILURE, "option: %s", option_name);
    R_ASSERT_LOG(BOOLEAN_TRUE == is_option_type_valid(option_type), RETURN_FAILURE, "option_type: %d", option_type);
    R_ASSERT_LOG(BOOLEAN_TRUE == is_arg_type_valid(arg_type), RETURN_FAILURE, "arg_type: %d", arg_type);
    R_ASSERT(handler != NULL, RETURN_FAILURE);
    R_ASSERT(help_info != NULL, RETURN_FAILURE);

    STRU_OPTION_CONTROL_BLOCK *p_new = NULL;
    p_new = get_a_new_option_cb(subcmd_name, option_name, option_type, arg_type, handler, help_info);
    R_ASSERT(p_new != NULL, RETURN_FAILURE);

    ENUM_RETURN ret_val;
    ret_val = add_a_new_option_cb_to_subcmd_cb(p_new);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    debug_print_option_cb(p_new);

    return RETURN_SUCCESS;
}

PRIVATE STRU_OPTION_CONTROL_BLOCK *get_option_cb_by_name(const char* subcmd_name, const char* option_name)
{
    STRU_OPTION_CONTROL_BLOCK *p = get_option_cb_list_head(subcmd_name);

    while(p != NULL)
    {
        if(strcmp(option_name, p->option) == 0)
        {
            return p;
        }
        p = p->next;
    }

    return NULL;
}

PRIVATE ENUM_RETURN get_a_new_option_rb_do(STRU_OPTION_RUN_BLOCK **pp_new)
{
    R_ASSERT(pp_new != NULL, RETURN_FAILURE);
    
    *pp_new = (STRU_OPTION_RUN_BLOCK*)malloc(sizeof(STRU_OPTION_RUN_BLOCK));
    R_ASSERT(*pp_new != NULL, RETURN_FAILURE);

    (*pp_new)->subcmd = NULL;
    (*pp_new)->option = NULL;
    (*pp_new)->arg = NULL;
    (*pp_new)->next = NULL;

    return RETURN_SUCCESS;
}

PRIVATE void get_a_new_option_rb_do_error(STRU_OPTION_RUN_BLOCK *p_new)
{
    if(p_new == NULL)
    {
        return;
    }

    free(p_new);
}

PRIVATE STRU_OPTION_RUN_BLOCK *get_a_new_option_rb(void)
{
    ENUM_RETURN ret_val;
    STRU_OPTION_RUN_BLOCK *p_new = NULL;
    ret_val = get_a_new_option_rb_do(&p_new);
    R_ASSERT_DO(ret_val == RETURN_SUCCESS, NULL, get_a_new_option_rb_do_error(p_new));

    return p_new;
}

PRIVATE STRU_OPTION_RUN_BLOCK *get_option_rb_by_name(const char* subcmd_name, const char* option_name)
{
    STRU_OPTION_RUN_BLOCK *p = get_option_rb_list_head(subcmd_name);

    while(p != NULL)
    {
        if(strcmp(option_name, p->option) == 0)
        {
            return p;
        }
        p = p->next;
    }

    return NULL;
}

PRIVATE ENUM_RETURN add_arg_to_option_rb(STRU_OPTION_RUN_BLOCK *p, const char *value)
{
    STRU_ARG* temp_arg = (STRU_ARG*)malloc(sizeof(STRU_ARG));
    R_ASSERT(temp_arg != NULL, RETURN_FAILURE);

    temp_arg->value = (char *)malloc(strlen(value) + 1);
    R_ASSERT(temp_arg->value != NULL, RETURN_FAILURE);
    
    strcpy(temp_arg->value, value);

    temp_arg->next = NULL;

    if(p->arg != NULL)
    {
        p->arg->next = temp_arg;
    }
    else
    {
        p->arg = temp_arg;
    }

    return RETURN_SUCCESS;
}

/* ��ѡ�ֵ�������� */
ENUM_RETURN parse_options(int argc, char **argv)
{
    STRU_OPTION_CONTROL_BLOCK *p_cb = NULL;
    STRU_OPTION_RUN_BLOCK *p_rb = NULL;
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    const char *current_subcmd_name = NULL;
    
    int i = get_argv_indicator();
    
    current_subcmd_name = get_current_subcmd_name();
    R_ASSERT(current_subcmd_name != NULL, RETURN_FAILURE);
    

    while(i < argc)
    {
        // ��ǰoption�Ƿ��ڿ��ƿ���ע���
        p_cb = get_option_cb_by_name(current_subcmd_name, argv[i]);
        if(p_cb == NULL)
        {
            ret_val = add_current_system_error(ERROR_CODE_UNKONWN_OPTION, argv[i]);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

            break;   
        }

        p_rb = get_option_rb_by_name(current_subcmd_name, p_cb->option);

        /* ��ǰoption�Ѿ������ */
        if(p_rb != NULL)
        {
            ret_val = add_current_system_error(ERROR_CODE_REPETITIVE_OPTION, p_cb->option);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            
            break;
        }
        else
        {
            p_rb = get_a_new_option_rb();
            R_ASSERT(p_rb != NULL, RETURN_FAILURE);
            
            p_rb->subcmd = current_subcmd_name;
            p_rb->option = p_cb->option;
        }

        if(p_cb->arg_type == ARG_TYPE_SWITCH)
        {
            ret_val = add_arg_to_option_rb(p_rb, "enable");
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            i++;
        }
        else
        {
            i++;
            
            while(i < argc)
            {
                if(argv[i][0] == '-')
                {
                    break;
                }

                ret_val = add_arg_to_option_rb(p_rb, argv[i]);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                i++;
            }
        }

        if(p_rb->arg == NULL)
        {
            ret_val = add_current_system_error(ERROR_CODE_MISSING_ARGS, p_cb->option);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }

        ret_val = add_a_new_option_rb_to_subcmd_rb(p_rb);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

        //i++;
    }

    set_argv_indicator(i);
    
    return RETURN_SUCCESS;
}

/* �������ѡ�ֵ ��������һ��optionʧ�ܣ�����������ʧ�� */
ENUM_RETURN process_options(STRU_OPTION_RUN_BLOCK *p_option_rb)
{
    ENUM_RETURN ret_val;
    while(p_option_rb != NULL)
    {
        STRU_OPTION_CONTROL_BLOCK *p_option_cb = NULL;
        p_option_cb = get_option_cb_by_name(p_option_rb->subcmd, p_option_rb->option);
        R_ASSERT(p_option_cb != NULL, RETURN_FAILURE);
        
        ret_val = p_option_cb->handler(p_option_rb->arg);
        R_FALSE_RET_LOG(ret_val == RETURN_SUCCESS, RETURN_FAILURE,"process option [%s] failed!\n", p_option_rb->option);

        p_option_rb = p_option_rb->next;
    }

    return RETURN_SUCCESS;
}

