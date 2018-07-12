#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;



TEST_GROUP(s_expand)
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

TEST(s_expand, null_string1)
{
    retval = s_expand(NULL, s2, len);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_expand, null_string2)
{
    s1 = "a-z";
    retval = s_expand(s1, NULL, len);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_expand, a)
{
    s1 = "a";
    expected_s = "a";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, prefix_a)
{
    s1 = "-a";
    expected_s = "-a";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, a_suffix)
{
    s1 = "a-";
    expected_s = "a-";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, a_conc_conc_z)
{
    s1 = "a--z";
    expected_s = "a--z";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, az)
{
    s1 = "a-z";
    expected_s = "abcdefghijklmnopqrstuvwxyz";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, a_z)
{
    s1 = "a-z";
    expected_s = "abcdefghijklmnopqrstuvwxyz";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, prefix_a_z)
{
    s1 = "-a-z";
    expected_s = "-abcdefghijklmnopqrstuvwxyz";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, a_z_suffix)
{
    s1 = "a-z-";
    expected_s = "abcdefghijklmnopqrstuvwxyz-";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, prefix_a_z_suffix)
{
    s1 = "-a-z-";
    expected_s = "-abcdefghijklmnopqrstuvwxyz-";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, prefix_a_z_suffix_prefix_0_9)
{
    s1 = "-a-z--0-9";
    expected_s = "-abcdefghijklmnopqrstuvwxyz--0123456789";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, prefix_a_z_suffix_prefix_0_9_suffix_prefix_A_Z_suffix)
{
    s1 = "-a-z--0-9--A-Z-";
    expected_s = "-abcdefghijklmnopqrstuvwxyz--0123456789--ABCDEFGHIJKLMNOPQRSTUVWXYZ-";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, prefix_a_z_0_9_suffix_prefix_A_Z_suffix)
{
    s1 = "-a-z0-9--A-Z-";
    expected_s = "-abcdefghijklmnopqrstuvwxyz0123456789--ABCDEFGHIJKLMNOPQRSTUVWXYZ-";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, prefix_a_z_0_9_A_Z_suffix)
{
    s1 = "-a-z0-9A-Z-";
    expected_s = "-abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, a_z_0_9_A_Z)
{
    s1 = "a-z0-9A-Z";
    expected_s = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, a_suffix_0_9_A_Z)
{
    s1 = "a-0-9A-Z";
    expected_s = "a-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, a_a_0_9_A_Z)
{
    s1 = "a-a0-9A-Z";
    expected_s = "a0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, a_a_b0_9_A_Z)
{
    s1 = "a-a-b0-9A-Z";
    expected_s = "ab0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, a_a_b_a0_9_A_Z)
{
    s1 = "a-a-ba0-9A-Z";
    expected_s = "aba0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, a_a_b_a_b0_9_A_Z)
{
    s1 = "a-a-bab0-9A-Z";
    expected_s = "abab0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, a_a_b_a_c_b0_9_A_Z)
{
    s1 = "a-a-bacb0-9A-Z";
    expected_s = "abacb0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_expand, 0_9_0)
{
    s1 = "0-9-0";
    expected_s = "0123456789-0";
    retval = s_expand(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

