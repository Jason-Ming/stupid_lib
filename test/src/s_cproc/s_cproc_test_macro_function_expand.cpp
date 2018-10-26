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
    	//�����Լ��Ĳ���׼��


    }

    void teardown()
    {
        //�����������
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
     �� c_init()
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

//-----------------------3.2 Function-like Macros------------------------------------------------------
/*
������庯����ʱ,�����б�����һ��ʡ�ԺŽ���,��ô���������ʱ,ʵ��(������Щ����Ԥ����Ǻŵ�ʵ��)����ĿӦ���ڶ����ʱ�βε���Ŀ.
����,ʵ�ε���Ŀ�ȶ����ʱ(δʹ��ʡ�Ժ�...)�β���Ŀ��Ҳ����(��Ĳ����ԣ��ᱨ��).��Ĳ����������ԣ��ٵĲ�����ռλ�����
#define ADD_PARAMETER(x)  5 + x##1
void foo(int x)
{
    int x1 = 0;
    x1 = ADD_PARAMETER() + x1;
}
�� 
void foo(int x)
{
    int x1 = 0;
    x1 = 5 + 1 + x1;
}

*/

/*
������������ÿ��ʵ��,����֮��Ӧ����һ����Բ���Ţ�,
�����Բ����Ӧ�Ǻ���֮�����һ��Ԥ����ǺŢ�,����ʼ��һ��Ԥ����Ǻŵ�����,
��Щ���Тܶ����궨���е��滻�б�������(��һ�κ����).
���滻��Ԥ����Ǻŵ���������ƥ�����Բ�����ս�,�ڼ�������������ƥ�����p��Բ���Ŷ�.
�����һ����������õ�Ԥ����Ǻ�������,���з���������ͨ�Ŀհ׷���.

---------------------

�� ������Ϊ�ú�������һ�κ����,�಻���滻,����: 
int t=3;
#define t(a) a
printf("%d\n", t(5) );//t�Ǻ���,t(5)���滻Ϊ5,��������5 
printf("%d\n", t );//t���Ǻ���,�����κ��滻,�������t��ֵ3
�� 
��հ׷�������Ԥ����Ǻ�,���Ժ�������Բ���ż�����пհ׷�,���붨�庯����ʱ��ͬ
�� 
�Լ�������Բ����
�� ����:
#define conncat(x,y) x y
puts(conncat("a"
,"b"));//ʵ��"a"�����л��з�,����չ���Ľ����"ab",������"a"\n"b"

---------------------

*/


/*
�������ƥ���Բ�������޶���Ԥ����Ǻ�����,�γ��˺������ʵ���б�.
�б���ÿ������֮���Զ��ŷָ�,���ڲ���ƥ���Բ����֮��Ķ��Ų�����Ϊ��ǰ���ʵ�ηָ���.
�����ʵ���б��г�����һЩԤ����Ǻ�����,��Щ�����ֿ��ܱ�������Ԥ����ָ��,����Ϊ��δ�����.
#define MIN(x, y) ((x) < (y)? (x):(y))
#define MAX(x, y) ((x) > (y)? (x):(y))

void foo(int x)
{
    int x1 = 0;
    x1 = MAX(MIN(4,5), 6) + x1;
}

�� 
void foo(int x)
{
    int x1 = 0;
    x1 = ((((4) < (5)? (4):(5))) > (6)? (((4) < (5)? (4):(5))):(6)) + x1;
}

*/


/*
A function-like macro is only expanded if its name appears with a pair of parentheses after it. If you write just the name, it is left 
alone. This can be useful when you have a function and a macro of the same name, and you wish to use the function sometimes.
Here the call to foo() will use the macro, but the function pointer will get the address of the real function. If the macro were to be 
expanded, it would cause a syntax error.

                extern void foo(void);
#define foo() /* optimized inline version */
/*
                ��
                  foo();
                  funcptr = foo;

*/


/*
If you put spaces between the macro name and the parentheses in the macro definition, that does not define a function-like macro, it 
defines an object-like macro whose expansion happens to begin with a pair of parentheses.
The first two pairs of parentheses in this expansion come from the macro. The third is the pair that was originally after the macro 
invocation. Since lang_init is an object-like macro, it does not consume those parentheses.

#define lang_init ()    c_init()
lang_init()
     �� () c_init()()
*/

//-------------------3.3 Macro Arguments-------------------------------------------------------------
/*
  The invocation of the macro need not be restricted to a single logical line��it can cross as many lines in the source file as you wish. 
  The number of arguments you give must match the number of parameters in the macro definition. When the macro is expanded, each use of a 
  parameter in its body is replaced by the tokens of the corresponding argument. 

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
  x = min(a, b);          ��  x = ((a) < (b) ? (a) : (b));
  y = min (1, 2);          ��  y = ((1) < (2) ? (1) : (2));
  z = min
  (a + 28, *p);    ��  z = ((a + 28) < (*p) ? (a + 28) : (*p));

*/

/*
Leading and trailing whitespace in each argument is dropped, and all whitespace between the tokens of an argument is reduced to a single 
space. 
#define STR(s) #s
char* s = STR(   abc    def  ghi    );
��  
char* s = "abc def ghi";
*/

/*
Parentheses within each argument must balance; a comma within such parentheses does not end the argument. However, there is no 
requirement for square brackets or braces to balance, and they do not prevent a comma from separating arguments. Thus,

macro (array[x = y, x + 1])
passes two arguments to macro: array[x = y and x + 1]. 
#define MACRO(a,b) a,b
MACRO (array[x = y, x + 1])
�� array[x = y, x + 1]


If you want to supply array[x = y, x + 1] as an argument, you can write it as array
[(x = y, x + 1)], which is equivalent C code.
#define MACRO(a) a
MACRO (array[(x = y, x + 1)])
�� array[(x = y, x + 1)]

*/


/*
All arguments to a macro are completely macro-expanded before they are substituted into the macro body. After substitution, the complete 
text is scanned again for macros to expand, including the arguments. This rule may seem strange, but it is carefully designed so you need 
not worry about whether any function call is actually a macro invocation.
#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
min (min (a, b), c)
�� min (((a) < (b) ? (a) : (b)), (c))
�� 

((((a) < (b) ? (a) : (b))) < (c)
 ? (((a) < (b) ? (a) : (b)))
 : (c))

*/


/*
the argument which is expanded is considered as a new argument passed to the macro, even it looks like more than one
for example, ADD_PARAMETER(ADD_PARAMETER(0))�� ADD_PARAMETER(0,1) is ok, 0,1 is considered as one argument, but foo(ADD_PARAMETER(ADD_PARAMETER(0)));
is error, because of 0,1,1 is three argument passed to a function
#define ADD_PARAMETER(x)  x, 1
void foo(int x)
{
}
foo(ADD_PARAMETER(ADD_PARAMETER(0)));
�� 
foo(0,1,1)

*/


/*
You can leave macro arguments empty; this is not an error to the preprocessor (but many macros will then expand to invalid code). You 
cannot leave out arguments entirely; if a macro takes two arguments, there must be exactly one comma at the top level of its argument list
. Here are some silly examples using min:

min(, b)        �� ((   ) < (b) ? (   ) : (b))
min(a, )        �� ((a  ) < ( ) ? (a  ) : ( ))
min(,)          �� ((   ) < ( ) ? (   ) : ( ))
min((,),)       �� (((,)) < ( ) ? ((,)) : ( ))

min()      error�� macro "min" requires 2 arguments, but only 1 given
min(,,)    error�� macro "min" passed 3 arguments, but takes just 2

*/


/*
Whitespace is not a preprocessing token, so if a macro foo takes one argument, foo () and foo ( ) both supply it an empty argument. 
#define foo(x) (x + 1)
foo() �� ( + 1)
foo( ) �� ( + 1)
*/


/*
Macro parameters appearing inside string literals are not replaced by their corresponding actual arguments.

#define foo(x) x, "x"
foo(bar)        �� bar, "x"

*/


//-----------------3.4 Stringizing------------------------------------------------------------------
/*
������滻�б���,һ���β���������һ��#Ԥ����Ǻ�֮��, ��ô����߶����滻��һ��������խ�ַ���������(character string literal)�ܼǺ�,����������Ԥ����Ǻ�,
���Ӧʵ�ε�ƴд���Т���ȫ��ͬ.

�� ע�������ʵ����չ���ƴд����,��ʹ��Ӧ��ʵ��Ҳ�Ǹ���.

��1:
#define format(x) #x "=%d/n",x
#define N 100
printf( format(N) );//���N=100

����format�滻�б����β�x,����1�γ���ʱ��Ϊ#�Ĳ�����,��ֱ���滻��N;��2�γ���ʱ����#�Ĳ�����,����չ����Ӧ��ʵ��N,����չ�����100�滻��γ���.
���չ����printf( "N" "=%d/n",100 );���ַ������Ӻ��Ϊprintf( "N=%d/n",100 );

��2:
#define str(x) #x
#define str2(x) str(x)
#define myfun(a) a
puts( str( myfun(34) ) );   //��չ����myfun(34),ֱ���ø����������ַ���,�õ����"myfun(34)" 
puts( str2( myfun(34) ) );  //��չ�������myfun(34)(��6.10.3.1��1��,P153),����չ�����34�滻��Ӧ���β�x,�õ�str(34),����չ���õ������"34"

*/


/*
��ʵ��Ԥ����Ǻ��е�ÿ��[������ÿ��]�հ׷�,���������ַ����е�һ���ո��.
ʵ���е�һ��Ԥ����Ǻ�֮ǰ�Ŀհ׷�,�Լ����һ��Ԥ����Ǻ�֮��Ŀհ׷����ᱻɾ����.
�� ����:
#define str(s) #s
puts(str(
����������a
����������a
����������a
����������));���穸����ʾһ���ո��, ��������ʾһ���Ʊ��,��������Ϊa����a����a,
������ԭʵ�Ω���������a����������a����������a����������


*/


/*
ʵ���ڲ�����Ԥ����Ǻŵ�ԭʼƴд���ᱣ��������ַ�������,
�����������ڴ����ַ������������ַ�����ƴд���������:���ʵ���ڲ������ַ��������ַ���������,
��ô��Щ�ַ��������ַ����������е�ÿ��"(������Ϊ�ַ��������һ��")��\�ַ�֮ǰ�����һ��\�ַ���.
�� ����:
#define str(s) #s
printf("%s", str(a\nb "a\nb" \\ '\\' \n '\n'));
��һ�侭���滻��,���:
printf("%s", "a\nb \"a\\nb\" \\ '\\\\' \n '\\n'");
�Ӷ��õ��������:
a(���з�)
b "a\nb" \ '\\' (���з�)
  '\n'
���û�������涨,��ô��һ�侭���滻��,�ͻ���˫���ŵ�Ƕ�׶����ִʷ�������:
printf("%s", "a\nb "a\nb" \\ '\\' \n '\n'");


*/


/*
��ʵ�ζ�Ӧ�Ľ���ǿմ�""

*/


/*
When a macro parameter is used with a leading ��#��, the preprocessor replaces it with the literal text of the actual argument, 
converted to a string constant. Unlike normal parameter replacement, the argument is not macro-expanded first. 
#define STR(s) #s
#define CALL_FUNC(f, x) f(x)
CALL_FUNC(foo, STR(string));
��
foo("string");

#define X 1
#define Y 2
STR(X + Y)
��
"x + y"

not "3"

another more complicated example:
#define WARN_IF(EXP) \
do { if (EXP) \
        fprintf (stderr, "Warning: " #EXP "\n"); } \
while (0)
WARN_IF (x == 0);
     �� do { if (x == 0)
           fprintf (stderr, "Warning: " "x == 0" "\n"); } while (0);

The argument for EXP is substituted once, as-is, into the if statement, and once, stringized, into the argument to fprintf. If x were a 
macro, it would be expanded in the if statement, but not in the string.

*/

/*
Stringizing in C involves more than putting double-quote characters around the fragment. The preprocessor backslash-escapes the quotes 
surrounding embedded string constants, and all backslashes within string and character constants, in order to get a valid C string 
constant with the proper contents. Thus, stringizing p = "foo\n"; results in "p = \"foo\\n\";". However, backslashes that are not inside 
string or character constants are not duplicated: ��\n�� by itself stringizes to "\n".
STR(p = "foo\n";)
��
"p = \"foo\\n\";"


*/

/*
All leading and trailing whitespace in text being stringized is ignored. Any sequence of whitespace in the middle of the text is 
converted to a single space in the stringized result. Comments are replaced by whitespace long before stringizing happens, so they never 
appear in stringized text.
#define STR(s) #s
char* s = STR(   abc    def  ghi    );
��  
char* s = "abc def ghi";
*/
//#define STR(s) #s
//char* s = STR(   abc/* a long comment */def  ghi    );
//��  
//char* s = "abc def ghi";

/*
There is no way to convert a macro argument into a character constant.

If you want to stringize the result of expansion of a macro argument, you have to use two levels of macros.

#define xstr(s) str(s)
#define str(s) #s
#define foo 4
str (foo)
     �� "foo"
xstr (foo)
     �� xstr (4)
     �� str (4)
     �� "4"
s is stringized when it is used in str, so it is not macro-expanded first. But s is an ordinary argument to xstr, so it is completely 
macro-expanded before xstr itself is expanded (see Argument Prescan). Therefore, by the time str gets to its argument, it has already 
been macro-expanded.

*/

/*
-------------------------------3.5 Concatenation-------------------------------------------------------------------------------------------
It is often useful to merge two tokens into one while expanding macros. This is called token pasting or token concatenation. The ��##�� 
preprocessing operator performs token pasting. When a macro is expanded, the two tokens on either side of each ��##�� operator are 
combined into a single token, which then replaces the ��##�� and the two original tokens in the macro expansion. Usually both will be 
identifiers, or one will be an identifier and the other a preprocessing number. When pasted, they make a longer identifier. This isn��t 
the only valid case. It is also possible to concatenate two numbers (or a number and a name, such as 1.5 and e3) into a number. Also, 
multi-character operators such as += can be formed by token pasting.

*/

/*
�ں궨����κ�һ����ʽ��,##��Ӧ�������滻�б�Ŀ�ͷ���βλ�â�.
�� ����ζ�Ų���ֱ�ӽ�##�����һ����,��������д������:#define MACRO ##,��ȷд����:#define MACRO # ## #,
����ҳ��4�������


*/


/*
����ں�������滻�б���,һ���β���������##Ԥ����Ǻ�֮ǰ��֮��,
��ô����βν�����Ӧʵ�ε�Ԥ����Ǻ����Т����滻.Ȼ��,���һ��ʵ���в���Ԥ����ǺŢ�,
��ô��Ӧ���βν���һ��ռλ��(placemarker)Ԥ����Ǻ�������.

�� ������ʵ����չ��Ľ��,��ʹ��Ӧ��ʵ��Ҳ�Ǹ���,����??? 
#define connect(x) i ## x
#define connect2(x) connect(x)
#define s(a) a
#define is(a) a
int i2=2;
printf("%d/n", connect(s(1)) ); //connect���β�x��##�Ĳ�����,�ʲ�չ������Ӧ��ʵ��s,ֱ�����ӼǺ�i��ʵ������s(1),�õ�is(1),�����滻�õ������1? 
printf("%d/n", connect2(s(2)) ); //connect2���β�x����##�Ĳ�����,����չ������Ӧ��ʵ��s,����չ�����2�滻֮,�õ�connect(2),�����滻�õ������i2*

�� ����ʵ��Ϊ��


*/


/*
���ڶ����ͺ��������ߵĵ���,�滻�б���ÿ��##Ԥ����Ǻ�(����������ʵ�ε�##)��ɾ��,
������ǰ���Ԥ����Ǻ�������Ԥ����Ǻ������Ӣ�.֮��,���ǵ��滻�б��ظ����,
���ø���ĺ����ܱ��滻.ռλ��Ԥ����Ǻű����⴦��:����ռλ�����ӵĽ����һ��ռλ��;
һ��ռλ����һ����ռλ�����ӵĽ�����Ǹ���ռλ��.������ӵĽ������һ���Ϸ���Ԥ����Ǻ�,
��ô��Ϊ��δ�����.��Ϊ���ӽ���ļǺŻ������ڸ���һ���ĺ��滻.��##����������ֵ˳����δȷ����.

�� ע��Ԥ����Ǻ��ǲ����հ׷���,���##������������2��������(��ǰ��2���β�)��Ŀհ׷�������,
������ͨ���ʽ������

�� ��������ĳ���Ƭ��
#define hash_hash # ## #
#define mkstr(a) # a
#define in_between(a) mkstr(a)
#define join(c, d) in_between(c hash_hash d)

char p[] = join(x, y); // ��ͬ��char p[] = "x ## y";
չ�����̱仯�Ľ׶�:
join(x, y)��
in_between(x hash_hash y)
in_between(x ## y)
mkstr(x ## y)
"x ## y"
���仰˵,��hash_hash��չ��������һ���µļǺ�,�����������������ڵķ���##,������µļǺŲ�����##������.

�� ���ں����join(x, y)��չ��˳��:����ʵ��x,y�滻join���β�,join���滻�б��Ϊin_between(x hash_hash y).
Ȼ���ظ�ɨ���滻�б�,����in_between(x hash_hash y)���Ǻ�,��������չʵ���еĺ�hash_hash,�õ�in_between(x ## y),
������չ�����x ## y�滻in_between��Ӧ���β�a,�õ�mkstr(x ## y).��һ��չ���õ����ս��"x ## y"


*/



/*
���滻�б��������βζ����滻�p#��##�����ѷ���֮��,��ɾ�����е�ռλ��.Ȼ��,���Ԥ����Ǻ�����
,��ͬԴ�ļ������к��Ԥ����Ǻ�һ��,���ظ�ɨ��,��ʹ����ĺ������滻��.
�� ����
#define s(a) a
#define t(b) b
printf("%d/n", s(t)(3) ); //�Ƚ�s(t)�滻��t,Ȼ��t��ͬ���Ԥ����Ǻ�(3)һ���ظ�ɨ��,��ʱ����t(3)���Ǻ�,�����滻�������3

*/



/*
��������滻�ĺ��������ɨ���滻�б�(��δɨ�赽Դ�ļ�������Ԥ����ǺŲ���ʱ)
�������ֱ����ֵĻ�,�ú������ٱ��滻��.��һ��˵,
����κ�Ƕ�׵��滻�����������������滻�ĺ������,��ô�ú������ٱ��滻��.��Щ���滻�ĺ����Ǻ�,
���ٴ�ɨ��p��һ�����滻�����಻�ٿ���,��ʹ�Ժ����ǽ����������б��ظ����,������Щ��������,
���ǿ��ܻ������滻��.

�� ����:
#define puts(s) printf("%s is ",#s);puts(s));//������һ����,���Զ�����д�⺯��puts
char translator[]={-70,-6,-47,-27,0};
puts(translator);
����ɨ��puts���滻�б�ʱ,�������������滻�ĺ���puts,
�ʲ����滻,չ�����Ϊprintf("%s? is",translator);puts(translator)

�� ����:
int M=3;
#define M N
#define N M*M
printf("%d/n",M); 
//���滻��M�Ĺ������ַ����˺�N,��ô�Ժ�N�滻�Ĺ���,��������M���滻������˵,����Ƕ�׵��滻����.
//���ڴ˹������������������滻�ĺ���M,��ÿ�ζ������滻,����չ�������M*M,������Ϊ9

ע��:Ҫ���֡�Ƕ�׵��滻���̡��롰���Ƕ�׵��á�.Ƕ�׵��滻����ָ���滻�б����ֳ�����(��Ƕ��)
��,�����Ҫ(����Ƕ��)���������滻,��ǳ����,�������.���Ƕ�׵���ָ�ں������ʵ�����ֳ����˺�
(�����������),�����Ҫ��չʵ���еĺ�(�ڲ��),��չ������(����)��,���ڶ���,��㱬��,
����������ʱʵ�ε���ֵ����.

����������÷�:
#define DOUBLE(a) a*2
printf("%d/n", DOUBLE(DOUBLE(2)) ); //���8

�����������: ��ɨ�赽��1��DOUBLE(����)ʱ,������DOUBLE�滻�Ĺ���.��ɨ�赽��2��DOUBLE(�ڲ��)
ʱ,���ڵ�ǰ�����滻(����)DOUBLE,�㲻���滻�ڲ��DOUBLE��.��Ϊͨ�����﷨�������̲���������,����:(1
)��ɨ�赽��1��DOUBLE(ʱ,�����ƽ�(2)���ƽ���DOUBLE(DOUBLE(2)ʱ,ʶ���DOUBLE(2)Ϊһ�κ����,
��Լ��ִ����Ӧ���嶯��,ͨ����չ��DOUBLE(2),�õ�2*2,���滻��ԭDOUBLE(2).��ʱ�ƽ����ݱ�ΪDOUBLE(2*2(3)
�����ƽ�������),�õ�DOUBLE(2*2),ʶ���DOUBLE(2*2)Ϊһ�κ����,��Լ��ִ����Ӧ���嶯��,չ��DOUBLE(2*2),
�õ����ս��2*2*2.

�� P156��3���������


*/


/*
����������ȫ�滻�Ľ������,��ʹ������һ��Ԥ����ָ��,Ҳ�����ٲ�����Ԥ����ָ��ȥ�����.
�����ڽ������������_PragmaһԪ���������ʽ,�ᰴ������6.10.9[P161]��ָ���ķ�ʽ������.
�� ����ζ�Ų���ָ���ú�������һ��Ԥ����ָ��,���������÷����Ǵ����.����ԭ�򻹿����P146��2��,P147��7��:
#define INCL_STD #include<stdio.h> //��ͼ�ú����һ��#includeָ��
INCL_STD//����

#define ENDIF #endif//��ͼ�ú����һ��#endifָ��
#if 1
ENDIF//����

#define DEF_PI #define PI 3.1415926//��ͼ�ú����һ��#defineָ��
DEF_PI//����


[С��:����ĺ��滻�㷨�䲻������,ȴ����������˼·]��
�� void expand(x)//��x�滻�㷨
{
    if(x�Ǻ�����),���x���滻�б���,ÿ���β�p�ĳ��ֶ�ִ������Ĳ���
    {
        if(p��Ӧ��ʵ�β��Ƕ���� && p��Ӧ��ʵ�β�����һ�κ�����ĵ��� //��:p��Ӧ��ʵ�β��Ǻ�
            || p�˴���Ϊ##�Ĳ�����������)
        {
            ��p�滻�ɶ�Ӧʵ�ε�ƴд����
        }
        else if(p�˴���Ϊ#�Ĳ�����������)
        {
            ��p��������#������һ���滻��p��Ӧʵ�ε�ƴд����
        }
        else//p��Ӧ��ʵ��a�Ǻ�
        {
            expand(a);//�ݹ����expandչ����a��p�滻��aչ����Ľ������
        }
    }
    if(x���滻�б��к���##)
    {
        ��##��ͬ��ǰ��Ŀհ׷�һ��ɾ��; //�Ǻ�����
    }
    ��x���ڵ��滻�б�,����Դ�ļ���x����ε���; //���滻
    ���ղŶ�x�滻�Ľ��,��ͬԴ�ļ�������һ���ظ�ɨ��,��ʱ���������һ������y

    if(y�ǳ�����������)
    {
        expand(y);//�ݹ����expandչ����y
    }
    else//y�ǳ������滻�б���
    {
        if(y!=x //y���ǵ�ǰ�����滻�ĺ���
            &&yδ�����Ϊ������) //��y���Ǹ����滻���������滻�ĺ���
        {
            expand(y);//�ݹ����expandչ����y
        }
        else
        {
            ��y���Ϊ������;//ʹy���Ӳ��滻������Ҳ�����滻
        }
    }
}

*/



/*
However, two tokens that don��t together form a valid token cannot be pasted together. For example, you cannot concatenate x with + in 
either order. If you try, the preprocessor issues a warning and emits the two tokens. Whether it puts white space between the tokens is 
undefined.

#define CONC(x, y) x##y
CONC(x, +);

s_cproc_replace.c:46:10: error: pasting "x" and "+" does not give a valid preprocessing token
     CONC(x, +);
          ^
/home/jason/github/stupid_lib/include/s_text.h:24:20: note: in definition of macro ��CONC��
 #define CONC(x, y) x##y

*/

/*
As with stringizing, the actual argument is not macro-expanded first. If the argument is empty, that ��##�� has no effect.
#define CONC(x, y) x##y
#define __CONC(x, y) CONC(x, y)
#define foo 4
CONC(foo, 5)
    �� foo5
__CONC(foo, 5) 
    �� CONC(4, 5)
    �� 45
*/

/*
Keep in mind that the C preprocessor converts comments to whitespace before macros are even considered. Therefore, you cannot create a 
comment by concatenating ��/�� and ��*��.
#define CONC(x, y) x##y
CONC(/, *) this is not a comment


s_cproc_replace.c:46:10: error: pasting "/" and "*" does not give a valid preprocessing token
     CONC(/, *) this is not a comment 
          ^
/home/jason/github/stupid_lib/include/s_text.h:24:20: note: in definition of macro ��CONC��
 #define CONC(x, y) x##y
                    ^

*/

/*
You can put as much whitespace between ��##�� and its operands as you like, including comments, and you can put comments in arguments 
that will be concatenated.
*/
//#define CONC(x, y) x   ## /* a comment */y
//#define __CONC(x, y) CONC(x, y)
//#define foo 4
//CONC(foo ,/* another comment */ 5  )
//    �� foo5



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
  ��
};
It would be cleaner not to have to give each command name twice, once in the string constant and once in the function name. A macro which 
takes the name of a command as an argument can make this unnecessary. The string constant can be created with stringizing, and the 
function name by concatenating the argument with ��_command��. Here is how it is done:

