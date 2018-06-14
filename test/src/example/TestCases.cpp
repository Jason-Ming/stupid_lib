#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_text.h"

#include <iostream>

using namespace std;

TEST_GROUP(TEXT)
{
    char *p = NULL;
    void setup()
    {
        p = (char*)malloc(100);
    	//�����Լ��Ĳ���׼��
        //cout << "Test start ......" << endl;
    }

    void teardown()
    {
        free(p);
    	//�����������
        //cout << "Test end ......" << endl;
    }
};

TEST(TEXT, test0)
{
    char s1[] = "abcd 123";
    char s2[] = "cd 1";
    char s[] = "ab23";
    squeeze(s1, s2);
    CHECK_EQUAL(strcmp(s1, s), 0);
}

TEST(TEXT, test1)
{
    char s1[] = "abcd 123";
    char s2[] = "cd 1";
    char s[] = "cd 123";
    char *p = any(s1, s2);
    CHECK_EQUAL(strcmp(p, s), 0);
}

