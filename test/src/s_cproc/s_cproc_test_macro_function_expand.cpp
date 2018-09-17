#include "CppUTest/TestHarness.h"
#include <string.h>
#include <stdio.h>
#include"s_text.h"
#include "s_type.h"
#include "s_limits.h"
#include "s_mem.h"
#include "s_cproc.h"

#include <iostream>

using namespace std;

#define TEST_FILE_DIR001 "test_files/s_cproc/macro_function_expand/test_001"
#define TEST_FILE_DIR002 "test_files/s_cproc/macro_function_expand/test_002"
#define TEST_FILE_DIR003 "test_files/s_cproc/macro_function_expand/test_003"
#define TEST_FILE_DIR004 "test_files/s_cproc/macro_function_expand/test_004"
#define TEST_FILE_DIR005 "test_files/s_cproc/macro_function_expand/test_005"
#define TEST_FILE_DIR006 "test_files/s_cproc/macro_function_expand/test_006"
#define TEST_FILE_DIR007 "test_files/s_cproc/macro_function_expand/test_007"
#define TEST_FILE_DIR008 "test_files/s_cproc/macro_function_expand/test_008"
#define TEST_FILE_DIR009 "test_files/s_cproc/macro_function_expand/test_009"
#define TEST_FILE_DIR010 "test_files/s_cproc/macro_function_expand/test_010"
#define TEST_FILE_DIR011 "test_files/s_cproc/macro_function_expand/test_011"
#define TEST_FILE_DIR012 "test_files/s_cproc/macro_function_expand/test_012"
#define TEST_FILE_DIR013 "test_files/s_cproc/macro_function_expand/test_013"
#define TEST_FILE_DIR014 "test_files/s_cproc/macro_function_expand/test_014"
#define TEST_FILE_DIR015 "test_files/s_cproc/macro_function_expand/test_015"
#define TEST_FILE_DIR016 "test_files/s_cproc/macro_function_expand/test_016"
#define TEST_FILE_DIR017 "test_files/s_cproc/macro_function_expand/test_017"
#define TEST_FILE_DIR018 "test_files/s_cproc/macro_function_expand/test_018"
#define TEST_FILE_DIR019 "test_files/s_cproc/macro_function_expand/test_019"
#define TEST_FILE_DIR020 "test_files/s_cproc/macro_function_expand/test_020"
#define TEST_FILE_DIR021 "test_files/s_cproc/macro_function_expand/test_021"
#define TEST_FILE_DIR022 "test_files/s_cproc/macro_function_expand/test_022"
#define TEST_FILE_DIR023 "test_files/s_cproc/macro_function_expand/test_023"
#define TEST_FILE_DIR024 "test_files/s_cproc/macro_function_expand/test_024"
#define TEST_FILE_DIR025 "test_files/s_cproc/macro_function_expand/test_025"
#define TEST_FILE_DIR026 "test_files/s_cproc/macro_function_expand/test_026"
#define TEST_FILE_DIR027 "test_files/s_cproc/macro_function_expand/test_027"
#define TEST_FILE_DIR028 "test_files/s_cproc/macro_function_expand/test_028"
#define TEST_FILE_DIR029 "test_files/s_cproc/macro_function_expand/test_029"
#define TEST_FILE_DIR030 "test_files/s_cproc/macro_function_expand/test_030"
#define TEST_FILE_DIR031 "test_files/s_cproc/macro_function_expand/test_031"
#define TEST_FILE_DIR032 "test_files/s_cproc/macro_function_expand/test_032"
#define TEST_FILE_DIR033 "test_files/s_cproc/macro_function_expand/test_033"
#define TEST_FILE_DIR034 "test_files/s_cproc/macro_function_expand/test_034"
#define TEST_FILE_DIR035 "test_files/s_cproc/macro_function_expand/test_035"










#define TEST_FILE_INPUT "/i.input"

#define TEST_FILE_OUTPUT "/o.output"
#define TEST_FILE_ERRORS "/e.output"

#define TEST_FILE_OUTPUT_EXPECT "/o_expect.output"
#define TEST_FILE_ERRORS_EXPECT "/e_expect.output"

TEST_GROUP(s_cc_macro_function_expand)
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
TEST(s_cc_macro_function_expand, no_macro_name)
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
                extern void foo(void);
#define foo() /* optimized inline version */
/*
                …
                  foo();
                  funcptr = foo;

*/


/*
#define lang_init ()    c_init()
lang_init()
     → () c_init()()
*/

