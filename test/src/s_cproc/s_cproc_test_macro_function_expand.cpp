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

//-----------------------3.2 Function-like Macros------------------------------------------------------
/*
如果定义函数宏时,参数列表不是以一个省略号结束,那么调用这个宏时,实参(包括那些不含预处理记号的实参)的数目应等于定义宏时形参的数目.
另外,实参的数目比定义宏时(未使用省略号...)形参数目多也可以(多的不可以，会报错).多的参数将被忽略，少的参数被占位符替代
#define ADD_PARAMETER(x)  5 + x##1
void foo(int x)
{
    int x1 = 0;
    x1 = ADD_PARAMETER() + x1;
}
→ 
void foo(int x)
{
    int x1 = 0;
    x1 = 5 + 1 + x1;
}

*/

/*
对于随后函数宏的每个实例,宏名之后应跟着一个左圆括号②,
这个左圆括号应是宏名之后的下一个预处理记号③,它开始了一段预处理记号的序列,
这些序列④都被宏定义中的替换列表所代替(即一次宏调用).
被替换的预处理记号的序列以相匹配的右圆括号终结,期间跳过其它互相匹配的左﹑右圆括号对.
在组成一条函数宏调用的预处理记号序列中,换行符被当作普通的空白符⑤.

---------------------

② 否则不认为该宏名构成一次宏调用,亦不会替换,例如: 
int t=3;
#define t(a) a
printf("%d\n", t(5) );//t是宏名,t(5)被替换为5,输出结果是5 
printf("%d\n", t );//t不是宏名,不作任何替换,输出变量t的值3
③ 
因空白符不属于预处理记号,所以宏名与左圆括号间可以有空白符,这与定义函数宏时不同
④ 
以及宏名和圆括号
⑤ 例如:
#define conncat(x,y) x y
puts(conncat("a"
,"b"));//实参"a"后面有换行符,但宏展开的结果是"ab",而不是"a"\n"b"

---------------------

*/


/*
最外层相匹配的圆括号所限定的预处理记号序列,形成了函数宏的实参列表.
列表中每个参数之间以逗号分隔,但内层相匹配的圆括号之间的逗号并不作为当前宏的实参分隔符.
如果在实参列表中出现了一些预处理记号序列,这些序列又可能被当作是预处理指令,则行为是未定义的.
#define MIN(x, y) ((x) < (y)? (x):(y))
#define MAX(x, y) ((x) > (y)? (x):(y))

void foo(int x)
{
    int x1 = 0;
    x1 = MAX(MIN(4,5), 6) + x1;
}

→ 
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
                …
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
     → () c_init()()
*/

//-------------------3.3 Macro Arguments-------------------------------------------------------------
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


//-----------------3.4 Stringizing------------------------------------------------------------------
/*
如果在替换列表中,一个形参立即跟在一个#预处理记号之后, 那么这二者都被替换成一个单独的窄字符串文字量(character string literal)④记号,它所包含的预处理记号,
与对应实参的拼写序列⑤完全相同.

⑤ 注意而不是实参扩展后的拼写序列,即使对应的实参也是个宏.

例1:
#define format(x) #x "=%d/n",x
#define N 100
printf( format(N) );//输出N=100

对于format替换列表中形参x,它第1次出现时作为#的操作数,故直接替换成N;第2次出现时不是#的操作数,故先展开对应的实参N,再用展开结果100替换这次出现.
最后展开成printf( "N" "=%d/n",100 );经字符串连接后成为printf( "N=%d/n",100 );

例2:
#define str(x) #x
#define str2(x) str(x)
#define myfun(a) a
puts( str( myfun(34) ) );   //不展开宏myfun(34),直接用该序列生成字符串,得到结果"myfun(34)" 
puts( str2( myfun(34) ) );  //先展开宏调用myfun(34)(见6.10.3.1第1点,P153),再用展开结果34替换对应的形参x,得到str(34),继续展开得到最后结果"34"

*/


