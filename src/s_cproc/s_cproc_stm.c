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
#include "s_cproc_parse.h"
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
    CPROC_STM_PP_INCLUDE_MACRO,
    CPROC_STM_PP_INCLUDE_FINISH,
    CPROC_STM_PP_DEFINE_MACRO,
    CPROC_STM_PP_DEFINE_PARAMETER_START,
    CPROC_STM_PP_DEFINE_PARAMETER_LIST,
    CPROC_STM_PP_DEFINE_PARAMETER_ID,
    CPROC_STM_PP_DEFINE_PARAMETER_ID_VA,
    CPROC_STM_PP_DEFINE_PARAMETER_VA,
    CPROC_STM_PP_DEFINE_PARAMETER_FINISH,
    CPROC_STM_PP_DEFINE_REPLACEMENT,
    CPROC_STM_PP_UNDEF_MACRO,
    CPROC_STM_PP_UNDEF_FINISH,
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
    STRU_C_POSITION current_c_position;
    
    _S32 c;
    _S32 last_c;
    _S32 last_last_c;
    
    _S8 current_token[MAX_TOKEN_LEN];
    _S8 *p_current_token;
    size_t current_token_size;
    STRU_C_POSITION current_token_c_position;
    ENUM_C_TOKEN current_token_type;
    
    ENUM_BOOLEAN whether_any_error_exists;
}STRU_CPROC_STM_RUN_DATA;

#define TEXT_BUFFER g_cproc_stm_run_data.p_text_buffer
#define CURRENT_C_POINTER (TEXT_BUFFER + g_cproc_stm_run_data.current_c_position.offset)
#define CURRENT_C (g_cproc_stm_run_data.c)
#define NEXT_C (*(CURRENT_C_POINTER + 1))

#define LAST_C (g_cproc_stm_run_data.last_c)
#define LAST_LAST_C (g_cproc_stm_run_data.last_last_c)

#define CURRENT_C_OFFSET (g_cproc_stm_run_data.current_c_position.offset)
#define CURRENT_C_LINE (g_cproc_stm_run_data.current_c_position.line)
#define CURRENT_C_COLUMN (g_cproc_stm_run_data.current_c_position.column)

#define CURRENT_TOKEN_STR_BUFFER g_cproc_stm_run_data.current_token
#define CURRENT_TOKEN_STR_BUFFER_POINTER_TO_WRITE (g_cproc_stm_run_data.p_current_token)
#define CURRENT_TOKEN_STR_BUFFER_LEFT_SIZE_TO_WRITE (g_cproc_stm_run_data.current_token_size)
#define CURRENT_TOKEN_STR_BUFFER_LEN (MAX_TOKEN_LEN - CURRENT_TOKEN_STR_BUFFER_LEFT_SIZE_TO_WRITE)
#define CURRENT_TOKEN_TYPE (g_cproc_stm_run_data.current_token_type)
#define CURRENT_TOKEN_OFFSET (g_cproc_stm_run_data.current_token_c_position.offset)
#define CURRENT_TOKEN_LINE (g_cproc_stm_run_data.current_token_c_position.line)
#define CURRENT_TOKEN_COLUMN (g_cproc_stm_run_data.current_token_c_position.column)

#define CURRENT_STATE (get_current_stm_state(g_cproc_stm_run_data.stm))

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
            S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "can not parse token type following %s", S_CPROC_LAST_TOKEN_STRING);\
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
                S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "can not parse token type following %s", S_CPROC_LAST_TOKEN_STRING);\
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
            S_R_ASSERT(S_CPROC_LAST_TOKEN_TYPE == C_TOKEN_PAIR_COMMENT \
                || S_CPROC_LAST_TOKEN_TYPE == C_TOKEN_LINE_COMMENT, RETURN_FAILURE);\
            MOD_LAST_TOKEN_STRING_AND_TYPE(" ", C_TOKEN_BLANK);\
            RESET_TOKEN;\
            LAST_C = ' ';\
            CURRENT_C = ' ';\
            DEBUG_PRINT("replace comment by space");\
            break;\
        }while(0)

#define ADD_TOKEN\
    do{\
        if(!TOKEN_IS_READY_TO_BE_ADDED)\
        {\
            RESET_TOKEN;\
            break;\
        }\
        END_TOKEN;\
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
        RESET_TOKEN;\
    }while(0);

#define MERGE_LAST_2_TOKENS(new_type)\
    do{\
        ENUM_RETURN ___ret_val = s_cproc_token_merge_last_2_nodes(new_type);\
        S_R_ASSERT(___ret_val == RETURN_SUCCESS, RETURN_FAILURE);\
    }while(0);

#define MERGE_LAST_3_TOKENS(new_type)\
    do{\
        ENUM_RETURN ___ret_val = s_cproc_token_merge_last_3_nodes(new_type);\
        S_R_ASSERT(___ret_val == RETURN_SUCCESS, RETURN_FAILURE);\
    }while(0);