/*
  The invocation of the macro need not be restricted to a single logical line—it can cross as many lines in the source file as you wish. 
  The number of arguments you give must match the number of parameters in the macro definition. When the macro is expanded, each use of a 
  parameter in its body is replaced by the tokens of the corresponding argument. 

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
  x = min(a, b);          →  x = ((a) < (b) ? (a) : (b));
  y = min (1, 2);          →  y = ((1) < (2) ? (1) : (2));
  z = min
  (a + 28, *p);    →  z = ((a + 28) < (*p) ? (a + 28) : (*p));

*/

/*
Leading and trailing whitespace in each argument is dropped, and all whitespace between the tokens of an argument is reduced to a single 
space. 
#define STR(s) #s
char* s = STR(   abc    def  ghi    );
→  
char* s = "abc def ghi";
*/

/*
Parentheses within each argument must balance; a comma within such parentheses does not end the argument. However, there is no 
requirement for square brackets or braces to balance, and they do not prevent a comma from separating arguments. Thus,

macro (array[x = y, x + 1])
passes two arguments to macro: array[x = y and x + 1]. 
#define MACRO(a,b) a,b
MACRO (array[x = y, x + 1])
→ array[x = y, x + 1]


If you want to supply array[x = y, x + 1] as an argument, you can write it as array
[(x = y, x + 1)], which is equivalent C code.
#define MACRO(a) a
MACRO (array[(x = y, x + 1)])
→ array[(x = y, x + 1)]

*/

/*
All arguments to a macro are completely macro-expanded before they are substituted into the macro body. After substitution, the complete 
text is scanned again for macros to expand, including the arguments. This rule may seem strange, but it is carefully designed so you need 
not worry about whether any function call is actually a macro invocation.
#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
min (min (a, b), c)
→ min (((a) < (b) ? (a) : (b)), (c))
→ 

((((a) < (b) ? (a) : (b))) < (c)
 ? (((a) < (b) ? (a) : (b)))
 : (c))

*/

/*
the argument which is expanded is considered as a new argument passed to the macro, even it looks like more than one
for example, ADD_PARAMETER(ADD_PARAMETER(0))→ ADD_PARAMETER(0,1) is ok, 0,1 is considered as one argument, but foo(ADD_PARAMETER(ADD_PARAMETER(0)));
is error, because of 0,1,1 is three argument passed to a function
#define ADD_PARAMETER(x)  x, 1
void foo(int x)
{
}
foo(ADD_PARAMETER(ADD_PARAMETER(0)));
→ 
foo(0,1,1)

*/

/*
You can leave macro arguments empty; this is not an error to the preprocessor (but many macros will then expand to invalid code). You 
cannot leave out arguments entirely; if a macro takes two arguments, there must be exactly one comma at the top level of its argument list
. Here are some silly examples using min:

min(, b)        → ((   ) < (b) ? (   ) : (b))
min(a, )        → ((a  ) < ( ) ? (a  ) : ( ))
min(,)          → ((   ) < ( ) ? (   ) : ( ))
min((,),)       → (((,)) < ( ) ? ((,)) : ( ))

min()      error→ macro "min" requires 2 arguments, but only 1 given
min(,,)    error→ macro "min" passed 3 arguments, but takes just 2

*/

/*
Whitespace is not a preprocessing token, so if a macro foo takes one argument, foo () and foo ( ) both supply it an empty argument. 
#define foo(x) (x + 1)
foo() → ( + 1)
foo( ) → ( + 1)
*/

/*
Macro parameters appearing inside string literals are not replaced by their corresponding actual arguments.

#define foo(x) x, "x"
foo(bar)        → bar, "x"

*/

/*
When a macro parameter is used with a leading ‘#’, the preprocessor replaces it with the literal text of the actual argument, 
converted to a string constant. Unlike normal parameter replacement, the argument is not macro-expanded first. 
#define STR(s) #s
#define CALL_FUNC(f, x) f(x)
CALL_FUNC(foo, STR(string));
→
foo("string");

#define X 1
#define Y 2
STR(X + Y)
→
"x + y"

not "3"

another more complicated example:
#define WARN_IF(EXP) \
do { if (EXP) \
        fprintf (stderr, "Warning: " #EXP "\n"); } \
while (0)
WARN_IF (x == 0);
     → do { if (x == 0)
           fprintf (stderr, "Warning: " "x == 0" "\n"); } while (0);

The argument for EXP is substituted once, as-is, into the if statement, and once, stringized, into the argument to fprintf. If x were a 
macro, it would be expanded in the if statement, but not in the string.

*/

