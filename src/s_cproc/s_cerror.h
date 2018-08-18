#ifndef __S_CERROR_H__
#define __S_CERROR_H__
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"

#define S_C_EMIT_ERROR(filename, p_text_buffer, offset, line, column, fmt, args...)\
	do{\
		printf(WHITE"%s:%zd:%zd: "LIGHT_RED"error: "NONE""fmt"\n",\
			filename, line + 1, column +1, ##args);\
		display_token_and_line(p_text_buffer, offset);\
	}while(0)
#define S_C_EMIT_WARNING(filename, p_text_buffer, offset, line, column, fmt, args...)\
	do{\
		printf(WHITE"%s:%zd:%zd: "LIGHT_PURPLE"warning: "NONE""fmt"\n",\
			filename, line + 1, column +1, ##args);\
		display_token_and_line(p_text_buffer, offset);\
	}while(0)


#define CPROC_GEN_ERROR(p_token, fmt, args...)\
    do{\
        STRU_C_TOKEN_NODE *___p_token_temp = p_token;\
        size_t ___offset;\
        size_t ___line;\
        size_t ___column;\
        if(___p_token_temp != NULL)\
        {\
            ___offset = ___p_token_temp->info.line_position.offset;\
            ___line = ___p_token_temp->info.line_position.line;\
            ___column = ___p_token_temp->info.line_position.column;\
        }\
        else\
        {\
            ___offset = s_cproc_get_current_offset();\
            ___line = s_cproc_get_current_line();\
            ___column = s_cproc_get_current_column();\
        }\
        S_C_EMIT_ERROR(s_cproc_get_filename(), \
            s_cproc_get_text_buffer(), \
            ___offset,\
            ___line, \
            ___column, \
            fmt, ##args);\
        s_cproc_generate_error();\
    }while(0)

#define CPROC_GEN_WARNING(p_token, fmt, args...)\
    do{\
        STRU_C_TOKEN_NODE *___p_token_temp = p_token;\
        size_t ___offset;\
        size_t ___line;\
        size_t ___column;\
        if(___p_token_temp != NULL)\
        {\
            ___offset = ___p_token_temp->info.line_position.offset;\
            ___line = ___p_token_temp->info.line_position.line;\
            ___column = ___p_token_temp->info.line_position.column;\
        }\
        else\
        {\
            ___offset = s_cproc_get_current_offset();\
            ___line = s_cproc_get_current_line();\
            ___column = s_cproc_get_current_column();\
        }\
        S_C_EMIT_WARNING(s_cproc_get_filename(), \
            s_cproc_get_text_buffer(), \
            ___offset,\
            ___line, \
            ___column, \
            fmt, ##args);\
    }while(0)

__BEGIN_C_DECLS
_VOID display_token_and_line(const _S8 *p_text_buffer, size_t offset);
__END_C_DECLS


#endif

