#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "s_log.h"
#include "s_option.h"
#include "s_input_file.h"
#include "s_error.h"

#define FREE(p) do{ if(p != NULL){free(p);}}while(0);

typedef struct TAG_STRU_OPTION_PROC
{
    char* option;
    ENUM_OPTION_TYPE option_type;
    ENUM_BOOLEAN need_process;
    struct TAG_STRU_ARG *arg;
    ENUM_ARG_TYPE arg_type;
    FUNC_OPTION_PROC handler;
    char* help_info;
    struct TAG_STRU_OPTION_PROC *next;
} STRU_OPTION_PROC;

PRIVATE STRU_OPTION_PROC *p_list_head = NULL;
PRIVATE STRU_OPTION_PROC *p_list_tail = NULL;

PRIVATE STRU_OPTION_PROC *get_option_proc_list_head(void)
{
    return p_list_head;
}

PRIVATE void debug_print_option_proc_block(STRU_OPTION_PROC *p)
{
    R_LOG("%s = %s, %s = %d, %s = %s, %s = %d, %s = %p, %s = %s\n", 
        "option", p->option,
        "option type", p->option_type,
        "arg", p->arg!= NULL? p->arg->value: "NULL",
        "arg type", p->arg_type,
        "handler", p->handler,
        "help_info", p->help_info);
}

PRIVATE void debug_print_option_proc_list(void)
{
    STRU_OPTION_PROC *p = get_option_proc_list_head();

    while(p != NULL)
    {
        debug_print_option_proc_block(p);

        p = p->next;
    }
}

PRIVATE ENUM_RETURN add_node_to_option_proc_list(STRU_OPTION_PROC *p_new)
{
    R_ASSERT(p_new != NULL, RETURN_FAILURE);

    if(p_list_tail == NULL)
    {
        p_list_head = p_list_tail = p_new;
    }
    else
    {
        p_list_tail->next = p_new;
        p_list_tail = p_new;
    }
    
    return RETURN_SUCCESS;
}



PRIVATE ENUM_RETURN get_a_new_node_do(STRU_OPTION_PROC **pp_new, 
    const char* option_name, 
    ENUM_OPTION_TYPE option_type,
    ENUM_ARG_TYPE arg_type,
    FUNC_OPTION_PROC handler, 
    const char* help_info)
{
    R_ASSERT(pp_new != NULL, RETURN_FAILURE);
    
    *pp_new = (STRU_OPTION_PROC*)malloc(sizeof(STRU_OPTION_PROC));
    R_ASSERT(*pp_new != NULL, RETURN_FAILURE);

    (*pp_new)->option = NULL;
    (*pp_new)->arg = NULL;
    (*pp_new)->need_process = BOOLEAN_FALSE;
    (*pp_new)->handler = handler;
    (*pp_new)->help_info = NULL;
    (*pp_new)->next = NULL;

    (*pp_new)->option = (char*)malloc(strlen(option_name) + 1);
    R_ASSERT((*pp_new)->option != NULL, RETURN_FAILURE);
    strcpy((*pp_new)->option, option_name);

    (*pp_new)->option_type = option_type;

    (*pp_new)->arg = NULL;
    (*pp_new)->arg_type = arg_type;
    
    (*pp_new)->help_info = (char*)malloc(strlen(help_info) + 1);
    R_ASSERT((*pp_new)->help_info != NULL, RETURN_FAILURE);
    strcpy((*pp_new)->help_info, help_info);

    return RETURN_SUCCESS;
}

PRIVATE void get_a_new_node_do_error(STRU_OPTION_PROC *p_new)
{
    if(p_new == NULL)
    {
        return;
    }
    
    FREE(p_new->option);
    FREE(p_new->help_info);
    free(p_new);
}

PRIVATE STRU_OPTION_PROC *get_a_new_option_proc_node(
    const char* option_name, 
    ENUM_OPTION_TYPE option_type,
    ENUM_ARG_TYPE arg_type,
    FUNC_OPTION_PROC handler, 
    const char* help_info)
{
    ENUM_RETURN ret_val;
    STRU_OPTION_PROC *p_new = NULL;
    ret_val = get_a_new_node_do(&p_new, option_name, option_type, arg_type, handler, help_info);
    R_ASSERT_DO(ret_val == RETURN_SUCCESS, NULL, get_a_new_node_do_error(p_new));

    return p_new;
}

ENUM_BOOLEAN is_option_valid(const char* option_name)
{
    int position = 0;

    R_ASSERT(option_name[position] == '-', BOOLEAN_FALSE);
    position++;
    
    while(option_name[position] != '\0')
    {
        R_ASSERT(IS_ALPHABET(option_name[position]) == BOOLEAN_TURE, BOOLEAN_FALSE);
        position++;
    }
    
    return BOOLEAN_TURE;
}

