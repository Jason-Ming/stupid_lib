#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;

TEST_GROUP(s_any)
{
    void setup()
    {
    	//设置自己的测试准备
        target = NULL;
        source = NULL;
        p_occur = NULL;
        p_occur_expected = NULL;
        retval = RETURN_FAILURE;
    }

    void teardown()
    {
        //清理测试设置
        //cout << "Test end ......" << endl;
    }

    const _S8 *p_occur_expected;
    const _S8 *p_occur;
    const _S8 *source;
    const _S8 *target;
    ENUM_RETURN retval;
};

TEST(s_any, null_string1)
{
    target = "abc";
    retval = s_any(source, target, &p_occur);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_any, null_string2)
{
    source = "abc";
    retval = s_any(source, target, &p_occur);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_any, null_p_occur)
{
    source = "abc";
    target = "abc";
    retval = s_any(source, target, NULL);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_any, completely_match)
{
    source = "abc";
    target = "abc";
    p_occur_expected = source;
    retval = s_any(source, target, &p_occur);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(p_occur_expected, p_occur);
}

TEST(s_any, end_match)
{
    source = "some really long string.";
    target = "ng.";
    p_occur_expected = source + 14;
    retval = s_any(source, target, &p_occur);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(p_occur_expected, p_occur);
}

TEST(s_any, end_not_match)
{
    source = "some really long string.";
    target = "ng";
    p_occur_expected = source + 14;
    retval = s_any(source, target, &p_occur);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(p_occur_expected, p_occur);
}

TEST(s_any, source_is_empty)
{
    source = "";
    target = "ng";
    p_occur_expected = NULL;
    retval = s_any(source, target, &p_occur);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(p_occur_expected, p_occur);
}

TEST(s_any, target_is_empty)
{
    source = "abc";
    target = "";
    p_occur_expected = NULL;
    retval = s_any(source, target, &p_occur);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(p_occur_expected, p_occur);
}

TEST(s_any, both_are_empty)
{
    source = "";
    target = "";
    p_occur_expected = NULL;
    retval = s_any(source, target, &p_occur);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(p_occur_expected, p_occur);
}




