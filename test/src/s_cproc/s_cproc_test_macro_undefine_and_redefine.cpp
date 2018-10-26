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

#define TEST_FILE_DIR001 "test_files/s_cproc/macro_unefine_and_redefine/test_001"
#define TEST_FILE_DIR002 "test_files/s_cproc/macro_unefine_and_redefine/test_002"
#define TEST_FILE_DIR003 "test_files/s_cproc/macro_unefine_and_redefine/test_003"
#define TEST_FILE_DIR004 "test_files/s_cproc/macro_unefine_and_redefine/test_004"
#define TEST_FILE_DIR005 "test_files/s_cproc/macro_unefine_and_redefine/test_005"
#define TEST_FILE_DIR006 "test_files/s_cproc/macro_unefine_and_redefine/test_006"
#define TEST_FILE_DIR007 "test_files/s_cproc/macro_unefine_and_redefine/test_007"
#define TEST_FILE_DIR008 "test_files/s_cproc/macro_unefine_and_redefine/test_008"
#define TEST_FILE_DIR009 "test_files/s_cproc/macro_unefine_and_redefine/test_009"
#define TEST_FILE_DIR010 "test_files/s_cproc/macro_unefine_and_redefine/test_010"
#define TEST_FILE_DIR011 "test_files/s_cproc/macro_unefine_and_redefine/test_011"
#define TEST_FILE_DIR012 "test_files/s_cproc/macro_unefine_and_redefine/test_012"
#define TEST_FILE_DIR013 "test_files/s_cproc/macro_unefine_and_redefine/test_013"
#define TEST_FILE_DIR014 "test_files/s_cproc/macro_unefine_and_redefine/test_014"
#define TEST_FILE_DIR015 "test_files/s_cproc/macro_unefine_and_redefine/test_015"
#define TEST_FILE_DIR016 "test_files/s_cproc/macro_unefine_and_redefine/test_016"
#define TEST_FILE_DIR017 "test_files/s_cproc/macro_unefine_and_redefine/test_017"
#define TEST_FILE_DIR018 "test_files/s_cproc/macro_unefine_and_redefine/test_018"
#define TEST_FILE_DIR019 "test_files/s_cproc/macro_unefine_and_redefine/test_019"
#define TEST_FILE_DIR020 "test_files/s_cproc/macro_unefine_and_redefine/test_020"
#define TEST_FILE_DIR021 "test_files/s_cproc/macro_unefine_and_redefine/test_021"
#define TEST_FILE_DIR022 "test_files/s_cproc/macro_unefine_and_redefine/test_022"
#define TEST_FILE_DIR023 "test_files/s_cproc/macro_unefine_and_redefine/test_023"
#define TEST_FILE_DIR024 "test_files/s_cproc/macro_unefine_and_redefine/test_024"
#define TEST_FILE_DIR025 "test_files/s_cproc/macro_unefine_and_redefine/test_025"
#define TEST_FILE_DIR026 "test_files/s_cproc/macro_unefine_and_redefine/test_026"
#define TEST_FILE_DIR027 "test_files/s_cproc/macro_unefine_and_redefine/test_027"
#define TEST_FILE_DIR028 "test_files/s_cproc/macro_unefine_and_redefine/test_028"
#define TEST_FILE_DIR029 "test_files/s_cproc/macro_unefine_and_redefine/test_029"
#define TEST_FILE_DIR030 "test_files/s_cproc/macro_unefine_and_redefine/test_030"
#define TEST_FILE_DIR031 "test_files/s_cproc/macro_unefine_and_redefine/test_031"
#define TEST_FILE_DIR032 "test_files/s_cproc/macro_unefine_and_redefine/test_032"
#define TEST_FILE_DIR033 "test_files/s_cproc/macro_unefine_and_redefine/test_033"
#define TEST_FILE_DIR034 "test_files/s_cproc/macro_unefine_and_redefine/test_034"
#define TEST_FILE_DIR035 "test_files/s_cproc/macro_unefine_and_redefine/test_035"










#define TEST_FILE_INPUT "/i.input"

#define TEST_FILE_OUTPUT "/o.output"
#define TEST_FILE_ERRORS "/e.output"

#define TEST_FILE_OUTPUT_EXPECT "/o_expect.output"
#define TEST_FILE_ERRORS_EXPECT "/e_expect.output"