/*
在实参预处理记号中的每段[而不是每个]空白符,都变成这个字符串中的一个空格符.
实参中第一个预处理记号之前的空白符,以及最后一个预处理记号之后的空白符都会被删除⑥.
⑥ 例如:
#define str(s) #s
puts(str(
└┘└─┘a
└┘└─┘a
└┘└─┘a
└┘└─┘));假如└┘表示一个空格符, └─┘表示一个制表符,则输出结果为a└┘a└┘a,
而不是原实参└┘└─┘a└┘└─┘a└┘└─┘a└┘└─┘


*/


/*
实参内部其它预处理记号的原始拼写都会保留在这个字符串当中,
除了下面用于处理字符串文字量和字符常量拼写的特殊操作:如果实参内部含有字符常量或字符串文字量,
那么这些字符常量或字符串文字量中的每个"(包括作为字符串界符的一对")和\字符之前会插入一个\字符⑦.
⑦ 例如:
#define str(s) #s
printf("%s", str(a\nb "a\nb" \\ '\\' \n '\n'));
上一句经宏替换后,变成:
printf("%s", "a\nb \"a\\nb\" \\ '\\\\' \n '\\n'");
从而得到如下输出:
a(换行符)
b "a\nb" \ '\\' (换行符)
  '\n'
如果没有上述规定,那么上一句经宏替换后,就会因双引号的嵌套而出现词法错误了:
printf("%s", "a\nb "a\nb" \\ '\\' \n '\n'");


*/


/*
空实参对应的结果是空串""

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
-------------------------------3.5 Concatenation-------------------------------------------------------------------------------------------
It is often useful to merge two tokens into one while expanding macros. This is called token pasting or token concatenation. The ‘##’ 
preprocessing operator performs token pasting. When a macro is expanded, the two tokens on either side of each ‘##’ operator are 
combined into a single token, which then replaces the ‘##’ and the two original tokens in the macro expansion. Usually both will be 
identifiers, or one will be an identifier and the other a preprocessing number. When pasted, they make a longer identifier. This isn’t 
the only valid case. It is also possible to concatenate two numbers (or a number and a name, such as 1.5 and e3) into a number. Also, 
multi-character operators such as += can be formed by token pasting.

*/

/*
在宏定义的任何一种形式中,##不应出现在替换列表的开头或结尾位置①.
① 这意味着不能直接将##定义成一个宏,例如下面写法错误:#define MACRO ##,正确写法是:#define MACRO # ## #,
见本页第4点的例子


*/


/*
如果在函数宏的替换列表中,一个形参立即跟在##预处理记号之前或之后,
那么这个形参将被对应实参的预处理记号序列②所替换.然而,如果一个实参中不含预处理记号③,
那么对应的形参将被一个占位符(placemarker)预处理记号所代替.

② 而不是实参扩展后的结果,即使对应的实参也是个宏,例如??? 
#define connect(x) i ## x
#define connect2(x) connect(x)
#define s(a) a
#define is(a) a
int i2=2;
printf("%d/n", connect(s(1)) ); //connect的形参x是##的操作数,故不展开它对应的实参s,直接连接记号i和实参序列s(1),得到is(1),继续替换得到最后结果1? 
printf("%d/n", connect2(s(2)) ); //connect2的形参x不是##的操作数,故先展开它对应的实参s,再用展开结果2替换之,得到connect(2),继续替换得到最后结果i2*

③ 比如实参为空


*/


