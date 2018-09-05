#ifndef __S_CERROR_H__
#define __S_CERROR_H__
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"
#ifdef CPPUTEST
extern FILE *g_pfw_errors;
#define S_C_EMIT_ERROR(filename, p_text_buffer, offset, len, line, column, fmt, args...)\
        do{\
            printf(WHITE"%s:%zd:%zd: "LIGHT_RED"error: "NONE""fmt"\n",\
                filename, line + 1, column +1, ##args);\
            if(g_pfw_errors != NULL)\
            {\
                fprintf(g_pfw_errors, "%s:%zd:%zd: error: "fmt"\n",\
                    filename, line + 1, column +1, ##args);\
            }\
            display_token_and_line(p_text_buffer, offset, len, BOOLEAN_TRUE);\
            display_token_and_line_to_file(p_text_buffer, offset, len, g_pfw_errors);\
        }while(0)
#define S_C_EMIT_WARNING(filename, p_text_buffer, offset, len, line, column, fmt, args...)\
        do{\
            printf(WHITE"%s:%zd:%zd: "LIGHT_PURPLE"warning: "NONE""fmt"\n",\
                filename, line + 1, column +1, ##args);\
            if(g_pfw_errors != NULL)\
            {\
                fprintf(g_pfw_errors, "%s:%zd:%zd: warning: "fmt"\n",\
                    filename, line + 1, column +1, ##args);\
            }\
            display_token_and_line(p_text_buffer, offset, len, BOOLEAN_FALSE);\
            display_token_and_line_to_file(p_text_buffer, offset, len, g_pfw_errors);\
        }while(0)

#else
#define S_C_EMIT_ERROR(filename, p_text_buffer, offset, len, line, column, fmt, args...)\
	do{\
		printf(WHITE"%s:%zd:%zd: "LIGHT_RED"error: "NONE""fmt"\n",\
			filename, line + 1, column +1, ##args);\
		display_token_and_line(p_text_buffer, offset, len, BOOLEAN_TRUE);\
	}while(0)
#define S_C_EMIT_WARNING(filename, p_text_buffer, offset, len, line, column, fmt, args...)\
	do{\
		printf(WHITE"%s:%zd:%zd: "LIGHT_PURPLE"warning: "NONE""fmt"\n",\
			filename, line + 1, column +1, ##args);\
		display_token_and_line(p_text_buffer, offset, len, BOOLEAN_FALSE);\
	}while(0)
#endif


__BEGIN_C_DECLS
_VOID display_token_and_line(const _S8 *p_text_buffer, size_t offset, size_t len, ENUM_BOOLEAN is_error);
#ifdef CPPUTEST

_VOID display_token_and_line_to_file(const _S8 *p_text_buffer, size_t offset, size_t len, FILE *pfw);
#endif

__END_C_DECLS


#endif

