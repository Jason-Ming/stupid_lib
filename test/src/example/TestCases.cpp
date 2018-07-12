#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_type.h"

#include <iostream>

using namespace std;

TEST_GROUP(TYPE)
{
    void setup()
    {
    	//设置自己的测试准备
        //cout << "Test start ......" << endl;
    }

    void teardown()
    {
    	//清理测试设置
        //cout << "Test end ......" << endl;
    }
};

TEST(TYPE, test0)
{
    s_print_types();
}

