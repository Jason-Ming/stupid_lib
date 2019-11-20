#include "CppUTest/TestHarness.h"
#include <string.h>
#include <stdio.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"
#include "s_mem.h"
#include "s_cproc.h"

#include <iostream>

using namespace std;

#define TEST_FILE_DIR001 "test_files/s_cproc/macro_common_predefined/test_001"
#define TEST_FILE_DIR002 "test_files/s_cproc/macro_common_predefined/test_002"
#define TEST_FILE_DIR003 "test_files/s_cproc/macro_common_predefined/test_003"
#define TEST_FILE_DIR004 "test_files/s_cproc/macro_common_predefined/test_004"
#define TEST_FILE_DIR005 "test_files/s_cproc/macro_common_predefined/test_005"
#define TEST_FILE_DIR006 "test_files/s_cproc/macro_common_predefined/test_006"
#define TEST_FILE_DIR007 "test_files/s_cproc/macro_common_predefined/test_007"
#define TEST_FILE_DIR008 "test_files/s_cproc/macro_common_predefined/test_008"
#define TEST_FILE_DIR009 "test_files/s_cproc/macro_common_predefined/test_009"
#define TEST_FILE_DIR010 "test_files/s_cproc/macro_common_predefined/test_010"
#define TEST_FILE_DIR011 "test_files/s_cproc/macro_common_predefined/test_011"
#define TEST_FILE_DIR012 "test_files/s_cproc/macro_common_predefined/test_012"
#define TEST_FILE_DIR013 "test_files/s_cproc/macro_common_predefined/test_013"
#define TEST_FILE_DIR014 "test_files/s_cproc/macro_common_predefined/test_014"
#define TEST_FILE_DIR015 "test_files/s_cproc/macro_common_predefined/test_015"
#define TEST_FILE_DIR016 "test_files/s_cproc/macro_common_predefined/test_016"
#define TEST_FILE_DIR017 "test_files/s_cproc/macro_common_predefined/test_017"
#define TEST_FILE_DIR018 "test_files/s_cproc/macro_common_predefined/test_018"
#define TEST_FILE_DIR019 "test_files/s_cproc/macro_common_predefined/test_019"
#define TEST_FILE_DIR020 "test_files/s_cproc/macro_common_predefined/test_020"
#define TEST_FILE_DIR021 "test_files/s_cproc/macro_common_predefined/test_021"
#define TEST_FILE_DIR022 "test_files/s_cproc/macro_common_predefined/test_022"
#define TEST_FILE_DIR023 "test_files/s_cproc/macro_common_predefined/test_023"
#define TEST_FILE_DIR024 "test_files/s_cproc/macro_common_predefined/test_024"
#define TEST_FILE_DIR025 "test_files/s_cproc/macro_common_predefined/test_025"
#define TEST_FILE_DIR026 "test_files/s_cproc/macro_common_predefined/test_026"
#define TEST_FILE_DIR027 "test_files/s_cproc/macro_common_predefined/test_027"
#define TEST_FILE_DIR028 "test_files/s_cproc/macro_common_predefined/test_028"
#define TEST_FILE_DIR029 "test_files/s_cproc/macro_common_predefined/test_029"
#define TEST_FILE_DIR030 "test_files/s_cproc/macro_common_predefined/test_030"
#define TEST_FILE_DIR031 "test_files/s_cproc/macro_common_predefined/test_031"
#define TEST_FILE_DIR032 "test_files/s_cproc/macro_common_predefined/test_032"
#define TEST_FILE_DIR033 "test_files/s_cproc/macro_common_predefined/test_033"
#define TEST_FILE_DIR034 "test_files/s_cproc/macro_common_predefined/test_034"
#define TEST_FILE_DIR035 "test_files/s_cproc/macro_common_predefined/test_035"










#define TEST_FILE_INPUT "/i.input"

#define TEST_FILE_OUTPUT "/o.output"
#define TEST_FILE_ERRORS "/e.output"

#define TEST_FILE_OUTPUT_EXPECT "/o_expect.output"
#define TEST_FILE_ERRORS_EXPECT "/e_expect.output"

