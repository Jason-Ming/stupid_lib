#include "CppUTest/TestHarness.h"
#include <string.h>
#include <math.h>
#include "s_stack.h"
#include "s_type.h"
#include "s_mem.h"
#include "s_limits.h"

#include <iostream>

using namespace std;

TEST_GROUP(s_stack)
{
    _VOID setup()
    {
        printf("setup...\n");
    	//设置自己的测试准备
        stack = NULL;
        expected_value = 0;
        value = 0;
        data_count = 0;
        retval = RETURN_FAILURE;
        
        retval = stack_create(&stack);
        CHECK_EQUAL(RETURN_SUCCESS, retval);
    }

    _VOID teardown()
    {
        //清理测试设置
        printf("teardown...\n");
        retval = stack_delete(&stack);
        CHECK_EQUAL(RETURN_SUCCESS, retval);
    }

    _VOID push(_S32 value, ENUM_RETURN expected_retval)
    {
        data_count = count();
        
        retval = stack_push(stack, (_VOID*)&value, sizeof(value));
        CHECK_EQUAL(expected_retval, retval);

        CHECK_EQUAL(data_count + 1, count());
    }

    _S32 pop(ENUM_RETURN expected_retval)
    {
        _S32 value;
        size_t len;
        data_count = count();
        retval = stack_pop(stack, (_VOID*)&value, &len, sizeof(value));
        CHECK_EQUAL(expected_retval, retval);
        if(expected_retval == RETURN_SUCCESS)
        {
            CHECK_EQUAL(data_count - 1, count());
        }
        else
        {
            CHECK_EQUAL(data_count, count());
        }
        
        return value;
    }

    _S32 top(ENUM_RETURN expected_retval)
    {
        _S32 value;
        size_t len;
        data_count = count();
        retval = stack_get_top(stack, (_VOID*)&value, &len, sizeof(value));
        CHECK_EQUAL(expected_retval, retval);

        CHECK_EQUAL(data_count, count());
        
        return value;
    }

    _S32 duplicate(ENUM_RETURN expected_retval)
    {
        data_count = count();
        retval = stack_duplicate_top(stack);
        CHECK_EQUAL(expected_retval, retval);
        if(expected_retval == RETURN_SUCCESS)
        {
            CHECK_EQUAL(data_count + 1, count());
            value = top(RETURN_SUCCESS);
        }
        else
        {
            CHECK_EQUAL(data_count, count());
        }
        
        return value; 
    }

    _VOID swap(ENUM_RETURN expected_retval)
    {
        retval = stack_swap_top2(stack);
        CHECK_EQUAL(expected_retval, retval);
    }

    size_t count(_VOID)
    {
        size_t count;
        retval = stack_get_element_count(stack, &count);
        CHECK_EQUAL(RETURN_SUCCESS, retval);

        return count;
    }

    _VOID clear(_VOID)
    {
        stack_print(stack);
        retval = stack_clear(stack);
        CHECK_EQUAL(RETURN_SUCCESS, retval);

        CHECK_EQUAL(0, count());
        stack_print(stack);
    }
    
    STACK stack;
    _S32 expected_value;
    _S32 value;
    size_t data_count;
    ENUM_RETURN retval;
};

TEST(s_stack, push)
{
    value = 1349;
    push(value, RETURN_SUCCESS);
}

TEST(s_stack, pop_after_push)
{
    value = expected_value = 1349;
    push(value, RETURN_SUCCESS);

    value = pop(RETURN_SUCCESS);

    CHECK_EQUAL(expected_value, value)
}

TEST(s_stack, pop_empty_stack)
{
    value = pop(RETURN_FAILURE);
}

TEST(s_stack, top_after_push)
{
    value = expected_value = 1349;
    push(value, RETURN_SUCCESS);

    value = top(RETURN_SUCCESS);

    CHECK_EQUAL(expected_value, value)
}

TEST(s_stack, top_empty_stack)
{
    value = top(RETURN_FAILURE);
}

TEST(s_stack, duplicate_after_push)
{
    value = expected_value = 1349;
    push(value, RETURN_SUCCESS);

    value = duplicate(RETURN_SUCCESS);

    CHECK_EQUAL(expected_value, value)
}

TEST(s_stack, duplicate_empty_stack)
{
    value = duplicate(RETURN_FAILURE);
}

TEST(s_stack, swap_after_push)
{
    _S32 value1 = 1349;
    push(value1, RETURN_SUCCESS);

    _S32 value2 = 2348;
    push(value2, RETURN_SUCCESS);
    
    swap(RETURN_SUCCESS);

    expected_value = value1;
    value = pop(RETURN_SUCCESS);
    CHECK_EQUAL(expected_value, value);

    expected_value = value2;
    value = pop(RETURN_SUCCESS);
    CHECK_EQUAL(expected_value, value);
}

TEST(s_stack, swap_empty_stack)
{
    swap(RETURN_FAILURE);
}

TEST(s_stack, swap_1_element_stack)
{
    _S32 value1 = 1349;
    push(value1, RETURN_SUCCESS);
    
    swap(RETURN_FAILURE);
}

TEST(s_stack, clear_after_push)
{
    _S32 value1 = 1349;
    push(value1, RETURN_SUCCESS);

    _S32 value2 = 2348;
    push(value2, RETURN_SUCCESS);

    clear();
}

TEST(s_stack, clear_empty_stack)
{
    clear();
}


