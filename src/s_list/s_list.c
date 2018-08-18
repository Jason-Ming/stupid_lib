#include <stdio.h>

#if 0
typedef void* LIST;
typedef void* NODE;
typedef void* KEY;
typedef ENUM_BOOLEAN (*FUNC_LIST_NODE_COMPARE)(KEY key, NODE node);
typedef ENUM_RETURN (*FUNC_LIST_NODE_FREE)(NODE node);
typedef struct TAG_STRU_LIST_NODE
{
    NODE node;
    struct TAG_STRU_LIST_NODE *next;
    struct TAG_STRU_LIST_NODE *previous;
}STRU_LIST_NODE;

typedef struct TAG_STRU_LIST
{
    FUNC_LIST_NODE_FREE free_handler;
    FUNC_LIST_NODE_COMPARE compare_handler;
    STRU_LIST_NODE *p_list_node_head;
    STRU_LIST_NODE *p_list_node_tail;
}STRU_LIST;

ENUM_RETURN s_list_create(
    LIST *p_list, 
    FUNC_LIST_NODE_COMPARE compare_handler,
    FUNC_LIST_NODE_FREE free_handler)
{
    S_R_ASSERT(p_list != NULL, RETURN_FAILURE);
    S_R_ASSERT(compare_handler != NULL, RETURN_FAILURE);
    S_R_ASSERT(free_handler != NULL, RETURN_FAILURE);
    
    STRU_LIST p_list_temp = (STRU_LIST*)malloc(sizeof(STRU_LIST));
    S_R_ASSERT(p_list_temp != NULL, RETURN_FAILURE);
    p_list_temp->p_list_node_head = NULL;
    p_list_temp->p_list_node_tail = NULL;
    p_list_temp->compare_handler = compare_handler;
    p_list_temp->free_handler = free_handler;

    *p_list = (LIST)p_list_temp;
    return RETURN_SUCCESS;
}

ENUM_RETURN s_list_delete(LIST *p_list)
{
    S_R_ASSERT(p_list != NULL, RETURN_FAILURE);
    STRU_LIST p_list_temp = (STRU_LIST*)(*p_list);
    
    STRU_LIST_NODE *p_list_node_head = p_list_temp->p_list_node_head;
    STRU_LIST_NODE *p_list_node_temp;
    
    while(p_list_node_head != NULL)
    {
        p_list_node_temp = p_list_node_head;
        p_list_node_head = p_list_node_head->next;
        p_list_temp->free_handler(p_list_node_temp->node);
    };

    FREE(*p_list);

    return RETURN_SUCCESS;
}

ENUM_RETURN s_list_add_node_to_list(
    LIST list,
    NODE add_after, 
    NODE new)
{
    S_R_ASSERT(list != NULL, RETURN_FAILURE);
    S_R_ASSERT(list != NULL, RETURN_FAILURE);
    STRU_LIST p_list_temp = (STRU_LIST*)(list);

    STRU_LIST_NODE *p_list_node_head = p_list_temp->p_list_node_head;
    STRU_LIST_NODE *p_list_node_tail = p_list_temp->p_list_node_tail;

    if()
    if(*pp_token_list_head == NULL)
    {
        R_ASSERT(*pp_token_list_tail == NULL, RETURN_FAILURE);
        
        *pp_token_list_head = p_new_token_node;
        *pp_token_list_tail = p_new_token_node;
    }
    else
    {
        R_ASSERT(*pp_token_list_tail != NULL, RETURN_FAILURE);

        p_new_token_node->previous = *pp_token_list_tail;
        (*pp_token_list_tail)->next = p_new_token_node;
        *pp_token_list_tail = p_new_token_node;
    }
    
}
ENUM_RETURN s_list_delete_node_from_list(LIST list, NODE delete_after);
NODE s_list_get_node_by_key(LIST list, KEY key);
NODE s_list_get_head(LIST list);
NODE s_list_get_tail(LIST list);
#endif