TEST_GROUP(s_cc_macro_unefine_and_redefine)
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
TEST(s_cc_macro_unefine_and_redefine, no_macro_name)
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
If a macro ceases to be useful, it may be undefined with the ‘#undef’ directive. ‘#undef’ takes a single argument, the name of 
the macro to undefine. You use the bare macro name, even if the macro is function-like. 

#define FOO 4
x = FOO;        → x = 4;
#undef FOO
x = FOO;        → x = FOO;
*/

/*
It is an error if anything appears on the line after the macro name. ‘#undef’ has no effect if the name is not a macro.
../src/s_cproc/s_cproc_replace.c:37:12: warning: extra tokens at end of #undef directive
 #undef XXX 123
            ^~~
*/

/*
Once a macro has been undefined, that identifier may be redefined as a macro by a subsequent ‘#define’ directive. The new definition 
need not have any resemblance to the old definition.
#define MACRO 123
#undef MACRO
#define MACRO 345
int x = MACRO;
→ int x = 345;
*/

/*
However, if an identifier which is currently a macro is redefined, then the new definition must be effectively the same as the old one. 
Two macro definitions are effectively the same if:

Both are the same type of macro (object- or function-like).
All the tokens of the replacement list are the same.
If there are any parameters, they are the same.
Whitespace appears in the same places in both. It need not be exactly the same amount of whitespace, though. Remember that comments count 
as whitespace.
These definitions are effectively the same:
*/
//#define FOUR (2 + 2)
//#define FOUR         (2    +    2)
//#define FOUR (2 /* two */ + 2)
//but these are not:

//#define FOUR (2 + 2)
//#define FOUR ( 2+2 )
//#define FOUR (2 * 2)
//#define FOUR(score,and,seven,years,ago) (2 + 2)

/*
If a macro is redefined with a definition that is not effectively the same as the old one, the preprocessor issues a warning and changes 
the macro to use the new definition. If the new definition is effectively the same, the redefinition is silently ignored. This allows, 
for instance, two different headers to define a common macro. The preprocessor will only complain if the definitions do not match.


*/


