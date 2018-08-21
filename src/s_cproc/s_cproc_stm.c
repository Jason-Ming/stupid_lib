#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "s_defines.h"
#include "s_text.h"
#include "s_cmd.h"
#include "s_log.h"
#include "s_stack.h"
#include "s_stm.h"
#include "s_mem.h"
#include "s_ctoken.h"
#include "s_cproc_stm.h"
#include "s_cerror.h"
#include "s_cproc_token.h"
#include "s_cproc_ctoken_pp.h"
#include "s_cproc_macro.h"
#include "s_cproc_include_file.h"

/*
about:'\'
1.'\\' optional-' ' '\n' means the sequence can be deleted and the original token separated by it can be regarded as a whole token
2.'\\' occurs in "" or '' means the backescape charactor
3.other case , illegal

about: comment
1.comment can be deleted directly, but the original token separated by it can not be regarded as a whole token, it means two tokens

about: macro
1.start with # oneline
2.# define token optional(x,y,etc)
3.# must start at the beginning of line, caution: last line ending with '\'
*/

typedef enum TAG_ENUM_CPROC_STM
{
    CPROC_STM_NORMAL = 0,
    CPROC_STM_STRING_DOUBLE_QUOTE,
    CPROC_STM_STRING_SINGLE_QUOTE,
    CPROC_STM_ONELINE_COMMENT,
    CPROC_STM_PAIR_COMMENT,
    CPROC_STM_NEWLINE,
    CPROC_STM_BLANK,
    CPROC_STM_IDENTIFIER,
    CPROC_STM_NUMBER,
    CPROC_STM_PP,
    CPROC_STM_PP_DIRECTIVE,
    CPROC_STM_PP_IF_GROUP,
    CPROC_STM_PP_ELIF_GROUP,
    CPROC_STM_PP_ELSE_GROUP,
    CPROC_STM_PP_ENDIF_LINE,
    CPROC_STM_PP_INCLUDE,
    CPROC_STM_PP_DEFINE,
    CPROC_STM_PP_UNDEF,
    CPROC_STM_PP_LINE,
    CPROC_STM_PP_ERROR,
    CPROC_STM_PP_PRAGMA,
    CPROC_STM_PP_INCLUDE_H_HEADER,
    CPROC_STM_PP_INCLUDE_Q_HEADER,
    CPROC_STM_PP_INCLUDE_FINISH,
    CPROC_STM_PP_DEFINE_MACRO,
    CPROC_STM_PP_DEFINE_PARAMETER_START,
    CPROC_STM_PP_DEFINE_PARAMETER_LIST,
    CPROC_STM_PP_DEFINE_PARAMETER,
    CPROC_STM_PP_DEFINE_REPLACEMENT,
    CPROC_STM_PP_IDENTIFIER,
    CPROC_STM_PP_NUMBER,
    CPROC_STM_PP_HEADER,
    CPROC_STM_END,
    CPROC_STM_MAX,
}ENUM_CPROC_STM;

typedef struct TAG_STRU_CPROC_STM_PROC
{
    STM_STATE state;
    STM_PROC handler;
    const char *info;
}STRU_CPROC_STM_PROC;

typedef enum TAG_ENUM_CPROC_STM_CHAR_DIRECTION
{
    CPROC_STM_CHAR_DIRECTION_FORWARD = 0,
    CPROC_STM_CHAR_DIRECTION_BACKWARD,
    CPROC_STM_CHAR_DIRECTION_STAY,
    CPROC_STM_CHAR_DIRECTION_MAX,
}ENUM_CPROC_STM_CHAR_DIRECTION;
    
typedef struct TAG_STRU_CPROC_STM_RUN_DATA
{
    STM stm;
    //input and output data
    const _S8 *filename;
    const _S8 *p_text_buffer;

    //running data
    ENUM_CPROC_STM_CHAR_DIRECTION  c_direction;
    size_t c_step;
    ENUM_BOOLEAN move_to_next_line;
    size_t offset;
    size_t current_line_index;
    size_t current_char_position_in_line;
    
    int c;
    _S32 last_c;
    
    _S8 current_token[MAX_TOKEN_LEN];
    _S8 *p_current_token;
    size_t current_token_size;
    STRU_C_TOKEN_POSITION current_token_position;
    ENUM_C_TOKEN current_token_type;
    ENUM_C_TOKEN last_token_type;
    
    ENUM_BOOLEAN whether_any_error_exists;
}STRU_CPROC_STM_RUN_DATA;

#define TEXT_BUFFER g_cproc_stm_run_data.p_text_buffer
#define CURRENT_C_POINTER (TEXT_BUFFER + g_cproc_stm_run_data.offset)
#define CURRENT_C (g_cproc_stm_run_data.c)
#define LAST_C (g_cproc_stm_run_data.last_c)
#define NEXT_C (TEXT_BUFFER[g_cproc_stm_run_data.offset + 1])
#define NEXT_NEXT_C (TEXT_BUFFER[g_cproc_stm_run_data.offset + 2])

#define CURRENT_C_OFFSET (g_cproc_stm_run_data.offset)
#define CURRENT_C_LINE (g_cproc_stm_run_data.current_line_index)
#define CURRENT_C_COLUMN (g_cproc_stm_run_data.current_char_position_in_line)

#define LAST_TOKEN_STRING (LAST_TOKEN_POINTER?LAST_TOKEN_POINTER->info.p_string:"NULL")
#define LAST_TOKEN_POINTER (s_cproc_token_get_last_token())
#define CURRENT_TOKEN_STR_BUFFER g_cproc_stm_run_data.current_token
#define CURRENT_TOKEN_STR_BUFFER_POINTER_TO_WRITE (g_cproc_stm_run_data.p_current_token)
#define CURRENT_TOKEN_STR_BUFFER_LEFT_SIZE_TO_WRITE (g_cproc_stm_run_data.current_token_size)
#define CURRENT_TOKEN_STR_BUFFER_LEN (MAX_TOKEN_LEN - CURRENT_TOKEN_STR_BUFFER_LEFT_SIZE_TO_WRITE)
#define CURRENT_TOKEN_TYPE (g_cproc_stm_run_data.current_token_type)
#define CURRENT_TOKEN_OFFSET (g_cproc_stm_run_data.current_token_position.offset)
#define CURRENT_TOKEN_LINE (g_cproc_stm_run_data.current_token_position.line)
#define CURRENT_TOKEN_COLUMN (g_cproc_stm_run_data.current_token_position.column)


#define LAST_TOKEN_TYPE (g_cproc_stm_run_data.last_token_type)

#define TOKEN_IS_READY_TO_BE_ADDED\
    (g_cproc_stm_run_data.current_token_type != C_TOKEN_INVALID\
      && g_cproc_stm_run_data.current_token_size != MAX_TOKEN_LEN)

#define C_BACKWARD\
    do{\
        g_cproc_stm_run_data.c_direction = CPROC_STM_CHAR_DIRECTION_BACKWARD;\
    }while(0);

#define C_FORWARD(step)\
    do{\
        g_cproc_stm_run_data.c_direction = CPROC_STM_CHAR_DIRECTION_FORWARD;\
        g_cproc_stm_run_data.c_step += step;\
    }while(0);
#define C_DIRECTION_IS_FORWARD\
    (g_cproc_stm_run_data.c_direction == CPROC_STM_CHAR_DIRECTION_FORWARD)
#define C_STAY\
    do{\
        g_cproc_stm_run_data.c_direction = CPROC_STM_CHAR_DIRECTION_STAY;\
        g_cproc_stm_run_data.c_step = 0;\
    }while(0);

#define LINE_FORWARD\
    do{\
        g_cproc_stm_run_data.move_to_next_line = BOOLEAN_TRUE;\
    }while(0)
#define LINE_DIRECTION_IS_FORWARD\
    (g_cproc_stm_run_data.move_to_next_line == BOOLEAN_TRUE)

#define LINE_STAY\
    do{\
        g_cproc_stm_run_data.move_to_next_line = BOOLEAN_FALSE;\
    }while(0)

