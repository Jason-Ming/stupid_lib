#ifndef __S_STACK_H__
#define __S_STACK_H__

#include "s_clinkage.h"
#include "s_defines.h"

typedef void* STACK;

__BEGIN_C_DECLS
ENUM_RETURN stack_create(STACK *p_stack);
ENUM_RETURN stack_delete(STACK *p_stack);
ENUM_RETURN stack_push(STACK *p_stack, void * p_data, unsigned int data_size);
ENUM_RETURN stack_pop(STACK *p_stack, void * p_data, unsigned int *size_out, unsigned int size_in);

__END_C_DECLS

#endif