#define COMMAND(NAME)  { #NAME, NAME ## _command }

struct command commands[] =
{
  COMMAND (quit),
  COMMAND (help),
  ��
};

�� 
struct command commands[] =
{
  { "quit", quit_command },
  { "help", help_command },
  ��
};

*/


/*
��4 ����˵���ַ�����������Ǻ����ӵĹ���,�����ָ������??? 
#define str(s) # s
#define xstr(s) str(s)
#define debug(s, t) printf("x" # s "= %d, x" # t "= %s",\
    x ## s, x ## t)
#define INCFILE(n)? vers ## n // ����ǰ��#include������[��P150��2]
#define glue(a, b)? a ## b
#define xglue(a, b) glue(a, b)
#define HIGHLOW "hello"
#define LOW LOW ", world"
debug(1, 2);
fputs(str(strncmp("abc/0d", "abc", '/4') // ��ע�ͽ���ɾ��
    == 0) str(: @/n), s); ��

#include xstr(INCFILE(2).h) ��
glue(HIGH, LOW); ��
xglue(HIGH, LOW) ��

�����滻��,���������½��:

printf("x" "1" "= %d, x" "2" "= %s", x1, x2);
fputs(
    "strncmp(/"abc//0d/", /"abc/", '//4') == 0" ": @/n",
    s);
#include "vers2.h" (�ں��滻֮��p�����ļ�֮ǰ��)
"hello";
"hello" ", world"

����,���ַ�������֮��,

printf("x1= %d, x2= %s", x1, x2);
fputs(
    "strncmp(/"abc//0d/", /"abc/", '//4') == 0: @/n",
    s);
#include "vers2.h" (�ں��滻֮��p�����ļ�֮ǰ��)
"hello";
"hello, world"

�ں궨����,#��##�Ǻ��������ߵĿո��ǿ�ѡ��.

---------------------


�� ע��:str(: @/n)�ڵ�/֮ǰ�������/�ַ�,��6.10.3.2��2��,P153
�� ���ڵ���xstr(INCFILE(2).h)��չ������:
1.չ��xstr(INCFILE(2).h)��ʵ��INCFILE(2).h
    1.1.ʵ��INCFILE(2).h��INCFILE�Ǻ�,������չ��INCFILE(2).��INCFILE(2)��ʵ��2�滻��Ӧ���β�,�õ�vers2
    1.2.��INCFILE(2)��չ�����vers2,��ͬ����������.hһ���ظ�ɨ��(��6.10.3.4��1��,P155),δ������Ҫ�滻�ĺ�
    1.3.�õ�INCFILE(2).h��չ�����vers2.h
2.��ʵ��INCFILE(2).hչ���Ľ��vers2.h�滻��Ӧ���β�,�õ�str(vers2.h).�ظ�ɨ��,����str�Ǻ�,����ֱ����str��ʵ������vers2.h�����ַ���(��6.10.3.2��2��,P153),�õ�"vers2.h"
3.���ǵõ����ս��"vers2.h"
�� ���ڵ���glue(HIGH, LOW)��չ������:ʵ��LOW�Ǻ�,����������Ӧ���β���##�Ĳ�����,�ʲ���չ��.
����ֱ������ʵ������HIGH��LOW(��6.10.3.3��2��,P154),�õ�HIGHLOW.�ظ�ɨ���滻���(��6.10.3.4��1��,P155),
����HIGHLOW���Ǻ�,��һ��չ���õ������"hello".
�� ���ڵ���xglue(HIGH, LOW)��չ������:1.չ��xglue(HIGH, LOW)�е�ʵ��LOW,�õ�LOW ", world".�ظ�ɨ���滻���,
����LOW���Ǻ�,ȴ�ǵ�ǰ���ڴ���ĺ�,�ʲ��ٵݹ��滻(��6.10.3.4��2��,P155).���ǵõ�LOW���滻���LOW ", 
world".2.��ʵ��HIGH,��ʵ��LOW��չ������滻��Ӧ���β�,�õ�glue(HIGH, LOW ", world").�ظ�ɨ��,����glue�Ǻ�,
�����ټ���չ��glue(HIGH, LOW ", world").3.glue��ʵ��LOW ", world"�к��к�,����������Ӧ���β���##�Ĳ�����,
�ʲ���չ��.����ֱ������2��ʵ������HIGH��LOW ", world",�õ�HIGHLOW ", world".�ظ�ɨ���滻���,
����HIGHLOW���Ǻ�,��һ��չ���õ����ս��"hello" ", world".(
��2���ڽ����ַ������ں�������׶α����ӳ�һ��,��P10��6��)

�� �����˸��ļ���,����#includeָ��㱻ɾ����

*/


