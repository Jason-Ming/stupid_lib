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

#define TEST_FILE_DIR001 "test_files/s_cproc/macro_directives_within_arguments/test_001"
#define TEST_FILE_DIR002 "test_files/s_cproc/macro_directives_within_arguments/test_002"
#define TEST_FILE_DIR003 "test_files/s_cproc/macro_directives_within_arguments/test_003"
#define TEST_FILE_DIR004 "test_files/s_cproc/macro_directives_within_arguments/test_004"
#define TEST_FILE_DIR005 "test_files/s_cproc/macro_directives_within_arguments/test_005"
#define TEST_FILE_DIR006 "test_files/s_cproc/macro_directives_within_arguments/test_006"
#define TEST_FILE_DIR007 "test_files/s_cproc/macro_directives_within_arguments/test_007"
#define TEST_FILE_DIR008 "test_files/s_cproc/macro_directives_within_arguments/test_008"
#define TEST_FILE_DIR009 "test_files/s_cproc/macro_directives_within_arguments/test_009"
#define TEST_FILE_DIR010 "test_files/s_cproc/macro_directives_within_arguments/test_010"
#define TEST_FILE_DIR011 "test_files/s_cproc/macro_directives_within_arguments/test_011"
#define TEST_FILE_DIR012 "test_files/s_cproc/macro_directives_within_arguments/test_012"
#define TEST_FILE_DIR013 "test_files/s_cproc/macro_directives_within_arguments/test_013"
#define TEST_FILE_DIR014 "test_files/s_cproc/macro_directives_within_arguments/test_014"
#define TEST_FILE_DIR015 "test_files/s_cproc/macro_directives_within_arguments/test_015"
#define TEST_FILE_DIR016 "test_files/s_cproc/macro_directives_within_arguments/test_016"
#define TEST_FILE_DIR017 "test_files/s_cproc/macro_directives_within_arguments/test_017"
#define TEST_FILE_DIR018 "test_files/s_cproc/macro_directives_within_arguments/test_018"
#define TEST_FILE_DIR019 "test_files/s_cproc/macro_directives_within_arguments/test_019"
#define TEST_FILE_DIR020 "test_files/s_cproc/macro_directives_within_arguments/test_020"
#define TEST_FILE_DIR021 "test_files/s_cproc/macro_directives_within_arguments/test_021"
#define TEST_FILE_DIR022 "test_files/s_cproc/macro_directives_within_arguments/test_022"
#define TEST_FILE_DIR023 "test_files/s_cproc/macro_directives_within_arguments/test_023"
#define TEST_FILE_DIR024 "test_files/s_cproc/macro_directives_within_arguments/test_024"
#define TEST_FILE_DIR025 "test_files/s_cproc/macro_directives_within_arguments/test_025"
#define TEST_FILE_DIR026 "test_files/s_cproc/macro_directives_within_arguments/test_026"
#define TEST_FILE_DIR027 "test_files/s_cproc/macro_directives_within_arguments/test_027"
#define TEST_FILE_DIR028 "test_files/s_cproc/macro_directives_within_arguments/test_028"
#define TEST_FILE_DIR029 "test_files/s_cproc/macro_directives_within_arguments/test_029"
#define TEST_FILE_DIR030 "test_files/s_cproc/macro_directives_within_arguments/test_030"
#define TEST_FILE_DIR031 "test_files/s_cproc/macro_directives_within_arguments/test_031"
#define TEST_FILE_DIR032 "test_files/s_cproc/macro_directives_within_arguments/test_032"
#define TEST_FILE_DIR033 "test_files/s_cproc/macro_directives_within_arguments/test_033"
#define TEST_FILE_DIR034 "test_files/s_cproc/macro_directives_within_arguments/test_034"
#define TEST_FILE_DIR035 "test_files/s_cproc/macro_directives_within_arguments/test_035"










#define TEST_FILE_INPUT "/i.input"

#define TEST_FILE_OUTPUT "/o.output"
#define TEST_FILE_ERRORS "/e.output"

#define TEST_FILE_OUTPUT_EXPECT "/o_expect.output"
#define TEST_FILE_ERRORS_EXPECT "/e_expect.output"

TEST_GROUP(s_cc_macro_directives_within_arguments)
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
TEST(s_cc_macro_directives_within_arguments, no_macro_name)
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
Occasionally it is convenient to use preprocessor directives within the arguments of a macro. The C and C++ standards declare that 
behavior in these cases is undefined. GNU CPP processes arbitrary directives within macro arguments in exactly the same way as it would 
have processed the directive were the function-like macro invocation not present.

If, within a macro invocation, that macro is redefined, then the new definition takes effect in time for argument pre-expansion, but the 
original definition is still used for argument replacement. Here is a pathological example:

#define f(x) x x
f (1
#undef f
#define f 2
f)
which expands to

1 2 1 2
with the semantics described above.


*/






