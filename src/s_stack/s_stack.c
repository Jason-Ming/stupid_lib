#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "s_defines.h"
#include "s_mem.h"
#include "s_log.h"
#include "s_alg.h"

#include "s_stack.h"

typedef struct TAG_STRU_STACK_DATA
{
    _VOID *p_data;
    size_t data_size;
    struct TAG_STRU_STACK_DATA *next;
}STRU_STACK_DATA;

typedef struct TAG_STRU_STACK
{
    STRU_STACK_DATA *p_data_list_head;
    size_t data_count;
}STRU_STACK;

_VOID stack_print(STACK p_stack)
{
    V_ASSERT(p_stack != NULL);
    
    STRU_STACK *p_stack_temp = (STRU_STACK*)p_stack;

    printf("stack addr: %p\n", p_stack_temp);
    printf("  elements: %zd\n", p_stack_temp->data_count);
    
    STRU_STACK_DATA *p_head;
    p_head = p_stack_temp->p_data_list_head;

    while(p_head != NULL)
    {
        printf("  ---->>\n");
        printf("    element addr: %p\n", p_head);
        printf("       data addr: %p\n", p_head->p_data);
        printf("       data size: %zd\n", p_head->data_size);
        printf("       next addr: %p\n", p_head->next);
        display_mem(p_head->p_data, p_head->data_size, BOOLEAN_FALSE);
        
        p_head = p_head->next;
    };
}

ENUM_RETURN stack_create(STACK *p_stack)
{
    R_ASSERT(p_stack != NULL, RETURN_FAILURE);
    STRU_STACK *p_stack_temp = (STRU_STACK*)malloc(sizeof(STRU_STACK));
    R_ASSERT(p_stack_temp != NULL, RETURN_FAILURE);
    p_stack_temp->data_count = 0;
    p_stack_temp->p_data_list_head = NULL;
    *p_stack = (STACK)p_stack_temp;
    
    return RETURN_SUCCESS;
}

ENUM_RETURN stack_delete(STACK *p_stack)
{
    R_ASSERT(p_stack != NULL, RETURN_FAILURE);
    R_ASSERT(*p_stack != NULL, RETURN_FAILURE);

    STRU_STACK *p_stack_temp = (STRU_STACK*)*p_stack;

    STRU_STACK_DATA *p_head, *p_temp;
    p_head = p_temp= p_stack_temp->p_data_list_head;

    while(p_head != NULL)
    {
        p_temp = p_head;
        p_head = p_head->next;
        FREE(p_temp->p_data);
        FREE(p_temp);
    };
    p_stack_temp->p_data_list_head = NULL;
    
    FREE(p_stack_temp);
    
    *p_stack = NULL;
    
    return RETURN_SUCCESS;
}

ENUM_RETURN stack_push(STACK p_stack, _VOID * p_data, size_t data_size)
{
    R_ASSERT(p_stack != NULL, RETURN_FAILURE);
    R_ASSERT(p_data != NULL, RETURN_FAILURE);
    R_ASSERT(data_size != 0, RETURN_FAILURE);

    STRU_STACK *p_stack_temp = (STRU_STACK*)p_stack;
    
    _VOID * p_data_temp = malloc(data_size);
    R_ASSERT(p_data_temp != NULL, RETURN_FAILURE);
    memcpy(p_data_temp, p_data, data_size);
    
    STRU_STACK_DATA *p_stack_data_temp = (STRU_STACK_DATA*)malloc(sizeof(STRU_STACK_DATA));
    R_ASSERT_DO(p_stack_data_temp != NULL, RETURN_FAILURE, FREE(p_data_temp););

    p_stack_data_temp->p_data = p_data_temp;
    p_stack_data_temp->data_size = data_size;
    p_stack_data_temp->next = p_stack_temp->p_data_list_head;
    p_stack_temp->p_data_list_head = p_stack_data_temp;

    p_stack_temp->data_count++;
    
    return RETURN_SUCCESS;
}
ENUM_RETURN stack_pop(STACK p_stack, _VOID * p_data, size_t *size_out, size_t size_in)
{
    R_ASSERT(p_stack != NULL, RETURN_FAILURE);
    R_ASSERT(p_data != NULL, RETURN_FAILURE);
    R_ASSERT(size_out != NULL, RETURN_FAILURE);

    *size_out = 0;
    STRU_STACK *p_stack_temp = (STRU_STACK*)p_stack;

    R_ASSERT(p_stack_temp->data_count > 0, RETURN_FAILURE);

    STRU_STACK_DATA *p_stack_data_temp = p_stack_temp->p_data_list_head;
    R_ASSERT(p_stack_data_temp != NULL, RETURN_FAILURE);
    R_ASSERT(p_stack_data_temp->p_data != NULL, RETURN_FAILURE);
    
    *size_out = MIN(size_in, p_stack_data_temp->data_size);
    memcpy(p_data, p_stack_data_temp->p_data, *size_out);

    FREE(p_stack_data_temp->p_data);
    p_stack_data_temp->data_size = 0;

    p_stack_temp->p_data_list_head = p_stack_data_temp->next;
    FREE(p_stack_data_temp);
    p_stack_temp->data_count--;

    return RETURN_SUCCESS;
}