/*
��5 ����˵����ռλ����Ԥ����Ǻŵ���ع���,�����ָ������??? 
#define t(x,y,z) x ## y ## z
int j[] = { t(1,2,3), t(,4,5), t(6,,7), t(8,9,),
    t(10,,), t(,11,), t(,,12), t(,,) };

�����滻��,���������½��
int j[] = { 123, 45, 67, 89,
    10, 11, 12,? };


*/


//--------------------------3.6 Variadic Macros--------------------------------------------------------------------------------------------
/*
12    ����궨��ı�ʶ���б����и�ʡ�Ժ�,��ôβ�������ɸ�ʵ��,�����ú��κηָ�ʵ�εĶ���,
�����ϲ������һ����Ϊ�ɱ�ʵ��(variable arguments)�ĵ�һ��Ŀ.ʵ����˽��,ʹ���ںϲ�֮��,
ʵ�ε���Ŀ�Ⱥ궨��ʱ�βε���Ŀ���1��(����ʡ�Ժ�).

*/
/*
This kind of macro is called variadic. When the macro is invoked, all the tokens in its argument list after the last named argument (this 
macro has none), including any commas, become the variable argument. This sequence of tokens replaces the identifier __VA_ARGS__ in the 
macro body wherever it appears. Thus, we have this expansion:

case 1:
#define eprintf(...) fprintf (stderr, __VA_ARGS__)

eprintf ("%s:%d: ", input_file, lineno)
     ��  fprintf (stderr, "%s:%d: ", input_file, lineno)

case 2:
#define eprintf(args...) fprintf (stderr, args)
eprintf ("%s:%d: ", input_file, lineno)
     ��  fprintf (stderr, "%s:%d: ", input_file, lineno)

*/

