#include "CppUTest/TestHarness.h"
#include <string.h>
#include "s_bits.h"
#include "s_type.h"

#include <iostream>

using namespace std;

TEST_GROUP(GET_BITS)
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

TEST(GET_BITS, 8bits)
{
    _UC c = 0x83;
    _UC result;
    result = GET_BITS(c, 0, 0);
    BYTES_EQUAL(0x00, result);

    c = 0x83;
    result = GET_BITS(c, 1, 0);
    BYTES_EQUAL(0x00, result);
    
    c = 0x83;
    result = GET_BITS(c, 2, 0);
    BYTES_EQUAL(0x00, result);
    
    c = 0x83;
    result = GET_BITS(c, 8, 0);
    BYTES_EQUAL(0x00, result);

    c = 0x83;
    result = GET_BITS(c, 0, 1);
    BYTES_EQUAL(0x01, result);

    c = 0x83;
    result = GET_BITS(c, 1, 1);
    BYTES_EQUAL(0x02, result);

    c = 0x83;
    result = GET_BITS(c, 2, 1);
    BYTES_EQUAL(0x00, result);

    c = 0x83;
    result = GET_BITS(c, 7, 1);
    BYTES_EQUAL(0x80, result);
    
    c = 0x83;
    result = GET_BITS(c, 8, 1);
    BYTES_EQUAL(0x00, result);

    c = 0x83;
    result = GET_BITS(c, 0, 2);
    BYTES_EQUAL(0x01, result);

    c = 0x83;
    result = GET_BITS(c, 1, 2);
    BYTES_EQUAL(0x03, result);

    c = 0x83;
    result = GET_BITS(c, 2, 2);
    BYTES_EQUAL(0x02, result);

    c = 0x83;
    result = GET_BITS(c, 7, 2);
    BYTES_EQUAL(0x80, result);

    c = 0x83;
    result = GET_BITS(c, 8, 2);
    BYTES_EQUAL(0x80, result);

    c = 0x83;
    result = GET_BITS(c, 0, 7);
    BYTES_EQUAL(0x01, result);

    c = 0x83;
    result = GET_BITS(c, 1, 7);
    BYTES_EQUAL(0x03, result);
    
    c = 0x83;
    result = GET_BITS(c, 2, 7);
    BYTES_EQUAL(0x03, result);

    c = 0x83;
    result = GET_BITS(c, 7, 7);
    BYTES_EQUAL(0x82, result);

    c = 0x83;
    result = GET_BITS(c, 8, 7);
    BYTES_EQUAL(0x80, result);

        c = 0x83;
    result = GET_BITS(c, 0, 8);
    BYTES_EQUAL(0x01, result);

    c = 0x83;
    result = GET_BITS(c, 1, 8);
    BYTES_EQUAL(0x03, result);

    c = 0x83;
    result = GET_BITS(c, 2, 8);
    BYTES_EQUAL(0x03, result);

    c = 0x83;
    result = GET_BITS(c, 7, 8);
    BYTES_EQUAL(0x83, result);

    c = 0x83;
    result = GET_BITS(c, 8, 8);
    BYTES_EQUAL(0x82, result);
    
    c = 0x83;
    result = GET_BITS(c, 9, 8);
    BYTES_EQUAL(0x80, result);

}

TEST(GET_BITS, 16bits)
{
    _US s = 0x837e;/* 1000 0011 0111 1110 */
    _US result;
    result = GET_BITS(s, 0, 0);
    UNSIGNED_LONGS_EQUAL(0x0000, result);

    s = 0x837e;
    result = GET_BITS(s, 1, 0);
    UNSIGNED_LONGS_EQUAL(0x0000, result);
    
    s = 0x837e;
    result = GET_BITS(s, 2, 0);
    UNSIGNED_LONGS_EQUAL(0x0000, result);
    
    s = 0x837e;
    result = GET_BITS(s, 8, 0);
    UNSIGNED_LONGS_EQUAL(0x0000, result);

    s = 0x837e;
    result = GET_BITS(s, 0, 1);
    UNSIGNED_LONGS_EQUAL(0x0000, result);

    s = 0x837e;
    result = GET_BITS(s, 1, 1);
    UNSIGNED_LONGS_EQUAL(0x0002, result);

    s = 0x837e;
    result = GET_BITS(s, 2, 1);
    UNSIGNED_LONGS_EQUAL(0x0004, result);

    s = 0x837e;
    result = GET_BITS(s, 7, 1);
    UNSIGNED_LONGS_EQUAL(0x0000, result);
    
    s = 0x837e;
    result = GET_BITS(s, 8, 1);
    UNSIGNED_LONGS_EQUAL(0x0100, result);

    s = 0x837e;
    result = GET_BITS(s, 0, 2);
    UNSIGNED_LONGS_EQUAL(0x0000, result);

    s = 0x837e;
    result = GET_BITS(s, 1, 2);
    UNSIGNED_LONGS_EQUAL(0x0002, result);

    s = 0x837e;
    result = GET_BITS(s, 2, 2);
    UNSIGNED_LONGS_EQUAL(0x0006, result);

    s = 0x837e;
    result = GET_BITS(s, 7, 2);
    UNSIGNED_LONGS_EQUAL(0x0040, result);

    s = 0x837e;
    result = GET_BITS(s, 8, 2);
    UNSIGNED_LONGS_EQUAL(0x0100, result);

    s = 0x837e;
    result = GET_BITS(s, 0, 7);
    UNSIGNED_LONGS_EQUAL(0x0000, result);

    s = 0x837e;
    result = GET_BITS(s, 1, 7);
    UNSIGNED_LONGS_EQUAL(0x0002, result);
    
    s = 0x837e;
    result = GET_BITS(s, 2, 7);
    UNSIGNED_LONGS_EQUAL(0x0006, result);

    s = 0x837e;
    result = GET_BITS(s, 7, 7);
    UNSIGNED_LONGS_EQUAL(0x007e, result);

    s = 0x837e;
    result = GET_BITS(s, 8, 7);
    UNSIGNED_LONGS_EQUAL(0x017c, result);

    s = 0x837e;
    result = GET_BITS(s, 0, 8);
    UNSIGNED_LONGS_EQUAL(0x0000, result);

    s = 0x837e;
    result = GET_BITS(s, 1, 8);
    UNSIGNED_LONGS_EQUAL(0x0002, result);

    s = 0x837e;
    result = GET_BITS(s, 2, 8);
    UNSIGNED_LONGS_EQUAL(0x0006, result);

    s = 0x837e;
    result = GET_BITS(s, 7, 8);
    UNSIGNED_LONGS_EQUAL(0x007e, result);

    s = 0x837e;
    result = GET_BITS(s, 8, 8);
    UNSIGNED_LONGS_EQUAL(0x017e, result);
    
    s = 0x837e;
    result = GET_BITS(s, 9, 8);
    UNSIGNED_LONGS_EQUAL(0x037c, result);
    
    s = 0x837e;
    result = GET_BITS(s, 15, 8);
    UNSIGNED_LONGS_EQUAL(0x8300, result);

    s = 0x837e;
    result = GET_BITS(s, 16, 8);
    UNSIGNED_LONGS_EQUAL(0x8200, result);

}


