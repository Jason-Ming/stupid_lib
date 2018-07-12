#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;




TEST_GROUP(s_s32tostr)
{
    void setup()
    {
    	//设置自己的测试准备
        value = 0;
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
    _S8 dest[len];
    const _S8 *expected_s;
    ENUM_RETURN retval;
};

TEST(s_s32tostr, null_string1)
{
    retval = s_s32tostr(0, NULL, len);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_s32tostr, 0)
{
    value = 0;
    expected_s = "0";
    retval = s_s32tostr(value, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostr, 0_positive)
{
    value = +0;
    expected_s = "0";
    retval = s_s32tostr(value, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostr, 0_negative)
{
    value = -0;
    expected_s = "0";
    retval = s_s32tostr(value, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostr, max)
{
    value = S32_MAX;
    expected_s = "2147483647";
    retval = s_s32tostr(value, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostr, min)
{
    value = S32_MIN;
    expected_s = "-2147483648";
    retval = s_s32tostr(value, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

