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


/* read a line into line, return length */
ENUM_RETURN s_getline(FILE *fp, _S8 buffer[], _S32 buffer_size, _S32 *length);


/**
 * @author: Jason Ming
 * @description: get a word from source
 * @param source: the source string
 * @param word_buf: the buffer to store the word
 * @param buf_size: the size of word_buf
 * @param word_len: the word len
 * @param next: the pointer of the next word 
 * @return: RETURN_SUCCESS，RETURN_FAILUE
 */
ENUM_RETURN s_get_word(const _S8 *source, _S8 *word_buf, size_t buf_size, size_t *word_len, const _S8 **next);

/* reverses the character string s . */
ENUM_RETURN s_reverse(_S8 *pstr_buf);

/* fold long input lines into two or more shorter lines after the last
non-blank character that occurs before the n -th column of input */
ENUM_RETURN s_fold(_S8 *pstr_buf_source, _S8 *pstr_buf_temp, _S32 buf_temp_len, _S32 fold_num);


ENUM_RETURN s_hstrtou64(const _S8 *str, _U64 *value);
ENUM_RETURN s_hstrtos64(const _S8 *str, _S64 *value);
ENUM_RETURN s_strtos32(const _S8 *str, _S32 *value);
ENUM_RETURN s_s32tostr(_S32 value, _S8 *dest, size_t size);
ENUM_RETURN s_s32tostrb(_S32 value, _S32 b, _S8 *dest, size_t size);
ENUM_RETURN s_s32tostrbw(_S32 value, _S32 b, _S32 w, _S8 *dest, size_t size);
ENUM_RETURN s_strtosd(const _S8 *str, _SD *value);

/* deletes each character in the
string s1 that matches any character in the string s2 . */
_VOID squeeze(_S8 s1[], const _S8 s2[]);

/* returns the first location in the string s1
where any character from the string s2 occurs, or -1 if s1 contains no characters
from s2 . (The standard library function strpbrk does the same job but returns a
pointer to the location.) */
_S8* any(_S8 s1[], const _S8 s2[]);

/* expands shorthand notations like a-z in the
string s1 into the equivalent complete list abc...xyz in s2 . Allow for letters of either
case and digits, and be prepared to handle cases like a-b-c and a-z0-9 and -a-z .
Arrange that a leading or trailing - is taken literally. */
ENUM_RETURN s_expand(const _S8 *s1, _S8 *s2, size_t size);

/* converts characters like newline and tab into
visible escape sequences like \n and \t as it copies the string t to s . */
_VOID s_escape(_S8* source, _S8* dest, size_t size);

/* converting escape sequences into the real characters. */
_VOID s_unescape(_S8* source, _S8* dest, size_t size);

/* trim: remove trailing blanks, tabs, newlines */
ENUM_RETURN s_trim(_S8 *source);

/* strindex: return index of t in s, -1 if none */
ENUM_RETURN s_strindex(const _S8 *source, const _S8 *target, _S32 *index);

/* returns the position of the rightmost occurrence of t in s , or -1 if there is none */
ENUM_RETURN s_strrindex(const _S8 *source, const _S8 *target, _S32 *index);

/* if the string t occurs at the end of the string s */
ENUM_RETURN s_strend(const _S8 *source, const _S8 *target, ENUM_BOOLEAN *occur);

/* replaces strings of blanks with the minimum number of tabs and blanks to achieve the same spacing. */
ENUM_RETURN s_entab(const _S8 *source, _S8 *dest, size_t len, _S32 tab_stop);

/* replaces tabs in the input with the proper number of blanks to space to the next tab stop. */
ENUM_RETURN s_detab(const _S8 *source, _S8 *dest, size_t len, _S32 tab_stop);


__END_C_DECLS

#endif
