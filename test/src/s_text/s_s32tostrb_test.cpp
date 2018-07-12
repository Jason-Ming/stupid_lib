#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;


TEST_GROUP(s_s32tostrb)
{
    void setup()
    {
    	//设置自己的测试准备
        value = 0;
        b = 0;
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
    _S8 dest[len];
    const _S8 *expected_s;
    ENUM_RETURN retval;
};

TEST(s_s32tostrb, null_string1)
{
    b = 2;
    retval = s_s32tostrb(0, b, NULL, len);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_s32tostrb, b_less_than_2)
{
    b = 1;
    retval = s_s32tostrb(0, b, dest, len);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_s32tostrb, b_more_than_36)
{
    b = 37;
    retval = s_s32tostrb(0, b, dest, len);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_s32tostrb, value_0_b_2)
{
    value = 0;
    b = 2;
    expected_s = "0";
    retval = s_s32tostrb(value, b, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrb, value_255_b_2)
{
    value = 255;
    b = 2;
    expected_s = "11111111";
    retval = s_s32tostrb(value, b, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrb, value_max_b_2)
{
    value = S32_MAX;
    b = 2;
    expected_s = "1111111111111111111111111111111";
    retval = s_s32tostrb(value, b, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrb, value_min_b_2)
{
    value = S32_MIN;
    b = 2;
    expected_s = "-10000000000000000000000000000000";
    retval = s_s32tostrb(value, b, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrb, value_max_b_8)
{
    value = S32_MAX;
    b = 8;
    expected_s = "17777777777";
    retval = s_s32tostrb(value, b, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrb, value_min_b_8)
{
    value = S32_MIN;
    b = 8;
    expected_s = "-20000000000";
    retval = s_s32tostrb(value, b, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrb, value_max_b_10)
{
    value = S32_MAX;
    b = 10;
    expected_s = "2147483647";
    retval = s_s32tostrb(value, b, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrb, value_min_b_10)
{
    value = S32_MIN;
    b = 10;
    expected_s = "-2147483648";
    retval = s_s32tostrb(value, b, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrb, value_max_b_16)
{
    value = S32_MAX;
    b = 16;
    expected_s = "7FFFFFFF";
    retval = s_s32tostrb(value, b, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrb, value_min_b_16)
{
    value = S32_MIN;
    b = 16;
    expected_s = "-80000000";
    retval = s_s32tostrb(value, b, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrb, value_max_b_36)
{
    value = S32_MAX;
    b = 36;
    expected_s = "ZIK0ZJ";
    retval = s_s32tostrb(value, b, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrb, value_min_b_36)
{
    value = S32_MIN;
    b = 36;
    expected_s = "-ZIK0ZK";
    retval = s_s32tostrb(value, b, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

