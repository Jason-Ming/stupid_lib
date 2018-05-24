#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "s_log.h"
#include "s_cmd_proc.h"
#include "s_option.h"
#include "s_input_file.h"
#include "s_error.h"
#include "s_default.h"

#include "s_subcmd.h"

#define FREE(p) do{ if(p != NULL){free(p);}}while(0);

typedef struct TAG_STRU_SUBCMD_CONTROL_BLOCK
{
    char* subcmd;
    ENUM_BOOLEAN need_input_file;
    FUNC_SUBCMD_PROC handler;
    struct TAG_STRU_OPTION_CONTROL_BLOCK *option_cbs;
    char* help_info;
    struct TAG_STRU_SUBCMD_CONTROL_BLOCK *next;
} STRU_SUBCMD_CONTROL_BLOCK;

PRIVATE STRU_SUBCMD_CONTROL_BLOCK *p_subcmd_cb_list_head = NULL;
PRIVATE STRU_SUBCMD_CONTROL_BLOCK *p_subcmd_cb_list_tail = NULL;

typedef struct TAG_STRU_SUBCMD_RUN_BLOCK
{
    STRU_SUBCMD_CONTROL_BLOCK* p_subcmd_cb;
    struct TAG_STRU_OPTION_RUN_BLOCK * option_rb;
    struct TAG_STRU_SUBCMD_RUN_BLOCK *next;
} STRU_SUBCMD_RUN_BLOCK;

PRIVATE STRU_SUBCMD_RUN_BLOCK *p_subcmd_rb_list_head = NULL;
PRIVATE STRU_SUBCMD_RUN_BLOCK *p_subcmd_rb_list_tail = NULL;

STRU_SUBCMD_CONTROL_BLOCK *get_subcmd_cb_list_head(void)
{
    return p_subcmd_cb_list_head;
}

STRU_SUBCMD_CONTROL_BLOCK *get_subcmd_cb_by_name(const char* subcmd_name)
{
    R_ASSERT(subcmd_name != NULL, NULL);
    
    STRU_SUBCMD_CONTROL_BLOCK *p = get_subcmd_cb_list_head();

    while(p != NULL)
    {
        if(strcmp(subcmd_name, p->subcmd) == 0)
        {
            return p;
        }
        p = p->next;
    }

    return NULL;
}

STRU_OPTION_CONTROL_BLOCK *get_option_cb_list_head(const char *subcmd_name)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    R_ASSERT(subcmd_name != NULL, NULL);
    STRU_SUBCMD_CONTROL_BLOCK *p_subcmd_cb = get_subcmd_cb_by_name(subcmd_name);
    if(p_subcmd_cb == NULL)
    {
        ret_val = add_current_system_error(ERROR_CODE_UNKONWN_SUBCMD, subcmd_name);
        R_ASSERT(ret_val == RETURN_SUCCESS, NULL);
        return NULL;
    }
    else
    {
        return p_subcmd_cb->option_cbs;
    }
}

PRIVATE void debug_print_subcmd_cb(STRU_SUBCMD_CONTROL_BLOCK *p)
{
    R_LOG("%s = %s, %s = %d, %s = %p, %s = %s\n", 
        "subcmd", p->subcmd,
        "need_input_file", p->need_input_file,
        "handler", p->handler,
        "help_info", p->help_info);

    debug_print_option_cb_list(p->option_cbs);
}

void debug_print_subcmd_cb_list(void)
{
    STRU_SUBCMD_CONTROL_BLOCK *p = get_subcmd_cb_list_head();

    while(p != NULL)
    {
        debug_print_subcmd_cb(p);

        p = p->next;
    }
}

