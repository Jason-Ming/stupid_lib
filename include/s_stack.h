#ifndef __S_STACK_H__
#define __S_STACK_H__

#include "s_clinkage.h"
#include "s_defines.h"
#include "s_type.h"

typedef void* STACK;

__BEGIN_C_DECLS

_VOID stack_print(STACK p_stack);
ENUM_RETURN stack_create(STACK *p_stack);
ENUM_RETURN stack_delete(STACK *p_stack);
ENUM_RETURN stack_push(STACK p_stack, _VOID * p_data, size_t data_size);
ENUM_RETURN stack_pop(STACK p_stack, _VOID * p_data, size_t *size_out, size_t size_in);
ENUM_RETURN stack_get_top(STACK p_stack, _VOID * p_data, size_t *size_out, size_t size_in);
ENUM_RETURN stack_duplicate_top(STACK p_stack);
ENUM_RETURN stack_swap_top2(STACK p_stack);
ENUM_RETURN stack_clear(STACK p_stack);

ENUM_RETURN stack_get_element_count(STACK p_stack, size_t *count);

__END_C_DECLS

#endif

