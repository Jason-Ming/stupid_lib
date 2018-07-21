#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "s_log.h"
#include "s_text.h"
#include "s_mem.h"
#include "s_cmd.h"
#include "s_cmd_proc.h"

#include "s_subcmd.h"
#include "s_input_file.h"
#include "s_error.h"

#include "s_option.h"

PRIVATE void debug_print_option_cb(STRU_OPTION_CONTROL_BLOCK *p)
{
    printf("%s = %s, %s = %s, %s = %d, %s = %d, %s = %d, %s = %s, %s = %p, %s = %d, %s = %s\n", 
        "subcmd", p->subcmd,
        "option", p->option,
        "is_running", p->is_running,
        "option type", p->option_type,
        "arg type", p->arg_type,
        "arg value", p->arg_value,
        "handler", p->handler,
        "finish_handle", p->finish_handle,
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
    ENUM_BOOLEAN finish_handle,
    const char* help_info)
{
    R_ASSERT(pp_new != NULL, RETURN_FAILURE);
    
    *pp_new = (STRU_OPTION_CONTROL_BLOCK*)malloc(sizeof(STRU_OPTION_CONTROL_BLOCK));
    R_ASSERT(*pp_new != NULL, RETURN_FAILURE);

    (*pp_new)->subcmd = NULL;
    (*pp_new)->option = NULL;
    (*pp_new)->is_running = BOOLEAN_FALSE;
    (*pp_new)->option_type = option_type;
    (*pp_new)->arg_type = arg_type;
    (*pp_new)->arg_value = NULL;
    (*pp_new)->handler = handler;
    (*pp_new)->finish_handle = finish_handle;
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
    ENUM_BOOLEAN finish_handle,
    const char* help_info)
{
    ENUM_RETURN ret_val;
    STRU_OPTION_CONTROL_BLOCK *p_new = NULL;
    ret_val = get_a_new_option_cb_do(&p_new, 
        subcmd_name, 
        option_name, 
        option_type, 
        arg_type, 
        handler, 
        finish_handle,
        help_info);
    R_ASSERT_DO(ret_val == RETURN_SUCCESS, NULL, get_a_new_option_cb_do_error(p_new));

    return p_new;
}

ENUM_BOOLEAN whether_string_is_in_option_format(const char *option_name)
{
    int position = 0;

    R_FALSE_RET(option_name[position] == '-', BOOLEAN_FALSE);
    position++;
    
    while(option_name[position] != '\0')
    {
        if(IS_ALPHABET(option_name[position]) == BOOLEAN_FALSE)
        {
            break;
        }
        
        position++;
    }

    R_FALSE_RET(position > 1, BOOLEAN_FALSE);
    
    return BOOLEAN_TRUE;
}
PRIVATE ENUM_BOOLEAN whether_option_name_is_valid(const char* option_name)
{
    R_ASSERT(whether_string_is_in_option_format(option_name) == BOOLEAN_TRUE, BOOLEAN_FALSE);

    return BOOLEAN_TRUE;
}

PRIVATE ENUM_BOOLEAN whether_option_type_is_valid(ENUM_OPTION_TYPE type)
{
    return (0 <= type && type < OPTION_TYPE_MAX)?BOOLEAN_TRUE:BOOLEAN_FALSE;
}

PRIVATE ENUM_BOOLEAN whether_arg_type_is_valid(ENUM_ARG_TYPE type)
{
    return (0 <= type && type < ARG_TYPE_MAX)?BOOLEAN_TRUE:BOOLEAN_FALSE;
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

PRIVATE ENUM_BOOLEAN whether_option_has_been_registered(const char* subcmd_name, const char* option_name)
{
    return get_option_cb_by_name(subcmd_name, option_name) == NULL?BOOLEAN_FALSE:BOOLEAN_TRUE;
}

PRIVATE ENUM_RETURN set_current_running_option(const char* subcmd_name, const char* option_name)
{
    STRU_OPTION_CONTROL_BLOCK *p = get_option_cb_by_name(subcmd_name, option_name);
    R_ASSERT(p != NULL, RETURN_FAILURE);
    p->is_running = BOOLEAN_TRUE;

    return RETURN_SUCCESS;
}

PRIVATE STRU_OPTION_CONTROL_BLOCK *get_current_running_option_cb(const char* subcmd_name, const char* option_name)
{
    STRU_OPTION_CONTROL_BLOCK *p = get_option_cb_by_name(subcmd_name, option_name);
    R_ASSERT(p != NULL, NULL);
    
    R_FALSE_RET(p->is_running == BOOLEAN_TRUE, NULL);
    
    return p;
}

_S32 get_current_running_option_number(const char* subcmd_name)
{
    _S32 num = 0;
    STRU_OPTION_CONTROL_BLOCK *p = get_option_cb_list_head(subcmd_name);

    while(p != NULL)
    {
        if(p->is_running == BOOLEAN_TRUE)
        {
            num++;
        }
        p = p->next;
    }

    return num;
}

PRIVATE ENUM_RETURN get_current_running_option_arg_type(const char* subcmd_name, const char* option_name, ENUM_ARG_TYPE *arg_type)
{
    R_ASSERT(arg_type != NULL, RETURN_FAILURE);
    STRU_OPTION_CONTROL_BLOCK *p = get_current_running_option_cb(subcmd_name, option_name);
    R_ASSERT(p != NULL, RETURN_FAILURE);

    *arg_type = p->arg_type;

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN add_arg_to_current_running_option(const char* subcmd_name, const char* option_name, const char *arg)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    if(arg == NULL)
    {
        ret_val = generate_system_error(ERROR_CODE_MISSING_ARGS, option_name);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        return RETURN_SUCCESS;
    }
        
    STRU_OPTION_CONTROL_BLOCK *p = get_option_cb_by_name(subcmd_name, option_name);
    R_ASSERT(p != NULL, RETURN_FAILURE);

    p->arg_value = (char *)malloc(strlen(arg) + 1);
    R_ASSERT(p->arg_value != NULL, RETURN_FAILURE);
    
    strcpy(p->arg_value, arg);

    return RETURN_SUCCESS;
}

ENUM_RETURN register_option(
    const char* subcmd_name,
    const char* option_name,
    ENUM_OPTION_TYPE option_type,
    ENUM_ARG_TYPE arg_type,
    FUNC_OPTION_PROC handler, 
    ENUM_BOOLEAN finish_handle,
    const char* help_info)
{
    R_ASSERT(option_name != NULL, RETURN_FAILURE);
    R_ASSERT_LOG(BOOLEAN_TRUE == whether_subcmd_has_been_registered(subcmd_name), RETURN_FAILURE, "subcmd: %s", subcmd_name);
    R_ASSERT_LOG(BOOLEAN_TRUE == whether_option_name_is_valid(option_name), RETURN_FAILURE, "option: %s", option_name);
    R_ASSERT_LOG(BOOLEAN_TRUE == whether_option_type_is_valid(option_type), RETURN_FAILURE, "option_type: %d", option_type);
    R_ASSERT_LOG(BOOLEAN_TRUE == whether_arg_type_is_valid(arg_type), RETURN_FAILURE, "arg_type: %d", arg_type);
    R_ASSERT(handler != NULL, RETURN_FAILURE);
    R_ASSERT_LOG(finish_handle == BOOLEAN_FALSE || finish_handle == BOOLEAN_TRUE, RETURN_FAILURE, "finish_handle: %d", finish_handle);
    R_ASSERT(help_info != NULL, RETURN_FAILURE);

    R_ASSERT(whether_option_has_been_registered(subcmd_name, option_name) == BOOLEAN_FALSE, RETURN_FAILURE);

    R_LOG("%s = %s, %s = %s, %s = %d, %s = %d, %s = %p, %s = %d, %s = %s\n", 
        "subcmd", subcmd_name,
        "option", option_name,
        "option type", option_type,
        "arg type", arg_type,
        "handler", handler,
        "finish_handle", finish_handle,
        "help_info", help_info);
        
    STRU_OPTION_CONTROL_BLOCK *p_new = NULL;
    p_new = get_a_new_option_cb(subcmd_name, 
        option_name, 
        option_type, 
        arg_type, 
        handler, 
        finish_handle,
        help_info);
    R_ASSERT(p_new != NULL, RETURN_FAILURE);

    ENUM_RETURN ret_val;
    ret_val = add_a_new_option_cb_to_subcmd_cb(p_new);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    return RETURN_SUCCESS;
}

ENUM_BOOLEAN whether_option_h_has_been_processed(void)
{
    STRU_OPTION_CONTROL_BLOCK *p_option_cb = get_current_running_option_cb(get_current_running_subcmd_name(), "-h");
    R_FALSE_RET(p_option_cb == NULL, BOOLEAN_TRUE);

    return BOOLEAN_FALSE;
}

void display_option_help_info(STRU_OPTION_CONTROL_BLOCK *p_option_cb)
{
    PRIVATE _S8 temp_buf[64] = {'\0'};

    STRU_TABLE_TEXT_FORMAT format[2] = {{4, 0, 24, BOOLEAN_TRUE}, {1, 0, 71, BOOLEAN_TRUE}};
    const _S8 *text[2];
    
    while(p_option_cb != NULL)
    {
        sprintf(temp_buf, "%s%s", 
            p_option_cb->option, 
            (p_option_cb->arg_type == ARG_TYPE_DATA)?" <arg>":"");

        text[0] = temp_buf;
        text[1] = p_option_cb->help_info;
        V_ASSERT(s_print_table_text(text, 1, 2, format) == RETURN_SUCCESS);

        p_option_cb = p_option_cb->next;
    }
}

/* 将选项及值处理并保存 */
ENUM_RETURN parse_options(int argc, char **argv)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    const char *current_subcmd_name = NULL;

    /* do noting when there is any error */
    R_FALSE_RET_LOG(BOOLEAN_FALSE == whether_any_error_exists(), RETURN_SUCCESS, "");

    int i = get_argv_indicator();
    
    current_subcmd_name = get_current_running_subcmd_name();
    R_ASSERT(current_subcmd_name != NULL, RETURN_FAILURE);

    const char *option_name = NULL;
    while(i < argc)
    {
        /* do noting when there is any error */
        R_FALSE_RET_LOG(BOOLEAN_FALSE == whether_any_error_exists(), RETURN_SUCCESS, "");
 
        R_LOG("option: i = %d, argv = %s", i, argv[i]);
        option_name = argv[i];
        
        R_FALSE_DO_LOG(whether_string_is_in_option_format(option_name) == BOOLEAN_TRUE, 
            break, "");
        
        // 当前option是否在控制块中注册过
        if(BOOLEAN_FALSE == whether_option_has_been_registered(current_subcmd_name, option_name))
        {
            ret_val = generate_system_error(ERROR_CODE_UNKONWN_OPTION, option_name);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

            break;   
        }

        /* 当前option已经处理过 */
        if(get_current_running_option_cb(current_subcmd_name, option_name) != NULL)
        {
            ret_val = generate_system_error(ERROR_CODE_REPETITIVE_OPTION, option_name);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            
            break;
        }

        ret_val = set_current_running_option(current_subcmd_name, option_name);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

        ENUM_ARG_TYPE arg_type;
        ret_val = get_current_running_option_arg_type(current_subcmd_name, option_name, &arg_type);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
  
        const char *arg_value = NULL;
        if(arg_type == ARG_TYPE_SWITCH)
        {
            arg_value = "enable";
        }
        else
        {
            i++;
            
            if(i < argc)
            {
                R_LOG("option's arg value: i = %d, argv = %s", i, argv[i]);
                arg_value = argv[i];
            }
        }
        
        ret_val = add_arg_to_current_running_option(current_subcmd_name, option_name, arg_value);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

        i++;
    }

    ret_val = check_missing_options_of_subcmd(current_subcmd_name);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    set_argv_indicator(i);
    
    return RETURN_SUCCESS;
}

PRIVATE void display_ignored_options(
    STRU_OPTION_CONTROL_BLOCK *p_option_cb)
{
    char string_buf[256] = "These options are ignored:";
    int buffer_size = sizeof(string_buf);
    int ignored_option_num = 0;
    while(p_option_cb != NULL)
    {
        /* 当前option没有输入 */
        if(p_option_cb->is_running == BOOLEAN_TRUE)
        {
            V_ASSERT(buffer_size - strlen(string_buf - 1) > strlen(p_option_cb->option) + 1);
            strcat(string_buf, " ");
            strcat(string_buf, p_option_cb->option);
            R_LOG("option [%s] is ignored", p_option_cb->option);
            ignored_option_num++;
        }
        
        p_option_cb = p_option_cb->next;
    }

    if(ignored_option_num > 0)
    {
        printf(YELLOW"%s\n"NONE, string_buf);
    }
    
}

/* 处理保存的选项及值 处理任意一个option失败，则立即返回失败 */
ENUM_RETURN process_options(_VOID    )
{
    /* do noting when there is any error */
    R_FALSE_RET_LOG(BOOLEAN_FALSE == whether_any_error_exists(), RETURN_SUCCESS, "");

    ENUM_RETURN user_process_result = RETURN_SUCCESS;
    ENUM_RETURN ret_val = RETURN_SUCCESS;

    STRU_OPTION_CONTROL_BLOCK *p_option_cb = get_option_cb_list_head(get_current_running_subcmd_name());
    R_ASSERT(p_option_cb != NULL, RETURN_FAILURE);

    while(p_option_cb != NULL)
    {
        /* 当前option没有输入 */
        R_FALSE_DO_LOG(p_option_cb->is_running == BOOLEAN_TRUE, 
            p_option_cb = p_option_cb->next; continue, 
            "option [%s] is not input", 
            p_option_cb->option);

        user_process_result = p_option_cb->handler(p_option_cb->arg_value);
        if(user_process_result == RETURN_FAILURE)
        {
            ret_val = generate_system_error(ERROR_CODE_OPTION_PROC_FAIL, NULL);
            R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            
            R_LOG("option [%s] handler process failed!\n", p_option_cb->option);

            return RETURN_SUCCESS;
        }

        /* 成功处理一个option之后，判断是否停止处理 */
        R_FALSE_DO_LOG(p_option_cb->finish_handle == BOOLEAN_FALSE, 
            p_option_cb = p_option_cb->next;break, 
            "option [%s] will return!", 
            p_option_cb->option);

        p_option_cb = p_option_cb->next;
    }
    
    display_ignored_options(p_option_cb);
    
    return RETURN_SUCCESS;
}

