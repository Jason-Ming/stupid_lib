#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "s_defines.h"
#include "s_log.h"
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

    R_FALSE_RET(p_stack_temp->data_count > 0, RETURN_SUCCESS);

    STRU_STACK_DATA *p_stack_data_temp = p_stack_temp->p_data_list_head;
    
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

ENUM_RETURN stack_get_data_count(STACK p_stack, size_t *count)
{
    R_ASSERT(p_stack != NULL, RETURN_FAILURE);
    R_ASSERT(count != NULL, RETURN_FAILURE);

    STRU_STACK *p_stack_temp = (STRU_STACK*)(p_stack);
    *count = p_stack_temp->data_count;
    return RETURN_SUCCESS;
}