/*
The variable argument is completely macro-expanded before it is inserted into the macro expansion, just like an ordinary argument. You 
may use the ��#�� and ��##�� operators to stringize the variable argument or to paste its leading or trailing token with another token
. (But see below for an important special case for ��##��.)
case 1:
#define eprintf(...) fprintf (stderr, #__VA_ARGS__)

eprintf ("%s:%d: ", input_file, lineno)
     ��  fprintf (stderr, "\"%s:%d: \", input_file, lineno");

case 2:
#define eprintf(...) fprintf (stderr, ##__VA_ARGS__)

eprintf ("%s:%d: ", input_file, lineno)
     ��  fprintf (stderr, "%s:%d: ", input_file, lineno)

case 2:
#define eprintf(...) fprintf (stderr, __VA_ARGS__##0)

eprintf ("%s:%d: ", input_file, lineno)
     ��  fprintf (stderr, "%s:%d: ", input_file, lineno0)

*/

/*
#define eprintf(format, ...) fprintf (stderr, format, __VA_ARGS__)
eprintf("success!\n", );
     �� fprintf(stderr, "success!\n", );
s_cproc_replace.c:36:66: error: expected expression before ��)�� token
 #define eprintf(format, ...) fprintf (stderr, format, __VA_ARGS__)
                                                                  ^
s_cproc_replace.c:44:5: note: in expansion of macro ��eprintf��
     eprintf("success!\n", );
     ^~~~~~~

*/

