#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_type.h"

#include <iostream>

using namespace std;

TEST_GROUP(TYPE)
{
    void setup()
    {
    	//�����Լ��Ĳ���׼��
        //cout << "Test start ......" << endl;
    }

    void teardown()
    {
    	//�����������
        //cout << "Test end ......" << endl;
    }
};

TEST(TYPE, test0)
{
    //s_print_types();
}

