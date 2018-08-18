#ifndef __S_TEXT_H__
#define __S_TEXT_H__

#include <ctype.h>
#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"

#define INVALID_CHAR (-1)
//将一个字母转换为大写
#define UPPERCASE( c ) ( ((c) >= 'a' && (c) <= 'z') ? ((c) - 0x20) : (c) )
#define LOWERCASE( c ) ( ((c) >= 'A' && (c) <= 'Z') ? ((c) + 0x20) : (c) )

//判断字符是不是10进值的数字
#define IS_DEC( c ) ((c) >= '0' && (c) <= '9')

//判断字符是不是16进值的数字
#define IS_HEX( c ) ( ((c) >= '0' && (c) <= '9') ||((c) >= 'A' && (c) <= 'F') ||((c) >= 'a' && (c) <= 'f') )

#define IS_ALPHABET(c) (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))?(BOOLEAN_TRUE):(BOOLEAN_FALSE))

#define IS_DIR(c) ((IS_ALPHABET(c) || IS_DEC(c) || c == ' ')?BOOLEAN_TRUE:BOOLEAN_FALSE)

#define CONC(x, y) x##y
#define CONC_(x, y) x##_##y

/* c can not be '\0', if you output '\0', please use OUTPUT_END */
#define OUTPUT_C(c, dest, size)\
    do{\
        _S8 ___c_ = (c);\
        S_ASSERT_LOG_DO((size) > 1, break, "buffer '"#dest"''s size(%zd) is not enough!", (size));\
        *(dest)++ = ___c_;\
        (size)--;\
    }while(0);

#define OUTPUT_END(dest, size)\
    do{\
		S_ASSERT_LOG_DO((size) > 0, break, "buffer '"#dest"''s size(%zd) is not enough!", (size));\
        *(dest)++ = '\0';\
        (size)--;\
    }while(0);

#define OUTPUT_C_RANGE(begin, end, dest, size) \
    do{\
        _S8 ___b = (begin), ___e = (end);\
        if(___b != '\0' && ___e != '\0')\
        {\
            for(_S8 ___c = ___b; ___c <= ___e; ___c++)\
            {\
                OUTPUT_C(___c, (dest), (size));\
            }\
        }\
    }while(0);

#define OUTPUT_C_MULTI(c, num, dest, size) \
    do{\
        _S8 ___c = (c);\
        for(_S8 i = 0; i < (num); i++)\
        {\
            OUTPUT_C(___c, (dest), (size));\
        }\
    }while(0);

#define OUTPUT_STR_F(fpw, fpr, len)\
    do{\
        _S8 ___c;size_t ___len = (len);\
        while(___len-- > 0)\
        {;\
            ___c = fgetc(fpr);\
            fputc(___c, fpw);\
        };\
    }while(0);

#define OUTPUT_STR(dest, size, target, target_len)\
    do{\
		S_ASSERT_LOG_DO((size) > (target_len), break, "buffer '"#dest"''s size(%zd) is not enough!", (size));\
        memcpy((dest), (target), (target_len));\
        (dest) += (target_len);\
        (size) -= (target_len);\
    }while(0);

typedef struct TAG_STRU_TABLE_TEXT_FORMAT
{
    size_t margin_left;
    size_t margin_right;
    size_t table_column_size;
    ENUM_BOOLEAN align_left;
}STRU_TABLE_TEXT_FORMAT;

__BEGIN_C_DECLS

/* set separaors, word are separated by charactors in 'separators', if 'separators' is NULL or empty,
   the words are separeted by '\n', ' ', '\t', '\v' and '\f', this operation can influnce these functions: 
   s_count_word_f, s_count_word_s, s_get_word_f, s_get_word_s */
void s_set_separators(_S8 *separators);

/* count word in file, word are separated by charactors in 'separators', if 'separators' is NULL or empty,
   the words are separeted by '\n', ' ', '\t', '\v' and '\f' */
ENUM_RETURN s_count_word_f(FILE *pfr, size_t *word_num);

/* count word in string, word are separated by charactors in 'separators', if 'separators' is NULL or empty,
   the words are separeted by '\n', ' ', '\t', '\v' and '\f' */
ENUM_RETURN s_count_word_s(const _S8 *source, size_t *word_num);

/**
 * @author: Jason Ming
 * @description: get a word from file, word are separated by charactors in 'separators', 
        if 'separators' is NULL or empty, the words are separeted by '\n', ' ', '\t', '\v' and '\f'.
 * @param pfr: the pointer to file, after processing, source point to the next word or 'EOF'
 * @param word_buf: the buffer to store the word
 * @param buf_size: the size of word_buf
 * @param word_len: the word len
 * @param separators: string which included the separators
 * @return: RETURN_SUCCESS，RETURN_FAILUE
 */
ENUM_RETURN s_get_word_f(FILE *pfr, _S8 *word_buf, size_t buf_size, size_t *word_len);

/**
 * @author: Jason Ming
 * @description: get a word from source, word are separated by charactors in 'separators', 
        if 'separators' is NULL or empty, the words are separeted by '\n', ' ', '\t', '\v' and '\f'.
 * @param source: the pointer'pointer to source string, after processing, source point to the next word or '\0'
 * @param word_buf: the buffer to store the word
 * @param buf_size: the size of word_buf
 * @param word_len: the word len
 * @param separators: string which included the separators
 * @return: RETURN_SUCCESS，RETURN_FAILUE
 */
ENUM_RETURN s_get_word_s(const _S8 **source, _S8 *word_buf, size_t buf_size, size_t *word_len);


/* read a line into buffer(include '\n'), return length(not include '\0'), fp point to next line or EOF */
ENUM_RETURN s_getline_f(FILE *fp, _S8 buffer[], size_t buffer_size, size_t *length);

/* read a line into buffer, return length(not include '\0'), source point to next line or '\0' */
ENUM_RETURN s_getline_s(const _S8 **source, _S8 buffer[], size_t buffer_size, size_t *length);

/* read lines into line_ptr, return read line number, this function will call malloc to alloc memory 
   for line_ptr to store line, pfr point to EOF */
ENUM_RETURN s_getlines_f(FILE *pfr, _S8 *line_ptr[], size_t line_ptr_num, size_t *line_num);

/* read lines into line_ptr, return read line number, this function will call malloc to alloc memory 
   for line_ptr to store line, source point to '\0' */
ENUM_RETURN s_getlines_s(const _S8 **source, _S8 *line_ptr[], size_t line_ptr_num, size_t *line_num);

/* reverses the character string s . */
ENUM_RETURN s_reverse(_S8 *pstr_buf);

/* fold long input lines into two or more shorter lines after the last
non-blank character that occurs before the n -th column of input */
ENUM_RETURN s_fold_f(FILE *pfr, FILE *pfw, size_t fold_len);


/* fold long input string into two or more shorter lines which length is fold_len at most */
ENUM_RETURN s_fold_s(const _S8 *source, _S8 *dest, size_t size, size_t fold_len);

ENUM_RETURN s_hstrtou64(const _S8 *str, _U64 *value);
ENUM_RETURN s_hstrtos64(const _S8 *str, _S64 *value);
ENUM_RETURN s_strtos32(const _S8 *str, _S32 *value);
ENUM_RETURN s_s32tostr(_S32 value, _S8 *dest, size_t size);
ENUM_RETURN s_s32tostrb(_S32 value, _S32 b, _S8 *dest, size_t size);
ENUM_RETURN s_s32tostrbw(_S32 value, _S32 b, _S32 w, _S8 *dest, size_t size);
ENUM_RETURN s_strtosd(const _S8 *str, _SD *value);

/* deletes each character in the
string source that matches any character in the string target . */
ENUM_RETURN s_squeeze(_S8 *source, const _S8 *target);


/* returns the first location in the string s1
where any character from the string s2 occurs, or -1 if s1 contains no characters
from s2 . (The standard library function strpbrk does the same job but returns a
pointer to the location.) */
ENUM_RETURN s_any(const _S8 *source, const _S8 *target, const _S8**pp_occur);

/* expands shorthand notations like a-z in the
string source into the equivalent complete list abc...xyz in dest . Allow for letters of either
case and digits, and be prepared to handle cases like a-b-c and a-z0-9 and -a-z .
Arrange that a leading or trailing - is taken literally. */
ENUM_RETURN s_expand(const _S8 *source, _S8 *dest, size_t size);

/* converts characters like newline and tab into
visible escape sequences like \n and \t as it copies the string source to dest . */
ENUM_RETURN s_escape(const _S8* source, _S8* dest, size_t size);

/* converting escape sequences into the real characters. */
ENUM_RETURN s_unescape(const _S8* source, _S8* dest, size_t size);

/* trim: remove all trailing blanks, tabs, newlines */
ENUM_RETURN s_trim(_S8 *source);

/* trim: remove trailing all blanks, tabs and at most one newline, 
   after removement, if the string only contains newlines, clear the string */
ENUM_RETURN s_trim_nl(_S8 *source);

/* strindex: return index of target in source, -1 if none or target is empty */
ENUM_RETURN s_strindex(const _S8 *source, const _S8 *target, _S32 *index);

/* returns the position of the rightmost occurrence of target in source , -1 if there is none or target is empty */
ENUM_RETURN s_strrindex(const _S8 *source, const _S8 *target, _S32 *index);

/* if the string target occurs at the end of the string source, if target is empty, occur will be false */
ENUM_RETURN s_strend(const _S8 *source, const _S8 *target, ENUM_BOOLEAN *whether_target_occur);

/* replaces strings of blanks with the minimum number of tabs and blanks to achieve the same spacing. */
ENUM_RETURN s_entab(const _S8 *source, _S8 *dest, size_t len, _S32 tab_stop);

/* replaces tabs in the input with the proper number of blanks to space to the next tab stop. */
ENUM_RETURN s_detab(const _S8 *source, _S8 *dest, size_t len, _S32 tab_stop);

/* whether the charator between begin and end in the same range: 0~9, a~z, A~Z */
ENUM_BOOLEAN s_range(_S8 begin, _S8 end);

/**
 * @author: Jason Ming
 * @description: compare s1 to s2, case insensitive
 * @param s1: pointer to string
 * @param s2: pointer to string
 * @return: -1(s1 < s2)，0(s1 == s2), 1(s1 > s2)
 */
_S32 strcasecmp(const _S8*s1, const _S8*s2);

/**
 * @author: Jason Ming
 * @description: compare s1 to s2, case insensitive
 * @param s1: pointer to string
 * @param s2: pointer to string
 * @return: -1(s1 > s2)，0(s1 == s2), 1(s1 < s2)
 */
_S32 strcasecmp_r(const _S8 * s1, const _S8 * s2);

/**
 * @author: Jason Ming
 * @description: compare s1 to s2, case sensitive
 * @param s1: pointer to string
 * @param s2: pointer to string
 * @return: -1(s1 > s2)，0(s1 == s2), 1(s1 < s2)
 */
_S32 strcmp_r(const _S8 * s1, const _S8 * s2);

/**
 * @author: Jason Ming
 * @description: compare s1 to s2, case sensitive, directory order(makes comparisons only on
       letters, numbers and blanks)
 * @param s1: pointer to string
 * @param s2: pointer to string
 * @return: -1(s1 < s2)，0(s1 == s2), 1(s1 > s2)
 */
_S32 dircmp(const _S8 * s1, const _S8 * s2);

/**
 * @author: Jason Ming
 * @description: compare s1 to s2, case sensitive, directory order(makes comparisons only on
       letters, numbers and blanks)
 * @param s1: pointer to string
 * @param s2: pointer to string
 * @return: -1(s1 > s2)，0(s1 == s2), 1(s1 < s2)
 */
_S32 dircmp_r(const _S8 * s1, const _S8 * s2);

/**
 * @author: Jason Ming
 * @description: compare s1 to s2, case insensitive, directory order(makes comparisons only on
       letters, numbers and blanks)
 * @param s1: pointer to string
 * @param s2: pointer to string
 * @return: -1(s1 < s2)，0(s1 == s2), 1(s1 > s2)
 */
_S32 dircasecmp(const _S8 * s1, const _S8 * s2);

/**
 * @author: Jason Ming
 * @description: compare s1 to s2, case insensitive, directory order(makes comparisons only on
       letters, numbers and blanks)
 * @param s1: pointer to string
 * @param s2: pointer to string
 * @return: -1(s1 > s2)，0(s1 == s2), 1(s1 < s2)
 */
_S32 dircasecmp_r(const _S8 * s1, const _S8 * s2);

/**
 * @author: Jason Ming
 * @description: compare s1 and s2 numerically, the string will be converted to float by 'atof'
 * @param s1: pointer to string
 * @param s2: pointer to string
 * @return: -1(s1 < s2)，0(s1 == s2), 1(s1 > s2), 
 */
_S32 numcmp(const _S8 * s1, const _S8 * s2);

/**
 * @author: Jason Ming
 * @description: compare s1 and s2 numerically, the string will be converted to float by 'atof'
 * @param s1: pointer to string
 * @param s2: pointer to string
 * @return: -1(s1 > s2)，0(s1 == s2), 1(s1 < s2)
 */
_S32 numcmp_r(const _S8 * s1, const _S8 * s2);


ENUM_RETURN s_print_text_table(const _S8 *text[], size_t rows, size_t columns, STRU_TABLE_TEXT_FORMAT format[]);

_UL s_get_inode_by_filename(const _S8 *p_filename);

ENUM_RETURN s_save_file_to_text_buffer(
    FILE *pfr, 
    _S8 **pp_text_buffer, 
    size_t *p_buffer_size);

__END_C_DECLS

#endif