/*
Stringizing in C involves more than putting double-quote characters around the fragment. The preprocessor backslash-escapes the quotes 
surrounding embedded string constants, and all backslashes within string and character constants, in order to get a valid C string 
constant with the proper contents. Thus, stringizing p = "foo\n"; results in "p = \"foo\\n\";". However, backslashes that are not inside 
string or character constants are not duplicated: ‘\n’ by itself stringizes to "\n".
STR(p = "foo\n";)
→
"p = \"foo\\n\";"


*/

/*
All leading and trailing whitespace in text being stringized is ignored. Any sequence of whitespace in the middle of the text is 
converted to a single space in the stringized result. Comments are replaced by whitespace long before stringizing happens, so they never 
appear in stringized text.
#define STR(s) #s
char* s = STR(   abc    def  ghi    );
→  
char* s = "abc def ghi";
*/
//#define STR(s) #s
//char* s = STR(   abc/* a long comment */def  ghi    );
//→  
//char* s = "abc def ghi";

/*
There is no way to convert a macro argument into a character constant.

If you want to stringize the result of expansion of a macro argument, you have to use two levels of macros.

#define xstr(s) str(s)
#define str(s) #s
#define foo 4
str (foo)
     → "foo"
xstr (foo)
     → xstr (4)
     → str (4)
     → "4"
s is stringized when it is used in str, so it is not macro-expanded first. But s is an ordinary argument to xstr, so it is completely 
macro-expanded before xstr itself is expanded (see Argument Prescan). Therefore, by the time str gets to its argument, it has already 
been macro-expanded.

*/

/*
However, two tokens that don’t together form a valid token cannot be pasted together. For example, you cannot concatenate x with + in 
either order. If you try, the preprocessor issues a warning and emits the two tokens. Whether it puts white space between the tokens is 
undefined.

#define CONC(x, y) x##y
CONC(x, +);

s_cproc_replace.c:46:10: error: pasting "x" and "+" does not give a valid preprocessing token
     CONC(x, +);
          ^
/home/jason/github/stupid_lib/include/s_text.h:24:20: note: in definition of macro ‘CONC’
 #define CONC(x, y) x##y

*/

/*
As with stringizing, the actual argument is not macro-expanded first. If the argument is empty, that ‘##’ has no effect.
#define CONC(x, y) x##y
#define __CONC(x, y) CONC(x, y)
#define foo 4
CONC(foo, 5)
    → foo5
__CONC(foo, 5) 
    → CONC(4, 5)
    → 45
*/

/*
Keep in mind that the C preprocessor converts comments to whitespace before macros are even considered. Therefore, you cannot create a 
comment by concatenating ‘/’ and ‘*’.
#define CONC(x, y) x##y
CONC(/, *) this is not a comment


s_cproc_replace.c:46:10: error: pasting "/" and "*" does not give a valid preprocessing token
     CONC(/, *) this is not a comment 
          ^
/home/jason/github/stupid_lib/include/s_text.h:24:20: note: in definition of macro ‘CONC’
 #define CONC(x, y) x##y
                    ^

*/

/*
You can put as much whitespace between ‘##’ and its operands as you like, including comments, and you can put comments in arguments 
that will be concatenated.
*/
//#define CONC(x, y) x   ## /* a comment */y
//#define __CONC(x, y) CONC(x, y)
//#define foo 4
//CONC(foo ,/* another comment */ 5  )
//    → foo5



/*
Consider a C program that interprets named commands. There probably needs to be a table of commands, perhaps an array of structures 
declared as follows:

struct command
{
  char *name;
  void (*function) (void);
};

struct command commands[] =
{
  { "quit", quit_command },
  { "help", help_command },
  …
};
It would be cleaner not to have to give each command name twice, once in the string constant and once in the function name. A macro which 
takes the name of a command as an argument can make this unnecessary. The string constant can be created with stringizing, and the 
function name by concatenating the argument with ‘_command’. Here is how it is done:

#define COMMAND(NAME)  { #NAME, NAME ## _command }

struct command commands[] =
{
  COMMAND (quit),
  COMMAND (help),
  …
};

→ 
struct command commands[] =
{
  { "quit", quit_command },
  { "help", help_command },
  …
};

*/

