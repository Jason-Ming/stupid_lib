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

#define TEST_FILE_DIR001 "test_files/s_cproc/include/test_001"
#define TEST_FILE_DIR002 "test_files/s_cproc/include/test_002"
#define TEST_FILE_DIR003 "test_files/s_cproc/include/test_003"
#define TEST_FILE_DIR004 "test_files/s_cproc/include/test_004"
#define TEST_FILE_DIR005 "test_files/s_cproc/include/test_005"
#define TEST_FILE_DIR006 "test_files/s_cproc/include/test_006"
#define TEST_FILE_DIR007 "test_files/s_cproc/include/test_007"
#define TEST_FILE_DIR008 "test_files/s_cproc/include/test_008"
#define TEST_FILE_DIR009 "test_files/s_cproc/include/test_009"
#define TEST_FILE_DIR010 "test_files/s_cproc/include/test_010"
#define TEST_FILE_DIR011 "test_files/s_cproc/include/test_011"
#define TEST_FILE_DIR012 "test_files/s_cproc/include/test_012"
#define TEST_FILE_DIR013 "test_files/s_cproc/include/test_013"
#define TEST_FILE_DIR014 "test_files/s_cproc/include/test_014"
#define TEST_FILE_DIR015 "test_files/s_cproc/include/test_015"

#define TEST_FILE_INPUT "/i.input"

#define TEST_FILE_OUTPUT "/o.output"
#define TEST_FILE_ERRORS "/e.output"

#define TEST_FILE_OUTPUT_EXPECT "/o_expect.output"
#define TEST_FILE_ERRORS_EXPECT "/e_expect.output"

TEST_GROUP(s_cc_include)
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
#include/"include.input"

/home/jason/github/stupid_lib/test_files/s_cproc/include/test_001/i.input:1:9: error: #include expects "FILENAME" or <FILENAME>
 #include/"include.input"
         ^

*/
TEST(s_cc_include, unexpected_FILENAME_slash)
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
#include
"include.input"

/home/jason/github/stupid_lib/test_files/s_cproc/include/test_002/i.input:1:9: error: #include expects "FILENAME" or <FILENAME>
 #include
         ^

