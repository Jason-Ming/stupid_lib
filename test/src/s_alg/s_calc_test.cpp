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

