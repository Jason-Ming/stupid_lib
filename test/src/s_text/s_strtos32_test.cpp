#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;


TEST_GROUP(s_strtos32)
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

TEST(s_strtos32, null_string)
{
    retval = s_strtos32(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strtos32, null_value)
{
    str = "0";

    retval = s_strtos32(str, NULL);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strtos32, empty_str)
{
    str = "";

    retval = s_strtos32(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strtos32, invalid_str__)
{
    str = "_";

    retval = s_strtos32(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strtos32, invalid_str_00_plus)
{
    str = "00+";

    retval = s_strtos32(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strtos32, invalid_str_plus_empty)
{
    str = "+";

    retval = s_strtos32(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strtos32, invalid_str_minus_empty)
{
    str = "-";
    
    retval = s_strtos32(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);

}

TEST(s_strtos32, invalid_str_plus_q)
{
    str = "+q";
    retval = s_strtos32(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);

}

TEST(s_strtos32, invalid_str_q)
{
    str = "q";
    retval = s_strtos32(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);

}

TEST(s_strtos32, invalid_str_suffix_blank)
{
    str = "34 ";
    retval = s_strtos32(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);

}

TEST(s_strtos32, invalid_str_prefix_blank)
{
    str = "  034";

    retval = s_strtos32(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 34;
    CHECK_EQUAL(expected_result, result);
}

TEST(s_strtos32, zero)
{
    str = "0";
    retval = s_strtos32(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 0;
    CHECK_EQUAL(expected_result, result);
}

TEST(s_strtos32, zero_plus)
{
    str = "+0";
    retval = s_strtos32(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 0;
    CHECK_EQUAL(expected_result, result);
}

TEST(s_strtos32, zero_minus)
{
    str = "-0";
    retval = s_strtos32(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 0;
    CHECK_EQUAL(expected_result, result);
}

TEST(s_strtos32, max)
{
    str = "2147483647";
    retval = s_strtos32(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 2147483647;
    CHECK_EQUAL(expected_result, result);
}

TEST(s_strtos32, min)
{
    str = "-2147483648";
    retval = s_strtos32(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = INT_MIN;
    CHECK_EQUAL(expected_result, result);
}

TEST(s_strtos32, out_of_range_max)
{
    str = "2147483648";
    retval = s_strtos32(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = INT_MIN;
    CHECK_EQUAL(expected_result, result);

}

TEST(s_strtos32, out_of_range_min)
{
    str = "-2147483649";
    retval = s_strtos32(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 2147483647;
    CHECK_EQUAL(expected_result, result);
}

