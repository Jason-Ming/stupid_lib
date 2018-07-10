#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;
TEST_GROUP(print_types)
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

TEST(print_types, print_types)
{
    //print_types();
}

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

TEST_GROUP(s_get_word)
{
    void setup()
    {
    	//设置自己的测试准备
        source = expected_word = next = NULL;
        retval = RETURN_FAILURE;
        word_len = expected_word_len = 0;
    }

    void teardown()
    {
        //清理测试设置
        //cout << "Test end ......" << endl;
    }

    const static size_t len = 1024;
    const _S8 *source, *expected_word, *next;
    _S8 word[len];
    size_t word_len, expected_word_len;
    ENUM_RETURN retval;
};

TEST(s_get_word, null_source)
{
    retval = s_get_word(NULL, word, len, &word_len, &next);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_get_word, null_word)
{
    source = "abc";
    retval = s_get_word(source, NULL, len, &word_len, &next);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_get_word, null_wordlen)
{
    source = "abc";
    retval = s_get_word(source, word, len, NULL, &next);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_get_word, null_next)
{
    source = "abc";
    retval = s_get_word(source, word, len, &word_len, NULL);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_get_word, normal_no_word)
{
    source = "";
    expected_word = "";
    expected_word_len = 0;
    retval = s_get_word(source, word, len, &word_len, &next);

    CHECK_EQUAL(RETURN_SUCCESS, retval);

    STRCMP_EQUAL(expected_word, word);
    CHECK_EQUAL(expected_word_len, word_len);
    POINTERS_EQUAL(source+strlen(word), next);
}


TEST(s_get_word, normal_one_word)
{
    source = "abc";
    expected_word = "abc";
    expected_word_len = 3;
    retval = s_get_word(source, word, len, &word_len, &next);

    CHECK_EQUAL(RETURN_SUCCESS, retval);

    STRCMP_EQUAL(expected_word, word);
    CHECK_EQUAL(expected_word_len, word_len);
    POINTERS_EQUAL(source+strlen(word), next);

    expected_word = "";
    expected_word_len = 0;

    retval = s_get_word(next, word, len, &word_len, &next);

    CHECK_EQUAL(RETURN_SUCCESS, retval);

    STRCMP_EQUAL(expected_word, word);
    CHECK_EQUAL(expected_word_len, word_len);
    POINTERS_EQUAL(next+strlen(word), next);
}

TEST(s_get_word, normal_two_word)
{
    source = "  abc   3456 ";
    expected_word = "abc";
    expected_word_len = 3;
    retval = s_get_word(source, word, len, &word_len, &next);

    CHECK_EQUAL(RETURN_SUCCESS, retval);

    STRCMP_EQUAL(expected_word, word);
    CHECK_EQUAL(expected_word_len, word_len);
    POINTERS_EQUAL(source+ 2 + strlen(word) + 3, next);

    expected_word = "3456";
    expected_word_len = 4;

    retval = s_get_word(next, word, len, &word_len, &next);

    CHECK_EQUAL(RETURN_SUCCESS, retval);

    STRCMP_EQUAL(expected_word, word);
    CHECK_EQUAL(expected_word_len, word_len);
    POINTERS_EQUAL(source + 8 + strlen(word) + 1, next);

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

TEST_GROUP(s_trim)
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

TEST(s_trim, null_string1)
{
    retval = s_trim(NULL);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_trim, no_blank)
{
    expected_s = "abc";
    strcpy(dest, "abc");
    retval = s_trim(dest);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);
}

TEST(s_trim, blank_space)
{
    expected_s = "abc";
    strcpy(dest, "abc ");
    retval = s_trim(dest);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);

}

TEST(s_trim, blank_tab)
{
    expected_s = "abc";
    strcpy(dest, "abc\t");
    retval = s_trim(dest);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);

}

TEST(s_trim, blank_newline)
{
    expected_s = "abc";
    strcpy(dest, "abc\n");
    retval = s_trim(dest);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, dest);

}

TEST_GROUP(s_strindex)
{
    void setup()
    {
    	//设置自己的测试准备
        target = NULL;
        source = NULL;
        index = 0;
        index_expected = 0;
        retval = RETURN_FAILURE;
    }

    void teardown()
    {
        //清理测试设置
        //cout << "Test end ......" << endl;
    }

    _S32 index_expected;
    _S32 index;
    const _S8 *source;
    const _S8 *target;
    ENUM_RETURN retval;
};

TEST(s_strindex, null_string1)
{
    target = "abc";
    retval = s_strindex(source, target, &index);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strindex, null_string2)
{
    source = "abc";
    retval = s_strindex(source, target, &index);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strindex, null_index)
{
    source = "abc";
    target = "abc";
    retval = s_strindex(source, target, NULL);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strindex, completely_match)
{
    source = "abc";
    target = "abc";
    index_expected = 0;
    retval = s_strindex(source, target, &index);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(index_expected, index);
}

TEST(s_strindex, not_match)
{
    source = "abbc";
    target = "abc";
    index_expected = -1;
    retval = s_strindex(source, target, &index);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(index_expected, index);
}

TEST(s_strindex, one_match)
{
    source = "abbcabcab";
    target = "abc";
    index_expected = 4;
    retval = s_strindex(source, target, &index);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(index_expected, index);
}