TEST_GROUP(s_cc_macro_common_predefined)
{
    void setup()
    {
    	//设置自己的测试准备


    }

    void teardown()
    {
        //清理测试设置
        //cout << "Test end ......" << endl;
        S_FCLOSE(pf_output);
        S_FCLOSE(pf_errors);
        S_FCLOSE(pf_output_expect);
        S_FCLOSE(pf_errors_expect);

        //S_FREE(file_name_input);
        S_FREE(file_name_input);
        S_FREE(file_name_output);
        S_FREE(file_name_errors);
        S_FREE(file_name_output_expect);
        S_FREE(file_name_errors_expect);

    }
    
    void file_init(const _S8 *file_dir)
    {
        file_name_input = s_concatenate_string(file_dir, TEST_FILE_INPUT);
        file_name_output = s_concatenate_string(file_dir, TEST_FILE_OUTPUT);
        file_name_errors = s_concatenate_string(file_dir, TEST_FILE_ERRORS);
        file_name_output_expect = s_concatenate_string(file_dir, TEST_FILE_OUTPUT_EXPECT);
        file_name_errors_expect = s_concatenate_string(file_dir, TEST_FILE_ERRORS_EXPECT);

        pf_output = fopen(file_name_output, "wt+");
        pf_errors = fopen(file_name_errors, "wt+");
        pf_output_expect = fopen(file_name_output_expect, "rt");
        pf_errors_expect = fopen(file_name_errors_expect, "rt");
    }
    
    _S8 *file_name_input = NULL;
    _S8 *file_name_output = NULL;
    _S8 *file_name_errors = NULL;
    _S8 *file_name_output_expect = NULL;
    _S8 *file_name_errors_expect = NULL;
    
    FILE *pf_output;
    FILE *pf_errors;
    FILE *pf_output_expect;
    FILE *pf_errors_expect;
    
    ENUM_RETURN ret_val;
    _S32 file_compare_result;
};

/*
#define lang_init()  c_init()
lang_init()
     → c_init()
*/
TEST(s_cc_macro_common_predefined, no_macro_name)
{
    file_init(TEST_FILE_DIR001);
    ret_val = s_cc(file_name_input, pf_output, pf_errors);
    CHECK_EQUAL(RETURN_FAILURE, ret_val);

    ret_val = s_file_compare(pf_output, pf_output_expect, &file_compare_result);
    CHECK_EQUAL(RETURN_SUCCESS, ret_val);
    CHECK_EQUAL(0, file_compare_result);

    ret_val = s_file_compare(pf_errors, pf_errors_expect, &file_compare_result);
    CHECK_EQUAL(RETURN_SUCCESS, ret_val);
    CHECK_EQUAL(0, file_compare_result);
}

/*
The common predefined macros are GNU C extensions. They are available with the same meanings regardless of the machine or operating 
system on which you are using GNU C or GNU Fortran. Their names all start with double underscores.
*/

