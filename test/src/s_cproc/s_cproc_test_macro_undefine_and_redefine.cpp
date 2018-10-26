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
If a macro ceases to be useful, it may be undefined with the ��#undef�� directive. ��#undef�� takes a single argument, the name of 
the macro to undefine. You use the bare macro name, even if the macro is function-like. 

#define FOO 4
x = FOO;        �� x = 4;
#undef FOO
x = FOO;        �� x = FOO;
*/

/*
It is an error if anything appears on the line after the macro name. ��#undef�� has no effect if the name is not a macro.
../src/s_cproc/s_cproc_replace.c:37:12: warning: extra tokens at end of #undef directive
 #undef XXX 123
            ^~~
*/

/*
Once a macro has been undefined, that identifier may be redefined as a macro by a subsequent ��#define�� directive. The new definition 
need not have any resemblance to the old definition.
#define MACRO 123
#undef MACRO
#define MACRO 345
int x = MACRO;
�� int x = 345;
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
��3 ����˵���ض������ؼ�����,�����ָ�����Т�??? 
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

f(y+1) + f(f(z)) % t(t(g)(0) + t) (1); �ڢۢ�
g(x+(3,4)-w) | h 5) & m(f)^m(m); ��
p() i[q()] = { q(1), r(2,3), r(4,), r(,5), r(,) }; ��
char c[2][6] = { str(hello), str() };

�����滻��,���������½��
f(2 * (y+1)) + f(2 * (f(2 * (z[0])))) % f(2 * (0)) + t(1);
f(2 * (2+(3,4)-0,1)) | f(2 * (~ 5)) & f(2 * (0,1))^m(0,1);
int i[] = { 1, 23, 4, 5,? };
char c[2][6] = { "hello", "" };

�� ��Ȿ��ʮ�ֱ�Ҫ.Ҫ��ȷ������δ���,��������δ���֮ǰ,�����������ı����뺯������,�������е�����������:??? 
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

�� ��f(y+1) չ������:
��ʵ��y+1�滻��Ӧ���β�,�õ�f(x*(y+1)).�ظ�ɨ���滻���,����f���Ǻ�,��ȴ�ǵ�ǰ���ڴ���ĺ�,�ʲ��ٵݹ��滻(��6.10.3.4��2��,P155),
����ɨ�跢��x�Ǻ�,�����õ�ǰ���滻�б�2(������3)�滻,���Ϊf(2*(y+1)).�ɺ�x���滻�ɼ�,����ɨ��궨����滻�б�ʱ,���Ὣ���еĺ��������滻
(����������Ч�ʸ�),������ĳ�κ���ô��������е��滻����.����,��x�Ķ��岻����f����֮ǰ����.����,�����������#define x 3һ��ɾ��,���н����Ȼ��ͬ.
���Ƶ���������ں�����:
#define m1(x) m2(x)//��m1���ú�m2,������ʱm2��δ����
#define m2(x) x
printf("%d/n",m1(5));//���5.��ʱm2����Ϊ�궨�����
Ϊ������,����ע���жԺ�x��չ�����̲��ٸ���.

�� ��f(f(z))��չ������:
1.�滻f(f(z))��ʵ��f(z)
    11.�滻f(z)��ʵ��z
        111.��z�滻��z[0],�ظ�ɨ��z[0],����z�ǵ�ǰ���ڴ���ĺ�,�ʲ��ٵݹ��滻,����z���Ϊ������(��6.10.3.4��2��,P155).
        112.�õ�z���滻���z[0].
    12.��ʵ��zչ���Ľ��z[0]�滻��Ӧ���β�,�õ�f(2*(z[0])).�ظ�ɨ���滻���,���ٵݹ��滻���еĺ�f,��z����111�б��Ϊ������,Ҳ�����滻.
    13.�õ�f(z)���滻���f(2*(z[0])).
2.��ʵ��f(z)չ���Ľ��f(2*(z[0]))�滻��Ӧ���β�,�õ�f(2*(f(2*(z[0])))).�ظ�ɨ���滻���,���ٵݹ��滻���еĺ�f,��z����111�б��Ϊ������,Ҳ�����滻.
3.���ǵõ����ս��f(2*(f(2*(z[0])))).

�� ��t(t(g)(0)+t)(1)��չ������:
1.չ��t(t(g)(0)+t)����ò���
    11.չ��t(t(g)(0)+t)��ʵ��t(g)(0)+t
        111.չ��t(g)����ò���
            1111.�滻t(g)��ʵ��g,�õ�f.�ظ�ɨ��f,����fδ������,�ʲ���Ϊ�Ǻ����,����չ��(��P152ע��1).
            1112.��ʵ��gչ���Ľ��f�滻��Ӧ���β�,�õ�f.�ظ�ɨ��f,����fδ������,�ʲ���Ϊ�Ǻ����,����չ��.
            1113.�õ�t(g)���滻���f.
        112.��t(g)չ���Ľ��f,��ͬ����������(0)һ���ظ�ɨ��(��6.10.3.4��1��,P155),����f(0)�Ǻ�,����չ����f(0)
            1121.��f(0)��ʵ��0�滻��Ӧ���β�,�õ�f(2*(0)).�ظ�ɨ���滻���,����f���Ǻ�,ȴ�ǵ�ǰ���ڴ���ĺ�,�ʲ��ٵݹ��滻f(2*(0)),����f���Ϊ������(��6.10.3.4��2��,P155).
            1122.�õ�f(0)���滻���f(2*(0)).
        113.��t(g)չ���Ľ��f(2*(0)),��ͬ����������+ tһ���ظ�ɨ��,����f����1121�б��Ϊ������,tδ������,�ʶ�f(2*(0))��t�������滻.
        114.�õ�t(g)(0)+t���滻���f(2*(0))+t.
    12.��ʵ��t(g)(0)+tչ���Ľ��f(2*(0))+t�滻��Ӧ���β�,�õ�f(2*(0))+t.�ظ�ɨ���滻���,����f����1121�б��Ϊ������,tδ������,�ʶ�f(2*(0))��t�������滻.
    13.�õ�t(t(g)(0)+t)���滻���f(2*(0))+t.
2.��t(t(g)(0) + t)չ���Ľ��,��ͬ����������(1)һ���ظ�ɨ��,���ظ�ɨ��f(2*(0)) + t(1),����f����1121�б��Ϊ������,�ʶ�f(2*(0))�����滻.t�ǵ�ǰ���ڴ���ĺ�,�ʶ�t(1)���ٵݹ��滻.
3.���ǵõ����ս��f(2*(0))+t(1).

�� ��m(m)��չ������:
1.��ͼչ��m(m)��ʵ��m.����mδ������,�ʲ���Ϊ�Ǻ����,����չ��(��P152ע��1).
2.��ʵ��m�滻��Ӧ���β�,�õ�m(w).�ظ�ɨ��,����m���Ǻ�,ȴ�ǵ�ǰ���ڴ���ĺ�,�ʲ��ٵݹ��滻m(��6.10.3.4��2��,P155).����ɨ�跢��w�Ǻ�,չ��w�õ�0,1.
3.���ǵõ����ս��m(0,1)

�� ��������Կ���,���ú�����ʱ,����ʡ�Բ�������ȫ��ʵ��,����ʡ�����ڷָ�ʵ�εĶ���.�������������������Ҳʡ����

*/


//��6 ��ʾ����ض������,�����ָ�������ǺϷ���
//#define OBJ_LIKE (1-1)
//#define OBJ_LIKE /* �հ� */ (1-1) /* �����ַ� */
//#define FUNC_LIKE(a) ( a )
//#define FUNC_LIKE( a ) ( /* �հ�ע�� */ /
//    a /* ���е���������
//    */ )
//
//��������ض����ǷǷ���:[��������������4��ָ��,���ٳ�������ָ���ǷǷ���]
//#define OBJ_LIKE??? (0)// �Ǻ����в�ͬ[ǰ����滻�б���(1-1)]
//#define OBJ_LIKE??? (1 - 1) // �հ׷���ͬ[����ǰ�����˿հ׷�]
//#define FUNC_LIKE(b) ( a )? // ʹ�õ��ββ�ͬ[ǰ����β���a]
//#define FUNC_LIKE(b) ( b )? // ����ƴд��ͬ[ǰ����滻�б���( a )]