PRIVATE ENUM_RETURN add_a_new_subcmd_cb_to_subcmd_cb_list(STRU_SUBCMD_CONTROL_BLOCK *p_new)
{
    R_ASSERT(p_new != NULL, RETURN_FAILURE);

    if(p_subcmd_cb_list_tail == NULL)
    {
        p_subcmd_cb_list_head = p_subcmd_cb_list_tail = p_new;
    }
    else
    {
        p_subcmd_cb_list_tail->next = p_new;
        p_subcmd_cb_list_tail = p_new;
    }
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN get_a_new_subcmd_cb_do(STRU_SUBCMD_CONTROL_BLOCK **pp_new, 
    const char* subcmd_name, 
    ENUM_BOOLEAN need_input_file,
    FUNC_SUBCMD_PROC handler, 
    const char* help_info)
{
    R_ASSERT(pp_new != NULL, RETURN_FAILURE);
    
    *pp_new = (STRU_SUBCMD_CONTROL_BLOCK*)malloc(sizeof(STRU_SUBCMD_CONTROL_BLOCK));
    R_ASSERT(*pp_new != NULL, RETURN_FAILURE);

    (*pp_new)->subcmd = NULL;
    (*pp_new)->need_input_file = need_input_file;
    (*pp_new)->option_cbs = NULL;
    (*pp_new)->handler = handler;
    (*pp_new)->help_info = NULL;
    (*pp_new)->next = NULL;

    (*pp_new)->subcmd = (char*)malloc(strlen(subcmd_name) + 1);
    R_ASSERT((*pp_new)->subcmd != NULL, RETURN_FAILURE);
    strcpy((*pp_new)->subcmd, subcmd_name);
    
    (*pp_new)->help_info = (char*)malloc(strlen(help_info) + 1);
    R_ASSERT((*pp_new)->help_info != NULL, RETURN_FAILURE);
    strcpy((*pp_new)->help_info, help_info);

    return RETURN_SUCCESS;
}

PRIVATE void get_a_new_subcmd_cb_do_error(STRU_SUBCMD_CONTROL_BLOCK *p_new)
{
    if(p_new == NULL)
    {
        return;
    }
    
    FREE(p_new->subcmd);
    FREE(p_new->help_info);
    free(p_new);
}

PRIVATE STRU_SUBCMD_CONTROL_BLOCK *get_a_new_subcmd_cb(
    const char* subcmd_name, 
    ENUM_BOOLEAN need_input_file,
    FUNC_SUBCMD_PROC handler, 
    const char* help_info)
{
    ENUM_RETURN ret_val;
    STRU_SUBCMD_CONTROL_BLOCK *p_new = NULL;
    ret_val = get_a_new_subcmd_cb_do(&p_new, subcmd_name, need_input_file, handler, help_info);
    R_ASSERT_DO(ret_val == RETURN_SUCCESS, NULL, get_a_new_subcmd_cb_do_error(p_new));

    return p_new;
}

ENUM_BOOLEAN is_subcmd_name_valid(const char* subcmd_name)
{
    int position = 0;

    while(subcmd_name[position] != '\0')
    {
        R_ASSERT(IS_ALPHABET(subcmd_name[position]) == BOOLEAN_TRUE, BOOLEAN_FALSE);
        position++;
    }

    R_ASSERT(position > 0, BOOLEAN_FALSE);
    
    return BOOLEAN_TRUE;
}

ENUM_RETURN register_subcmd(
    const char* subcmd_name,
    ENUM_BOOLEAN need_input_file,
    FUNC_SUBCMD_PROC handler, 
    const char* help_info)
{
    R_ASSERT(subcmd_name != NULL, RETURN_FAILURE);
    R_ASSERT_LOG(BOOLEAN_TRUE == is_subcmd_name_valid(subcmd_name), RETURN_FAILURE, "subcmd: %s", subcmd_name);
    R_ASSERT(handler != NULL, RETURN_FAILURE);
    R_ASSERT(help_info != NULL, RETURN_FAILURE);

    STRU_SUBCMD_CONTROL_BLOCK *p_new = NULL;
    p_new = get_a_new_subcmd_cb(subcmd_name, need_input_file, handler, help_info);
    R_ASSERT(p_new != NULL, RETURN_FAILURE);

    ENUM_RETURN ret_val;
    ret_val = add_a_new_subcmd_cb_to_subcmd_cb_list(p_new);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    /* 为每个subcmd注册默认的选项 */
    ret_val = register_default_option(subcmd_name);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    debug_print_subcmd_cb(p_new);

    return RETURN_SUCCESS;
}

ENUM_RETURN add_a_new_option_cb_to_subcmd_cb(STRU_OPTION_CONTROL_BLOCK *p_new)
{
    R_ASSERT(p_new != NULL, RETURN_FAILURE);

    STRU_SUBCMD_CONTROL_BLOCK *p_subcmd_cb = get_subcmd_cb_by_name(p_new->subcmd);
    R_ASSERT(p_subcmd_cb != NULL, RETURN_FAILURE);
    
    if(p_subcmd_cb->option_cbs == NULL)
    {
        p_subcmd_cb->option_cbs = p_new;
    }
    else
    {
        struct TAG_STRU_OPTION_CONTROL_BLOCK *option_cb_temp = p_subcmd_cb->option_cbs;
        while(option_cb_temp->next != NULL)
        {
            option_cb_temp = option_cb_temp->next;
        }
        
        option_cb_temp->next = p_new;
    }
    
    return RETURN_SUCCESS;
}

ENUM_BOOLEAN is_subcmd_registered(const char *subcmd_name)
{
    R_FALSE_RET_LOG(subcmd_name != NULL, BOOLEAN_FALSE, "subcmd_name: %s", subcmd_name);
    
    return (get_subcmd_cb_by_name(subcmd_name) != NULL)? BOOLEAN_TRUE:BOOLEAN_FALSE;
}

ENUM_BOOLEAN is_subcmd_need_input_files(const char *subcmd_name)
{
    STRU_SUBCMD_CONTROL_BLOCK *p_subcmd_cb = get_subcmd_cb_by_name(subcmd_name);
    R_ASSERT(p_subcmd_cb != NULL, BOOLEAN_FALSE);

    return p_subcmd_cb->need_input_file;
}

PRIVATE STRU_SUBCMD_RUN_BLOCK *get_subcmd_rb_list_head(void)
{
    return p_subcmd_rb_list_head;
}

PRIVATE STRU_SUBCMD_RUN_BLOCK *get_subcmd_rb_by_name(const char* subcmd_name)
{
    STRU_SUBCMD_RUN_BLOCK *p = get_subcmd_rb_list_head();

    while(p != NULL)
    {
        if(strcmp(subcmd_name, p->p_subcmd_cb->subcmd) == 0)
        {
            return p;
        }
        p = p->next;
    }

    return NULL;
}

STRU_OPTION_RUN_BLOCK *get_option_rb_list_head_by_name(const char *subcmd_name)
{
    STRU_SUBCMD_RUN_BLOCK *p_current_subcmd_rb = get_subcmd_rb_by_name(subcmd_name);
    R_ASSERT(p_current_subcmd_rb != NULL, NULL);
    
    return p_current_subcmd_rb->option_rb;
}

STRU_SUBCMD_RUN_BLOCK *get_current_subcmd_rb(void)
{
    return get_subcmd_rb_list_head();
}

const char *get_current_subcmd_name(void)
{
    STRU_SUBCMD_RUN_BLOCK *p_current_subcmd_rb = get_current_subcmd_rb();
    R_ASSERT(p_current_subcmd_rb != NULL, NULL);
    
    return p_current_subcmd_rb->p_subcmd_cb->subcmd;
}

PRIVATE ENUM_RETURN add_a_new_subcmd_rb_to_subcmd_rb_list(STRU_SUBCMD_RUN_BLOCK *p_new)
{
    R_ASSERT(p_new != NULL, RETURN_FAILURE);

    if(p_subcmd_rb_list_tail == NULL)
    {
        p_subcmd_rb_list_head = p_subcmd_rb_list_tail = p_new;
    }
    else
    {
        p_subcmd_rb_list_tail->next = p_new;
        p_subcmd_rb_list_tail = p_new;
    }
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN get_a_new_subcmd_rb_do(STRU_SUBCMD_RUN_BLOCK **pp_new)
{
    R_ASSERT(pp_new != NULL, RETURN_FAILURE);
    
    *pp_new = (STRU_SUBCMD_RUN_BLOCK*)malloc(sizeof(STRU_SUBCMD_RUN_BLOCK));
    R_ASSERT(*pp_new != NULL, RETURN_FAILURE);

    (*pp_new)->p_subcmd_cb = NULL;
    (*pp_new)->option_rb = NULL;
    (*pp_new)->next = NULL;

    return RETURN_SUCCESS;
}

PRIVATE void get_a_new_subcmd_rb_do_error(STRU_SUBCMD_RUN_BLOCK *p_new)
{
    if(p_new == NULL)
    {
        return;
    }

    free(p_new);
}

PRIVATE STRU_SUBCMD_RUN_BLOCK *get_a_new_subcmd_rb(void)
{
    ENUM_RETURN ret_val;
    STRU_SUBCMD_RUN_BLOCK *p_new = NULL;
    ret_val = get_a_new_subcmd_rb_do(&p_new);
    R_ASSERT_DO(ret_val == RETURN_SUCCESS, NULL, get_a_new_subcmd_rb_do_error(p_new));

    return p_new;
}

ENUM_RETURN add_subcmd_rb_to_subcmd_rb_list_by_name(const char *subcmd_name)
{
    R_ASSERT(subcmd_name != NULL, RETURN_FAILURE);
    
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    STRU_SUBCMD_CONTROL_BLOCK *p_cb = NULL;
    STRU_SUBCMD_RUN_BLOCK *p_rb = NULL;

    p_cb = get_subcmd_cb_by_name(subcmd_name);
    R_ASSERT(p_cb != NULL, RETURN_FAILURE);

    p_rb = get_a_new_subcmd_rb();
    R_ASSERT(p_rb != NULL, RETURN_FAILURE);
    
    p_rb->p_subcmd_cb = p_cb;
    ret_val = add_a_new_subcmd_rb_to_subcmd_rb_list(p_rb);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    return RETURN_SUCCESS;
}

ENUM_RETURN add_a_new_option_rb_to_subcmd_rb(STRU_OPTION_RUN_BLOCK *p_new)
{
    R_ASSERT(p_new != NULL, RETURN_FAILURE);
    
    STRU_SUBCMD_RUN_BLOCK *p_subcmd_rb = NULL;
    p_subcmd_rb = get_subcmd_rb_by_name(p_new->subcmd);
    R_ASSERT(p_subcmd_rb != NULL, RETURN_FAILURE);

    if(p_subcmd_rb->option_rb == NULL)
    {
        p_subcmd_rb->option_rb = p_new;
    }
    else
    {
        struct TAG_STRU_OPTION_RUN_BLOCK * temp_option_rb = p_subcmd_rb->option_rb;
        while(temp_option_rb->next != NULL)
        {
            temp_option_rb = temp_option_rb->next;
        }
        
        temp_option_rb->next = p_new;
    }
    
    return RETURN_SUCCESS;
}

STRU_OPTION_RUN_BLOCK *get_option_rb_by_name(const char* subcmd_name, const char* option_name)
{
    STRU_OPTION_RUN_BLOCK *p = get_option_rb_list_head_by_name(subcmd_name);

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

ENUM_BOOLEAN is_option_miss_to_subcmd(const char *subcmd_name)
{
    R_ASSERT(subcmd_name != NULL, BOOLEAN_TRUE);
    STRU_SUBCMD_CONTROL_BLOCK *p_subcmd_cb = get_subcmd_cb_by_name(subcmd_name);
    R_ASSERT(p_subcmd_cb != NULL, BOOLEAN_TRUE);

    STRU_OPTION_RUN_BLOCK *p_option_rb = get_option_rb_list_head_by_name(subcmd_name);
    if(p_option_rb != NULL)
    {
        return BOOLEAN_FALSE;
    }

    STRU_OPTION_CONTROL_BLOCK *p_option_cb = p_subcmd_cb->option_cbs;

    while(p_option_cb != NULL)
    {
        if(p_option_cb->option_type == OPTION_TYPE_MANDATORY)
        {
            return BOOLEAN_TRUE;
        }
        p_option_cb = p_option_cb->next;
    }

    return BOOLEAN_FALSE;
}

void display_subcmd_help_info(const char *subcmd_name)
{
    V_ASSERT(subcmd_name != NULL);
    
    STRU_SUBCMD_CONTROL_BLOCK *p_subcmd_cb = NULL;
    
    p_subcmd_cb = get_subcmd_cb_by_name(subcmd_name);
    V_ASSERT(p_subcmd_cb != NULL);

    printf("usage: %s %s [option <arg>]\n", get_bin_name(), subcmd_name);

    display_option_help_info(p_subcmd_cb->option_cbs);
}

void display_all_subcmd_help_info(void)
{
    STRU_SUBCMD_CONTROL_BLOCK *p = get_subcmd_cb_list_head();
    printf("usage: %s %s\n", get_bin_name(), get_usage());

    printf("\nuse %s <subcmd> -h to see help information about a specific subcommand\n", get_bin_name());
    
    printf("\nThese are common sub commands used in various situations:\n\n");
    
    while(p != NULL)
    {
        printf("   %-10s %s\n", p->subcmd, p->help_info);

        p = p->next;
    }
}

PRIVATE ENUM_RETURN parse_subcmds_do(int argc, char **argv)
{
    STRU_SUBCMD_CONTROL_BLOCK *p_cb = NULL;
    STRU_SUBCMD_RUN_BLOCK *p_rb = NULL;
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    int i = get_argv_indicator();

    R_FALSE_RET_LOG(i < argc, RETURN_SUCCESS, "i = %d, argc = %d", i, argc);

    R_LOG("i = %d, argv = %s", i, argv[i]);
    
    /* 当前subcmd未在控制块中注册过则停止处理 */
    p_cb = get_subcmd_cb_by_name(argv[i]);
    if(p_cb == NULL)
    {
        ret_val = add_current_system_error(ERROR_CODE_UNKONWN_SUBCMD, argv[i]);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        return RETURN_SUCCESS;
    }

    p_rb = get_subcmd_rb_by_name(p_cb->subcmd);

    /* 重复输入subcmd则停止处理 */
    if(p_rb != NULL)
    {
        ret_val = add_current_system_error(ERROR_CODE_REPETITIVE_OPTION, p_cb->subcmd);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        return RETURN_SUCCESS;
    }

    /* 目前只处理一个子命令 */
    p_rb = get_a_new_subcmd_rb();
    R_ASSERT(p_rb != NULL, RETURN_FAILURE);
    
    p_rb->p_subcmd_cb = p_cb;
    ret_val = add_a_new_subcmd_rb_to_subcmd_rb_list(p_rb);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    i++;
    set_argv_indicator(i);

    return RETURN_SUCCESS;
}

/* 将子命令处理并保存 */
ENUM_RETURN parse_subcmds(int argc, char **argv)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;

    ret_val = parse_subcmds_do(argc, argv);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    if(get_current_subcmd_rb() == NULL)
    {
        ret_val = add_current_system_error(ERROR_CODE_MISSING_SUBCMD, NULL);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }

    return RETURN_SUCCESS;
}

/* 处理保存的选项及值 */
ENUM_RETURN process_subcmds(void)
{
    ENUM_RETURN user_process_result = RETURN_SUCCESS;

    STRU_SUBCMD_RUN_BLOCK *p = get_subcmd_rb_list_head();
    R_ASSERT(p != NULL, RETURN_FAILURE);
    ENUM_RETURN ret_val = RETURN_SUCCESS;

    while(p != NULL)
    {        
        /* 处理options失败时，直接停止处理 */
        ret_val = process_options(p->p_subcmd_cb->option_cbs, p->option_rb, &user_process_result);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        R_FALSE_RET_LOG(user_process_result == RETURN_SUCCESS, RETURN_SUCCESS, "process subcmd [%s] options failed!\n", p->p_subcmd_cb->subcmd);

        user_process_result = p->p_subcmd_cb->handler(p->option_rb);
        R_FALSE_RET_LOG(user_process_result == RETURN_SUCCESS, RETURN_SUCCESS, "process subcmd [%s] handler failed!\n", p->p_subcmd_cb->subcmd);

        p = p->next;
    }

    return RETURN_SUCCESS;
}