*/
TEST(s_cc_include, unexpected_FILENAME_newline)
{
    file_init(TEST_FILE_DIR002);
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
#include"include.input

/home/jason/github/stupid_lib/test_files/s_cproc/include/test_003/i.input:1:9: warning: missing terminating " character
 #include"include.input
         ^
/home/jason/github/stupid_lib/test_files/s_cproc/include/test_003/i.input:1:9: error: #include expects "FILENAME" or <FILENAME>
 #include"include.input
         ^

*/
TEST(s_cc_include, unterminated_q_header)
{
    file_init(TEST_FILE_DIR003);
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
#include<include.input

/home/jason/github/stupid_lib/test_files/s_cproc/include/test_004/i.input:1:9: error: missing terminating > character
 #include<include.input
         ^
/home/jason/github/stupid_lib/test_files/s_cproc/include/test_004/i.input:1:9: error: #include expects "FILENAME" or <FILENAME>
 #include<include.input
         ^

*/
TEST(s_cc_include, unterminated_h_header)
{
    file_init(TEST_FILE_DIR004);
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
#include""

/home/jason/github/stupid_lib/test_files/s_cproc/include/test_005/i.input:1:9: error: empty filename in #include
 #include""
         ^

*/
TEST(s_cc_include, q_header_empty)
{
    file_init(TEST_FILE_DIR005);
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
#include<>

/home/jason/github/stupid_lib/test_files/s_cproc/include/test_006/i.input:1:9: error: empty filename in #include
 #include<>
         ^

*/
TEST(s_cc_include, h_header_empty)
{
    file_init(TEST_FILE_DIR006);
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
#include"include.input"dsaf

/home/jason/github/stupid_lib/test_files/s_cproc/include/test_007/i.input:1:24: warning: extra tokens at end of #include directive
 #include"include.input"dsaf
                        ^

*/
TEST(s_cc_include, extra_tokens)
{
    file_init(TEST_FILE_DIR007);
    ret_val = s_cc(file_name_input, pf_output, pf_errors);
    CHECK_EQUAL(RETURN_SUCCESS, ret_val);

    ret_val = s_file_compare(pf_output, pf_output_expect, &file_compare_result);
    CHECK_EQUAL(RETURN_SUCCESS, ret_val);
    CHECK_EQUAL(0, file_compare_result);

    ret_val = s_file_compare(pf_errors, pf_errors_expect, &file_compare_result);
    CHECK_EQUAL(RETURN_SUCCESS, ret_val);
    CHECK_EQUAL(0, file_compare_result);
}

//#include/* a comment */"include.input"

TEST(s_cc_include, comment_after_include)
{
    file_init(TEST_FILE_DIR008);
    ret_val = s_cc(file_name_input, pf_output, pf_errors);
    CHECK_EQUAL(RETURN_SUCCESS, ret_val);

    ret_val = s_file_compare(pf_output, pf_output_expect, &file_compare_result);
    CHECK_EQUAL(RETURN_SUCCESS, ret_val);
    CHECK_EQUAL(0, file_compare_result);

    ret_val = s_file_compare(pf_errors, pf_errors_expect, &file_compare_result);
    CHECK_EQUAL(RETURN_SUCCESS, ret_val);
    CHECK_EQUAL(0, file_compare_result);
}

//#include <x/*y>

///home/jason/github/stupid_lib/test_files/s_cproc/include/test_009/i.input:1:11: error: x/*y: No such file or directory
// #include <x/*y>
//           ^

TEST(s_cc_include, filename_has_comment)
{
    file_init(TEST_FILE_DIR009);
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
#include "x\n\\y"

/home/jason/github/stupid_lib/test_files/s_cproc/include/test_010/i.input:1:11: error: x\n\\y: No such file or directory
 #include "x\n\\y"
           ^

*/
TEST(s_cc_include, filename_has_newline_and_backslash)
{
    file_init(TEST_FILE_DIR010);
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
all characters in <> will reserved
#include <stdio.h >
*/
TEST(s_cc_include, h_header_has_space)
{
    file_init(TEST_FILE_DIR011);
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
after replacing macro, string concatonation will not execute:

#define FILE_B() "s_cproc_include_file.h"
#include FILE_B()FILE_B()

s_cproc_replace.c:12:25: warning: extra tokens at end of #include directive
 #include FILE_B()FILE_B()
                         ^

*/


/*
Search Path

*/

/*Once-Only Headers*/
/* File foo.  */
//#ifndef FILE_FOO_SEEN
//#define FILE_FOO_SEEN
//
//the entire file
//
//#endif /* !FILE_FOO_SEEN */



/*
#define HEADER "a\"b"
#include HEADER


s_cproc_replace.c:39:16: fatal error: a\"b: No such file or directory
 #define HEADER "a\"b"
                ^~~~~~

*/


/*
If the line expands to a token stream beginning with a ‘<’ token and including a ‘>’ token, then the tokens between the ‘<’ 
and the first ‘>’ are combined to form the filename to be included. Any whitespace between tokens is reduced to a single space; then 
any space after the initial ‘<’ is retained, but a trailing space before the closing ‘>’ is ignored. CPP searches for the file 
according to the rules for angle-bracket includes.

#define STD <std
#define IO io.
#define H h >
#include STD IO H </usr/include/stdio.h >asdff

*/

//#define STD__ "stdio\"
//#define IO__ .h"
//#define STDIO STD__ IO__
//#include STDIO
//#include STD__

/*
邻近字符串的拼接操作是在预处理之后进行的,如下指令无法正确包含myfile.h:
    #include "myfile" ".h"

s_cproc_replace.c:1:19: warning: extra tokens at end of #include directive
 #include "s_text.""h"
                   ^~~
s_cproc_replace.c:1:10: fatal error: s_text.: No such file or directory
 #include "s_text.""h"
          ^~~~~~~~~

*/