/*
例3 举例说明重定义与重检查规则,下面的指令序列①??? 
#define x 3
#define f(a) f(x * (a))
#undef x
#define x 2
#define g f
#define z z[0]
#define h g(~
#define m(a) a(w)
#define w 0,1
#define t(a) a
#define p() int
#define q(x) x
#define r(x,y) x ## y
#define str(x) # x

f(y+1) + f(f(z)) % t(t(g)(0) + t) (1); ②③④
g(x+(3,4)-w) | h 5) & m(f)^m(m); ⑤
p() i[q()] = { q(1), r(2,3), r(4,), r(,5), r(,) }; ⑥
char c[2][6] = { str(hello), str() };

经宏替换后,将产生如下结果
f(2 * (y+1)) + f(2 * (f(2 * (z[0])))) % f(2 * (0)) + t(1);
f(2 * (2+(3,4)-0,1)) | f(2 * (~ 5)) & f(2 * (0,1))^m(0,1);
int i[] = { 1, 23, 4, 5,? };
char c[2][6] = { "hello", "" };

① 理解本例十分必要.要正确编译这段代码,还需在这段代码之前,添加类似下面的变量与函数定义,函数体中的语句可以随意:??? 
int y=1;
int z[1]={1};
int f(int a)
{
    return a==0 ? 1 : a;
}
int t(int a)
{
    return a;
}
int m(int a,int b)
{
    return b;
}

② 对f(y+1) 展开过程:
用实参y+1替换对应的形参,得到f(x*(y+1)).重复扫描替换结果,发现f虽是宏,但却是当前正在处理的宏,故不再递归替换(见6.10.3.4第2点,P155),
继续扫描发现x是宏,将其用当前的替换列表2(而不是3)替换,结果为f(2*(y+1)).由宏x的替换可见,当在扫描宏定义的替换列表时,不会将其中的宏名立刻替换
(尽管那样做效率高),而是由某次宏调用触发了所有的替换过程.所以,宏x的定义不必在f定义之前出现.例如,将上面程序中#define x 3一行删除,运行结果仍然相同.
类似的情况适用于函数宏:
#define m1(x) m2(x)//宏m1调用宏m2,尽管这时m2还未定义
#define m2(x) x
printf("%d/n",m1(5));//输出5.此时m2已作为宏定义过了
为简便起见,下面注解中对宏x的展开过程不再复述.

③ 对f(f(z))的展开过程:
1.替换f(f(z))的实参f(z)
    11.替换f(z)的实参z
        111.将z替换成z[0],重复扫描z[0],发现z是当前正在处理的宏,故不再递归替换,并将z标记为不可用(见6.10.3.4第2点,P155).
        112.得到z的替换结果z[0].
    12.用实参z展开的结果z[0]替换对应的形参,得到f(2*(z[0])).重复扫描替换结果,不再递归替换其中的宏f,宏z已在111中标记为不可用,也不再替换.
    13.得到f(z)的替换结果f(2*(z[0])).
2.用实参f(z)展开的结果f(2*(z[0]))替换对应的形参,得到f(2*(f(2*(z[0])))).重复扫描替换结果,不再递归替换其中的宏f,宏z已在111中标记为不可用,也不再替换.
3.于是得到最终结果f(2*(f(2*(z[0])))).

④ 对t(t(g)(0)+t)(1)的展开过程:
1.展开t(t(g)(0)+t)宏调用部分
    11.展开t(t(g)(0)+t)的实参t(g)(0)+t
        111.展开t(g)宏调用部分
            1111.替换t(g)的实参g,得到f.重复扫描f,由于f未带括号,故不认为是宏调用,不予展开(见P152注解1).
            1112.用实参g展开的结果f替换对应的形参,得到f.重复扫描f,由于f未带括号,故不认为是宏调用,不予展开.
            1113.得到t(g)的替换结果f.
        112.将t(g)展开的结果f,连同后续上下文(0)一起重复扫描(见6.10.3.4第1点,P155),发现f(0)是宏,于是展开宏f(0)
            1121.用f(0)的实参0替换对应的形参,得到f(2*(0)).重复扫描替换结果,发现f虽是宏,却是当前正在处理的宏,故不再递归替换f(2*(0)),并将f标记为不可用(见6.10.3.4第2点,P155).
            1122.得到f(0)的替换结果f(2*(0)).
        113.将t(g)展开的结果f(2*(0)),连同后续上下文+ t一起重复扫描,由于f已在1121中标记为不可用,t未带括号,故对f(2*(0))和t都不再替换.
        114.得到t(g)(0)+t的替换结果f(2*(0))+t.
    12.用实参t(g)(0)+t展开的结果f(2*(0))+t替换对应的形参,得到f(2*(0))+t.重复扫描替换结果,由于f已在1121中标记为不可用,t未带括号,故对f(2*(0))和t都不再替换.
    13.得到t(t(g)(0)+t)的替换结果f(2*(0))+t.
2.将t(t(g)(0) + t)展开的结果,连同后续上下文(1)一起重复扫描,即重复扫描f(2*(0)) + t(1),由于f已在1121中标记为不可用,故对f(2*(0))不再替换.t是当前正在处理的宏,故对t(1)不再递归替换.
3.于是得到最终结果f(2*(0))+t(1).

⑤ 对m(m)的展开过程:
1.试图展开m(m)的实参m.由于m未带括号,故不认为是宏调用,不予展开(见P152注解1).
2.用实参m替换对应的形参,得到m(w).重复扫描,发现m虽是宏,却是当前正在处理的宏,故不再递归替换m(见6.10.3.4第2点,P155).继续扫描发现w是宏,展开w得到0,1.
3.于是得到最终结果m(0,1)

⑥ 由这里可以看出,调用函数宏时,可以省略部分乃至全部实参,可以省略用于分隔实参的逗号.但不能连宏名后的括号也省略了

*/


//例6 演示宏的重定义规则,下面的指令序列是合法的
//#define OBJ_LIKE (1-1)
//#define OBJ_LIKE /* 空白 */ (1-1) /* 其它字符 */
//#define FUNC_LIKE(a) ( a )
//#define FUNC_LIKE( a ) ( /* 空白注释 */ /
//    a /* 此行的其它东西
//    */ )
//
//但下面的重定义是非法的:[假如已有了上面4条指令,则再出现下面指令是非法的]
//#define OBJ_LIKE??? (0)// 记号序列不同[前面的替换列表是(1-1)]
//#define OBJ_LIKE??? (1 - 1) // 空白符不同[减号前后多出了空白符]
//#define FUNC_LIKE(b) ( a )? // 使用的形参不同[前面的形参是a]
//#define FUNC_LIKE(b) ( b )? // 参数拼写不同[前面的替换列表是( a )]


