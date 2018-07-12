#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;

TEST_GROUP(s_s32tostrbw)
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

TEST(s_s32tostrbw, null_string1)
{
    b = 2;
    retval = s_s32tostrbw(0, b, w, NULL, len);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_s32tostrbw, b_less_than_2)
{
    b = 1;
    retval = s_s32tostrbw(0, b, w, dest, len);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_s32tostrbw, b_more_than_36)
{
    b = 37;
    retval = s_s32tostrbw(0, b, w, dest, len);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_s32tostrbw, value_0_b_2_w_8)
{
    value = 0;
    b = 2;
    w = 8;
    expected_s = "       0";
    retval = s_s32tostrbw(value, b, w, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrbw, value_255_b_2_w_8)
{
    value = 255;
    b = 2;
    w = 8;
    expected_s = "11111111";
    retval = s_s32tostrbw(value, b, w, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrbw, value_max_b_2_w_32)
{
    value = S32_MAX;
    b = 2;
    w = 32;
    expected_s = " 1111111111111111111111111111111";
    retval = s_s32tostrbw(value, b, w, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrbw, value_min_b_2_w_32)
{
    value = S32_MIN;
    b = 2;
    w = 32;
    expected_s = "-10000000000000000000000000000000";
    retval = s_s32tostrbw(value, b, w, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrbw, value_max_b_8_w_32)
{
    value = S32_MAX;
    b = 8;
    w = 32;
    expected_s = "                     17777777777";
    retval = s_s32tostrbw(value, b, w, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrbw, value_min_b_8_w_32)
{
    value = S32_MIN;
    b = 8;
    w = 32;
    expected_s = "                    -20000000000";
    retval = s_s32tostrbw(value, b, w, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrbw, value_max_b_10_w_32)
{
    value = S32_MAX;
    b = 10;
    w = 32;
    expected_s = "                      2147483647";
    retval = s_s32tostrbw(value, b, w, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrbw, value_min_b_10_w_32)
{
    value = S32_MIN;
    b = 10;
    w = 32;
    expected_s = "                     -2147483648";
    retval = s_s32tostrbw(value, b, w, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrbw, value_max_b_16_w_32)
{
    value = S32_MAX;
    b = 16;
    w = 32;
    expected_s = "                        7FFFFFFF";
    retval = s_s32tostrbw(value, b, w, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrbw, value_min_b_16_w_32)
{
    value = S32_MIN;
    b = 16;
    w = 32;
    expected_s = "                       -80000000";
    retval = s_s32tostrbw(value, b, w, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrbw, value_max_b_36_w_32)
{
    value = S32_MAX;
    b = 36;
    w = 32;
    expected_s = "                          ZIK0ZJ";
    retval = s_s32tostrbw(value, b, w, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_s32tostrbw, value_min_b_36_w_32)
{
    value = S32_MIN;
    b = 36;
    w = 32;
    expected_s = "                         -ZIK0ZK";
    retval = s_s32tostrbw(value, b, w, dest, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

