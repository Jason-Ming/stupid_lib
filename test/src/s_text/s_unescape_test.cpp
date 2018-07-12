#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;



TEST_GROUP(s_unescape)
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

TEST(s_unescape, null_string1)
{
    retval = s_unescape(NULL, s2, len);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_unescape, null_string2)
{
    s1 = "a-z";
    retval = s_unescape(s1, NULL, len);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_unescape, source_is_empty)
{
    s1 = "";
    expected_s = "";
    retval = s_unescape(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_unescape, source_is_normal1)
{
    s1 = "\aHello,\n\tWorld! Mistakee\b was \"Extra'e'\"!\n";
    expected_s = "\aHello,\n\tWorld! Mistakee\b was \"Extra'e'\"!\n";
    retval = s_unescape(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}

TEST(s_unescape, source_is_normal2)
{
    s1 = "\\aHello,\\n\\tWorld! Mistakee\\b was \\\"Extra\\\'e\\\'\\\"!\\n";
    expected_s = "\aHello,\n\tWorld! Mistakee\b was \"Extra'e'\"!\n";
    retval = s_unescape(s1, s2, len);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(expected_s, s2);
}