ENUM_RETURN stack_get_top(STACK p_stack, _VOID * p_data, size_t *size_out, size_t size_in)
{
    R_ASSERT(p_stack != NULL, RETURN_FAILURE);
    R_ASSERT(p_data != NULL, RETURN_FAILURE);
    R_ASSERT(size_out != NULL, RETURN_FAILURE);

    *size_out = 0;
    STRU_STACK *p_stack_temp = (STRU_STACK*)p_stack;

    R_ASSERT(p_stack_temp->data_count > 0, RETURN_FAILURE);

    STRU_STACK_DATA *p_stack_data_temp = p_stack_temp->p_data_list_head;
    R_ASSERT(p_stack_data_temp != NULL, RETURN_FAILURE);
    R_ASSERT(p_stack_data_temp->p_data != NULL, RETURN_FAILURE);
    
    *size_out = MIN(size_in, p_stack_data_temp->data_size);
    memcpy(p_data, p_stack_data_temp->p_data, *size_out);

    return RETURN_SUCCESS;
}

ENUM_RETURN stack_duplicate_top(STACK p_stack)
{
    R_ASSERT(p_stack != NULL, RETURN_FAILURE);
    STRU_STACK *p_stack_temp = (STRU_STACK*)p_stack;
    R_ASSERT(p_stack_temp->data_count > 0, RETURN_FAILURE);

    ENUM_RETURN ret_val = stack_push(p_stack, p_stack_temp->p_data_list_head->p_data, p_stack_temp->p_data_list_head->data_size);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

ENUM_RETURN stack_swap_top2(STACK p_stack)
{
    R_ASSERT(p_stack != NULL, RETURN_FAILURE);
    STRU_STACK *p_stack_temp = (STRU_STACK*)p_stack;
    R_ASSERT(p_stack_temp->data_count >= 2, RETURN_FAILURE);

    STRU_STACK_DATA *p_stack_data_temp1 = p_stack_temp->p_data_list_head;
    R_ASSERT(p_stack_data_temp1 != NULL, RETURN_FAILURE);
    
    STRU_STACK_DATA *p_stack_data_temp2 = p_stack_temp->p_data_list_head->next;
    R_ASSERT(p_stack_data_temp2 != NULL, RETURN_FAILURE);

    p_stack_data_temp1->next = p_stack_data_temp2->next;
    p_stack_data_temp2->next = p_stack_data_temp1;
    p_stack_temp->p_data_list_head = p_stack_data_temp2;

    return RETURN_SUCCESS;
}

ENUM_RETURN stack_clear(STACK p_stack)
{
    R_ASSERT(p_stack != NULL, RETURN_FAILURE);
    STRU_STACK *p_stack_temp = (STRU_STACK*)p_stack;
    S_R_FALSE(p_stack_temp->data_count > 0, RETURN_SUCCESS);

    R_ASSERT(p_stack_temp->p_data_list_head != NULL, RETURN_FAILURE);
    STRU_STACK_DATA *p_head, *p_temp;
    p_head = p_temp= p_stack_temp->p_data_list_head;

    while(p_head != NULL)
    {
        p_temp = p_head;
        p_head = p_head->next;
        FREE(p_temp->p_data);
        FREE(p_temp);
    };
    p_stack_temp->p_data_list_head = NULL;
    p_stack_temp->data_count = 0;

    return RETURN_SUCCESS;
}

ENUM_RETURN stack_get_element_count(STACK p_stack, size_t *count)
{
    R_ASSERT(p_stack != NULL, RETURN_FAILURE);
    R_ASSERT(count != NULL, RETURN_FAILURE);

    STRU_STACK *p_stack_temp = (STRU_STACK*)(p_stack);
    *count = p_stack_temp->data_count;
    return RETURN_SUCCESS;
}

ENUM_BOOLEAN stack_is_empty(STACK p_stack)
{
    R_ASSERT(p_stack != NULL, BOOLEAN_TRUE);

    STRU_STACK *p_stack_temp = (STRU_STACK*)(p_stack);
    if(p_stack_temp->data_count > 0)
    {
        return BOOLEAN_FALSE;
    }

    return BOOLEAN_TRUE;
}

