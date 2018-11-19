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

#define TEST_FILE_DIR001 "test_files/s_cproc/macro_object_expand/test_001"
#define TEST_FILE_DIR002 "test_files/s_cproc/macro_object_expand/test_002"
#define TEST_FILE_DIR003 "test_files/s_cproc/macro_object_expand/test_003"
#define TEST_FILE_DIR004 "test_files/s_cproc/macro_object_expand/test_004"
#define TEST_FILE_DIR005 "test_files/s_cproc/macro_object_expand/test_005"
#define TEST_FILE_DIR006 "test_files/s_cproc/macro_object_expand/test_006"
#define TEST_FILE_DIR007 "test_files/s_cproc/macro_object_expand/test_007"
#define TEST_FILE_DIR008 "test_files/s_cproc/macro_object_expand/test_008"
#define TEST_FILE_DIR009 "test_files/s_cproc/macro_object_expand/test_009"
#define TEST_FILE_DIR010 "test_files/s_cproc/macro_object_expand/test_010"
#define TEST_FILE_DIR011 "test_files/s_cproc/macro_object_expand/test_011"
#define TEST_FILE_DIR012 "test_files/s_cproc/macro_object_expand/test_012"
#define TEST_FILE_DIR013 "test_files/s_cproc/macro_object_expand/test_013"
#define TEST_FILE_DIR014 "test_files/s_cproc/macro_object_expand/test_014"
#define TEST_FILE_DIR015 "test_files/s_cproc/macro_object_expand/test_015"
#define TEST_FILE_DIR016 "test_files/s_cproc/macro_object_expand/test_016"
#define TEST_FILE_DIR017 "test_files/s_cproc/macro_object_expand/test_017"
#define TEST_FILE_DIR018 "test_files/s_cproc/macro_object_expand/test_018"
#define TEST_FILE_DIR019 "test_files/s_cproc/macro_object_expand/test_019"
#define TEST_FILE_DIR020 "test_files/s_cproc/macro_object_expand/test_020"
#define TEST_FILE_DIR021 "test_files/s_cproc/macro_object_expand/test_021"
#define TEST_FILE_DIR022 "test_files/s_cproc/macro_object_expand/test_022"
#define TEST_FILE_DIR023 "test_files/s_cproc/macro_object_expand/test_023"
#define TEST_FILE_DIR024 "test_files/s_cproc/macro_object_expand/test_024"
#define TEST_FILE_DIR025 "test_files/s_cproc/macro_object_expand/test_025"
#define TEST_FILE_DIR026 "test_files/s_cproc/macro_object_expand/test_026"
#define TEST_FILE_DIR027 "test_files/s_cproc/macro_object_expand/test_027"
#define TEST_FILE_DIR028 "test_files/s_cproc/macro_object_expand/test_028"
#define TEST_FILE_DIR029 "test_files/s_cproc/macro_object_expand/test_029"
#define TEST_FILE_DIR030 "test_files/s_cproc/macro_object_expand/test_030"
#define TEST_FILE_DIR031 "test_files/s_cproc/macro_object_expand/test_031"
#define TEST_FILE_DIR032 "test_files/s_cproc/macro_object_expand/test_032"
#define TEST_FILE_DIR033 "test_files/s_cproc/macro_object_expand/test_033"
#define TEST_FILE_DIR034 "test_files/s_cproc/macro_object_expand/test_034"
#define TEST_FILE_DIR035 "test_files/s_cproc/macro_object_expand/test_035"










#define TEST_FILE_INPUT "/i.input"

#define TEST_FILE_OUTPUT "/o.output"
#define TEST_FILE_ERRORS "/e.output"

#define TEST_FILE_OUTPUT_EXPECT "/o_expect.output"
#define TEST_FILE_ERRORS_EXPECT "/e_expect.output"

TEST_GROUP(s_cc_macro_object_expand)
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
#define BUFFER_SIZE 1024
foo = (char *) malloc (BUFFER_SIZE);
�� 

foo = (char *) malloc (1024);

*/
TEST(s_cc_macro_object_expand, simple_one)
{
    file_init(TEST_FILE_DIR001);
    ret_val = s_cc(file_name_input, pf_output, pf_errors);
    CHECK_EQUAL(RETURN_SUCCESS, ret_val);

    ret_val = s_file_compare(pf_output, pf_output_expect, &file_compare_result);
    CHECK_EQUAL(RETURN_SUCCESS, ret_val);
    CHECK_EQUAL(0, file_compare_result);

    ret_val = s_file_compare(pf_errors, pf_errors_expect, &file_compare_result);
    CHECK_EQUAL(RETURN_SUCCESS, ret_val);
    CHECK_EQUAL(0, file_compare_result);
}

//--------------------3.1 Object-like Macros-----------------------------------------------------
/*
An object-like macro is a simple identifier which will be replaced by a code fragment. It is called object-like because it looks like a 
data object in code that uses it. They are most commonly used to give symbolic names to numeric constants.
*/

/*
���һ��#Ԥ����Ǻź������һ����ʶ�����������ϳ�����һ��Ԥ����ָ����Կ�ʼ��λ��, ��ô�ñ�ʶ�����Ǻ��滻�Ķ���

#define INCL_STD include<stdio.h>
#INCL_STD   //��INCL_STD����ִ���������滻

s_cproc_replace.c:35:2: error: invalid preprocessing directive #INCL_STD
 #INCL_STD   //o�hCL_STD2????���˦̨��I
  ^~~~~~~~

*/


/*
#define NUMBERS 1, \
                2, \
                3
int x[] = { NUMBERS };
�� 

int x[] = { 1, 2, 3 };

*/


/*
foo = X;
#define X 4
bar = X;
�� 

foo = X;
bar = 4;

*/

/*
#define TABLESIZE BUFSIZE
#define BUFSIZE 1024
foo = (char *) malloc (TABLESIZE);

�� 

foo = (char *) malloc (1024);

*/

/*
#define BUFSIZE 1020
#define TABLESIZE BUFSIZE
#undef BUFSIZE
#define BUFSIZE 37
foo = (char *) malloc (TABLESIZE);

�� 

foo = (char *) malloc (37);
*/



/*
�궨������÷�Χ����������ṹ��,������ֱ������һ����Ӧ��#undefָ��,���һ��Ҳû������,
��һֱ������Ԥ�����뵥Ԫ�Ľ���.�ڷ���׶�4����֮��,��Щ�궨��㲻�پ�������.
�� ��P9�ķ���׶�.����:
void fun2();
void fun1()
{
    #define MACRO1 "MACRO1"//�˺�����÷�Χ�Ӵ��п�ʼ����
}

void main()
{
    puts(MACRO1);//��ȷ,���ܿ���������MACRO1��fun1()��δ���ù�
    fun2();
    puts(MACRO2);//����,MACRO2δ����,���ܿ�����fun2()�Ѿ����ù���
}

void fun2()
{
    #define MACRO2 "MACRO2"//�˺�����÷�Χ�Ӵ��п�ʼ����
}

*/