#define MOD_LAST_TOKEN_TYPE(new_type)\
    do{\
        ENUM_RETURN ___ret_val = s_cproc_token_mod_last_node_type(new_type);\
        S_R_ASSERT(___ret_val == RETURN_SUCCESS, RETURN_FAILURE);\
    }while(0);

#define MOD_LAST_TOKEN_STRING(new_string)\
    do{\
        ENUM_RETURN ___ret_val = s_cproc_token_mod_last_node_string(new_string);\
        S_R_ASSERT(___ret_val == RETURN_SUCCESS, RETURN_FAILURE);\
    }while(0);

#define MOD_LAST_TOKEN_STRING_AND_TYPE(new_string, new_type)\
    do{\
        ENUM_RETURN ___ret_val = s_cproc_token_mod_last_node_string_and_type(new_string, new_type);\
        S_R_ASSERT(___ret_val == RETURN_SUCCESS, RETURN_FAILURE);\
    }while(0);

#define SKIP_CONTINUED_NEWLINE(skip, space_num)\
    do{\
        DEBUG_PRINT("skip = %zd", skip);\
        if((skip) == 0)\
        {\
            break;\
        }\
        if(get_current_stm_state(g_cproc_stm_run_data.stm) == CPROC_STM_ONELINE_COMMENT)\
        {\
            S_CPROC_STM_GEN_WARNING(S_CPROC_LAST_TOKEN_C_POSITION, "multi-line comment [-Wcomment]");\
        }\
        if((space_num) > 0 && get_current_stm_state(g_cproc_stm_run_data.stm) != CPROC_STM_PAIR_COMMENT && get_current_stm_state(g_cproc_stm_run_data.stm) != CPROC_STM_ONELINE_COMMENT)\
        {\
            S_CPROC_STM_GEN_WARNING(S_CPROC_STM_CURRENT_C_POSITION, "backslash and newline separated by space");\
        }\
        if(TEXT_BUFFER[CURRENT_C_OFFSET + skip + 1] == '\0')\
        {\
            S_CPROC_STM_GEN_WARNING(S_CPROC_STM_CURRENT_C_POSITION, "backslash-newline at end of file");\
        }\
        size_t ___skip = (skip);\
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
    printf("%-20s:%zd\n", "current_c_position.offset", g_cproc_stm_run_data.current_c_position.offset);
    printf("%-20s:%zd\n", "current_c_position.line", g_cproc_stm_run_data.current_c_position.line);
    printf("%-20s:%zd\n", "current_c_position.column", g_cproc_stm_run_data.current_c_position.column);
    printf("%-20s:%c\n", "c", g_cproc_stm_run_data.c);
    printf("%-20s:%c\n", "last_c", g_cproc_stm_run_data.last_c);
    printf("%-20s:%c\n", "last_last_c", g_cproc_stm_run_data.last_last_c);
    printf("%-20s:%p\n", "current_token", g_cproc_stm_run_data.current_token);
    printf("%-20s:%p\n", "p_current_token", g_cproc_stm_run_data.p_current_token);
    printf("%-20s:%zd\n", "current_token_size", g_cproc_stm_run_data.current_token_size);
    printf("%-20s:%zd\n", "current_token_position.offset", g_cproc_stm_run_data.current_token_c_position.offset);
    printf("%-20s:%zd\n", "current_token_position.line", g_cproc_stm_run_data.current_token_c_position.line);
    printf("%-20s:%zd\n", "current_token_position.column", g_cproc_stm_run_data.current_token_c_position.column);
    printf("%-20s:%d\n", "current_token_type", g_cproc_stm_run_data.current_token_type);
    printf("%-20s:%d\n", "whether_any_error_exists", g_cproc_stm_run_data.whether_any_error_exists);
    printf("%-20s:%p\n", "g_stack", g_stack);
}

_VOID s_cproc_stm_generate_error(_VOID)
{
    printf("g_cproc_stm_run_data.whether_any_error_exists = BOOLEAN_TRUE");
    g_cproc_stm_run_data.whether_any_error_exists = BOOLEAN_TRUE;
}

const _S8 *s_cproc_stm_get_text_buffer(_VOID)
{
    return TEXT_BUFFER;
}

const _S8 *s_cproc_stm_get_filename(_VOID)
{
    return g_cproc_stm_run_data.filename;
}

STRU_C_POSITION *s_cproc_stm_get_current_c_position(_VOID)
{
    return &g_cproc_stm_run_data.current_c_position;
}

