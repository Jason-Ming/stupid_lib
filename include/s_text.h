#ifndef __S_TEXT_H__
#define __S_TEXT_H__

#include "s_clinkage.h"
#include "s_defines.h"

//��һ����ĸת��Ϊ��д
#define UPCASE( c ) ( ((c) >= 'a' && (c) <= 'z') ? ((c) - 0��20) : (c) )
#define LOWCASE( c ) ( ((c) >= 'A' && (c) <= 'Z') ? ((c) + 0��20) : (c) )

//�ж��ַ��ǲ���10��ֵ������
#define IS_DEC( c ) ((c) >= '0' && (c) <= '9')

//�ж��ַ��ǲ���16��ֵ������
#define IS_HEX( c ) ( ((c) >= '0' && (c) <= '9') ||((c) >= 'A' && (c) <= 'F') ||((c) >= 'a' && (c) <= 'f') )

#define IS_ALPHABET(c) (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))?(BOOLEAN_TRUE):(BOOLEAN_FALSE))

#define CONC(x, y) x##y

__BEGIN_C_DECLS
int count_words(const char* filename);
int format_words(const char* filename, const char *separator);
int get_line(FILE *fp, char line[], int maxline);
ENUM_RETURN reverse(char *pstr_buf);
ENUM_RETURN fold(char *pstr_buf_source, char *pstr_buf_temp, int buf_temp_len, int fold_num);
unsigned long long htou(const char *str);
long long htoi(const char *str);
void squeeze(char s1[], const char s2[]);


__END_C_DECLS

#endif