ENUM_BOOLEAN is_option_type_valid(ENUM_OPTION_TYPE type)
{
    return (0 <= type && type < OPTION_TYPE_MAX)?BOOLEAN_TURE:BOOLEAN_FALSE;
}

ENUM_BOOLEAN is_arg_type_valid(ENUM_ARG_TYPE type)
{
    return (0 <= type && type < ARG_TYPE_MAX)?BOOLEAN_TURE:BOOLEAN_FALSE;
}

ENUM_RETURN register_option(
    const char* option_name, 
    ENUM_OPTION_TYPE option_type,
    ENUM_ARG_TYPE arg_type,
    FUNC_OPTION_PROC handler, 
    const char* help_info)
{
    R_ASSERT(option_name != NULL, RETURN_FAILURE);
    R_ASSERT_LOG(BOOLEAN_TURE == is_option_valid(option_name), RETURN_FAILURE, "option: %s", option_name);
    R_ASSERT_LOG(BOOLEAN_TURE == is_option_type_valid(option_type), RETURN_FAILURE, "option_type: %d", option_type);
    R_ASSERT_LOG(BOOLEAN_TURE == is_arg_type_valid(arg_type), RETURN_FAILURE, "arg_type: %d", arg_type);
    R_ASSERT(handler != NULL, RETURN_FAILURE);
    R_ASSERT(help_info != NULL, RETURN_FAILURE);

    STRU_OPTION_PROC *p_new = NULL;
    p_new = get_a_new_option_proc_node(option_name, option_type, arg_type, handler, help_info);
    R_ASSERT(p_new != NULL, RETURN_FAILURE);

    ENUM_RETURN ret_val;
    ret_val = add_node_to_option_proc_list(p_new);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    debug_print_option_proc_block(p_new);

    return RETURN_SUCCESS;
}

PRIVATE STRU_OPTION_PROC *get_option_proc_block(char* option)
{
    STRU_OPTION_PROC *p = get_option_proc_list_head();

    while(p != NULL)
    {    
        if(strcmp(option, p->option) == 0)
        {
            return p;
        }
        p = p->next;
    }

    return NULL;
}
PRIVATE ENUM_RETURN add_option_arg(STRU_OPTION_PROC *p, const char *value)
{
    STRU_ARG* temp_value = (STRU_ARG*)malloc(sizeof(STRU_ARG));
    R_ASSERT(temp_value != NULL, RETURN_FAILURE);

    temp_value->value = (char *)malloc(strlen(value) + 1);
    R_ASSERT(temp_value->value != NULL, RETURN_FAILURE);
    
    strcpy(temp_value->value, value);

    temp_value->next = NULL;

    if(p->arg != NULL)
    {
        p->arg->next = temp_value;
    }
    else
    {
        p->arg = temp_value;
    }

    return RETURN_SUCCESS;
}

/* 将选项及值处理并保存 */
ENUM_RETURN parse_options(int argc, char **argv)
{
    STRU_OPTION_PROC *p = NULL;
    ENUM_RETURN ret_val;
    int i = 1 + get_input_file_num();

    while(i < argc)
    {
        p = get_option_proc_block(argv[i]);
        if(p == NULL)
        {
            ret_val = add_current_error(ERROR_CODE_UNKONWN_OPTION, argv[i]);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            i++;
            continue;   
        }

        p->need_process = BOOLEAN_TURE;

        if(p->arg != NULL)
        {
            ret_val = add_current_error(ERROR_CODE_REPETITIVE_OPTION, p->option);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        }

        if(p->arg_type == ARG_TYPE_SWITCH)
        {            
            ret_val = add_option_arg(p, "enable");
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            i++;
            continue;
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
                
                ret_val = add_option_arg(p, argv[i]);
                R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
                i++;
            }
        }

        if(p->arg == NULL)
        {
            ret_val = add_current_error(ERROR_CODE_MISSING_ARGS, p->option);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        }

    }
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_BOOLEAN is_option_need_process(STRU_OPTION_PROC *p)
{
    return p->need_process;
}

void display_option_help_info(void)
{
    STRU_OPTION_PROC *p = get_option_proc_list_head();

    while(p != NULL)
    {
        printf("    %10s: %s\n", p->option, p->help_info);

        p = p->next;
    }
}

/* 处理保存的选项及值 */
ENUM_RETURN process_options(void)
{
    STRU_OPTION_PROC *p = get_option_proc_list_head();
    ENUM_RETURN ret_val;
    while(p != NULL)
    {
        if(is_option_need_process(p) == BOOLEAN_TURE)
        {
            ret_val = p->handler(p->arg);
            R_FALSE_LOG(ret_val == RETURN_SUCCESS, "process option [%s] failed!\n", p->option);
        };

        p = p->next;
    }

    return RETURN_SUCCESS;
}

