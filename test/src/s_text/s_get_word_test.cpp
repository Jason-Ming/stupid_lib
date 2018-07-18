#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;



TEST_GROUP(s_get_word_s)
{
    void setup()
    {
    	//设置自己的测试准备
        source = source_temp = expected_word = next = NULL;
        retval = RETURN_FAILURE;
        word_len = expected_word_len = 0;
        s_set_separators(NULL);
    }

    void teardown()
    {
        //清理测试设置
        //cout << "Test end ......" << endl;
    }

    const static size_t len = 1024;
    const _S8 *source, *source_temp, *expected_word, *next;
    _S8 word[len];
    size_t word_len, expected_word_len;
    ENUM_RETURN retval;
};

TEST(s_get_word_s, null_source1)
{
    retval = s_get_word_s(NULL, word, len, &word_len);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_get_word_s, null_source2)
{
    retval = s_get_word_s(&source, word, len, &word_len);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_get_word_s, null_word)
{
    source = "abc";
    retval = s_get_word_s(&source, NULL, len, &word_len);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_get_word_s, null_wordlen)
{
    source = "abc";
    retval = s_get_word_s(&source, word, len, NULL);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_get_word_s, normal_no_word)
{
    source = "";
    source_temp = source;
    expected_word = "";
    expected_word_len = 0;
    retval = s_get_word_s(&source_temp, word, len, &word_len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);

    STRCMP_EQUAL(expected_word, word);
    CHECK_EQUAL(expected_word_len, word_len);
    POINTERS_EQUAL(source+strlen(word), source_temp);
}


TEST(s_get_word_s, normal_one_word)
{
    source = "abc";
    source_temp = source;
    expected_word = "abc";
    expected_word_len = 3;
    retval = s_get_word_s(&source_temp, word, len, &word_len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);

    STRCMP_EQUAL(expected_word, word);
    CHECK_EQUAL(expected_word_len, word_len);
    POINTERS_EQUAL(source+strlen(word), source_temp);

    source = source_temp;

    expected_word = "";
    expected_word_len = 0;

    retval = s_get_word_s(&source_temp, word, len, &word_len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);

    STRCMP_EQUAL(expected_word, word);
    CHECK_EQUAL(expected_word_len, word_len);
    POINTERS_EQUAL(source+strlen(word), source_temp);
}

TEST(s_get_word_s, normal_two_word)
{
    source = "  abc   3456 ";
    source_temp = source;
    expected_word = "abc";
    expected_word_len = 3;
    retval = s_get_word_s(&source_temp, word, len, &word_len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);

    STRCMP_EQUAL(expected_word, word);
    CHECK_EQUAL(expected_word_len, word_len);
    POINTERS_EQUAL(source+ 2 + strlen(word) + 3, source_temp);

    expected_word = "3456";
    expected_word_len = 4;
    source = source_temp;

    retval = s_get_word_s(&source_temp, word, len, &word_len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);

    STRCMP_EQUAL(expected_word, word);
    CHECK_EQUAL(expected_word_len, word_len);
    POINTERS_EQUAL(source + strlen(word) + 1, source_temp);

}

