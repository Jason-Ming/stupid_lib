#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;

TEST_GROUP(s_squeeze)
{
    void setup()
    {
    	//设置自己的测试准备
        target = NULL;
        source[0] = '\0';
        source_expected = NULL;
        retval = RETURN_FAILURE;
    }

    void teardown()
    {
        //清理测试设置
        //cout << "Test end ......" << endl;
    }

    const _S8 *source_expected;
    const static size_t str_len = 256;
    _S8 source[str_len];
    const _S8 *target;
    ENUM_RETURN retval;
};

TEST(s_squeeze, null_string1)
{
    target = "abc";
    retval = s_squeeze(NULL, target);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_squeeze, null_string2)
{
    strcpy(source, "abc");
    retval = s_squeeze(source, target);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_squeeze, completely_match)
{
    strcpy(source, "abc");
    target = "abc";
    source_expected = "";
    retval = s_squeeze(source, target);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(source_expected, source);
}

TEST(s_squeeze, end_match)
{
    strcpy(source, "some really long string.");
    target = "ng.";
    source_expected = "some really lo stri";
    retval = s_squeeze(source, target);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(source_expected, source);
}

TEST(s_squeeze, end_not_match)
{
    strcpy(source, "some really long string.");
    target = "ng";
    source_expected = "some really lo stri.";
    retval = s_squeeze(source, target);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(source_expected, source);
}

TEST(s_squeeze, source_is_empty)
{
    target = "ng";
    source_expected = "";
    retval = s_squeeze(source, target);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(source_expected, source);
}

TEST(s_squeeze, target_is_empty)
{
    strcpy(source, "abc");
    target = "";
    source_expected = "abc";
    retval = s_squeeze(source, target);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(source_expected, source);
}

TEST(s_squeeze, both_are_empty)
{
    target = "";
    source_expected = "";
    retval = s_squeeze(source, target);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    STRCMP_EQUAL(source_expected, source);
}