#define STATE_BACK\
    do{\
        ENUM_RETURN ___ret_val;\
        STM_STATE ___state = get_last_stm_state(g_cproc_stm_run_data.stm);\
        if(___state == CPROC_STM_PP_DEFINE_REPLACEMENT)\
        {\
            ___ret_val = s_cproc_macro_add_replacement(LAST_TOKEN_POINTER);\
            S_R_ASSERT(___ret_val == RETURN_SUCCESS, RETURN_FAILURE);\
        }\
        ___ret_val = set_current_stm_state(g_cproc_stm_run_data.stm, ___state);\
        S_R_ASSERT(___ret_val == RETURN_SUCCESS, RETURN_FAILURE);\
    }while(0);

#define STATE_TO(state)\
    do{\
        ENUM_RETURN ___ret_val = set_current_stm_state(g_cproc_stm_run_data.stm, state);\
        S_R_ASSERT(___ret_val == RETURN_SUCCESS, RETURN_FAILURE);\
    }while(0);

#define RESET_TOKEN\
    do{\
        CURRENT_TOKEN_STR_BUFFER[0] = '\0';\
        CURRENT_TOKEN_STR_BUFFER_LEFT_SIZE_TO_WRITE = MAX_TOKEN_LEN;\
        CURRENT_TOKEN_STR_BUFFER_POINTER_TO_WRITE = &(CURRENT_TOKEN_STR_BUFFER[0]);\
        CURRENT_TOKEN_TYPE = C_TOKEN_INVALID;\
    }while(0);

#define OUTPUT_TOKEN_C\
    do{\
        if(CURRENT_TOKEN_TYPE == C_TOKEN_INVALID)\
        {\
            CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "can not parse token type following %s", LAST_TOKEN_STRING);\
            RESET_TOKEN;\
            break;\
        }\
        if(CURRENT_TOKEN_STR_BUFFER_LEFT_SIZE_TO_WRITE == MAX_TOKEN_LEN)\
        {\
            CURRENT_TOKEN_OFFSET = CURRENT_C_OFFSET;\
            CURRENT_TOKEN_LINE = CURRENT_C_LINE;\
            CURRENT_TOKEN_COLUMN = CURRENT_C_COLUMN;\
        }\
        C_FORWARD(1);\
        if((CURRENT_C == '\n') || (CURRENT_C == '\r' && NEXT_C != '\n'))\
        {\
            LINE_FORWARD;\
        }\
        OUTPUT_C(CURRENT_C, CURRENT_TOKEN_STR_BUFFER_POINTER_TO_WRITE, CURRENT_TOKEN_STR_BUFFER_LEFT_SIZE_TO_WRITE);\
    }while(0);

#define OUTPUT_TOKEN_STR(len)\
        do{\
            if(CURRENT_TOKEN_TYPE == C_TOKEN_INVALID || len == 0)\
            {\
                CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "can not parse token type following %s", LAST_TOKEN_STRING);\
                RESET_TOKEN;\
                break;\
            }\
            if(CURRENT_TOKEN_STR_BUFFER_LEFT_SIZE_TO_WRITE == MAX_TOKEN_LEN)\
            {\
                CURRENT_TOKEN_OFFSET = CURRENT_C_OFFSET;\
                CURRENT_TOKEN_LINE = CURRENT_C_LINE;\
                CURRENT_TOKEN_COLUMN = CURRENT_C_COLUMN;\
            }\
            C_FORWARD(len);\
            OUTPUT_STR(CURRENT_TOKEN_STR_BUFFER_POINTER_TO_WRITE, \
                CURRENT_TOKEN_STR_BUFFER_LEFT_SIZE_TO_WRITE, \
                CURRENT_C_POINTER, len);\
        }while(0);

#define END_TOKEN\
    do{\
        OUTPUT_END(CURRENT_TOKEN_STR_BUFFER_POINTER_TO_WRITE, CURRENT_TOKEN_STR_BUFFER_LEFT_SIZE_TO_WRITE);\
    }while(0);

#define TYPE_TOKEN(token_type)\
    do{\
        CURRENT_TOKEN_TYPE = token_type;\
    }while(0);
#define REPLACE_TOKEN\
    do{\
        if(CURRENT_TOKEN_TYPE == C_TOKEN_PAIR_COMMENT || CURRENT_TOKEN_TYPE == C_TOKEN_LINE_COMMENT)\
        {\
            CURRENT_TOKEN_STR_BUFFER[0] = ' ';\
            CURRENT_TOKEN_STR_BUFFER[1] = '\0';\
            CURRENT_TOKEN_TYPE = C_TOKEN_BLANK;\
            DEBUG_PRINT("replace comment by space");\
        }\
    }while(0)
#define ADD_TOKEN\
    do{\
        if(!TOKEN_IS_READY_TO_BE_ADDED)\
        {\
            RESET_TOKEN;\
            break;\
        }\
        END_TOKEN;\
        REPLACE_TOKEN;\
        ENUM_RETURN ___ret_val = s_cproc_token_add_node_to_list(CURRENT_TOKEN_STR_BUFFER,\
            CURRENT_TOKEN_TYPE, \
            CURRENT_TOKEN_OFFSET,\
            CURRENT_TOKEN_LINE, \
            CURRENT_TOKEN_COLUMN);\
        S_R_ASSERT(___ret_val == RETURN_SUCCESS, RETURN_FAILURE);\
        DEBUG_PRINT("add token: "LIGHT_GREEN"%s"NONE", offset: %zd, line: %zd, column: %zd, type: %s", \
            CURRENT_TOKEN_STR_BUFFER, \
            CURRENT_TOKEN_OFFSET,\
            CURRENT_TOKEN_LINE,\
            CURRENT_TOKEN_COLUMN,\
            s_ctoken_get_str(CURRENT_TOKEN_TYPE));\
        LAST_TOKEN_TYPE = CURRENT_TOKEN_TYPE;\
        RESET_TOKEN;\
        STM_STATE ___state = get_current_stm_state(g_cproc_stm_run_data.stm);\
        if(___state == CPROC_STM_PP_DEFINE_REPLACEMENT)\
        {\
            ___ret_val = s_cproc_macro_add_replacement(LAST_TOKEN_POINTER);\
            S_R_ASSERT(___ret_val == RETURN_SUCCESS, RETURN_FAILURE);\
        }\
    }while(0);


#define SKIP_CONTINUED_NEWLINE(skip, space_num)\
    do{\
        DEBUG_PRINT("skip = %zd", skip);\
        if(skip == 0)\
        {\
            break;\
        }\
        if(get_current_stm_state(g_cproc_stm_run_data.stm) == CPROC_STM_ONELINE_COMMENT)\
        {\
            CPROC_GEN_WARNING(NULL, "multi-line comment [-Wcomment]");\
        }\
        if(space_num > 0 && get_current_stm_state(g_cproc_stm_run_data.stm) != CPROC_STM_PAIR_COMMENT && get_current_stm_state(g_cproc_stm_run_data.stm) != CPROC_STM_ONELINE_COMMENT)\
        {\
            CPROC_GEN_WARNING(NULL, "backslash and newline separated by space");\
        }\
        if(TEXT_BUFFER[CURRENT_C_OFFSET + skip] == '\0')\
        {\
            CPROC_GEN_WARNING(NULL, "backslash-newline at end of file");\
        }\
        size_t ___skip = skip;\
        while(___skip > 0)\
        {\
            DEBUG_PRINT("skip \033[7m%c"NONE, TEXT_BUFFER[CURRENT_C_OFFSET]);\
            CURRENT_C_OFFSET++;\
            ___skip--;\
        };\
        /*move to the next line*/\
        CURRENT_C_LINE++;\
        CURRENT_C_COLUMN = 0;\
    }while(0);

PRIVATE STRU_CPROC_STM_RUN_DATA g_cproc_stm_run_data;
PRIVATE STACK g_stack = NULL;
PRIVATE ENUM_RETURN cproc_stm_postproc();