/*
Historically, GNU CPP has also had another extension to handle the trailing comma: the ��##�� token paste operator has a special 
meaning when placed between a comma and a variable argument. Despite the introduction of __VA_OPT__, this extension remains supported in 
GNU CPP, for backward compatibility. If you write


and the variable argument is left out when the eprintf macro is used, then the comma before the ��##�� will be deleted. This does not 
happen if you pass an empty argument, nor does it happen if the token preceding ��##�� is anything other than a comma.

case 1:
#define eprintf(format, ...) fprintf (stderr, format, ##__VA_ARGS__)

eprintf ("success!\n")
     �� fprintf(stderr, "success!\n");

case 2:
#define eprintf(format, ...) fprintf (stderr, format, ##__VA_ARGS__)

eprintf ("success!\n", )
     �� fprintf(stderr, "success!\n", );
s_cproc_replace.c:36:68: error: expected expression before ��)�� token
 #define eprintf(format, ...) fprintf (stderr, format, ##__VA_ARGS__)
                                                                    ^
s_cproc_replace.c:44:5: note: in expansion of macro ��eprintf��
     eprintf("success!\n", );
     ^~~~~~~

case 3:
#define eprintf(format, ...) fprintf (stderr, format ##__VA_ARGS__)

eprintf ("success!\n", )
     �� fprintf(stderr, "success!\n");

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



/*
��7 ���,չʾ���пɱ�����б�ĺ�:
#define debug(...) fprintf(stderr, __VA_ARGS__)
#define showlist(...) puts(#__VA_ARGS__)
#define report(test, ...) ((test)?puts(#test):\
    printf(__VA_ARGS__))
debug("Flag");
debug("X = %d/n", x);
showlist(The first, second, and third items.);
report(x>y, "x is %d but y is %d", x, y);

�����滻��,���������½��
fprintf(stderr,? "Flag" );
fprintf(stderr,? "X = %d/n", x );
puts("The first, second, and third items." );
((x>y)?puts("x>y"):
    printf("x is %d but y is %d", x, y))


*/
