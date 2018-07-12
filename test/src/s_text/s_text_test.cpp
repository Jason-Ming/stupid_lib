#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;

TEST_GROUP(xxx)
{
    void setup()
    {
    	//�����Լ��Ĳ���׼��
        target = NULL;
        source = NULL;
        index = 0;
        index_expected = 0;
        retval = RETURN_FAILURE;
    }

    void teardown()
    {
        //�����������
        //cout << "Test end ......" << endl;
    }

    _S32 index_expected;
    _S32 index;
    const _S8 *source;
    const _S8 *target;
    ENUM_RETURN retval;
};

TEST(xxx, null_string1)
{
    target = "abc";
    retval = s_strrindex(source, target, &index);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(xxx, null_string2)
{
    source = "abc";
    retval = s_strrindex(source, target, &index);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(xxx, null_index)
{
    source = "abc";
    target = "abc";
    retval = s_strrindex(source, target, NULL);

    CHECK_EQUAL(RETURN_FAILURE, retval);
}

TEST(xxx, completely_match)
{
    source = "abc";
    target = "abc";
    index_expected = 0;
    retval = s_strrindex(source, target, &index);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(index_expected, index);
}

TEST(xxx, not_match)
{
    source = "abbc";
    target = "abc";
    index_expected = -1;
    retval = s_strrindex(source, target, &index);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(index_expected, index);
}

TEST(xxx, one_match)
{
    source = "abbcabcab";
    target = "abc";
    index_expected = 4;
    retval = s_strrindex(source, target, &index);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(index_expected, index);
}

TEST(xxx, multi_match)
{
    source = "abbcabcabcc";
    target = "abc";
    index_expected = 7;
    retval = s_strrindex(source, target, &index);

    CHECK_EQUAL(RETURN_SUCCESS, retval);
    CHECK_EQUAL(index_expected, index);
}