/*
对于对象宏和函数宏两者的调用,替换列表中每个##预处理记号(而不是来自实参的##)被删除,
并且它前面的预处理记号与后面的预处理记号相连接④.之后,它们的替换列表被重复检查,
以让更多的宏名能被替换.占位符预处理记号被特殊处理:两个占位符连接的结果是一个占位符;
一个占位符与一个非占位符连接的结果是那个非占位符.如果连接的结果不是一个合法的预处理记号,
那么行为是未定义的.作为连接结果的记号还可用于更进一步的宏替换.对##操作符的求值顺序是未确定的.

④ 注意预处理记号是不含空白符的,因此##操作符与它的2个操作数(即前后2个形参)间的空白符被忽略,
就像普通表达式中那样

例 对于下面的程序片段
#define hash_hash # ## #
#define mkstr(a) # a
#define in_between(a) mkstr(a)
#define join(c, d) in_between(c hash_hash d)

char p[] = join(x, y); // 等同于char p[] = "x ## y";
展开过程变化的阶段:
join(x, y)⑤
in_between(x hash_hash y)
in_between(x ## y)
mkstr(x ## y)
"x ## y"
换句话说,宏hash_hash的展开产生了一个新的记号,它含有两个明显相邻的符号##,但这个新的记号并不是##操作符.

⑤ 对于宏调用join(x, y)的展开顺序:先用实参x,y替换join的形参,join的替换列表变为in_between(x hash_hash y).
然后重复扫描替换列表,发现in_between(x hash_hash y)仍是宏,于是先扩展实参中的宏hash_hash,得到in_between(x ## y),
接着用展开结果x ## y替换in_between对应的形参a,得到mkstr(x ## y).进一步展开得到最终结果"x ## y"


*/



/*
在替换列表中所有形参都被替换﹑#和##处理都已发生之后,便删除所有的占位符.然后,结果预处理记号序列
,连同源文件的所有后继预处理记号一起,被重复扫描,以使更多的宏名被替换①.
① 例如
#define s(a) a
#define t(b) b
printf("%d/n", s(t)(3) ); //先将s(t)替换成t,然后t连同后继预处理记号(3)一起重复扫描,这时发现t(3)又是宏,将其替换成最后结果3

*/



/*
如果正在替换的宏的名字在扫描替换列表(尚未扫描到源文件的其它预处理记号部分时)
过程中又被发现的话,该宏名不再被替换②.进一步说,
如果任何嵌套的替换过程内遇到了正在替换的宏的名字,那么该宏名不再被替换③.这些不替换的宏名记号,
对再次扫描﹑进一步的替换过程亦不再可用,即使稍后它们将在上下文中被重复检查,而在这些上下文中,
它们可能会另作替换④.

② 例如:
#define puts(s) printf("%s is ",#s);puts(s));//利用这一特性,用自定义宏改写库函数puts
char translator[]={-70,-6,-47,-27,0};
puts(translator);
由于扫描puts的替换列表时,又遇到了正在替换的宏名puts,
故不再替换,展开结果为printf("%s? is",translator);puts(translator)

③ 例如:
int M=3;
#define M N
#define N M*M
printf("%d/n",M); 
//在替换宏M的过程中又发现了宏N,那么对宏N替换的过程,相对于外层M的替换过程来说,便是嵌套的替换过程.
//由于此过程中又遇到了正在替换的宏名M,故每次都不再替换,最终展开结果是M*M,输出结果为9

注意:要区分“嵌套的替换过程”与“宏的嵌套调用”.嵌套的替换过程指在替换列表中又出现了(内嵌的)
宏,因此需要(对内嵌宏)进行深层的替换,由浅入深,层层入内.宏的嵌套调用指在函数宏的实参中又出现了宏
(函数宏或对象宏),因此需要先展实参中的宏(内层宏),再展开外层的(函数)宏,由内而外,层层爆开,
就像函数调用时实参的求值那样.

例如对下面用法:
#define DOUBLE(a) a*2
printf("%d/n", DOUBLE(DOUBLE(2)) ); //输出8

不能这样理解: 当扫描到第1个DOUBLE(外层的)时,便进入宏DOUBLE替换的过程.当扫描到第2个DOUBLE(内层的)
时,由于当前正在替换(外层的)DOUBLE,便不再替换内层的DOUBLE了.因为通常的语法分析过程不是这样的,而是:(1
)当扫描到第1个DOUBLE(时,继续移进(2)当移进了DOUBLE(DOUBLE(2)时,识别出DOUBLE(2)为一次宏调用,
归约并执行相应语义动作,通常是展开DOUBLE(2),得到2*2,并替换掉原DOUBLE(2).此时移进内容变为DOUBLE(2*2(3)
继续移进右括号),得到DOUBLE(2*2),识别出DOUBLE(2*2)为一次宏调用,归约并执行相应语义动作,展开DOUBLE(2*2),
得到最终结果2*2*2.

④ P156例3有相关例子


*/