/*
__COUNTER__
This macro expands to sequential integral values starting from 0. In conjunction with the ## operator, this provides a convenient means 
to generate unique identifiers. Care must be taken to ensure that __COUNTER__ is not expanded prior to inclusion of precompiled headers 
which use it. Otherwise, the precompiled headers will not be used.

__GFORTRAN__
The GNU Fortran compiler defines this.

__GNUC__
__GNUC_MINOR__
__GNUC_PATCHLEVEL__
These macros are defined by all GNU compilers that use the C preprocessor: C, C++, Objective-C and Fortran. Their values are the major 
version, minor version, and patch level of the compiler, as integer constants. For example, GCC version x.y.z defines __GNUC__ to x, 
__GNUC_MINOR__ to y, and __GNUC_PATCHLEVEL__ to z. These macros are also defined if you invoke the preprocessor directly.

If all you need to know is whether or not your program is being compiled by GCC, or a non-GCC compiler that claims to accept the GNU C 
dialects, you can simply test __GNUC__. If you need to write code which depends on a specific version, you must be more careful. Each 
time the minor version is increased, the patch level is reset to zero; each time the major version is increased, the minor version and 
patch level are reset. If you wish to use the predefined macros directly in the conditional, you will need to write it like this:

//Test for GCC > 3.2.0
#if __GNUC__ > 3 || \
    (__GNUC__ == 3 && (__GNUC_MINOR__ > 2 || \
                       (__GNUC_MINOR__ == 2 && \
                        __GNUC_PATCHLEVEL__ > 0))
Another approach is to use the predefined macros to calculate a single number, then compare that against a threshold:

#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)
…
// Test for GCC > 3.2.0
#if GCC_VERSION > 30200
Many people find this form easier to understand.

__GNUG__
The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus).

__STRICT_ANSI__
GCC defines this macro if and only if the -ansi switch, or a -std switch specifying strict conformance to some version of ISO C or ISO C++
, was specified when GCC was invoked. It is defined to ‘1’. This macro exists primarily to direct GNU libc’s header files to use 
only definitions found in standard C.

__BASE_FILE__
This macro expands to the name of the main input file, in the form of a C string constant. This is the source file that was specified on 
the command line of the preprocessor or C compiler.

__INCLUDE_LEVEL__
This macro expands to a decimal integer constant that represents the depth of nesting in include files. The value of this macro is 
incremented on every ‘#include’ directive and decremented at the end of every included file. It starts out at 0, its value within the 
base file specified on the command line.

__ELF__
This macro is defined if the target uses the ELF object format.

__VERSION__
This macro expands to a string constant which describes the version of the compiler in use. You should not rely on its contents having 
any particular form, but it can be counted on to contain at least the release number.

__OPTIMIZE__
__OPTIMIZE_SIZE__
__NO_INLINE__
These macros describe the compilation mode. __OPTIMIZE__ is defined in all optimizing compilations. __OPTIMIZE_SIZE__ is defined if the 
compiler is optimizing for size, not speed. __NO_INLINE__ is defined if no functions will be inlined into their callers (when not 
optimizing, or when inlining has been specifically disabled by -fno-inline).

These macros cause certain GNU header files to provide optimized definitions, using macros or inline functions, of system library 
functions. You should not use these macros in any way unless you make sure that programs will execute with the same effect whether or not 
they are defined. If they are defined, their value is 1.

__GNUC_GNU_INLINE__
GCC defines this macro if functions declared inline will be handled in GCC’s traditional gnu90 mode. Object files will contain 
externally visible definitions of all functions declared inline without extern or static. They will not contain any definitions of any 
functions declared extern inline.

__GNUC_STDC_INLINE__
GCC defines this macro if functions declared inline will be handled according to the ISO C99 or later standards. Object files will 
contain externally visible definitions of all functions declared extern inline. They will not contain definitions of any functions 
declared inline without extern.

If this macro is defined, GCC supports the gnu_inline function attribute as a way to always get the gnu90 behavior.

__CHAR_UNSIGNED__
GCC defines this macro if and only if the data type char is unsigned on the target machine. It exists to cause the standard header file 
limits.h to work correctly. You should not use this macro yourself; instead, refer to the standard macros defined in limits.h.

__WCHAR_UNSIGNED__
Like __CHAR_UNSIGNED__, this macro is defined if and only if the data type wchar_t is unsigned and the front-end is in C++ mode.

__REGISTER_PREFIX__
This macro expands to a single token (not a string constant) which is the prefix applied to CPU register names in assembly language for 
this target. You can use it to write assembly that is usable in multiple environments. For example, in the m68k-aout environment it 
expands to nothing, but in the m68k-coff environment it expands to a single ‘%’.

__USER_LABEL_PREFIX__
This macro expands to a single token which is the prefix applied to user labels (symbols visible to C code) in assembly. For example, in 
the m68k-aout environment it expands to an ‘_’, but in the m68k-coff environment it expands to nothing.

This macro will have the correct definition even if -f(no-)underscores is in use, but it will not be correct if target-specific options 
that adjust this prefix are used (e.g. the OSF/rose -mno-underscores option).

__SIZE_TYPE__
__PTRDIFF_TYPE__
__WCHAR_TYPE__
__WINT_TYPE__
__INTMAX_TYPE__
__UINTMAX_TYPE__
__SIG_ATOMIC_TYPE__
__INT8_TYPE__
__INT16_TYPE__
__INT32_TYPE__
__INT64_TYPE__
__UINT8_TYPE__
__UINT16_TYPE__
__UINT32_TYPE__
__UINT64_TYPE__
__INT_LEAST8_TYPE__
__INT_LEAST16_TYPE__
__INT_LEAST32_TYPE__
__INT_LEAST64_TYPE__
__UINT_LEAST8_TYPE__
__UINT_LEAST16_TYPE__
__UINT_LEAST32_TYPE__
__UINT_LEAST64_TYPE__
__INT_FAST8_TYPE__
__INT_FAST16_TYPE__
__INT_FAST32_TYPE__
__INT_FAST64_TYPE__
__UINT_FAST8_TYPE__
__UINT_FAST16_TYPE__
__UINT_FAST32_TYPE__
__UINT_FAST64_TYPE__
__INTPTR_TYPE__
__UINTPTR_TYPE__
These macros are defined to the correct underlying types for the size_t, ptrdiff_t, wchar_t, wint_t, intmax_t, uintmax_t, sig_atomic_t, 
int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, int_least8_t, int_least16_t, int_least32_t, int_least64_t, 
uint_least8_t, uint_least16_t, uint_least32_t, uint_least64_t, int_fast8_t, int_fast16_t, int_fast32_t, int_fast64_t, uint_fast8_t, 
uint_fast16_t, uint_fast32_t, uint_fast64_t, intptr_t, and uintptr_t typedefs, respectively. They exist to make the standard header files 
stddef.h, stdint.h, and wchar.h work correctly. You should not use these macros directly; instead, include the appropriate headers and 
use the typedefs. Some of these macros may not be defined on particular systems if GCC does not provide a stdint.h header on those systems.

__CHAR_BIT__
Defined to the number of bits used in the representation of the char data type. It exists to make the standard header given numerical 
limits work correctly. You should not use this macro directly; instead, include the appropriate headers.

__SCHAR_MAX__
__WCHAR_MAX__
__SHRT_MAX__
__INT_MAX__
__LONG_MAX__
__LONG_LONG_MAX__
__WINT_MAX__
__SIZE_MAX__
__PTRDIFF_MAX__
__INTMAX_MAX__
__UINTMAX_MAX__
__SIG_ATOMIC_MAX__
__INT8_MAX__
__INT16_MAX__
__INT32_MAX__
__INT64_MAX__
__UINT8_MAX__
__UINT16_MAX__
__UINT32_MAX__
__UINT64_MAX__
__INT_LEAST8_MAX__
__INT_LEAST16_MAX__
__INT_LEAST32_MAX__
__INT_LEAST64_MAX__
__UINT_LEAST8_MAX__
__UINT_LEAST16_MAX__
__UINT_LEAST32_MAX__
__UINT_LEAST64_MAX__
__INT_FAST8_MAX__
__INT_FAST16_MAX__
__INT_FAST32_MAX__
__INT_FAST64_MAX__
__UINT_FAST8_MAX__
__UINT_FAST16_MAX__
__UINT_FAST32_MAX__
__UINT_FAST64_MAX__
__INTPTR_MAX__
__UINTPTR_MAX__
__WCHAR_MIN__
__WINT_MIN__
__SIG_ATOMIC_MIN__
Defined to the maximum value of the signed char, wchar_t, signed short, signed int, signed long, signed long long, wint_t, size_t, 
ptrdiff_t, intmax_t, uintmax_t, sig_atomic_t, int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, int_least8_t, 
int_least16_t, int_least32_t, int_least64_t, uint_least8_t, uint_least16_t, uint_least32_t, uint_least64_t, int_fast8_t, int_fast16_t, 
int_fast32_t, int_fast64_t, uint_fast8_t, uint_fast16_t, uint_fast32_t, uint_fast64_t, intptr_t, and uintptr_t types and to the minimum 
value of the wchar_t, wint_t, and sig_atomic_t types respectively. They exist to make the standard header given numerical limits work 
correctly. You should not use these macros directly; instead, include the appropriate headers. Some of these macros may not be defined on 
particular systems if GCC does not provide a stdint.h header on those systems.

__INT8_C
__INT16_C
__INT32_C
__INT64_C
__UINT8_C
__UINT16_C
__UINT32_C
__UINT64_C
__INTMAX_C
__UINTMAX_C
Defined to implementations of the standard stdint.h macros with the same names without the leading __. They exist the make the 
implementation of that header work correctly. You should not use these macros directly; instead, include the appropriate headers. Some of 
these macros may not be defined on particular systems if GCC does not provide a stdint.h header on those systems.

__SCHAR_WIDTH__
__SHRT_WIDTH__
__INT_WIDTH__
__LONG_WIDTH__
__LONG_LONG_WIDTH__
__PTRDIFF_WIDTH__
__SIG_ATOMIC_WIDTH__
__SIZE_WIDTH__
__WCHAR_WIDTH__
__WINT_WIDTH__
__INT_LEAST8_WIDTH__
__INT_LEAST16_WIDTH__
__INT_LEAST32_WIDTH__
__INT_LEAST64_WIDTH__
__INT_FAST8_WIDTH__
__INT_FAST16_WIDTH__
__INT_FAST32_WIDTH__
__INT_FAST64_WIDTH__
__INTPTR_WIDTH__
__INTMAX_WIDTH__
Defined to the bit widths of the corresponding types. They exist to make the implementations of limits.h and stdint.h behave correctly. 
You should not use these macros directly; instead, include the appropriate headers. Some of these macros may not be defined on particular 
systems if GCC does not provide a stdint.h header on those systems.

__SIZEOF_INT__
__SIZEOF_LONG__
__SIZEOF_LONG_LONG__
__SIZEOF_SHORT__
__SIZEOF_POINTER__
__SIZEOF_FLOAT__
__SIZEOF_DOUBLE__
__SIZEOF_LONG_DOUBLE__
__SIZEOF_SIZE_T__
__SIZEOF_WCHAR_T__
__SIZEOF_WINT_T__
__SIZEOF_PTRDIFF_T__
Defined to the number of bytes of the C standard data types: int, long, long long, short, void *, float, double, long double, size_t, 
wchar_t, wint_t and ptrdiff_t.

__BYTE_ORDER__
__ORDER_LITTLE_ENDIAN__
__ORDER_BIG_ENDIAN__
__ORDER_PDP_ENDIAN__
__BYTE_ORDER__ is defined to one of the values __ORDER_LITTLE_ENDIAN__, __ORDER_BIG_ENDIAN__, or __ORDER_PDP_ENDIAN__ to reflect the 
layout of multi-byte and multi-word quantities in memory. If __BYTE_ORDER__ is equal to __ORDER_LITTLE_ENDIAN__ or __ORDER_BIG_ENDIAN__, 
then multi-byte and multi-word quantities are laid out identically: the byte (word) at the lowest address is the least significant or 
most significant byte (word) of the quantity, respectively. If __BYTE_ORDER__ is equal to __ORDER_PDP_ENDIAN__, then bytes in 16-bit 
words are laid out in a little-endian fashion, whereas the 16-bit subwords of a 32-bit quantity are laid out in big-endian fashion.

You should use these macros for testing like this:

// Test for a little-endian machine 
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
__FLOAT_WORD_ORDER__
__FLOAT_WORD_ORDER__ is defined to one of the values __ORDER_LITTLE_ENDIAN__ or __ORDER_BIG_ENDIAN__ to reflect the layout of the words 
of multi-word floating-point quantities.

__DEPRECATED
This macro is defined, with value 1, when compiling a C++ source file with warnings about deprecated constructs enabled. These warnings 
are enabled by default, but can be disabled with -Wno-deprecated.

__EXCEPTIONS
This macro is defined, with value 1, when compiling a C++ source file with exceptions enabled. If -fno-exceptions is used when compiling 
the file, then this macro is not defined.

__GXX_RTTI
This macro is defined, with value 1, when compiling a C++ source file with runtime type identification enabled. If -fno-rtti is used when 
compiling the file, then this macro is not defined.

__USING_SJLJ_EXCEPTIONS__
This macro is defined, with value 1, if the compiler uses the old mechanism based on setjmp and longjmp for exception handling.

__GXX_EXPERIMENTAL_CXX0X__
This macro is defined when compiling a C++ source file with the option -std=c++0x or -std=gnu++0x. It indicates that some features likely 
to be included in C++0x are available. Note that these features are experimental, and may change or be removed in future versions of GCC.

__GXX_WEAK__
This macro is defined when compiling a C++ source file. It has the value 1 if the compiler will use weak symbols, COMDAT sections, or 
other similar techniques to collapse symbols with “vague linkage” that are defined in multiple translation units. If the compiler 
will not collapse such symbols, this macro is defined with value 0. In general, user code should not need to make use of this macro; the 
purpose of this macro is to ease implementation of the C++ runtime library provided with G++.

__NEXT_RUNTIME__
This macro is defined, with value 1, if (and only if) the NeXT runtime (as in -fnext-runtime) is in use for Objective-C. If the GNU 
runtime is used, this macro is not defined, so that you can use this macro to determine which runtime (NeXT or GNU) is being used.

__LP64__
_LP64
These macros are defined, with value 1, if (and only if) the compilation is for a target where long int and pointer both use 64-bits and 
int uses 32-bit.

__SSP__
This macro is defined, with value 1, when -fstack-protector is in use.

__SSP_ALL__
This macro is defined, with value 2, when -fstack-protector-all is in use.

__SSP_STRONG__
This macro is defined, with value 3, when -fstack-protector-strong is in use.

__SSP_EXPLICIT__
This macro is defined, with value 4, when -fstack-protector-explicit is in use.

__SANITIZE_ADDRESS__
This macro is defined, with value 1, when -fsanitize=address or -fsanitize=kernel-address are in use.

__SANITIZE_THREAD__
This macro is defined, with value 1, when -fsanitize=thread is in use.

__TIMESTAMP__
This macro expands to a string constant that describes the date and time of the last modification of the current source file. The string 
constant contains abbreviated day of the week, month, day of the month, time in hh:mm:ss form, year and looks like "Sun Sep 16 01:03:52 
1973". If the day of the month is less than 10, it is padded with a space on the left.

If GCC cannot determine the current date, it will emit a warning message (once per compilation) and __TIMESTAMP__ will expand to "??? ??? 
?? ??:??:?? ????".

__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1
__GCC_HAVE_SYNC_COMPARE_AND_SWAP_2
__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8
__GCC_HAVE_SYNC_COMPARE_AND_SWAP_16
These macros are defined when the target processor supports atomic compare and swap operations on operands 1, 2, 4, 8 or 16 bytes in 
length, respectively.

__HAVE_SPECULATION_SAFE_VALUE
This macro is defined with the value 1 to show that this version of GCC supports __builtin_speculation_safe_value.

__GCC_HAVE_DWARF2_CFI_ASM
This macro is defined when the compiler is emitting DWARF CFI directives to the assembler. When this is defined, it is possible to emit 
those same directives in inline assembly.

__FP_FAST_FMA
__FP_FAST_FMAF
__FP_FAST_FMAL
These macros are defined with value 1 if the backend supports the fma, fmaf, and fmal builtin functions, so that the include file math.h 
can define the macros FP_FAST_FMA, FP_FAST_FMAF, and FP_FAST_FMAL for compatibility with the 1999 C standard.

__FP_FAST_FMAF16
__FP_FAST_FMAF32
__FP_FAST_FMAF64
__FP_FAST_FMAF128
__FP_FAST_FMAF32X
__FP_FAST_FMAF64X
__FP_FAST_FMAF128X
These macros are defined with the value 1 if the backend supports the fma functions using the additional _Floatn and _Floatnx types that 
are defined in ISO/IEC TS 18661-3:2015. The include file math.h can define the FP_FAST_FMAFn and FP_FAST_FMAFnx macros if the user 
defined __STDC_WANT_IEC_60559_TYPES_EXT__ before including math.h.

__GCC_IEC_559
This macro is defined to indicate the intended level of support for IEEE 754 (IEC 60559) floating-point arithmetic. It expands to a 
nonnegative integer value. If 0, it indicates that the combination of the compiler configuration and the command-line options is not 
intended to support IEEE 754 arithmetic for float and double as defined in C99 and C11 Annex F (for example, that the standard rounding 
modes and exceptions are not supported, or that optimizations are enabled that conflict with IEEE 754 semantics). If 1, it indicates that 
IEEE 754 arithmetic is intended to be supported; this does not mean that all relevant language features are supported by GCC. If 2 or more
, it additionally indicates support for IEEE 754-2008 (in particular, that the binary encodings for quiet and signaling NaNs are as 
specified in IEEE 754-2008).

This macro does not indicate the default state of command-line options that control optimizations that C99 and C11 permit to be 
controlled by standard pragmas, where those standards do not require a particular default state. It does not indicate whether 
optimizations respect signaling NaN semantics (the macro for that is __SUPPORT_SNAN__). It does not indicate support for decimal floating 
point or the IEEE 754 binary16 and binary128 types.

__GCC_IEC_559_COMPLEX
This macro is defined to indicate the intended level of support for IEEE 754 (IEC 60559) floating-point arithmetic for complex numbers, 
as defined in C99 and C11 Annex G. It expands to a nonnegative integer value. If 0, it indicates that the combination of the compiler 
configuration and the command-line options is not intended to support Annex G requirements (for example, because -fcx-limited-range was 
used). If 1 or more, it indicates that it is intended to support those requirements; this does not mean that all relevant language 
features are supported by GCC.

__NO_MATH_ERRNO__
This macro is defined if -fno-math-errno is used, or enabled by another option such as -ffast-math or by default.


*/

