#ifndef __S_TEXT_H__
#define __S_TEXT_H__

#include "s_clinkage.h"
#include "s_defines.h"

__BEGIN_C_DECLS
int count_words(const char* filename);
int format_words(const char* filename, const char *separator);
int getline(FILE *fp, char line[], int maxline);
ENUM_RETURN reverse(char *pstr_buf);
ENUM_RETURN fold(char *pstr_buf_source, char *pstr_buf_temp, int buf_temp_len, int fold_num);

__END_C_DECLS

#endif
