#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;


TEST_GROUP(s_hstrtos64)
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

TEST(s_hstrtos64, null_string)
{
    const _S8 *str = NULL;
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = 0x00;

    retval = s_hstrtos64(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //CHECK_EQUAL(0x00, result);
}

TEST(s_hstrtos64, null_value)
{
    const _S8 *str = "0x00";
    ENUM_RETURN retval = RETURN_FAILURE;
    //_S64 result = 0x00;

    retval = s_hstrtos64(str, NULL);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //CHECK_EQUAL(0x00, result);
}

TEST(s_hstrtos64, empty_str)
{
    const _S8 *str = "";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = 0x00;

    retval = s_hstrtos64(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //CHECK_EQUAL(0x00, result);
}

TEST(s_hstrtos64, invalid_str_x)
{
    const _S8 *str = "x00";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = 0x00;

    retval = s_hstrtos64(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //CHECK_EQUAL(0x00, result);
}

TEST(s_hstrtos64, invalid_str_00x)
{
    const _S8 *str = "00x00";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = 0x00;

    retval = s_hstrtos64(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //CHECK_EQUAL(0x00, result);
}

TEST(s_hstrtos64, invalid_str_0x_empty)
{
    const _S8 *str = "0x";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = 0x00;

    retval = s_hstrtos64(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //CHECK_EQUAL(0x00, result);
}

TEST(s_hstrtos64, invalid_str_0x_q)
{
    const _S8 *str = "0xfg";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = 0x00;

    retval = s_hstrtos64(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //CHECK_EQUAL(0x00, result);
}

TEST(s_hstrtos64, invalid_str_q)
{
    const _S8 *str = "34fq";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = 0x00;

    retval = s_hstrtos64(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //CHECK_EQUAL(0x00, result);
}

TEST(s_hstrtos64, invalid_str_suffix_blank)
{
    const _S8 *str = "34f ";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = 0x00;

    retval = s_hstrtos64(str, &result);

    CHECK_EQUAL(RETURN_FAILURE, retval);
    //CHECK_EQUAL(0x00, result);
}

TEST(s_hstrtos64, invalid_str_prefix_blank)
{
    const _S8 *str = "  034f";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = S64_INVALID;

    retval = s_hstrtos64(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    _S64 expected_result = 0x34f;
    MEMCMP_EQUAL((_VOID*)&expected_result, (_VOID*)&result, sizeof(_S64));
}

TEST(s_hstrtos64, 8bits)
{
    const _S8 *str = "0x83";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = S64_INVALID;

    retval = s_hstrtos64(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    _S64 expected_result = 0x83;
    MEMCMP_EQUAL((_VOID*)&expected_result, (_VOID*)&result, sizeof(_S64));
}

TEST(s_hstrtos64, 16bits)
{
    const _S8 *str = "0x837e";/* 1000 0011 0111 1110 */
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = S64_INVALID;

    retval = s_hstrtos64(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    _S64 expected_result = 0x837e;
    MEMCMP_EQUAL((_VOID*)&expected_result, (_VOID*)&result, sizeof(_S64));
}

TEST(s_hstrtos64, 32bits)
{    
    const _S8 *str = "0x837d4e75";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = S64_INVALID;

    retval = s_hstrtos64(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    _S64 expected_result = 0x837d4e75;
    MEMCMP_EQUAL((_VOID*)&expected_result, (_VOID*)&result, sizeof(_S64));


}

TEST(s_hstrtos64, 64bits)
{
    const _S8 *str = "0x837d4e7518ef60bc";
    ENUM_RETURN retval = RETURN_FAILURE;
    _S64 result = S64_INVALID;
    

    retval = s_hstrtos64(str, &result);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    _S64 expected_result = 0x837d4e7518ef60bc;
    MEMCMP_EQUAL((_VOID*)&expected_result, (_VOID*)&result, sizeof(_S64));
}

