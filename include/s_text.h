#ifndef __S_TEXT_H__
#define __S_TEXT_H__

#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"

//将一个字母转换为大写
#define UPPERCASE( c ) ( ((c) >= 'a' && (c) <= 'z') ? ((c) - 0×20) : (c) )
#define LOWERCASE( c ) ( ((c) >= 'A' && (c) <= 'Z') ? ((c) + 0×20) : (c) )

//判断字符是不是10进值的数字
#define IS_DEC( c ) ((c) >= '0' && (c) <= '9')

//判断字符是不是16进值的数字
#define IS_HEX( c ) ( ((c) >= '0' && (c) <= '9') ||((c) >= 'A' && (c) <= 'F') ||((c) >= 'a' && (c) <= 'f') )

#define IS_ALPHABET(c) (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))?(BOOLEAN_TRUE):(BOOLEAN_FALSE))

#define CONC(x, y) x##y
#define CONC_(x, y) x##_##y

__BEGIN_C_DECLS
_S32 count_words(const _S8* filename);
_S32 format_words(const _S8* filename, const _S8 *separator);
_S32 get_line(FILE *fp, _S8 line[], _S32 maxline);
ENUM_RETURN reverse(_S8 *pstr_buf);
ENUM_RETURN fold(_S8 *pstr_buf_source, _S8 *pstr_buf_temp, _S32 buf_temp_len, _S32 fold_num);
_U64 htou(const _S8 *str);
_S64 htoi(const _S8 *str);
_VOID squeeze(_S8 s1[], const _S8 s2[]);
_S8* any(_S8 s1[], const _S8 s2[]);


__END_C_DECLS

#endif
