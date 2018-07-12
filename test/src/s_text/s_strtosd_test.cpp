#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;



TEST_GROUP(s_strtosd)
{
    void setup()
    {
    	//设置自己的测试准备
        str = NULL;
        retval = RETURN_FAILURE;
        result = 0.0;
        expected_result = 0.0;
    }

    void teardown()
    {
        //清理测试设置
        //cout << "Test end ......" << endl;
    }

    const _S8 *str;
    ENUM_RETURN retval;
    _SD result;
    _SD expected_result;
};

TEST(s_strtosd, null_string)
{
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strtosd, null_value)
{
    str = "0";

    retval = s_strtosd(str, NULL);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strtosd, empty_str)
{
    str = "";

    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strtosd, invalid_str__)
{
    str = "_";

    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strtosd, invalid_str_00_plus)
{
    str = "00+";

    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strtosd, invalid_str_plus_empty)
{
    str = "+";

    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strtosd, invalid_str_minus_empty)
{
    str = "-";
    
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);

}

TEST(s_strtosd, invalid_str_plus_q)
{
    str = "+q";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);

}

TEST(s_strtosd, invalid_str_q)
{
    str = "q";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);

}

TEST(s_strtosd, invalid_str_suffix_blank)
{
    str = "34 ";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);

}

TEST(s_strtosd, dot_only)
{
    str = " .";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);

}

TEST(s_strtosd, invalid_str_prefix_blank)
{
    str = "  034";

    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 34;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, zero)
{
    str = "0";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 0;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, zero_dot)
{
    str = "0.";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 0;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, dot_zero)
{
    str = ".0";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 0;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, zero_dot_zero)
{
    str = "0.0";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 0;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, zero_double_positive)
{
    str = "++0";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //expected_result = 0;
    //DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, zero_positive)
{
    str = "+0";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 0;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, dot_zero_positive)
{
    str = "+.0";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 0;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, zero_dot_positive)
{
    str = "+0.";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 0;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, zero_dot_zero_positive)
{
    str = "+0.0";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 0;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, zero_negative)
{
    str = "-0";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 0;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, zero_double_negative)
{
    str = "--0";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //expected_result = 0;
    //DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, zero_dot_negative)
{
    str = "-0.";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 0;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, dot_zero_negative)
{
    str = "-.0";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 0;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, zero_dot_zero_negative)
{
    str = "-0.0";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 0;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, positive_x_x)
{
    str = "+10.56";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 10.56;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, positive_x_x_dot)
{
    str = "+10.56.";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //expected_result = 10.56;
    //DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, positive_x_x_e)
{
    str = "+10.56e";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //expected_result = 0;
    //DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, positive_x_x_e_positive)
{
    str = "+10.56e+";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //expected_result = 0;
    //DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, positive_x_x_e_negative)
{
    str = "+10.56e-";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //expected_result = 0;
    //DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, positive_x_x_double_e)
{
    str = "+10.56ee";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //expected_result = 0;
    //DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, positive_x_x_e_dot)
{
    str = "+10.56e.";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //expected_result = 0;
    //DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, positive_x_x_e_double_dot)
{
    str = "+10.56e..";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //expected_result = 0;
    //DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, positive_x_x_e_x_dot)
{
    str = "+10.56e10.";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //expected_result = 0;
    //DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, positive_x_x_e_dot_x)
{
    str = "+10.56e.10";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //expected_result = 0;
    //DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, positive_x_x_e_x_dot_x)
{
    str = "+10.56e10.10";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //expected_result = 0;
    //DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, positive_x_x_positive_e_x)
{
    str = "+10.56-e10";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //expected_result = +10.56e10;
    //DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, positive_x_x_negative_e_x)
{
    str = "+10.56+e10";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //expected_result = +10.56e10;
    //DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, positive_x_x_e_positive_x)
{
    str = "+10.56e+10";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = +10.56e+10;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, positive_x_x_e_negative_x)
{
    str = "+10.56e-10";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = +10.56e-10;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, positive_x_x_e_x)
{
    str = "+10.56e10";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = +10.56e10;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, positive_x_x_E_x)
{
    str = "+10.56E10";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = +10.56e10;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, max)
{
    str = "2147483647";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 2147483647;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, min)
{
    str = "-2147483648";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = -2147483648.0;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_strtosd, out_of_range_max)
{
    str = "2147483648";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = 2147483648.0;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);

}

TEST(s_strtosd, out_of_range_min)
{
    str = "-2147483649";
    retval = s_strtosd(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    expected_result = -2147483649.0;
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

