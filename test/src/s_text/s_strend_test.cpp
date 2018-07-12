#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;

TEST_GROUP(s_strend)
{
    void setup()
    {
    	//设置自己的测试准备
        target = NULL;
        source = NULL;
        occur = BOOLEAN_FALSE;
        occur_expected = BOOLEAN_FALSE;
        retval = RETURN_FAILURE;
    }

    void teardown()
    {
        //清理测试设置
        //cout << "Test end ......" << endl;
    }

    ENUM_BOOLEAN occur_expected;
    ENUM_BOOLEAN occur;
    const _S8 *source;
    const _S8 *target;
    ENUM_RETURN retval;
};

TEST(s_strend, null_string1)
{
    target = "abc";
    retval = s_strend(source, target, &occur);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strend, null_string2)
{
    source = "abc";
    retval = s_strend(source, target, &occur);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strend, null_index)
{
    source = "abc";
    target = "abc";
    retval = s_strend(source, target, NULL);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(s_strend, completely_match)
{
    source = "abc";
    target = "abc";
    occur_expected = BOOLEAN_TRUE;
    retval = s_strend(source, target, &occur);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(occur_expected, occur);
}

TEST(s_strend, end_match)
{
    source = "some really long string.";
    target = "ng.";
    occur_expected = BOOLEAN_TRUE;
    retval = s_strend(source, target, &occur);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(occur_expected, occur);
}

TEST(s_strend, end_not_match)
{
    source = "some really long string.";
    target = "ng";
    occur_expected = BOOLEAN_FALSE;
    retval = s_strend(source, target, &occur);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(occur_expected, occur);
}

TEST(s_strend, source_is_empty)
{
    source = "";
    target = "ng";
    occur_expected = BOOLEAN_FALSE;
    retval = s_strend(source, target, &occur);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(occur_expected, occur);
}

TEST(s_strend, target_is_empty)
{
    source = "abc";
    target = "";
    occur_expected = BOOLEAN_FALSE;
    retval = s_strend(source, target, &occur);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(occur_expected, occur);
}

TEST(s_strend, both_are_empty)
{
    source = "";
    target = "";
    occur_expected = BOOLEAN_FALSE;
    retval = s_strend(source, target, &occur);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(occur_expected, occur);
}