/*
对于最终完全替换的结果序列,即使它像是一条预处理指令,也不会再步当作预处理指令去处理⑤.
但对于结果序列中所有_Pragma一元操作符表达式,会按照下面6.10.9[P161]所指定的方式被处理.
⑤ 这意味着不能指望用宏来代替一条预处理指令,例如下面用法都是错误的.错误原因还可另见P146第2点,P147第7点:
#define INCL_STD #include<stdio.h> //试图用宏代替一条#include指令
INCL_STD//错误

#define ENDIF #endif//试图用宏代替一条#endif指令
#if 1
ENDIF//错误

#define DEF_PI #define PI 3.1415926//试图用宏代替一条#define指令
DEF_PI//错误


[小结:下面的宏替换算法虽不够严密,却有助于理清思路]⑥
⑥ void expand(x)//宏x替换算法
{
    if(x是函数宏),则对x的替换列表中,每次形参p的出现都执行下面的步骤
    {
        if(p对应的实参不是对象宏 && p对应的实参不构成一次函数宏的调用 //即:p对应的实参不是宏
            || p此次作为##的操作数而出现)
        {
            将p替换成对应实参的拼写序列
        }
        else if(p此次作为#的操作数而出现)
        {
            将p连带它的#操作符一起替换成p对应实参的拼写序列
        }
        else//p对应的实参a是宏
        {
            expand(a);//递归调用expand展开宏a将p替换成a展开后的结果序列
        }
    }
    if(x的替换列表中含有##)
    {
        将##连同它前后的空白符一起删除; //记号连接
    }
    用x现在的替换列表,代替源文件中x的这次调用; //宏替换
    将刚才对x替换的结果,连同源文件的下文一起重复扫描,这时如果遇到了一个宏名y

    if(y是出现在下文中)
    {
        expand(y);//递归调用expand展开宏y
    }
    else//y是出现在替换列表中
    {
        if(y!=x //y不是当前正在替换的宏名
            &&y未被标记为不可用) //且y不是父层替换过程正在替换的宏名
        {
            expand(y);//递归调用expand展开宏y
        }
        else
        {
            将y标记为不可用;//使y在子层替换过程中也不再替换
        }
    }
}

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
例4 举例说明字符串的生成与记号连接的规则,下面的指令序列??? 
#define str(s) # s
#define xstr(s) str(s)
#define debug(s, t) printf("x" # s "= %d, x" # t "= %s",\
    x ## s, x ## t)
#define INCFILE(n)? vers ## n // 来自前面#include的例子[见P150例2]
#define glue(a, b)? a ## b
#define xglue(a, b) glue(a, b)
#define HIGHLOW "hello"
#define LOW LOW ", world"
debug(1, 2);
fputs(str(strncmp("abc/0d", "abc", '/4') // 此注释将被删除
    == 0) str(: @/n), s); ⑦

#include xstr(INCFILE(2).h) ⑧
glue(HIGH, LOW); ⑨
xglue(HIGH, LOW) ⑩

经宏替换后,将产生如下结果:

printf("x" "1" "= %d, x" "2" "= %s", x1, x2);
fputs(
    "strncmp(/"abc//0d/", /"abc/", '//4') == 0" ": @/n",
    s);
#include "vers2.h" (在宏替换之后﹑访问文件之前①)
"hello";
"hello" ", world"

或者,在字符串连接之后,

printf("x1= %d, x2= %s", x1, x2);
fputs(
    "strncmp(/"abc//0d/", /"abc/", '//4') == 0: @/n",
    s);
#include "vers2.h" (在宏替换之后﹑访问文件之前①)
"hello";
"hello, world"

在宏定义中,#和##记号左右两边的空格是可选的.

---------------------


⑦ 注意:str(: @/n)内的/之前不会插入/字符,见6.10.3.2第2点,P153
⑧ 对于调用xstr(INCFILE(2).h)的展开过程:
1.展开xstr(INCFILE(2).h)的实参INCFILE(2).h
    1.1.实参INCFILE(2).h中INCFILE是宏,故需先展开INCFILE(2).用INCFILE(2)的实参2替换对应的形参,得到vers2
    1.2.将INCFILE(2)的展开结果vers2,连同后续上下文.h一起重复扫描(见6.10.3.4第1点,P155),未发现需要替换的宏
    1.3.得到INCFILE(2).h的展开结果vers2.h
2.用实参INCFILE(2).h展开的结果vers2.h替换对应的形参,得到str(vers2.h).重复扫描,发现str是宏,于是直接用str的实参序列vers2.h生成字符串(见6.10.3.2第2点,P153),得到"vers2.h"
3.于是得到最终结果"vers2.h"
⑨ 对于调用glue(HIGH, LOW)的展开过程:实参LOW是宏,但由于它对应的形参是##的操作数,故不予展开.
而是直接连接实参序列HIGH和LOW(见6.10.3.3第2点,P154),得到HIGHLOW.重复扫描替换结果(见6.10.3.4第1点,P155),
发现HIGHLOW仍是宏,进一步展开得到最后结果"hello".
⑩ 对于调用xglue(HIGH, LOW)的展开过程:1.展开xglue(HIGH, LOW)中的实参LOW,得到LOW ", world".重复扫描替换结果,
发现LOW虽是宏,却是当前正在处理的宏,故不再递归替换(见6.10.3.4第2点,P155).于是得到LOW的替换结果LOW ", 
world".2.用实参HIGH,和实参LOW的展开结果替换对应的形参,得到glue(HIGH, LOW ", world").重复扫描,发现glue是宏,
于是再继续展开glue(HIGH, LOW ", world").3.glue的实参LOW ", world"中含有宏,但由于它对应的形参是##的操作数,
故不予展开.而是直接连接2个实参序列HIGH和LOW ", world",得到HIGHLOW ", world".重复扫描替换结果,
发现HIGHLOW仍是宏,进一步展开得到最终结果"hello" ", world".(
这2个邻近的字符串将在后续编译阶段被连接成一个,见P10第6点)

① 包含了该文件后,这条#include指令便被删除了

*/