STRU_C_POSITION *s_cproc_stm_get_current_token_c_position(_VOID)
{
    return TOKEN_IS_READY_TO_BE_ADDED ? &g_cproc_stm_run_data.current_token_c_position: &g_cproc_stm_run_data.current_c_position;
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
    //g_cproc_stm_run_data.move_to_next_line
    LINE_STAY;

    //g_cproc_stm_run_data.current_c_position.offset
    //g_cproc_stm_run_data.current_c_position.line
    //g_cproc_stm_run_data.current_c_position.column
    CURRENT_C_OFFSET = 0;
    CURRENT_C_LINE = 0;
    CURRENT_C_COLUMN = 0;

    //g_cproc_stm_run_data.c
    CURRENT_C = INVALID_CHAR;

    //g_cproc_stm_run_data.last_c
    LAST_C = INVALID_CHAR;

    //g_cproc_stm_run_data.last_last_c
    LAST_LAST_C = INVALID_CHAR;

    //g_cproc_stm_run_data.c_direction
    //g_cproc_stm_run_data.c_step;
    C_FORWARD(1);

    //g_cproc_stm_run_data.current_token
    //g_cproc_stm_run_data.current_token_size
    //g_cproc_stm_run_data.p_current_token
    //g_cproc_stm_run_data.current_token_type
    RESET_TOKEN;

    //g_cproc_stm_run_data.current_token_type
    TYPE_TOKEN(C_TOKEN_INVALID);

    //g_cproc_stm_run_data.current_token_c_position.offset
    //g_cproc_stm_run_data.current_token_c_position.line
    //g_cproc_stm_run_data.current_token_c_position.column
    CURRENT_TOKEN_OFFSET = CURRENT_C_OFFSET;
    CURRENT_TOKEN_LINE = CURRENT_C_LINE;
    CURRENT_TOKEN_COLUMN = CURRENT_C_COLUMN;

    g_cproc_stm_run_data.whether_any_error_exists = BOOLEAN_FALSE;
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

    LAST_LAST_C = LAST_C;
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
    _S8 c = CURRENT_C;
    
    switch (c)
    {
        case '"'://this is a test comment
        {/* this is a test comment */
            TYPE_TOKEN(C_TOKEN_STRING);
            OUTPUT_TOKEN_C;
            STATE_TO(CPROC_STM_STRING_DOUBLE_QUOTE);
            break;
        }
        case '\'':
        {
            TYPE_TOKEN(C_TOKEN_CHAR);
            OUTPUT_TOKEN_C;
            STATE_TO(CPROC_STM_STRING_SINGLE_QUOTE);
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
            TYPE_TOKEN(C_TOKEN_OPERATOR_MEMBER);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            if(LAST_C == '.' && LAST_LAST_C == '.')
            {
                MERGE_LAST_3_TOKENS(C_TOKEN_VA_ARGS);
            }
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
            TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_ADD);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            
            if(LAST_C == '+')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_ARITHMETIC_INCREASE);
            }
            
			break;
		}
		case '-':
		{
            TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_SUBTRACT);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;

            if(LAST_C == '-')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_ARITHMETIC_DECREASE);
            }
            
            break;
		}
        case '*':
        {
            TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            if(LAST_C == '/')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_PAIR_COMMENT);
                STATE_TO(CPROC_STM_PAIR_COMMENT);
            }
            
            break;
        }
        case '/':
        {
            TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            
            if(LAST_C == '/')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_LINE_COMMENT);
                STATE_TO(CPROC_STM_ONELINE_COMMENT);
            }             
            break;
        }
        case '%':
		{
            TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_MOD);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
			break;
		}
		case '>':
		{
            TYPE_TOKEN(C_TOKEN_OPERATOR_LOGICAL_GREATER);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;

            if(LAST_C == '-')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_MEMBER_POINTER);
            }
            else if(LAST_C == '>')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_BIT_RIGHT_SHIFT);
            }
            
			break;
		}
		case '<':
		{
            TYPE_TOKEN(C_TOKEN_OPERATOR_LOGICAL_LESS);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
			if(LAST_C == '<')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_BIT_LEFT_SHIFT);
            }
            
			break;
		}
		case '=':
		{
            TYPE_TOKEN(C_TOKEN_OPERATOR_ASSIGN);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            if(LAST_C == '+')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_ARITHMETIC_ADD_ASSIGN);
            }
            else if(LAST_C == '-')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_ARITHMETIC_SUBTRACT_ASSIGN);
            }
            else if(LAST_C == '*')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY_ASSIGN);
            }
            else if(LAST_C == '/')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE_ASSIGN);
            }
            else if(LAST_C == '%')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_ARITHMETIC_MOD_ASSIGN);
            }
            else if(LAST_C == '>')
            {
                if(LAST_LAST_C == '>')
                {
                    MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_BIT_RIGHT_SHIFT_ASSIGN);
                }
                else
                {
                    MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_LOGICAL_GREATER_EQUAL);
                }
            }
            else if(LAST_C == '<')
            {
                
                if(LAST_LAST_C == '<')
                {
                    MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_BIT_LEFT_SHIFT_ASSIGN);
                }
                else
                {
                    MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_LOGICAL_LESS_EQUAL);
                }
            }
            else if(LAST_C == '=')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_LOGICAL_EQUAL);
            }
            else if(LAST_C == '!')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_LOGICAL_NOT_EQUAL);
            }
            else if(LAST_C == '&')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_BIT_AND_ASSIGN);
            }
            else if(LAST_C == '|')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_BIT_OR_ASSIGN);
            }
            else if(LAST_C == '^')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_BIT_XOR_ASSIGN);
            }
            else if(LAST_C == '^')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_BIT_XOR_ASSIGN);
            }
			break;
		}
		case '&':
		{
            TYPE_TOKEN(C_TOKEN_OPERATOR_BIT_AND);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            if(LAST_C == '&')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_LOGICAL_AND);
            }
			break;
		}
		case '|':
		{
            TYPE_TOKEN(C_TOKEN_OPERATOR_BIT_OR);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            if(LAST_C == '&')
            {
                MERGE_LAST_2_TOKENS(C_TOKEN_OPERATOR_LOGICAL_OR);
            }

			break;
		}
        case '^':
		{
            TYPE_TOKEN(C_TOKEN_OPERATOR_BIT_XOR);
            OUTPUT_TOKEN_C;
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
            TYPE_TOKEN(C_TOKEN_OPERATOR_LOGICAL_NOT);
            OUTPUT_TOKEN_C;
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
            TYPE_TOKEN(C_TOKEN_PUNCTUATOR);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            //S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "stray '%s' in program", S_CPROC_LAST_TOKEN_STRING);
            break;
        }
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
        case '#':
        {
            if(s_cproc_token_all_blank_in_line() == BOOLEAN_TRUE)
            {
                TYPE_TOKEN(C_TOKEN_PPD);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                
                STATE_TO(CPROC_STM_PP);  
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_PUNCTUATOR);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                //S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "stray '%s' in program", S_CPROC_LAST_TOKEN_STRING);
            }
            break;
        }
        default:
        {
            if(IS_DEC(c))
            {
                STATE_TO(CPROC_STM_NUMBER);
            }
            else if(C_TOKEN_IS_IDENTIFIER_STARTER(c))
            {
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
            ADD_TOKEN;
            S_CPROC_STM_GEN_WARNING(S_CPROC_LAST_TOKEN_C_POSITION, "missing terminating \" character");
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
            ADD_TOKEN;
            S_CPROC_STM_GEN_WARNING(S_CPROC_LAST_TOKEN_C_POSITION, "missing terminating ' character");
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

PRIVATE ENUM_RETURN cproc_stm_proc_oneline_comment()
{
    TYPE_TOKEN(C_TOKEN_LINE_COMMENT);
    switch (CURRENT_C)
    {
        case '\n':
        case '\r':
        {
            REPLACE_TOKEN;
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
                REPLACE_TOKEN;
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
            TYPE_TOKEN(C_TOKEN_NEWLINE);
            OUTPUT_TOKEN_C;
            if(NEXT_C != '\n')
            {
                ADD_TOKEN;
            }
            break;
        }
        case '\n':
        {
            TYPE_TOKEN(C_TOKEN_NEWLINE);
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
    ENUM_RETURN ret_val;
    _S8 c = CURRENT_C;
    if(C_TOKEN_IS_IDENTIFIER_CHAR(c))
    {
        OUTPUT_TOKEN_C;
        return RETURN_SUCCESS;
    }

    ADD_TOKEN;

    ENUM_C_TOKEN token_type;
    ret_val = s_cproc_parse_word(S_CPROC_LAST_TOKEN_STRING, &token_type);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    MOD_LAST_TOKEN_TYPE(token_type);
    
    
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
    if(LAST_C == '/')
    {
        switch(c)
        {
            case '*':
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                MERGE_LAST_2_TOKENS(C_TOKEN_PAIR_COMMENT);
                STATE_TO(CPROC_STM_PAIR_COMMENT);
                break;
            }
            case '/':
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                
                MERGE_LAST_2_TOKENS(C_TOKEN_LINE_COMMENT);
                STATE_TO(CPROC_STM_ONELINE_COMMENT);
                break;
            }
            default:
            {
                printf("9999999999999999\n");
                S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "invalid preprocessing directive #%s", S_CPROC_LAST_TOKEN_STRING);
            }
        }

        return RETURN_SUCCESS;
    }
    
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
            TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
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
                S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "invalid preprocessing directive #%s", S_CPROC_LAST_TOKEN_STRING);
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

    ADD_TOKEN;
    ret_val = s_cproc_parse_pp_directive(S_CPROC_LAST_TOKEN_STRING, &token_type);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    MOD_LAST_TOKEN_TYPE(token_type);
    

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
            S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "invalid preprocessing directive #%s", S_CPROC_LAST_TOKEN_STRING);
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_if_group()
{
    S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "invalid preprocessing directive #%s", S_CPROC_LAST_TOKEN_STRING);
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_elif_group()
{
    S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "invalid preprocessing directive #%s", S_CPROC_LAST_TOKEN_STRING);
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_else_group()
{
    S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "invalid preprocessing directive #%s", S_CPROC_LAST_TOKEN_STRING);
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_endif_line()
{
    S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "invalid preprocessing directive #%s", S_CPROC_LAST_TOKEN_STRING);
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_include()
{
    _S8 c = CURRENT_C;
    if(LAST_C == '/')
    {
        switch(c)
        {
            case '*':
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                MERGE_LAST_2_TOKENS(C_TOKEN_PAIR_COMMENT);
                STATE_TO(CPROC_STM_PAIR_COMMENT);
                break;
            }
            case '/':
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                
                MERGE_LAST_2_TOKENS(C_TOKEN_LINE_COMMENT);
                STATE_TO(CPROC_STM_ONELINE_COMMENT);
                break;
            }
            default:
            {
                S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "#include expects \"FILENAME\" or <FILENAME>");
            }
        }

        return RETURN_SUCCESS;
    }

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
            TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
                               
            break;
        }
        case '\r':
        case '\n':
        {
            S_CPROC_STM_GEN_ERROR(S_CPROC_STM_CURRENT_C_POSITION, "#include expects \"FILENAME\" or <FILENAME>");
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
            if(C_TOKEN_IS_IDENTIFIER_STARTER(c))
            {
                TYPE_TOKEN(C_TOKEN_PP_IDENTIFIER);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_PP_INCLUDE_MACRO);
            }
            TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "#include expects \"FILENAME\" or <FILENAME>");
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
            S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_LAST_TOKEN_C_POSITION, "missing terminating > character");
            S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_LAST_TOKEN_C_POSITION, "#include expects \"FILENAME\" or <FILENAME>");
            break;
        }
        case '>':
        {
            ADD_TOKEN;
            if(S_CPROC_LAST_TOKEN_TYPE == C_TOKEN_PP_HEADER_H_START)
            {
                S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "empty filename in #include");
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
            TYPE_TOKEN(C_TOKEN_PP_HEADER_H_NAME);
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
            S_CPROC_STM_GEN_WARNING(S_CPROC_LAST_LAST_TOKEN_C_POSITION, "missing terminating \" character");
            S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_LAST_TOKEN_C_POSITION, "#include expects \"FILENAME\" or <FILENAME>");
            break;
        }
        case '\"':
        {
            ADD_TOKEN;
            if(S_CPROC_LAST_TOKEN_TYPE == C_TOKEN_PP_HEADER_Q_START)
            {
                S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "empty filename in #include");
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
            TYPE_TOKEN(C_TOKEN_PP_HEADER_Q_NAME);
            OUTPUT_TOKEN_C;
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_include_macro()
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
    ret_val = s_cproc_macro_name_exist(S_CPROC_LAST_TOKEN_STRING, &macro_exist);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    if(macro_exist == BOOLEAN_FALSE)
    {
        S_CPROC_STM_GEN_WARNING(S_CPROC_LAST_TOKEN_C_POSITION, "#include expects \"FILENAME\" or <FILENAME>");
    }
    
    ret_val = s_cproc_macro_add_name(S_CPROC_LAST_TOKEN_POINTER);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    STATE_TO(CPROC_STM_PP_DEFINE_PARAMETER_START);
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_include_finish()
{
    ENUM_RETURN ret_val;
    _S8 c = CURRENT_C;
    if(LAST_C == '/')
    {
        switch(c)
        {
            case '*':
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                MERGE_LAST_2_TOKENS(C_TOKEN_PAIR_COMMENT);
                STATE_TO(CPROC_STM_PAIR_COMMENT);
                return RETURN_SUCCESS;
                break;
            }
            case '/':
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                
                MERGE_LAST_2_TOKENS(C_TOKEN_LINE_COMMENT);
                STATE_TO(CPROC_STM_ONELINE_COMMENT);
                return RETURN_SUCCESS;
                break;
            }
            default:
            {
            }
        }
    }
    
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
        case '\r':
        case '\n':
        {
            ADD_TOKEN;
            ENUM_RETURN result = RETURN_FAILURE;
            ret_val = s_cproc_include_file(&result);
            S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            if(result == RETURN_FAILURE)
            {
                printf("g_cproc_stm_run_data.whether_any_error_exists = BOOLEAN_TRUE");
                g_cproc_stm_run_data.whether_any_error_exists = BOOLEAN_TRUE;
            }
            STATE_TO(CPROC_STM_NEWLINE);
            break;
        }
        case '/':
        {
            ADD_TOKEN;
            TYPE_TOKEN(C_TOKEN_PP_IGNORE);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
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
    if(LAST_C == '/')
    {
        switch(c)
        {
            case '*':
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                MERGE_LAST_2_TOKENS(C_TOKEN_PAIR_COMMENT);
                STATE_TO(CPROC_STM_PAIR_COMMENT);
                break;
            }
            case '/':
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                
                MERGE_LAST_2_TOKENS(C_TOKEN_LINE_COMMENT);
                STATE_TO(CPROC_STM_ONELINE_COMMENT);
                break;
            }
            default:
            {
                S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "macro names must be identifiers");
            }
        }

        return RETURN_SUCCESS;
    }

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
            TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            break;
        }
        case '\r':
        case '\n':
        {
            S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "no macro name given in #define directive");
            break;
        }
        default:
        { 
            if(C_TOKEN_IS_IDENTIFIER_STARTER(c))
            {
                TYPE_TOKEN(C_TOKEN_PP_MACRO);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_PP_DEFINE_MACRO);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "macro names must be identifiers");
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
    ret_val = s_cproc_macro_name_exist(S_CPROC_LAST_TOKEN_STRING, &macro_exist);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    if(macro_exist == BOOLEAN_TRUE)
    {
        S_CPROC_STM_GEN_WARNING(S_CPROC_LAST_TOKEN_C_POSITION, "\"%s\" redefined", S_CPROC_LAST_TOKEN_STRING);
    }
    
    ret_val = s_cproc_macro_add_name(S_CPROC_LAST_TOKEN_POINTER);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    STATE_TO(CPROC_STM_PP_DEFINE_PARAMETER_START);
    return RETURN_SUCCESS;
}
#define abc/
PRIVATE ENUM_RETURN cproc_stm_proc_pp_define_parameter_start()
{
    _S8 c = CURRENT_C;

    switch(c)
    {
        case '(':
        {

            TYPE_TOKEN(C_TOKEN_PP_PARAMETER_START);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            STATE_TO(CPROC_STM_PP_DEFINE_PARAMETER_LIST);
            break;
        }
        default:
        {
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
    if(LAST_C == '/')
    {
        switch(c)
        {
            case '*':
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                MERGE_LAST_2_TOKENS(C_TOKEN_PAIR_COMMENT);
                STATE_TO(CPROC_STM_PAIR_COMMENT);
                break;
            }
            case '/':
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                
                MERGE_LAST_2_TOKENS(C_TOKEN_LINE_COMMENT);
                STATE_TO(CPROC_STM_ONELINE_COMMENT);
                break;
            }
            default:
            {
                S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "\"%s\" may not appear in macro parameter list", S_CPROC_LAST_TOKEN_STRING);
            }
        }

        return RETURN_SUCCESS;
    }
    
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
            TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;  
            break;
        }
        case '\r':
        case '\n':
        {
            S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "missing ')' in macro parameter list");
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
                S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "parameter name missing");
            }

            break;
        }
        case ')':
        {
            STATE_TO(CPROC_STM_PP_DEFINE_PARAMETER_FINISH);
            break;
        }
        case '.':
        {
            STATE_TO(CPROC_STM_PP_DEFINE_PARAMETER_VA);
            break;
        }
        default:
        {
            if(C_TOKEN_IS_IDENTIFIER_STARTER(CURRENT_C))
            {
                TYPE_TOKEN(C_TOKEN_PP_PARAMETER_ID);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_PP_DEFINE_PARAMETER_ID);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "\"%s\" may not appear in macro parameter list", S_CPROC_LAST_TOKEN_STRING);
            }
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_define_parameter_id()
{
    ENUM_RETURN ret_val;
    _S8 c = CURRENT_C;
    if(C_TOKEN_IS_IDENTIFIER_CHAR(c))
    {
        OUTPUT_TOKEN_C;
        return RETURN_SUCCESS;
    }
    ADD_TOKEN;

    if(c == '.')
    {
        STATE_TO(CPROC_STM_PP_DEFINE_PARAMETER_ID_VA);
        return RETURN_SUCCESS;
    }

    ret_val = s_cproc_macro_add_parameter(S_CPROC_LAST_TOKEN_POINTER);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    STATE_TO(CPROC_STM_PP_DEFINE_PARAMETER_LIST);

    return RETURN_SUCCESS;
}


