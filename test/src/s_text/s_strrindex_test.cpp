#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;

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

TEST(s_strrindex, both_are_empty)
{
    source = "";
    target = "";
    index_expected = -1;
    retval = s_strrindex(source, target, &index);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(index_expected, index);
}

TEST(s_strrindex, source_is_empty)
{
    source = "";
    target = "abc";
    index_expected = -1;
    retval = s_strrindex(source, target, &index);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(index_expected, index);
}

TEST(s_strrindex, target_is_empty)
{
    source = "abc";
    target = "";
    index_expected = -1;
    retval = s_strrindex(source, target, &index);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(index_expected, index);
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


