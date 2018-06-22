#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;

TEST_GROUP(s_htoi)
{
    void setup()
    {
    	//设置自己的测试准备
        //cout << "Test start ......" << endl;
    }

    void teardown()
    {
        //清理测试设置
        //cout << "Test end ......" << endl;
    }
};

TEST(s_htoi, null_string)
{
    const _S8 *str = NULL;
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = 0x00;

    retval = s_htoi(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //CHECK_EQUAL(0x00, result);
}

TEST(s_htoi, null_value)
{
    const _S8 *str = "0x00";
    ENUM_RETURN retval = RETURN_FAILURE;
    //_S64 result = 0x00;

    retval = s_htoi(str, NULL);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //CHECK_EQUAL(0x00, result);
}

TEST(s_htoi, empty_str)
{
    const _S8 *str = "";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = 0x00;

    retval = s_htoi(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //CHECK_EQUAL(0x00, result);
}

TEST(s_htoi, invalid_str_x)
{
    const _S8 *str = "x00";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = 0x00;

    retval = s_htoi(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //CHECK_EQUAL(0x00, result);
}

TEST(s_htoi, invalid_str_00x)
{
    const _S8 *str = "00x00";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = 0x00;

    retval = s_htoi(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //CHECK_EQUAL(0x00, result);
}

TEST(s_htoi, invalid_str_0x_empty)
{
    const _S8 *str = "0x";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = 0x00;

    retval = s_htoi(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //CHECK_EQUAL(0x00, result);
}

TEST(s_htoi, invalid_str_0x_q)
{
    const _S8 *str = "0xfg";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = 0x00;

    retval = s_htoi(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //CHECK_EQUAL(0x00, result);
}

TEST(s_htoi, invalid_str_q)
{
    const _S8 *str = "34fq";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = 0x00;

    retval = s_htoi(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //CHECK_EQUAL(0x00, result);
}

TEST(s_htoi, invalid_str_suffix_blank)
{
    const _S8 *str = "34f ";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = 0x00;

    retval = s_htoi(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //CHECK_EQUAL(0x00, result);
}

TEST(s_htoi, invalid_str_prefix_blank)
{
    const _S8 *str = "  034f";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = S64_INVALID;

    retval = s_htoi(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    _S64 expected_result = 0x34f;
    MEMCMP_EQUAL((_VOID*)&expected_result, (_VOID*)&result, sizeof(_S64));
}

TEST(s_htoi, 8bits)
{
    const _S8 *str = "0x83";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = S64_INVALID;

    retval = s_htoi(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    _S64 expected_result = 0x83;
    MEMCMP_EQUAL((_VOID*)&expected_result, (_VOID*)&result, sizeof(_S64));
}

TEST(s_htoi, 16bits)
{
    const _S8 *str = "0x837e";/* 1000 0011 0111 1110 */
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = S64_INVALID;

    retval = s_htoi(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    _S64 expected_result = 0x837e;
    MEMCMP_EQUAL((_VOID*)&expected_result, (_VOID*)&result, sizeof(_S64));
}

TEST(s_htoi, 32bits)
{    
    const _S8 *str = "0x837d4e75";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = S64_INVALID;

    retval = s_htoi(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    _S64 expected_result = 0x837d4e75;
    MEMCMP_EQUAL((_VOID*)&expected_result, (_VOID*)&result, sizeof(_S64));


}

TEST(s_htoi, 64bits)
{
    const _S8 *str = "0x837d4e7518ef60bc";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = S64_INVALID;
    

    retval = s_htoi(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    _S64 expected_result = 0x837d4e7518ef60bc;
    MEMCMP_EQUAL((_VOID*)&expected_result, (_VOID*)&result, sizeof(_S64));
}



TEST_GROUP(s_atoi)
{
    void setup()
    {
    	//设置自己的测试准备
        str = NULL;
        retval = RETURN_FAILURE;
        result = 0x00;
        expected_result = 0x00;
    }

    void teardown()
    {
        //清理测试设置
        //cout << "Test end ......" << endl;
    }

    const _S8 *str;
    ENUM_RETURN retval;
    _S32 result;
    _S32 expected_result;
};

TEST(s_atoi, null_string)
{
    retval = s_atoi(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_atoi, null_value)
{
    str = "0";

    retval = s_atoi(str, NULL);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_atoi, empty_str)
{
    str = "";

    retval = s_atoi(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_atoi, invalid_str__)
{
    str = "_";

    retval = s_atoi(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_atoi, invalid_str_00_plus)
{
    str = "00+";

    retval = s_atoi(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_atoi, invalid_str_plus_empty)
{
    str = "+";

    retval = s_atoi(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_atoi, invalid_str_minus_empty)
{
    str = "-";
    
    retval = s_atoi(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);

}

TEST(s_atoi, invalid_str_plus_q)
{
    str = "+q";
    retval = s_atoi(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);

}

TEST(s_atoi, invalid_str_q)
{
    str = "q";
    retval = s_atoi(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);

}

TEST(s_atoi, invalid_str_suffix_blank)
{
    str = "34 ";
    retval = s_atoi(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);

}

TEST(s_atoi, invalid_str_prefix_blank)
{
    str = "  034";

    retval = s_atoi(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 34;
    CHECK_EQUAL(expected_result, result);
}

TEST(s_atoi, zero)
{
    str = "0";
    retval = s_atoi(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 0;
    CHECK_EQUAL(expected_result, result);
}

TEST(s_atoi, zero_plus)
{
    str = "+0";
    retval = s_atoi(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 0;
    CHECK_EQUAL(expected_result, result);
}

TEST(s_atoi, zero_minus)
{
    str = "-0";
    retval = s_atoi(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 0;
    CHECK_EQUAL(expected_result, result);
}

TEST(s_atoi, max)
{
    str = "2147483647";
    retval = s_atoi(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 2147483647;
    CHECK_EQUAL(expected_result, result);
}

TEST(s_atoi, min)
{
    str = "-2147483648";
    retval = s_atoi(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = INT_MIN;
    CHECK_EQUAL(expected_result, result);
}

TEST(s_atoi, out_of_range_max)
{
    str = "2147483648";
    retval = s_atoi(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = INT_MIN;
    CHECK_EQUAL(expected_result, result);

}

TEST(s_atoi, out_of_range_min)
{
    str = "-2147483649";
    retval = s_atoi(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 2147483647;
    CHECK_EQUAL(expected_result, result);
}

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

