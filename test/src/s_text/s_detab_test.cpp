#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"

#include <iostream>

using namespace std;

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


