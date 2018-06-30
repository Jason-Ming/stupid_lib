#include "CppUTest/TestHarness.h"
#include <string.h>
#include <math.h>
#include "s_alg.h"
#include "s_type.h"
#include "s_mem.h"
#include "s_limits.h"

#include <iostream>

using namespace std;

TEST_GROUP(s_calc_rp)
{
    void setup()
    {
    	//设置自己的测试准备
        expression = NULL;
        result = 0;
        expected_result = 0.0;
        retval = RETURN_FAILURE;
    }

    void teardown()
    {
        //清理测试设置
        //cout << "Test end ......" << endl;
    }

    _SD expected_result;
    _SD result;
    const _S8 *expression;
    ENUM_RETURN retval;
};

TEST(s_calc_rp, null_expression)
{
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_calc_rp, null_result)
{
    expression = "1 1 +";
    retval = s_calc_rp(expression, NULL);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_calc_rp, expression_error)
{
    expression = "1 1 + 1";
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_calc_rp, expression_op_unknown)
{
    expression = "1 1 !";
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_calc_rp, expression_number_unknown)
{
    expression = "1 1.x -";
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_calc_rp, expression_add_normal)
{
    expression = "1 8 + ";
    expected_result = 1.0 + 8.0;
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_calc_rp, expression_mutiply_normal)
{
    expression = "1 8 * ";
    expected_result = 1.0 * 8.0;
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_calc_rp, expression_substract_normal)
{
    expression = "1 8 - ";
    expected_result = 1.0 - 8.0;
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_calc_rp, expression_devide_normal)
{
    expression = "1 8 / ";
    expected_result = 1.0/8;
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_calc_rp, expression_devide_zero)
{
    expression = "1  0 /";
    SET_VAR_VALUE(expected_result, DOUBLE_INFINITY_PLUS);
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    MEMCMP_EQUAL(&expected_result, &result, sizeof(expected_result));
}

TEST(s_calc_rp, expression_modular_normal)
{
    expression = "1 8 % ";
    expected_result = fmod(1.0, 8);
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_calc_rp, expression_modular_zero)
{
    expression = "1  0 %";
    expected_result = fmod(1.0, 0);
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    MEMCMP_EQUAL(&expected_result, &result, sizeof(expected_result));
}

TEST(s_calc_rp, expression_sin_normal)
{
    expression = "30 sin ";
    expected_result = sin(30);
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_calc_rp, expression_cos_normal)
{
    expression = "30 cos ";
    expected_result = cos(30);
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_calc_rp, expression_pow_normal)
{
    expression = "30 2 pow ";
    expected_result = pow(30.0, 2);
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_calc_rp, expression_exp_normal)
{
    expression = "2 exp ";
    expected_result = exp(2);
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_calc_rp, expression_e_format)
{
    expression = "-1.0e-2 100 * ";
    expected_result = -1.0e-2 * 100;
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_calc_rp, expression_include_var_letter)
{
    expression = "-1.0e-2 100 x = * x *";
    expected_result = -1.0e-2 * 100 * 100;
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_calc_rp, expression_include_var_letter_digit)
{
    expression = "-1.0e-2 100 x1 = * x1 *";
    expected_result = -1.0e-2 * 100 * 100;
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_calc_rp, expression_include_var_max_len)
{
    expression = "-1.0e-2 100 abcdefghijklmnopqrstuvwxyz012345 = * abcdefghijklmnopqrstuvwxyz012345 *";
    expected_result = -1.0e-2 * 100 * 100;
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_calc_rp, expression_include_var_larger_than_max_len)
{
    expression = "-1.0e-2 100 abcdefghijklmnopqrstuvwxyz0123456 = abcdefghijklmnopqrstuvwxyz0123457 * abcdefghijklmnopqrstuvwxyz0123458 *";
    expected_result = -1.0e-2 * 100 * 100;
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_calc_rp, expression_include_var_assigned_multitimes)
{
    expression = "-1.0e-2 100 x1 = x2 = x3 = x4 = x5 = x6 = x7 = x8 = x9 = x10 ="
        " x11 = x12 = x13 = x14 = x15 = x16 = x17 = x18 = x19 = x20 ="
        " x21 = x22 = x23 = x24 = x25 = x26 = x27 = x28 = x29 = x30 ="
        " x31 = x32 = x33 = x34 = x35 = x36 = x37 = x38 = x39 = x40 ="
        " x41 = x42 = x43 = x44 = x45 = x46 = x47 = x48 = x49 = x50 ="
        " x51 = x52 = x53 = x54 = x55 = x56 = x57 = x58 = x59 = x60 ="
        " x61 = x62 = x63 = x64 = 10 x64 ="
        " * * x64 *";
    expected_result = -1.0e-2 * 100 * 10 * 10;
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_calc_rp, expression_include_var_max_num)
{
    expression = "-1.0e-2 100 x1 = x2 = x3 = x4 = x5 = x6 = x7 = x8 = x9 = x10 ="
        " x11 = x12 = x13 = x14 = x15 = x16 = x17 = x18 = x19 = x20 ="
        " x21 = x22 = x23 = x24 = x25 = x26 = x27 = x28 = x29 = x30 ="
        " x31 = x32 = x33 = x34 = x35 = x36 = x37 = x38 = x39 = x40 ="
        " x41 = x42 = x43 = x44 = x45 = x46 = x47 = x48 = x49 = x50 ="
        " x51 = x52 = x53 = x54 = x55 = x56 = x57 = x58 = x59 = x60 ="
        " x61 = x62 = x63 = x64 ="
        " * x64 *";
    expected_result = -1.0e-2 * 100 * 100;
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

TEST(s_calc_rp, expression_include_var_larger_than_max_num)
{
    expression = "-1.0e-2 100 x1 = x2 = x3 = x4 = x5 = x6 = x7 = x8 = x9 = x10 ="
        " x11 = x12 = x13 = x14 = x15 = x16 = x17 = x18 = x19 = x20 ="
        " x21 = x22 = x23 = x24 = x25 = x26 = x27 = x28 = x29 = x30 ="
        " x31 = x32 = x33 = x34 = x35 = x36 = x37 = x38 = x39 = x40 ="
        " x41 = x42 = x43 = x44 = x45 = x46 = x47 = x48 = x49 = x50 ="
        " x51 = x52 = x53 = x54 = x55 = x56 = x57 = x58 = x59 = x60 ="
        " x61 = x62 = x63 = x64 = x65 = x66 = x67 = x68 = x69 = x70 ="
        " * x70 *";
    expected_result = -1.0e-2 * 100 * 100;
    retval = s_calc_rp(expression, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //DOUBLES_EQUAL(expected_result, result, 0.1E-10);
}