TEST(s_strindex, multi_match)
{
    source = "abbcabcabcc";
    target = "abc";
    index_expected = 4;
    retval = s_strindex(source, target, &index);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(index_expected, index);
}

TEST_GROUP(s_strrindex)
{
    void setup()
    {
    	//设置自己的测试准备
        target = NULL;
        source = NULL;
        index = 0;
        index_expected = 0;
        retval = RETURN_FAILURE;
    }

    void teardown()
    {
        //清理测试设置
        //cout << "Test end ......" << endl;
    }

    _S32 index_expected;
    _S32 index;
    const _S8 *source;
    const _S8 *target;
    ENUM_RETURN retval;
};

TEST(s_strrindex, null_string1)
{
    target = "abc";
    retval = s_strrindex(source, target, &index);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strrindex, null_string2)
{
    source = "abc";
    retval = s_strrindex(source, target, &index);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strrindex, null_index)
{
    source = "abc";
    target = "abc";
    retval = s_strrindex(source, target, NULL);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strrindex, completely_match)
{
    source = "abc";
    target = "abc";
    index_expected = 0;
    retval = s_strrindex(source, target, &index);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(index_expected, index);
}

TEST(s_strrindex, not_match)
{
    source = "abbc";
    target = "abc";
    index_expected = -1;
    retval = s_strrindex(source, target, &index);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(index_expected, index);
}

TEST(s_strrindex, one_match)
{
    source = "abbcabcab";
    target = "abc";
    index_expected = 4;
    retval = s_strrindex(source, target, &index);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(index_expected, index);
}

TEST(s_strrindex, multi_match)
{
    source = "abbcabcabcc";
    target = "abc";
    index_expected = 7;
    retval = s_strrindex(source, target, &index);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(index_expected, index);
}

TEST_GROUP(s_entab)
{
    void setup()
    {
    	//设置自己的测试准备
        source = NULL;
        target[0] = '\0';
        target_expected = NULL;
        retval = RETURN_FAILURE;
        tab_stop = 8;
    }

    void teardown()
    {
        //清理测试设置
        //cout << "Test end ......" << endl;
    }

    const _S8 * target_expected;
    const _S8 *source;
    _S8 target[1000];
    static const size_t len = 1000;
    _S32 tab_stop;
    ENUM_RETURN retval;
};

TEST(s_entab, null_source)
{
    retval = s_entab(source, target, len, tab_stop);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_entab, null_target)
{
    source = "abc        123   1234567  str";
    retval = s_entab(source, NULL, len, tab_stop);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_entab, invalid_len)
{
    source = "abc        123   1234567  str";
    retval = s_entab(source, target, 0, tab_stop);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_entab, invalid_tab_stop)
{
    source = "abc        123   1234567  str";
    retval = s_entab(source, target, len, 0);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_entab, normal1)
{
    source = "abc        123   1234567  str";
    target_expected = "abc\t   123\t 1234567  str";
    retval = s_entab(source, target, len, tab_stop);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(target_expected, target);
}

TEST(s_entab, normal2)
{
             source = "abc\t123   abc  \t abc  \n ggg     123\n";
    target_expected = "abc\t123   abc\t abc  \n ggg\t 123\n";
    retval = s_entab(source, target, len, tab_stop);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(target_expected, target);
}

TEST(s_entab, normal_not_need_to_replace)
{
    source = "abc\t   123\t 1234567  str";
    target_expected = "abc\t   123\t 1234567  str";
    retval = s_entab(source, target, len, tab_stop);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(target_expected, target);
}

TEST_GROUP(s_detab)
{
    void setup()
    {
    	//设置自己的测试准备
        source = NULL;
        target[0] = '\0';
        target_expected = NULL;
        retval = RETURN_FAILURE;
        tab_stop = 8;
    }

    void teardown()
    {
        //清理测试设置
        //cout << "Test end ......" << endl;
    }

    const _S8 * target_expected;
    const _S8 *source;
    _S8 target[1000];
    static const size_t len = 1000;
    _S32 tab_stop;
    ENUM_RETURN retval;
};

TEST(s_detab, null_source)
{
    retval = s_detab(source, target, len, tab_stop);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_detab, null_target)
{
    source = "abc        123   1234567  str";
    retval = s_detab(source, NULL, len, tab_stop);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_detab, invalid_len)
{
    source = "abc        123   1234567  str";
    retval = s_detab(source, target, 0, tab_stop);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_detab, invalid_tab_stop)
{
    source = "abc        123   1234567  str";
    retval = s_detab(source, target, len, 0);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_detab, normal1)
{
    source = "abc\t   123\t 1234567  str";
    target_expected = "abc        123   1234567  str";
    retval = s_detab(source, target, len, tab_stop);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(target_expected, target);
}

TEST(s_detab, normal2)
{
             source = "abc\t   123   abc\t abc\n";
    target_expected = "abc        123   abc     abc\n";
    retval = s_detab(source, target, len, tab_stop);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(target_expected, target);
}

TEST(s_detab, normal3)
{
              source= "abc\t123   abc  \t abc  \n ggg     123\n";
    target_expected = "abc     123   abc        abc  \n ggg     123\n";
    retval = s_detab(source, target, len, tab_stop);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(target_expected, target);
}

TEST(s_detab, normal_not_need_to_replace)
{
    source = "abc        123   1234567  str";
    target_expected = "abc        123   1234567  str";
    retval = s_detab(source, target, len, tab_stop);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(target_expected, target);
}

