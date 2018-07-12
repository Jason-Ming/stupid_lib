#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;



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