/*
例5 举例说明“占位符”预处理记号的相关规则,下面的指令序列??? 
#define t(x,y,z) x ## y ## z
int j[] = { t(1,2,3), t(,4,5), t(6,,7), t(8,9,),
    t(10,,), t(,11,), t(,,12), t(,,) };

经宏替换后,将产生如下结果
int j[] = { 123, 45, 67, 89,
    10, 11, 12,? };


*/


//--------------------------3.6 Variadic Macros--------------------------------------------------------------------------------------------
/*
12    如果宏定义的标识符列表中有个省略号,那么尾部的若干个实参,包括该宏任何分隔实参的逗号,
都被合并以组成一个称为可变实参(variable arguments)的单一项目.实参如此结合,使得在合并之后,
实参的数目比宏定义时形参的数目多出1个(不含省略号).

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



/*
例7 最后,展示带有可变参数列表的宏:
#define debug(...) fprintf(stderr, __VA_ARGS__)
#define showlist(...) puts(#__VA_ARGS__)
#define report(test, ...) ((test)?puts(#test):\
    printf(__VA_ARGS__))
debug("Flag");
debug("X = %d/n", x);
showlist(The first, second, and third items.);
report(x>y, "x is %d but y is %d", x, y);

经宏替换后,将产生如下结果
fprintf(stderr,? "Flag" );
fprintf(stderr,? "X = %d/n", x );
puts("The first, second, and third items." );
((x>y)?puts("x>y"):
    printf("x is %d but y is %d", x, y))


*/