PRIVATE _VOID s_cproc_stm_print_debug_info(_VOID)
{
    printf("%-20s:%p\n", "stm", g_cproc_stm_run_data.stm);
    printf("%-20s:%s\n", "filename", g_cproc_stm_run_data.filename);
    printf("%-20s:%p\n", "p_text_buffer", g_cproc_stm_run_data.p_text_buffer);

    printf("%-20s:%d\n", "c_direction", g_cproc_stm_run_data.c_direction);
    printf("%-20s:%zd\n", "c_step", g_cproc_stm_run_data.c_step);
    printf("%-20s:%d\n", "move_to_next_line", g_cproc_stm_run_data.move_to_next_line);
    printf("%-20s:%zd\n", "offset", g_cproc_stm_run_data.offset);
    printf("%-20s:%zd\n", "current_line_index", g_cproc_stm_run_data.current_line_index);
    printf("%-20s:%zd\n", "current_char_position_in_line", g_cproc_stm_run_data.current_char_position_in_line);
    printf("%-20s:%c\n", "c", g_cproc_stm_run_data.c);
    printf("%-20s:%c\n", "last_c", g_cproc_stm_run_data.last_c);
    printf("%-20s:%p\n", "current_token", g_cproc_stm_run_data.current_token);
    printf("%-20s:%p\n", "p_current_token", g_cproc_stm_run_data.p_current_token);
    printf("%-20s:%zd\n", "current_token_size", g_cproc_stm_run_data.current_token_size);
    printf("%-20s:%zd\n", "current_token_position.offset", g_cproc_stm_run_data.current_token_position.offset);
    printf("%-20s:%zd\n", "current_token_position.line", g_cproc_stm_run_data.current_token_position.line);
    printf("%-20s:%zd\n", "current_token_position.column", g_cproc_stm_run_data.current_token_position.column);
    printf("%-20s:%d\n", "current_token_type", g_cproc_stm_run_data.current_token_type);
    printf("%-20s:%d\n", "last_token_type", g_cproc_stm_run_data.last_token_type);
    printf("%-20s:%d\n", "whether_any_error_exists", g_cproc_stm_run_data.whether_any_error_exists);
    printf("%-20s:%p\n", "g_stack", g_stack);
}

_VOID s_cproc_generate_error()
{
    g_cproc_stm_run_data.whether_any_error_exists = BOOLEAN_TRUE;
}

const _S8 *s_cproc_get_text_buffer()
{
    return TEXT_BUFFER;
}

const _S8 *s_cproc_get_filename()
{
    return g_cproc_stm_run_data.filename;
}

size_t s_cproc_get_current_offset()
{
    return CURRENT_C_OFFSET;
}

size_t s_cproc_get_current_line()
{
    return CURRENT_C_LINE;
}

size_t s_cproc_get_current_column()
{
    return CURRENT_C_COLUMN;
}

PRIVATE ENUM_BOOLEAN cproc_all_blank_at_begin_of_line(_VOID)
{
    STRU_C_TOKEN_NODE *p_token_list_tail;
    ENUM_BOOLEAN all_blank_at_begin_of_line = BOOLEAN_TRUE;

    struct list_head *pos;
    list_for_each_all_reverse(pos, &s_cproc_token_get_list_head()->list)
    {
        p_token_list_tail = list_entry(pos, STRU_C_TOKEN_NODE, list);
        if(p_token_list_tail->info.token_type == C_TOKEN_NEWLINE_LINUX
            || p_token_list_tail->info.token_type == C_TOKEN_NEWLINE_WINDOWS
            || p_token_list_tail->info.token_type == C_TOKEN_NEWLINE_MAC)
        {
            break;
        }

		if(p_token_list_tail->info.token_type != C_TOKEN_BLANK)
		{
			all_blank_at_begin_of_line = BOOLEAN_FALSE;
			break;
		}
    }

    return all_blank_at_begin_of_line;
}

