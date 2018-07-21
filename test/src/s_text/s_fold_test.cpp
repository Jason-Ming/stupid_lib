#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;



TEST_GROUP(s_fold_s)
{
    void setup()
    {
    	//设置自己的测试准备
        s1 = NULL;
        expected_s = NULL;
        retval = RETURN_FAILURE;
    }

    void teardown()
    {
        //清理测试设置
        //cout << "Test end ......" << endl;
    }

    const static size_t len = 1024;
    const _S8 *s1, *expected_s;
    _S8 s2[len];
    ENUM_RETURN retval;
};

TEST(s_fold_s, null_string1)
{
    retval = s_fold_s(NULL, s2, len, 10);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_fold_s, null_string2)
{
    s1 = "a-z";
    retval = s_fold_s(s1, NULL, len, 10);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_fold_s, string2_size_is_0)
{
    s1 = "a-z";
    retval = s_fold_s(s1, s2, 0, 10);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_fold_s, fold_num_is_0)
{
    s1 = "a-z";
    retval = s_fold_s(s1, s2, len, 0);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_fold_s, fold_num_is_4)
{
    s1 = "a-z";
    retval = s_fold_s(s1, s2, len, 4);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_fold_s, s1_empty)
{
    s1 = "";
    expected_s = "";
    retval = s_fold_s(s1, s2, len, 10);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_fold_s, s1_newline)
{
    s1 = "\n";
    expected_s = "\n";
    retval = s_fold_s(s1, s2, len, 10);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_fold_s, s1_space)
{
    s1 = " ";
    expected_s = " ";
    retval = s_fold_s(s1, s2, len, 10);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_fold_s, s1_fold_len_non_blanks)
{
    s1 = "1234567890";
    expected_s = "1234567890";
    retval = s_fold_s(s1, s2, len, 10);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_fold_s, s1_fold_len_non_blanks_followed_a_newline)
{
    s1 = "1234567890\n";
    expected_s = "1234567890\n";
    retval = s_fold_s(s1, s2, len, 10);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_fold_s, s1_fold_len_non_blanks_with_newline)
{
    s1 = "123456789\n";
    expected_s = "123456789\n";
    retval = s_fold_s(s1, s2, len, 10);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_fold_s, s1_fold_len_blanks)
{
    s1 = "          ";
    expected_s = "          ";
    retval = s_fold_s(s1, s2, len, 10);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_fold_s, s1_fold_len_blanks_with_newline)
{
    s1 = "         \n";
    expected_s = "         \n";
    retval = s_fold_s(s1, s2, len, 10);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_fold_s, s1_longer_than_fold_len_non_blanks)
{
    s1 = "1234567890123";
    expected_s = "1234567890\n123";
    retval = s_fold_s(s1, s2, len, 10);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_fold_s, s1_2_times_longer_than_fold_len_non_blanks)
{
    s1 = "12345678901234567890";
    expected_s = "1234567890\n1234567890";
    retval = s_fold_s(s1, s2, len, 10);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_fold_s, s1_2_times_longer_than_fold_len_non_blanks_with_newline)
{
    s1 = "1234567890123456789\n";
    expected_s = "1234567890\n123456789\n";
    retval = s_fold_s(s1, s2, len, 10);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_fold_s, s1_longer_than_fold_len_blanks)
{
    s1 = "             ";
    expected_s = "          \n   ";
    retval = s_fold_s(s1, s2, len, 10);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_fold_s, s1_2_times_longer_than_fold_len_blanks)
{
    s1 = "                    ";
    expected_s = "          \n          ";
    retval = s_fold_s(s1, s2, len, 10);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_fold_s, s1_2_times_longer_than_fold_len_blanks_with_newline)
{
    s1 = "                   \n";
    expected_s = "          \n         \n";
    retval = s_fold_s(s1, s2, len, 10);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_fold_s, s1_second_word_is_too_long)
{
    s1 = "abcde abcde";
    expected_s = "abcde \nabcde";
    retval = s_fold_s(s1, s2, len, 10);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}


