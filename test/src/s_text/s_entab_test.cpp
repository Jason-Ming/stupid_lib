#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;


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