/* get the skip number of the next availible char */
PRIVATE ENUM_RETURN cproc_continued_newline(size_t *skip, size_t *space_num)
{
    S_R_ASSERT(skip != NULL, RETURN_FAILURE);
    S_R_ASSERT(space_num != NULL, RETURN_FAILURE);
    *skip = 0;
    //CALC(1,2,3);
    size_t offset_temp = CURRENT_C_OFFSET;
    
    _S8 c;
    c = TEXT_BUFFER[offset_temp];
    S_R_ASSERT(c == '\\', RETURN_FAILURE);
    offset_temp++;
    *space_num = 0;
    
    ENUM_BOOLEAN is_all_space_between_backslash_and_newline = BOOLEAN_TRUE;

    while((c = TEXT_BUFFER[offset_temp]) != '\0' && c != '\n' && c != '\r')
    {
        if(!isspace(c))
        {
            is_all_space_between_backslash_and_newline = BOOLEAN_FALSE;
            *space_num = 0;
            break;
        }
        else
        {
            (*space_num)++;
        }
        
        (offset_temp)++;
    }
    
    if(c == '\r' || c == '\n')
    {
        if(c == '\r')
        {
            if(TEXT_BUFFER[offset_temp + 1] == '\n')
            {
                offset_temp++;
            }
        }

        if(is_all_space_between_backslash_and_newline)
        {
            *skip = offset_temp - CURRENT_C_OFFSET + 1;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE void display_check_correct_info(void)
{
    //printf(LIGHT_GREEN"Correct!\n"NONE);
}

PRIVATE ENUM_RETURN cproc_stm_prepare_proc()
{
    
    LINE_STAY;
    CURRENT_C_OFFSET = 0;
    CURRENT_C_LINE = 0;
    CURRENT_C_COLUMN = 0;
    CURRENT_C = INVALID_CHAR;
    LAST_C = INVALID_CHAR;
	
    C_FORWARD(1);
    RESET_TOKEN;
    TYPE_TOKEN(C_TOKEN_INVALID);
    CURRENT_TOKEN_OFFSET = CURRENT_C_OFFSET;
    CURRENT_TOKEN_LINE = CURRENT_C_LINE;
    CURRENT_TOKEN_COLUMN = CURRENT_C_COLUMN;

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_clear_proc()
{
    if(g_cproc_stm_run_data.whether_any_error_exists == BOOLEAN_FALSE)
    {
        display_check_correct_info();
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_preproc()
{
    if(g_cproc_stm_run_data.whether_any_error_exists == BOOLEAN_TRUE)
    {
        STATE_TO(CPROC_STM_END);
        return RETURN_SUCCESS;
    }
    
    CURRENT_C = TEXT_BUFFER[CURRENT_C_OFFSET];

    if(CURRENT_C == '\0')
    {
        STATE_TO(CPROC_STM_END);
        return RETURN_SUCCESS;
    }
    DEBUG_PRINT("current c = \033[7m%c"NONE", offset = %zd, line = %zd, column = %zd\n", CURRENT_C, CURRENT_C_OFFSET, CURRENT_C_LINE, CURRENT_C_COLUMN);

    size_t skip = 0;
    size_t space_num = 0;
    if(CURRENT_C == '\\')
    {
        cproc_continued_newline(&skip, &space_num);
        if(skip != 0)
        {
            SKIP_CONTINUED_NEWLINE(skip, space_num);
            return cproc_stm_preproc();
        }
    }

    C_STAY;
    LINE_STAY;
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_postproc()
{
    if(!C_DIRECTION_IS_FORWARD)
    {
        return RETURN_SUCCESS;
    }

    LAST_C = CURRENT_C;

    CURRENT_C_OFFSET += g_cproc_stm_run_data.c_step;
    CURRENT_C_COLUMN += g_cproc_stm_run_data.c_step;
    if(LINE_DIRECTION_IS_FORWARD)
    {
        CURRENT_C_LINE++;
        CURRENT_C_COLUMN = 0;
    }
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_state_notifier()
{
    return RETURN_SUCCESS;
}



PRIVATE ENUM_RETURN cproc_stm_proc_normal()
{
    TYPE_TOKEN(C_TOKEN_NORMAL);
    _S8 c = CURRENT_C;
    
    switch (c)
    {
        case '"'://this is a test comment
        {/* this is a test comment */
            ADD_TOKEN;

            TYPE_TOKEN(C_TOKEN_STRING);
            OUTPUT_TOKEN_C;
            STATE_TO(CPROC_STM_STRING_DOUBLE_QUOTE);
            break;
        }
        case '\'':
        {
            ADD_TOKEN;
            
            TYPE_TOKEN(C_TOKEN_CHAR);
            OUTPUT_TOKEN_C;
            STATE_TO(CPROC_STM_STRING_SINGLE_QUOTE);
            break;
        }
        case '/':
        {
            ADD_TOKEN;
            if(NEXT_C == '/')
            {
                TYPE_TOKEN(C_TOKEN_LINE_COMMENT);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_ONELINE_COMMENT);
            }
            else if(NEXT_C == '*')
            {
                TYPE_TOKEN(C_TOKEN_PAIR_COMMENT);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_PAIR_COMMENT);
            }
            else if(NEXT_C == '=')
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE_ASSIGN);
                OUTPUT_TOKEN_STR(2);
                ADD_TOKEN;
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
            }                
            break;
        }
        case '(':
        {
            TYPE_TOKEN(C_TOKEN_PARENTHESIS_LEFT);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            break;
        }
        case ')':
        {
            TYPE_TOKEN(C_TOKEN_PARENTHESIS_RIGHT);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            break;
        }
        case '[':
        {
            TYPE_TOKEN(C_TOKEN_BRACKET_LEFT);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            break;
        }
        case ']':
        {
            TYPE_TOKEN(C_TOKEN_BRACKET_RIGHT);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            break;
        }
        case '{':
        {
            TYPE_TOKEN(C_TOKEN_BRACE_LEFT);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            break;
        }
        case '}':
        {
            TYPE_TOKEN(C_TOKEN_BRACE_RIGHT);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            break;
        }
		case '.':
        {
            if(NEXT_C == '.' && NEXT_NEXT_C == '.')
            {
                TYPE_TOKEN(C_TOKEN_VA_ARGS);
                OUTPUT_TOKEN_STR(3);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_MEMBER);
                OUTPUT_TOKEN_C;
            }
            ADD_TOKEN;
            break;
        }
        case ',':
        {
            TYPE_TOKEN(C_TOKEN_COMMA);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            break;
        }
        case ';':
        {
            TYPE_TOKEN(C_TOKEN_SEMICOLON);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            break;
        }
		case '+':
		{
            if(NEXT_C == '=')
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_ADD_ASSIGN);
                OUTPUT_TOKEN_STR(2);
            }
            else if(NEXT_C == '+')
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_INCREASE);
                OUTPUT_TOKEN_STR(2);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_ADD);
                OUTPUT_TOKEN_C;
            }
            ADD_TOKEN;
			break;
		}
		case '-':
		{
            if(NEXT_C == '=')
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_SUBTRACT_ASSIGN);
                OUTPUT_TOKEN_STR(2);
            }
            else if(NEXT_C == '-')
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_DECREASE);
                OUTPUT_TOKEN_STR(2);
            }
            else if(NEXT_C == '>')
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_MEMBER_POINTER);
                OUTPUT_TOKEN_STR(2);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_SUBTRACT);
                OUTPUT_TOKEN_C;
            }
            ADD_TOKEN;
            break;
		}
        case '*':
        {
            if(NEXT_C == '=')
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY_ASSIGN);
                OUTPUT_TOKEN_STR(2);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY);
                OUTPUT_TOKEN_C;
            }
            ADD_TOKEN;
            break;
        }
        case '%':
		{
            if(NEXT_C == '=')
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_MOD_ASSIGN);
                OUTPUT_TOKEN_STR(2);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_MOD);
                OUTPUT_TOKEN_C;
            }
            ADD_TOKEN;
			break;
		}
		case '>':
		{
            if(NEXT_C == '=')
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_LOGICAL_GREATER_EQUAL);
                OUTPUT_TOKEN_STR(2);
            }
            else if(NEXT_C == '>')
            {
                if(NEXT_NEXT_C == '=')
                {
                    TYPE_TOKEN(C_TOKEN_OPERATOR_BIT_RIGHT_SHIFT_ASSIGN);
                    OUTPUT_TOKEN_STR(3);
                }
                else
                {
                    TYPE_TOKEN(C_TOKEN_OPERATOR_BIT_RIGHT_SHIFT);
                    OUTPUT_TOKEN_STR(2);
                }
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_LOGICAL_GREATER);
                OUTPUT_TOKEN_C;
            }
            ADD_TOKEN;
			break;
		}
		case '<':
		{
			if(NEXT_C == '=')
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_LOGICAL_LESS_EQUAL);
                OUTPUT_TOKEN_STR(2);
            }
            else if(NEXT_C == '<')
            {
                if(NEXT_NEXT_C == '=')
                {
                    TYPE_TOKEN(C_TOKEN_OPERATOR_BIT_LEFT_SHIFT_ASSIGN);
                    OUTPUT_TOKEN_STR(3);
                }
                else
                {
                    TYPE_TOKEN(C_TOKEN_OPERATOR_BIT_LEFT_SHIFT);
                    OUTPUT_TOKEN_STR(2);
                }
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_LOGICAL_LESS);
                OUTPUT_TOKEN_C;
            }
            ADD_TOKEN;
			break;
		}
		case '=':
		{
            if(NEXT_C == '=')
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_LOGICAL_EQUAL);
                OUTPUT_TOKEN_STR(2);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ASSIGN);
                OUTPUT_TOKEN_C;
            }
            ADD_TOKEN;
			break;
		}
		case '&':
		{
            if(NEXT_C == '=')
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_BIT_AND_ASSIGN);
                OUTPUT_TOKEN_STR(2);
            }
            else if(NEXT_C == '&')
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_LOGICAL_AND);
                OUTPUT_TOKEN_STR(2);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_BIT_AND);
                OUTPUT_TOKEN_C;
            }
            ADD_TOKEN;
			break;
		}
		case '|':
		{
            if(NEXT_C == '=')
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_BIT_OR_ASSIGN);
                OUTPUT_TOKEN_STR(2);
            }
            else if(NEXT_C == '|')
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_LOGICAL_OR);
                OUTPUT_TOKEN_STR(2);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_BIT_OR);
                OUTPUT_TOKEN_C;
            }
            ADD_TOKEN;
			break;
		}
        case '^':
		{
            if(NEXT_C == '=')
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_BIT_XOR_ASSIGN);
                OUTPUT_TOKEN_STR(2);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_BIT_XOR);
                OUTPUT_TOKEN_C;
            }
            ADD_TOKEN;
			break;
		}
		case '~':
		{
            TYPE_TOKEN(C_TOKEN_OPERATOR_BIT_NOT);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
			break;
		}
		case '!':
		{
            if(NEXT_C == '=')
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_LOGICAL_NOT_EQUAL);
                OUTPUT_TOKEN_STR(2);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_LOGICAL_NOT);
                OUTPUT_TOKEN_C;
            }
            ADD_TOKEN;
			break;
		}
        case '?':
        {
            TYPE_TOKEN(C_TOKEN_OPERATOR_LOGICAL_QUESTION);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            break;
        }
        case ':':
        {
            TYPE_TOKEN(C_TOKEN_OPERATOR_LOGICAL_COLON);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            break;
        }
        case '\\':
        case '@':
        case '`':
        {
            ADD_TOKEN;
            TYPE_TOKEN(C_TOKEN_UNKNOWN);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "stray '%s' in program", LAST_TOKEN_STRING);
            break;
        }
        case ' ':
        case '\t':
        case '\v':
        case '\f':
        {
            ADD_TOKEN;
            STATE_TO(CPROC_STM_BLANK);
            break;
        }
        case '\r':
        case '\n':
        {
            ADD_TOKEN;
            STATE_TO(CPROC_STM_NEWLINE);
            break;
        }
        case '#':
        {
            ADD_TOKEN;
            if(cproc_all_blank_at_begin_of_line() == BOOLEAN_TRUE)
            {
                TYPE_TOKEN(C_TOKEN_PPD);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                
                STATE_TO(CPROC_STM_PP);  
            }
            else
            {
                CPROC_GEN_ERROR(NULL, "stray '%c' in program", CURRENT_C);
            }
            break;
        }
        default:
        {
            if(IS_DEC(c))
            {
                ADD_TOKEN;
                STATE_TO(CPROC_STM_NUMBER);
            }
            else if(C_TOKEN_IS_IDENTIFIER_STARTER(c))
            {
                ADD_TOKEN;
                TYPE_TOKEN(C_TOKEN_IDENTIFIER);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_IDENTIFIER);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_UNKNOWN);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
            }
            break;
            break;
        }
    }

    return RETURN_SUCCESS;
}
//this is a test comment
PRIVATE ENUM_RETURN cproc_stm_proc_string_double_quote()
{
    TYPE_TOKEN(C_TOKEN_STRING);

    switch (CURRENT_C)
    {
        case '"'://a line comment
        {
            OUTPUT_TOKEN_C;
            if(LAST_C != '\\')
            {
                ADD_TOKEN;

                STATE_BACK;
            }
            break;
        }
        case '\r':
        case '\n':
        {
            CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "missing terminating \" character");
            break;
        }
        default:
        {
            OUTPUT_TOKEN_C;
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_string_single_quote()
{
    TYPE_TOKEN(C_TOKEN_CHAR);

    switch (CURRENT_C)
    {
        case '\''://a line comment
        {
            OUTPUT_TOKEN_C;
    
            if(LAST_C != '\\')
            {
                ADD_TOKEN;

                STATE_BACK;
            }

            break;
        }
        case '\r':
        case '\n':
        {
            CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "missing terminating ' character");
            break;
        }
        default:
        {
            OUTPUT_TOKEN_C;
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_oneline_comment()
{
    TYPE_TOKEN(C_TOKEN_LINE_COMMENT);
    switch (CURRENT_C)
    {
        case '\n':
        case '\r':
        {
            ADD_TOKEN;
            STATE_BACK;
            break;
        }
        default:
        {
            OUTPUT_TOKEN_C;
            break;
        }
    }

    return RETURN_SUCCESS;
}


PRIVATE ENUM_RETURN cproc_stm_proc_pair_comment()
{
    TYPE_TOKEN(C_TOKEN_PAIR_COMMENT);

    switch (CURRENT_C)
    {
        case '/':
        {
            OUTPUT_TOKEN_C;
            if(LAST_C == '*')
            {
                ADD_TOKEN;
                STATE_BACK;
            }
            break;
        }
        default:
        {
            OUTPUT_TOKEN_C;
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_newline()
{
    switch (CURRENT_C)
    {
        case '\r':
        {
            TYPE_TOKEN(C_TOKEN_NEWLINE_MAC);
            OUTPUT_TOKEN_C;
            if(NEXT_C != '\n')
            {
                ADD_TOKEN;
            }
            break;
        }
        case '\n':
        {
            if(LAST_C == '\r')
            {
                TYPE_TOKEN(C_TOKEN_NEWLINE_WINDOWS);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_NEWLINE_LINUX);
            }
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            break;
        }

        default:
        {
            ADD_TOKEN;
            STATE_TO(CPROC_STM_NORMAL);
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_blank()
{
    TYPE_TOKEN(C_TOKEN_BLANK);    

    switch (CURRENT_C)
    {
        case ' ':
        case '\t':
        case '\v':
        case '\f':
        {
            OUTPUT_TOKEN_C;
            break;
        }

        default:
        {
            ADD_TOKEN;
            STATE_BACK;
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_identifier()
{
    _S8 c = CURRENT_C;
    if(C_TOKEN_IS_IDENTIFIER_CHAR(c))
    {
        OUTPUT_TOKEN_C;
        return RETURN_SUCCESS;
    }

    END_TOKEN;

    TYPE_TOKEN(s_ctoken_parse_identifier(CURRENT_TOKEN_STR_BUFFER));
    ADD_TOKEN;
    STATE_BACK;
    return RETURN_SUCCESS;
}
PRIVATE ENUM_RETURN cproc_stm_proc_number()
{
    _S8 c = CURRENT_C;
    TYPE_TOKEN(C_TOKEN_CONSTANT_INTEGER);
    if(IS_DEC(c))
    {
        OUTPUT_TOKEN_C;
        return RETURN_SUCCESS;
    }
    ADD_TOKEN;
    STATE_BACK;
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp()
{
    _S8 c = CURRENT_C;
    switch(c)
    {
        case ' ':
        case '\t':
        case '\v':
        case '\f':
        {
            STATE_TO(CPROC_STM_BLANK);
            break;
        }
        case '\r':
        case '\n':
        {
            STATE_TO(CPROC_STM_NEWLINE);
            break;
        }
        case '/':
        {
            if(NEXT_C == '/')
            {
                TYPE_TOKEN(C_TOKEN_LINE_COMMENT);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_ONELINE_COMMENT);
            }
            else if(NEXT_C == '*')
            {
                TYPE_TOKEN(C_TOKEN_PAIR_COMMENT);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_PAIR_COMMENT);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "invalid preprocessing directive #%s", LAST_TOKEN_STRING);
            }                
            break;
        }
        default:
        {
            if(C_TOKEN_IS_IDENTIFIER_STARTER(c))
            {
                TYPE_TOKEN(C_TOKEN_PPD_UNKNOWN);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_PP_DIRECTIVE);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "invalid preprocessing directive #%s", LAST_TOKEN_STRING);
            }
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_directive()
{
    ENUM_C_TOKEN token_type;
    ENUM_RETURN ret_val;
    _S8 c = CURRENT_C;
    if(C_TOKEN_IS_IDENTIFIER_CHAR(c))
    {
        OUTPUT_TOKEN_C;
        return RETURN_SUCCESS;
    }

    ret_val = s_cproc_get_ctoken_pp_keyword(CURRENT_TOKEN_STR_BUFFER, CURRENT_TOKEN_STR_BUFFER_LEN, &token_type);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    TYPE_TOKEN(token_type);
    ADD_TOKEN;

    switch(token_type)
    {
        case C_TOKEN_PPD_IF:
        case C_TOKEN_PPD_IFDEF:
        case C_TOKEN_PPD_IFNDEF:
        {
            STATE_TO(CPROC_STM_PP_IF_GROUP);
            break;
        }
        case C_TOKEN_PPD_ELIF:
        {
            STATE_TO(CPROC_STM_PP_ELIF_GROUP);
            break;
        }
        case C_TOKEN_PPD_ELSE:
        {
            STATE_TO(CPROC_STM_PP_ELSE_GROUP);
            break;
        }
        case C_TOKEN_PPD_ENDIF:
        {
            STATE_TO(CPROC_STM_PP_ENDIF_LINE);
            break;
        }
        case C_TOKEN_PP_CONTROL_INCLUDE:
        {
            STATE_TO(CPROC_STM_PP_INCLUDE);
            break;
        }
        case C_TOKEN_PP_CONTROL_DEFINE:
        {
            STATE_TO(CPROC_STM_PP_DEFINE);
            break;
        }
        case C_TOKEN_PP_CONTROL_UNDEF:
        {
            STATE_TO(CPROC_STM_PP_UNDEF);
            break;
        }
        case C_TOKEN_PP_CONTROL_LINE:
        {
            STATE_TO(CPROC_STM_PP_LINE);
            break;
        }
        case C_TOKEN_PP_CONTROL_ERROR:
        {
            STATE_TO(CPROC_STM_PP_ERROR);
            break;
        }
        case C_TOKEN_PP_CONTROL_PRAGMA:
        {
            STATE_TO(CPROC_STM_PP_PRAGMA);
            break;
        }
        default:
        {
            CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "invalid preprocessing directive #%s", LAST_TOKEN_STRING);
            break;
        }
    }

    return RETURN_SUCCESS;
}
PRIVATE ENUM_RETURN cproc_stm_proc_pp_if_group()
{
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_elif_group()
{
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_else_group()
{
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_endif_line()
{
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_include()
{
    _S8 c = CURRENT_C;

    switch(c)
    {
        case ' ':
        case '\t':
        case '\v':
        case '\f':
        {
            STATE_TO(CPROC_STM_BLANK);
            break;
        }
        case '/':
        {
            if(NEXT_C == '/')
            {
                TYPE_TOKEN(C_TOKEN_LINE_COMMENT);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_ONELINE_COMMENT);
            }
            else if(NEXT_C == '*')
            {
                TYPE_TOKEN(C_TOKEN_PAIR_COMMENT);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_PAIR_COMMENT);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "#include expects \"FILENAME\" or <FILENAME>");
            }                
            break;
        }
        case '\r':
        case '\n':
        {
            CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "#include expects \"FILENAME\" or <FILENAME>");
            break;
        }
        case '<':
        {
            TYPE_TOKEN(C_TOKEN_PP_HEADER_H_START);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            STATE_TO(CPROC_STM_PP_INCLUDE_H_HEADER)
            break;
        }
        case '\"':
        {
            TYPE_TOKEN(C_TOKEN_PP_HEADER_Q_START);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            STATE_TO(CPROC_STM_PP_INCLUDE_Q_HEADER)
            break;
        }
        default:
        {
            TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "#include expects \"FILENAME\" or <FILENAME>");
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_include_h_header()
{
    _S8 c = CURRENT_C;
    
    switch(c)
    {
        case '\r':
        case '\n':
        {
            ADD_TOKEN;
            CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "missing terminating > character");
            break;
        }
        case '>':
        {
            ADD_TOKEN;
            if(LAST_TOKEN_TYPE == C_TOKEN_PP_HEADER_H_START)
            {
                CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "empty filename in #include");
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_PP_HEADER_H_FINISH);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                STATE_TO(CPROC_STM_PP_INCLUDE_FINISH)
            }
            
            break;
        }
        default:
        {
            TYPE_TOKEN(C_TOKEN_PP_HEADER_NAME);
            OUTPUT_TOKEN_C;
            break;
        }
    }

    return RETURN_SUCCESS;
}


PRIVATE ENUM_RETURN cproc_stm_proc_pp_include_q_header()
{
    _S8 c = CURRENT_C;
    
    switch(c)
    {
        case '\r':
        case '\n':
        {
            ADD_TOKEN;
            CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "missing terminating \" character");
            break;
        }
        case '\"':
        {
            ADD_TOKEN;
            if(LAST_TOKEN_TYPE == C_TOKEN_PP_HEADER_Q_START)
            {
                CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "empty filename in #include");
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_PP_HEADER_Q_FINISH);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                STATE_TO(CPROC_STM_PP_INCLUDE_FINISH)
            }
            
            break;
        }
        default:
        {
            TYPE_TOKEN(C_TOKEN_PP_HEADER_NAME);
            OUTPUT_TOKEN_C;
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_include_finish()
{
    ENUM_RETURN ret_val;
    _S8 c = CURRENT_C;
    
    switch(c)
    {
        case ' ':
        case '\t':
        case '\v':
        case '\f':
        {
            ADD_TOKEN;
            if(LAST_TOKEN_TYPE == C_TOKEN_PP_IGNORE)
            {
                CPROC_GEN_WARNING(LAST_TOKEN_POINTER, "extra tokens at end of #include directive");
            }
            STATE_TO(CPROC_STM_BLANK);
            break;
        }
        case '\r':
        case '\n':
        {
            ADD_TOKEN;
            if(LAST_TOKEN_TYPE == C_TOKEN_PP_IGNORE)
            {
                CPROC_GEN_WARNING(LAST_TOKEN_POINTER, "extra tokens at end of #include directive");
            }
            ENUM_RETURN result = RETURN_FAILURE;
            ret_val = s_cproc_include_file(&result);
            S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            if(result == RETURN_FAILURE)
            {
                g_cproc_stm_run_data.whether_any_error_exists = BOOLEAN_TRUE;
            }
            STATE_TO(CPROC_STM_NEWLINE);
            break;
        }
        case '/':
        {
            if(NEXT_C == '/')
            {
                ADD_TOKEN;
                if(LAST_TOKEN_TYPE == C_TOKEN_PP_IGNORE)
                {
                    CPROC_GEN_WARNING(LAST_TOKEN_POINTER, "extra tokens at end of #include directive");
                }
                TYPE_TOKEN(C_TOKEN_LINE_COMMENT);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_ONELINE_COMMENT);
            }
            else if(NEXT_C == '*')
            {
                ADD_TOKEN;
                if(LAST_TOKEN_TYPE == C_TOKEN_PP_IGNORE)
                {
                    CPROC_GEN_WARNING(LAST_TOKEN_POINTER, "extra tokens at end of #include directive");
                }
                TYPE_TOKEN(C_TOKEN_PAIR_COMMENT);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_PAIR_COMMENT);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_PP_IGNORE);
                OUTPUT_TOKEN_C;
            }                
            break;
        }
        default:
        {
            TYPE_TOKEN(C_TOKEN_PP_IGNORE);
            OUTPUT_TOKEN_C;
            
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_define()
{
    _S8 c = CURRENT_C;

    switch(c)
    {
        case ' ':
        case '\t':
        case '\v':
        case '\f':
        {
            STATE_TO(CPROC_STM_BLANK);
            break;
        }
        case '/':
        {
            if(NEXT_C == '/')
            {
                TYPE_TOKEN(C_TOKEN_LINE_COMMENT);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_ONELINE_COMMENT);
            }
            else if(NEXT_C == '*')
            {
                TYPE_TOKEN(C_TOKEN_PAIR_COMMENT);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_PAIR_COMMENT);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "macro names must be identifiers");
            }                
            break;
        }
        case '\r':
        case '\n':
        {
            CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "no macro name given in #define directive");
            break;
        }
        default:
        {
            if(LAST_TOKEN_TYPE != C_TOKEN_BLANK)
            {
                CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "macro names must be identifiers");
                break;
            }
                
            if(C_TOKEN_IS_IDENTIFIER_STARTER(c))
            {
                TYPE_TOKEN(C_TOKEN_PP_IDENTIFIER);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_PP_DEFINE_MACRO);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "macro names must be identifiers");
            }
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_define_macro()
{
    ENUM_RETURN ret_val;
    _S8 c = CURRENT_C;
    if(C_TOKEN_IS_IDENTIFIER_CHAR(c))
    {
        OUTPUT_TOKEN_C;
        return RETURN_SUCCESS;
    }

    ADD_TOKEN;
    ENUM_BOOLEAN macro_exist = BOOLEAN_FALSE;
    ret_val = s_cproc_macro_name_exist(LAST_TOKEN_STRING, &macro_exist);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    if(macro_exist == BOOLEAN_TRUE)
    {
        CPROC_GEN_WARNING(LAST_TOKEN_POINTER, "\"%s\" redefined", LAST_TOKEN_STRING);
    }
    
    ret_val = s_cproc_macro_add_name(LAST_TOKEN_POINTER);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    STATE_TO(CPROC_STM_PP_DEFINE_PARAMETER_START);
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_define_parameter_start()
{
    _S8 c = CURRENT_C;
    switch(c)
    {
        case ' ':
        case '\t':
        case '\v':
        case '\f':
        {
            STATE_TO(CPROC_STM_BLANK);
            break;
        }
        case '/':
        {
            if(NEXT_C == '/')
            {
                TYPE_TOKEN(C_TOKEN_LINE_COMMENT);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_ONELINE_COMMENT);
            }
            else if(NEXT_C == '*')
            {
                TYPE_TOKEN(C_TOKEN_PAIR_COMMENT);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_PAIR_COMMENT);
            }
            else
            {
                if(LAST_TOKEN_TYPE == C_TOKEN_PP_IDENTIFIER)
                {
                    CPROC_GEN_WARNING(LAST_TOKEN_POINTER, "ISO C99 requires whitespace after the macro name");
                }
                STATE_TO(CPROC_STM_PP_DEFINE_REPLACEMENT);
            }                
            break;
        }
        case '\r':
        case '\n':
        {
            STATE_TO(CPROC_STM_NEWLINE);
            break;
        }
        case '(':
        {
            if(LAST_TOKEN_TYPE == C_TOKEN_PP_IDENTIFIER)
            {
                TYPE_TOKEN(C_TOKEN_PARENTHESIS_LEFT);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                STATE_TO(CPROC_STM_PP_DEFINE_PARAMETER_LIST);
            }
            else
            {
                STATE_TO(CPROC_STM_PP_DEFINE_REPLACEMENT);
            }
            break;
        }
        default:
        {
            if(LAST_TOKEN_TYPE == C_TOKEN_PP_IDENTIFIER)
            {
                CPROC_GEN_WARNING(LAST_TOKEN_POINTER, "ISO C99 requires whitespace after the macro name");
            }
            STATE_TO(CPROC_STM_PP_DEFINE_REPLACEMENT);
            break;
        }
    }
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_define_parameter_list()
{
    ENUM_RETURN ret_val;
    ENUM_RETURN check_result;
    
    _S8 c = CURRENT_C;
    switch(c)
    {
        case ' ':
        case '\t':
        case '\v':
        case '\f':
        {
            STATE_TO(CPROC_STM_BLANK);
            break;
        }
        case '/':
        {
            if(NEXT_C == '/')
            {
                TYPE_TOKEN(C_TOKEN_LINE_COMMENT);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_ONELINE_COMMENT);
            }
            else if(NEXT_C == '*')
            {
                TYPE_TOKEN(C_TOKEN_PAIR_COMMENT);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_PAIR_COMMENT);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "\"%s\" may not appear in macro parameter list", LAST_TOKEN_STRING);
            }                
            break;
        }
        case '\r':
        case '\n':
        {
            CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "missing ')' in macro parameter list");
            break;
        }
        case ',':
        {
            TYPE_TOKEN(C_TOKEN_COMMA);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            ret_val = s_cproc_macro_add_parameter_separator_comma(&check_result);
            S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            if(check_result == RETURN_FAILURE)
            {
                CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "parameter name missing");
            }

            break;
        }
        case ')':
        {
            TYPE_TOKEN(C_TOKEN_PARENTHESIS_RIGHT);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            ret_val = s_cproc_macro_finish_parameter(&check_result);
            S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            if(check_result == RETURN_FAILURE)
            {
                CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "parameter name missing");
            }
            else
            {
                STATE_TO(CPROC_STM_PP_DEFINE_REPLACEMENT);
            }
            break;
        }
        default:
        { 
            if(C_TOKEN_IS_IDENTIFIER_STARTER(c))
            {
                TYPE_TOKEN(C_TOKEN_PP_IDENTIFIER);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_PP_DEFINE_PARAMETER);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "\"%s\" may not appear in macro parameter list", LAST_TOKEN_STRING);
            }
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_define_parameter()
{
    ENUM_RETURN ret_val;
    ENUM_RETURN check_result;
    _S8 c = CURRENT_C;
    if(C_TOKEN_IS_IDENTIFIER_CHAR(c))
    {
        OUTPUT_TOKEN_C;
        return RETURN_SUCCESS;
    }

    ADD_TOKEN;
    ret_val = s_cproc_macro_add_parameter(LAST_TOKEN_POINTER, &check_result);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    if(check_result == RETURN_FAILURE)
    {
        CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "macro parameters must be comma-separated");
        return RETURN_SUCCESS;
    }

    STATE_BACK;
    return RETURN_SUCCESS;
}
#if 0
#def\
ine MAC\
RO    this is a valid\
 macro define      

