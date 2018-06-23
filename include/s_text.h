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
_S32 s_getline(FILE *fp, _S8 line[], _S32 maxline);
ENUM_RETURN s_reverse(_S8 *pstr_buf);
ENUM_RETURN s_fold(_S8 *pstr_buf_source, _S8 *pstr_buf_temp, _S32 buf_temp_len, _S32 fold_num);
ENUM_RETURN s_hstrtou64(const _S8 *str, _U64 *value);
ENUM_RETURN s_hstrtos64(const _S8 *str, _S64 *value);
ENUM_RETURN s_strtos32(const _S8 *str, _S32 *value);
ENUM_RETURN s_s32tostr(_S32 value, _S8 *dest, size_t size);
ENUM_RETURN s_s32tostrb(_S32 value, _S32 b, _S8 *dest, size_t size);
ENUM_RETURN s_s32tostrbw(_S32 value, _S32 b, _S32 w, _S8 *dest, size_t size);

_VOID squeeze(_S8 s1[], const _S8 s2[]);
_S8* any(_S8 s1[], const _S8 s2[]);
ENUM_RETURN s_expand(const _S8 *s1, _S8 *s2, size_t size);
_VOID s_escape(_S8* source, _S8* dest, size_t size);
_VOID s_unescape(_S8* source, _S8* dest, size_t size);
ENUM_RETURN s_trim(_S8 *source);



__END_C_DECLS

#endif