/*
This kind of macro is called variadic. When the macro is invoked, all the tokens in its argument list after the last named argument (this 
macro has none), including any commas, become the variable argument. This sequence of tokens replaces the identifier __VA_ARGS__ in the 
macro body wherever it appears. Thus, we have this expansion:

case 1:
#define eprintf(...) fprintf (stderr, __VA_ARGS__)

eprintf ("%s:%d: ", input_file, lineno)
     →  fprintf (stderr, "%s:%d: ", input_file, lineno)

case 2:
#define eprintf(args...) fprintf (stderr, args)
eprintf ("%s:%d: ", input_file, lineno)
     →  fprintf (stderr, "%s:%d: ", input_file, lineno)

*/

/*
The variable argument is completely macro-expanded before it is inserted into the macro expansion, just like an ordinary argument. You 
may use the ‘#’ and ‘##’ operators to stringize the variable argument or to paste its leading or trailing token with another token
. (But see below for an important special case for ‘##’.)
case 1:
#define eprintf(...) fprintf (stderr, #__VA_ARGS__)

eprintf ("%s:%d: ", input_file, lineno)
     →  fprintf (stderr, "\"%s:%d: \", input_file, lineno");

case 2:
#define eprintf(...) fprintf (stderr, ##__VA_ARGS__)

eprintf ("%s:%d: ", input_file, lineno)
     →  fprintf (stderr, "%s:%d: ", input_file, lineno)

case 2:
#define eprintf(...) fprintf (stderr, __VA_ARGS__##0)

eprintf ("%s:%d: ", input_file, lineno)
     →  fprintf (stderr, "%s:%d: ", input_file, lineno0)

*/

/*
#define eprintf(format, ...) fprintf (stderr, format, __VA_ARGS__)
eprintf("success!\n", );
     → fprintf(stderr, "success!\n", );
s_cproc_replace.c:36:66: error: expected expression before ‘)’ token
 #define eprintf(format, ...) fprintf (stderr, format, __VA_ARGS__)
                                                                  ^
s_cproc_replace.c:44:5: note: in expansion of macro ‘eprintf’
     eprintf("success!\n", );
     ^~~~~~~

*/

/*
Historically, GNU CPP has also had another extension to handle the trailing comma: the ‘##’ token paste operator has a special 
meaning when placed between a comma and a variable argument. Despite the introduction of __VA_OPT__, this extension remains supported in 
GNU CPP, for backward compatibility. If you write


and the variable argument is left out when the eprintf macro is used, then the comma before the ‘##’ will be deleted. This does not 
happen if you pass an empty argument, nor does it happen if the token preceding ‘##’ is anything other than a comma.

case 1:
#define eprintf(format, ...) fprintf (stderr, format, ##__VA_ARGS__)

eprintf ("success!\n")
     → fprintf(stderr, "success!\n");

case 2:
#define eprintf(format, ...) fprintf (stderr, format, ##__VA_ARGS__)

eprintf ("success!\n", )
     → fprintf(stderr, "success!\n", );
s_cproc_replace.c:36:68: error: expected expression before ‘)’ token
 #define eprintf(format, ...) fprintf (stderr, format, ##__VA_ARGS__)
                                                                    ^
s_cproc_replace.c:44:5: note: in expansion of macro ‘eprintf’
     eprintf("success!\n", );
     ^~~~~~~

case 3:
#define eprintf(format, ...) fprintf (stderr, format ##__VA_ARGS__)

eprintf ("success!\n", )
     → fprintf(stderr, "success!\n");

*/

/*
The C standard mandates that the only place the identifier __VA_ARGS__ can appear is in the replacement list of a variadic macro. It may 
not be used as a macro name, macro argument name, or within a different type of macro. It may also be forbidden in open text; the 
standard is ambiguous. We recommend you avoid using it except for its defined purpose.

case 1:
#define __VA_ARGS__ xxx
s_cproc_replace.c:36:9: warning: __VA_ARGS__ can only appear in the expansion of a C99 variadic macro
 #define __VA_ARGS__ xxx
         ^

case 2:
#define MACRO(__VA_ARGS__) #__VA_ARGS__
s_cproc_replace.c:35:15: warning: __VA_ARGS__ can only appear in the expansion of a C99 variadic macro
 #define MACRO(__VA_ARGS__) #__VA_ARGS__
               ^
case 3:
#define MACRO(abc__VA_ARGS__) __VA_ARGS__
../src/s_cproc/s_cproc_replace.c:35:31: warning: __VA_ARGS__ can only appear in the expansion of a C99 variadic macro
 #define MACRO(abc__VA_ARGS__) __VA_ARGS__
                               ^

*/