#define hash_hash1 #123 ## #
#define hash_hash(STR) # STR ## ## # STR
#define mkstr(a) # a
#define in_between(a) mkstr(a)
#define join(c, d) in_between(c hash_hash d)

#define hash #
#define STRSS(sd,sd2) #sd##"abc"###sd #sd###sd  
#endif
PRIVATE ENUM_RETURN cproc_stm_proc_pp_define_replacement()
{
    _S8 c = CURRENT_C;
    switch(c)
    {
        case ' ':
        case '\t':
        case '\v':
        case '\f':
        {
            ADD_TOKEN;
            STATE_TO(CPROC_STM_BLANK);
            break;
        }
        case '/':
        {
            if(NEXT_C == '/')
            {
                TYPE_TOKEN(C_TOKEN_LINE_COMMENT);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_ONELINE_COMMENT);
            }
            else if(NEXT_C == '*')
            {
                TYPE_TOKEN(C_TOKEN_PAIR_COMMENT);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_PAIR_COMMENT);
            }
            else
            {
                ADD_TOKEN;
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
            }                
            break;
        }
        case '"'://this is a test comment
        {/* this is a test comment */
            ADD_TOKEN;

            TYPE_TOKEN(C_TOKEN_STRING);
            OUTPUT_TOKEN_C;
            STATE_TO(CPROC_STM_STRING_DOUBLE_QUOTE);
            break;
        }
        case '\'':
        {
            ADD_TOKEN;
            
            TYPE_TOKEN(C_TOKEN_CHAR);
            OUTPUT_TOKEN_C;
            STATE_TO(CPROC_STM_STRING_SINGLE_QUOTE);
            break;
        }
        case '\r':
        case '\n':
        {
            ADD_TOKEN;
            ENUM_RETURN ret_val;
            ret_val = s_cproc_macro_finish_replacement();
            S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

            s_cproc_token_release_list_after_last_newline();

            STATE_TO(CPROC_STM_NEWLINE);
            break;
        }
        case '#':
        {
            ADD_TOKEN;
            if(s_cproc_macro_parameter_part_exist())
            {
                TYPE_TOKEN(C_TOKEN_PPD_STRINGIFICATION);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
            }
            OUTPUT_TOKEN_C;
            if(NEXT_C == '#')
            {
                TYPE_TOKEN(C_TOKEN_PPD_CONCATENATE);
                OUTPUT_TOKEN_C;
            }
            ADD_TOKEN;
            break;
        }
        default:
        {
            if(IS_DEC(c))
            {
                ADD_TOKEN;
                STATE_TO(CPROC_STM_PP_NUMBER);
            }
            else if(C_TOKEN_IS_IDENTIFIER_STARTER(c))
            {
                ADD_TOKEN;
                TYPE_TOKEN(C_TOKEN_PP_IDENTIFIER);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_PP_IDENTIFIER);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
            }
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_number()
{
    _S8 c = CURRENT_C;
    TYPE_TOKEN(C_TOKEN_PP_NUMBER);
    if(IS_DEC(c))
    {
        OUTPUT_TOKEN_C;
        return RETURN_SUCCESS;
    }
    ADD_TOKEN;
    STATE_BACK;
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_header()
{
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_undef()
{
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_line()
{
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_error()
{
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_pragma()
{
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_identifier()
{
    _S8 c = CURRENT_C;
    if(C_TOKEN_IS_IDENTIFIER_CHAR(c))
    {
        OUTPUT_TOKEN_C;
        return RETURN_SUCCESS;
    }

    ADD_TOKEN;
    STATE_BACK;
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_end()
{
    //if there is any error before return to the main process to display the error
    S_R_FALSE(g_cproc_stm_run_data.whether_any_error_exists == BOOLEAN_FALSE, RETURN_SUCCESS);

    //TODO:
    S_R_FALSE(TOKEN_IS_READY_TO_BE_ADDED, RETURN_SUCCESS);

    ADD_TOKEN;
    switch(CURRENT_TOKEN_TYPE)
    {
        case C_TOKEN_PAIR_COMMENT:
        {
            CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "unterminated comment");
            break;
        }
        default:
        {
            CPROC_GEN_ERROR(LAST_TOKEN_POINTER, "unterminated %s", s_ctoken_get_str(LAST_TOKEN_TYPE));
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE STRU_CPROC_STM_PROC cproc_stm_proc[CPROC_STM_MAX] = 
{
    {CPROC_STM_NORMAL, cproc_stm_proc_normal, "normal"},
    {CPROC_STM_STRING_DOUBLE_QUOTE, cproc_stm_proc_string_double_quote, "string double quote"},
    {CPROC_STM_STRING_SINGLE_QUOTE, cproc_stm_proc_string_single_quote, "string single quote"},
    {CPROC_STM_ONELINE_COMMENT, cproc_stm_proc_oneline_comment, "oneline comment"},
    {CPROC_STM_PAIR_COMMENT, cproc_stm_proc_pair_comment, "pair comment"},
    {CPROC_STM_NEWLINE, cproc_stm_proc_newline, "newline"},
    {CPROC_STM_BLANK, cproc_stm_proc_blank, "blank"},
    {CPROC_STM_IDENTIFIER, cproc_stm_proc_identifier, "identifier"},
    {CPROC_STM_NUMBER, cproc_stm_proc_number, "number"},
    {CPROC_STM_PP, cproc_stm_proc_pp, "PP: #"},
    {CPROC_STM_PP_DIRECTIVE, cproc_stm_proc_pp_directive, "PP: directive"},
    {CPROC_STM_PP_IF_GROUP, cproc_stm_proc_pp_if_group, "PP: if-group"},
    {CPROC_STM_PP_ELIF_GROUP, cproc_stm_proc_pp_elif_group, "PP: elif-group"},
    {CPROC_STM_PP_ELSE_GROUP, cproc_stm_proc_pp_else_group, "PP: else-group"},
    {CPROC_STM_PP_ENDIF_LINE, cproc_stm_proc_pp_endif_line, "PP: endif-line"},
    {CPROC_STM_PP_INCLUDE, cproc_stm_proc_pp_include, "PP: include"},
    {CPROC_STM_PP_DEFINE, cproc_stm_proc_pp_define, "PP: define"},
    {CPROC_STM_PP_UNDEF, cproc_stm_proc_pp_undef, "PP: undef"},
    {CPROC_STM_PP_LINE, cproc_stm_proc_pp_line, "PP: line"},
    {CPROC_STM_PP_ERROR, cproc_stm_proc_pp_error, "PP: error"},
    {CPROC_STM_PP_PRAGMA, cproc_stm_proc_pp_pragma, "PP: pragma"},
    {CPROC_STM_PP_INCLUDE_H_HEADER, cproc_stm_proc_pp_include_h_header, "PP: include-h-header"},
    {CPROC_STM_PP_INCLUDE_Q_HEADER, cproc_stm_proc_pp_include_q_header, "PP: include-h-header"},
    {CPROC_STM_PP_INCLUDE_FINISH, cproc_stm_proc_pp_include_finish, "PP: include-finish"},
    {CPROC_STM_PP_DEFINE_MACRO, cproc_stm_proc_pp_define_macro, "PP: define-macro"},
    {CPROC_STM_PP_DEFINE_PARAMETER_START, cproc_stm_proc_pp_define_parameter_start, "PP: define-("},
    {CPROC_STM_PP_DEFINE_PARAMETER_LIST, cproc_stm_proc_pp_define_parameter_list, "PP: define-parameter-list"},
    {CPROC_STM_PP_DEFINE_PARAMETER, cproc_stm_proc_pp_define_parameter, "PP: define-parameter"},
    {CPROC_STM_PP_DEFINE_REPLACEMENT, cproc_stm_proc_pp_define_replacement, "PP: define-replacement"},
    {CPROC_STM_PP_IDENTIFIER, cproc_stm_proc_pp_identifier, "PP: identifier"},
    {CPROC_STM_PP_NUMBER, cproc_stm_proc_pp_number, "PP: number"},
    {CPROC_STM_PP_HEADER, cproc_stm_proc_pp_header, "PP: header"},
    {CPROC_STM_END, cproc_stm_proc_end, "end"},
};

PRIVATE ENUM_RETURN cproc_stm_init(void)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    ret_val = stm_create(&g_cproc_stm_run_data.stm, CPROC_STM_MAX);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = set_stm_start_state(g_cproc_stm_run_data.stm, CPROC_STM_NORMAL);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = set_stm_end_state(g_cproc_stm_run_data.stm, CPROC_STM_END);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_prepare_handler(g_cproc_stm_run_data.stm, cproc_stm_prepare_proc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_clear_handler(g_cproc_stm_run_data.stm, cproc_stm_clear_proc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_preproc_handler(g_cproc_stm_run_data.stm, cproc_stm_preproc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_postproc_handler(g_cproc_stm_run_data.stm, cproc_stm_postproc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_state_notifier(g_cproc_stm_run_data.stm, cproc_stm_state_notifier);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    for(int i = 0; i < SIZE_OF_ARRAY(cproc_stm_proc); i++)
    {
        ret_val = add_stm_state_handler(
            g_cproc_stm_run_data.stm, 
            cproc_stm_proc[i].state, 
            cproc_stm_proc[i].handler, 
            cproc_stm_proc[i].info);
        
        R_ASSERT_LOG(
            ret_val == RETURN_SUCCESS, 
            RETURN_FAILURE, 
            "i = %d, state = %d",
            i, 
            cproc_stm_proc[i].state);
    }
    
    return RETURN_SUCCESS;
}

PRIVATE _VOID cproc_stm_clear(_VOID)
{
    ENUM_RETURN ret_val = stm_delete(&g_cproc_stm_run_data.stm);
    S_V_ASSERT(ret_val == RETURN_SUCCESS);
}

PRIVATE ENUM_RETURN s_cproc_stm_save_up_level_run_data(_VOID)
{
    ENUM_RETURN ret_val;
    if(g_stack == NULL)
    {
        printf("stack create!\n");
        ret_val = stack_create(&g_stack);
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }
    else
    {
        s_cproc_stm_print_debug_info();
        printf("push up level stm data!\n");
        ret_val = stack_push(g_stack, (_VOID *) &g_cproc_stm_run_data, sizeof(STRU_CPROC_STM_RUN_DATA));
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_cproc_stm_restore_up_level_run_data(_VOID)
{
    S_R_ASSERT(g_stack != NULL, RETURN_FAILURE);
    
    ENUM_RETURN ret_val;
    if(stack_is_empty(g_stack))
    {
        printf("stack delete!\n");
        ret_val = stack_delete(&g_stack);
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }
    else
    {
        printf("pop up level stm data!\n");
        size_t size_out = 0;
        ret_val = stack_pop(g_stack, 
            (_VOID *) &g_cproc_stm_run_data,
            &size_out,
            sizeof(STRU_CPROC_STM_RUN_DATA));
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        S_R_ASSERT(size_out == sizeof(STRU_CPROC_STM_RUN_DATA), RETURN_FAILURE);
        s_cproc_stm_print_debug_info();
    }
    
    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_stm_do(
    const _S8 *filename, 
    const _S8 *p_text_buffer, 
    ENUM_RETURN *check_result)
{
    R_ASSERT(p_text_buffer != NULL, RETURN_FAILURE);
    R_ASSERT(check_result != NULL, RETURN_FAILURE);
    R_ASSERT(filename != NULL, RETURN_FAILURE);

    *check_result = RETURN_FAILURE;

    ENUM_RETURN ret_val = cproc_stm_init();
    R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, cproc_stm_clear());

    g_cproc_stm_run_data.filename = filename;
    TEXT_BUFFER = p_text_buffer;
    
    ret_val = stm_run(g_cproc_stm_run_data.stm);
    R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, cproc_stm_clear());

    if(!g_cproc_stm_run_data.whether_any_error_exists)
    {
        *check_result = RETURN_SUCCESS;
    }

    cproc_stm_clear();
    
    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_stm(
    const _S8 *file_name, 
    const _S8 *p_text_buffer, 
    ENUM_RETURN *check_result)
{
    R_ASSERT(p_text_buffer != NULL, RETURN_FAILURE);
    R_ASSERT(check_result != NULL, RETURN_FAILURE);
    R_ASSERT(file_name != NULL, RETURN_FAILURE);

    *check_result = RETURN_FAILURE;

    ENUM_RETURN ret_val;
    ret_val = s_cproc_stm_save_up_level_run_data();
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = s_cproc_stm_do(
            file_name,
            p_text_buffer,
            check_result);
    S_ASSERT(ret_val == RETURN_SUCCESS);

    ret_val = s_cproc_stm_restore_up_level_run_data();
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    return RETURN_SUCCESS;
}
//