PRIVATE ENUM_RETURN cproc_stm_proc_pp_define_parameter_id_va()
{
    ENUM_RETURN ret_val;
    _S8 c = CURRENT_C;

    TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
    OUTPUT_TOKEN_C;
    
    if(c == '.')
    {
        return RETURN_SUCCESS;
    }
    
    ADD_TOKEN;

    ENUM_BOOLEAN is_va_suffix = BOOLEAN_FALSE;
    ret_val = s_cproc_parse_pp_is_va_suffix(S_CPROC_LAST_TOKEN_STRING, &is_va_suffix);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    if(is_va_suffix == BOOLEAN_TRUE)
    {
        MERGE_LAST_2_TOKENS(C_TOKEN_PP_PARAMETER_ID_VA);

        ret_val = s_cproc_macro_add_parameter(S_CPROC_LAST_TOKEN_POINTER);
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        
        STATE_TO(CPROC_STM_PP_DEFINE_PARAMETER_FINISH);
        return RETURN_SUCCESS;
    }
    
    S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "\"%s\" may not appear in macro parameter list", S_CPROC_LAST_TOKEN_STRING);
    STATE_TO(CPROC_STM_PP_DEFINE_PARAMETER_LIST);
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_define_parameter_va()
{
    ENUM_RETURN ret_val;
    _S8 c = CURRENT_C;
    TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
    OUTPUT_TOKEN_C;
    
    if(c == '.')
    {
        return RETURN_SUCCESS;
    }
    ADD_TOKEN;

    ENUM_BOOLEAN is_va_suffix = BOOLEAN_FALSE;
    ret_val = s_cproc_parse_pp_is_va_suffix(S_CPROC_LAST_TOKEN_STRING, &is_va_suffix);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    if(is_va_suffix == BOOLEAN_TRUE)
    {
        MOD_LAST_TOKEN_TYPE(C_TOKEN_PP_PARAMETER_ID_VA);
        
        ret_val = s_cproc_macro_add_parameter(S_CPROC_LAST_TOKEN_POINTER);
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        
        STATE_TO(CPROC_STM_PP_DEFINE_PARAMETER_FINISH);
        return RETURN_SUCCESS;
    }
    
    S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "\"%s\" may not appear in macro parameter list", S_CPROC_LAST_TOKEN_STRING);
    STATE_TO(CPROC_STM_PP_DEFINE_PARAMETER_LIST);
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_define_parameter_finish()
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
                S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "missing ')' in macro parameter list");
            }                
            break;
        }
        case '\r':
        case '\n':
        {
            S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "missing ')' in macro parameter list");
            break;
        }
        case ')':
        {
            TYPE_TOKEN(C_TOKEN_PP_PARAMETER_FINISH);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            ret_val = s_cproc_macro_finish_parameter(&check_result);
            S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            if(check_result == RETURN_FAILURE)
            {
                S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "parameter name missing");
            }
            else
            {
                STATE_TO(CPROC_STM_PP_DEFINE_REPLACEMENT);
            }
            break;
        }
        default:
        {

            TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "missing ')' in macro parameter list");

            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_define_replacement()
{
    _S8 c = CURRENT_C;
    if(LAST_C == '/')
    {
        switch(c)
        {
            case '*':
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;

                MERGE_LAST_2_TOKENS(C_TOKEN_PAIR_COMMENT);
                STATE_TO(CPROC_STM_PAIR_COMMENT);
                return RETURN_SUCCESS;
                break;
            }
            case '/':
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                
                MERGE_LAST_2_TOKENS(C_TOKEN_LINE_COMMENT);
                STATE_TO(CPROC_STM_ONELINE_COMMENT);
                return RETURN_SUCCESS;
                break;
            }
            default:
            {
            }
        }
    }
    else if(LAST_C == '#')
    {
        if(c == '#')
        {
            TYPE_TOKEN(C_TOKEN_PPD_CONCATENATE);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            MERGE_LAST_2_TOKENS(C_TOKEN_PPD_CONCATENATE);
            return RETURN_SUCCESS;
        }
    }
    
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
            TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            break;
        }
        case '"'://this is a test comment
        {/* this is a test comment */
            TYPE_TOKEN(C_TOKEN_STRING);
            OUTPUT_TOKEN_C;
            STATE_TO(CPROC_STM_STRING_DOUBLE_QUOTE);
            break;
        }
        case '\'':
        {
            TYPE_TOKEN(C_TOKEN_CHAR);
            OUTPUT_TOKEN_C;
            STATE_TO(CPROC_STM_STRING_SINGLE_QUOTE);
            break;
        }
        case '\r':
        case '\n':
        {
            ENUM_RETURN ret_val;
            ret_val = s_cproc_macro_finish_replacement();
            S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

            s_cproc_token_release_list_after_last_newline();

            STATE_TO(CPROC_STM_NEWLINE);
            break;
        }
        case '#':
        {
            if(s_cproc_macro_parameter_part_exist())
            {
                TYPE_TOKEN(C_TOKEN_PPD_STRINGIFICATION);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
            }
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            
            break;
        }
        default:
        {
            if(IS_DEC(c))
            {
                STATE_TO(CPROC_STM_PP_NUMBER);
            }
            else if(C_TOKEN_IS_IDENTIFIER_STARTER(c))
            {
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

PRIVATE ENUM_RETURN cproc_stm_proc_pp_undef()
{
    _S8 c = CURRENT_C;
    if(LAST_C == '/')
    {
        switch(c)
        {
            case '*':
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                MERGE_LAST_2_TOKENS(C_TOKEN_PAIR_COMMENT);
                STATE_TO(CPROC_STM_PAIR_COMMENT);
                break;
            }
            case '/':
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                
                MERGE_LAST_2_TOKENS(C_TOKEN_LINE_COMMENT);
                STATE_TO(CPROC_STM_ONELINE_COMMENT);
                break;
            }
            default:
            {
                S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "macro names must be identifiers");
            }
        }

        return RETURN_SUCCESS;
    }

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
            TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
            break;
        }
        case '\r':
        case '\n':
        {
            S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "no macro name given in #define directive");
            break;
        }
        default:
        {  
            if(C_TOKEN_IS_IDENTIFIER_STARTER(c))
            {
                TYPE_TOKEN(C_TOKEN_PP_MACRO);
                OUTPUT_TOKEN_C;
                STATE_TO(CPROC_STM_PP_UNDEF_MACRO);
            }
            else
            {
                TYPE_TOKEN(C_TOKEN_PP_PUNCTUATOR);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "macro names must be identifiers");
            }
            break;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_undef_macro()
{
    ENUM_RETURN ret_val;
    _S8 c = CURRENT_C;
    if(C_TOKEN_IS_IDENTIFIER_CHAR(c))
    {
        OUTPUT_TOKEN_C;
        return RETURN_SUCCESS;
    }

    ADD_TOKEN;

    ret_val = s_cproc_macro_remove_name(S_CPROC_LAST_TOKEN_POINTER);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    STATE_TO(CPROC_STM_PP_UNDEF_FINISH);
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_pp_undef_finish()
{
    _S8 c = CURRENT_C;
    if(LAST_C == '/')
    {
        switch(c)
        {
            case '*':
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_MULTIPLY);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                MERGE_LAST_2_TOKENS(C_TOKEN_PAIR_COMMENT);
                STATE_TO(CPROC_STM_PAIR_COMMENT);
                return RETURN_SUCCESS;
                break;
            }
            case '/':
            {
                TYPE_TOKEN(C_TOKEN_OPERATOR_ARITHMETIC_DIVIDE);
                OUTPUT_TOKEN_C;
                ADD_TOKEN;
                
                MERGE_LAST_2_TOKENS(C_TOKEN_LINE_COMMENT);
                STATE_TO(CPROC_STM_ONELINE_COMMENT);
                return RETURN_SUCCESS;
                break;
            }
            default:
            {
            }
        }
    }
    
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
        case '\r':
        case '\n':
        {
            ADD_TOKEN;

            STRU_C_TOKEN_NODE *p_token_temp = s_cproc_token_get_last_node_by_type(C_TOKEN_PP_IDENTIFIER);
            S_R_ASSERT(p_token_temp != NULL, RETURN_FAILURE);
            
            ENUM_BOOLEAN first_ignore_token = BOOLEAN_TRUE;
            while((p_token_temp = s_cproc_token_get_next_node(p_token_temp)) != NULL)
            {
                if(p_token_temp->info.token_type == C_TOKEN_PP_IGNORE && first_ignore_token)
                {
                    first_ignore_token = BOOLEAN_FALSE;
                    S_CPROC_STM_GEN_WARNING(&p_token_temp->info.c_position, "extra tokens at end of #undef directive");
                }
                else
                {
                    first_ignore_token = BOOLEAN_TRUE;
                }
            }
            
            STATE_TO(CPROC_STM_NEWLINE);
            break;
        }
        case '/':
        {
            ADD_TOKEN;
            TYPE_TOKEN(C_TOKEN_PP_IGNORE);
            OUTPUT_TOKEN_C;
            ADD_TOKEN;
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
    
    if(strcmp(S_CPROC_LAST_TOKEN_STRING, "__VA_ARGS__") == 0)
    {
        MOD_LAST_TOKEN_TYPE(C_TOKEN_PP_IDENTIFIER_VA);
    }

    STATE_BACK;
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN cproc_stm_proc_end()
{
    //if there is any error before return to the main process to display the error
    S_R_FALSE(g_cproc_stm_run_data.whether_any_error_exists == BOOLEAN_FALSE, RETURN_SUCCESS);

    //TODO:
    S_R_FALSE(TOKEN_IS_READY_TO_BE_ADDED, RETURN_SUCCESS);

    switch(CURRENT_TOKEN_TYPE)
    {
        case C_TOKEN_PAIR_COMMENT:
        {
            S_CPROC_STM_GEN_ERROR(S_CPROC_LAST_TOKEN_C_POSITION, "unterminated comment");
            break;
        }
        default:
        {
            S_CPROC_STM_GEN_ERROR(S_CPROC_STM_CURRENT_TOKEN_C_POSITION, "unterminated %s", s_ctoken_get_str(CURRENT_TOKEN_TYPE));
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
    {CPROC_STM_PP_INCLUDE_MACRO, cproc_stm_proc_pp_include_macro, "PP: include-macro"},
    {CPROC_STM_PP_INCLUDE_FINISH, cproc_stm_proc_pp_include_finish, "PP: include-finish"},
    {CPROC_STM_PP_DEFINE_MACRO, cproc_stm_proc_pp_define_macro, "PP: define-macro"},
    {CPROC_STM_PP_DEFINE_PARAMETER_START, cproc_stm_proc_pp_define_parameter_start, "PP: define-("},
    {CPROC_STM_PP_DEFINE_PARAMETER_LIST, cproc_stm_proc_pp_define_parameter_list, "PP: define-parameter-list"},
    {CPROC_STM_PP_DEFINE_PARAMETER_ID, cproc_stm_proc_pp_define_parameter_id, "PP: define-parameter-id"},
    {CPROC_STM_PP_DEFINE_PARAMETER_ID_VA, cproc_stm_proc_pp_define_parameter_id_va, "PP: define-parameter-id_va"},
    {CPROC_STM_PP_DEFINE_PARAMETER_VA, cproc_stm_proc_pp_define_parameter_va, "PP: define-parameter-va"},
    {CPROC_STM_PP_DEFINE_PARAMETER_FINISH, cproc_stm_proc_pp_define_parameter_finish, "PP: define-)"},
    {CPROC_STM_PP_DEFINE_REPLACEMENT, cproc_stm_proc_pp_define_replacement, "PP: define-replacement"},
    {CPROC_STM_PP_UNDEF_MACRO, cproc_stm_proc_pp_undef_macro, "PP: undef-macro"},
    {CPROC_STM_PP_UNDEF_FINISH, cproc_stm_proc_pp_undef_finish, "PP: undef-finish"},
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

    if(g_cproc_stm_run_data.whether_any_error_exists == BOOLEAN_FALSE)
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
