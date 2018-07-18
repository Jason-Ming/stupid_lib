#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;

TEST_GROUP(s_trim)
{
    void setup()
    {
    	//设置自己的测试准备
        value = 0;
        b = 0;
        w = 0;
        expected_s = NULL;
        retval = RETURN_FAILURE;
    }

    void teardown()
    {
        //清理测试设置
        //cout << "Test end ......" << endl;
    }

    const static size_t len = 1024;
    _S32 value;
    _S32 b;
    _S32 w;
    _S8 dest[len];
    const _S8 *expected_s;
    ENUM_RETURN retval;
};

TEST(s_trim, null_string1)
{
    retval = s_trim(NULL);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_trim, no_blank_empty)
{
    expected_s = "";
    dest[0] = '\0';
    retval = s_trim(dest);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_trim, no_blank)
{
    expected_s = "abc";
    strcpy(dest, "abc");
    retval = s_trim(dest);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_trim, blank_space)
{
    expected_s = "abc";
    strcpy(dest, "abc ");
    retval = s_trim(dest);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);

}

TEST(s_trim, blank_tab)
{
    expected_s = "abc";
    strcpy(dest, "abc\t");
    retval = s_trim(dest);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);

}

TEST(s_trim, blank_newline)
{
    expected_s = "abc";
    strcpy(dest, "abc\n");
    retval = s_trim(dest);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);

}

