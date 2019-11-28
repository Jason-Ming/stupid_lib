#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "s_defines.h"
#include "s_limits.h"
#include "s_mem.h"
#include "s_text.h"
#include "s_cmd.h"
#include "s_log.h"

#include "s_bintree.h"


typedef struct TAG_STRU_BINTREE_NODE
{
    struct TAG_STRU_BINTREE_NODE *p_left;
    struct TAG_STRU_BINTREE_NODE *p_right;
    void *p_data_container;
}STRU_BINTREE_NODE;

typedef struct TAG_STRU_BINTREE
{
    STRU_BINTREE_NODE *p_root;
    BINTREE_DATA_ALLOC_PROC data_alloc_handler;
    BINTREE_DATA_FREE_PROC data_free_handler;
    BINTREE_DATA_COMPARE_PROC data_compare_handler;
    BINTREE_DATA_UPDATE_PROC data_update_handler;
    BINTREE_DATA_PRINT_PROC data_print_handler;
}STRU_BINTREE;

PRIVATE ENUM_RETURN bintree_free_node(STRU_BINTREE *p_bintree, STRU_BINTREE_NODE **pp_node)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    //S_R_ASSERT(p_bintree != NULL, RETURN_FAILURE);
   // S_R_FALSE(pp_node != NULL, RETURN_SUCCESS);
    //S_R_FALSE(*pp_node != NULL, RETURN_SUCCESS);

    if((*pp_node)->p_left != NULL)
    {
        ret_val = bintree_free_node(p_bintree, &((*pp_node)->p_left));
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }

    if((*pp_node)->p_right != NULL)
    {
        ret_val = bintree_free_node(p_bintree, &((*pp_node)->p_right));
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }

    ret_val = p_bintree->data_free_handler(&((*pp_node)->p_data_container));
    //S_FREE((*pp_node)->p_data);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    S_FREE(*pp_node);
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN bintree_print_node(STRU_BINTREE *p_bintree, STRU_BINTREE_NODE *p_node)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    //S_R_ASSERT(p_bintree != NULL, RETURN_FAILURE);
    //S_R_FALSE(p_node != NULL, RETURN_SUCCESS);

    if(p_node->p_left != NULL)
    {
        ret_val = bintree_print_node(p_bintree, p_node->p_left);
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }
    
    ret_val = p_bintree->data_print_handler(p_node->p_data_container);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    if(p_node->p_right != NULL)
    {
        ret_val = bintree_print_node(p_bintree, p_node->p_right);
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN bintree_find_node(STRU_BINTREE *p_bintree, 
    STRU_BINTREE_NODE **pp_current_node, 
    STRU_BINTREE_NODE ***ppp_dest_node, 
    void *p_data_new)
{
    //S_R_ASSERT(p_bintree != NULL, RETURN_FAILURE);
    //S_R_ASSERT(ppp_dest_node != NULL, RETURN_FAILURE);
    //S_R_ASSERT(p_data_new != NULL, RETURN_FAILURE);
    
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    ENUM_BINTREE_DATA_COMPARE_RESULT compare_result = ENUM_BINTREE_DATA_COMPARE_EQUAL;

    *ppp_dest_node = pp_current_node;
    S_R_FALSE(*pp_current_node != NULL, RETURN_SUCCESS);

    //S_R_ASSERT((*pp_current_node)->p_data_container != NULL, RETURN_FAILURE);
    compare_result = p_bintree->data_compare_handler((*pp_current_node)->p_data_container, p_data_new);

    switch(compare_result)
    {
        case ENUM_BINTREE_DATA_COMPARE_BIG:
        {
            ret_val = bintree_find_node(p_bintree, &((*pp_current_node)->p_left), ppp_dest_node, p_data_new);
            S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }
        case ENUM_BINTREE_DATA_COMPARE_EQUAL:
        {
            break;
        }
        case ENUM_BINTREE_DATA_COMPARE_SMALL:
        {
            ret_val = bintree_find_node(p_bintree, &((*pp_current_node)->p_right), ppp_dest_node, p_data_new);
            S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
            break;
        }
        default:
        {
            S_LOG("compare: %d result out of range!", compare_result);
            return RETURN_FAILURE;
        }
    }

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN bintree_insert_node(STRU_BINTREE *p_bintree, STRU_BINTREE_NODE **pp_node, void *p_data_new)
{
    //S_R_ASSERT(p_bintree != NULL, RETURN_FAILURE);
    //S_R_ASSERT(pp_node != NULL, RETURN_FAILURE);
    //S_R_ASSERT(*pp_node == NULL, RETURN_FAILURE);
    
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    STRU_BINTREE_NODE *p_node = (STRU_BINTREE_NODE*)malloc(sizeof(STRU_BINTREE_NODE));
    S_R_ASSERT(p_node != NULL, RETURN_FAILURE);

    p_node->p_left = NULL;
    p_node->p_right = NULL;
    p_node->p_data_container = NULL;
    ret_val = p_bintree->data_alloc_handler(&(p_node->p_data_container), p_data_new);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    S_R_ASSERT(p_node->p_data_container != NULL, RETURN_FAILURE);

    *pp_node = p_node;
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN bintree_update_node(STRU_BINTREE *p_bintree, STRU_BINTREE_NODE *p_current_node, void *p_data_new)
{
    //S_R_ASSERT(p_bintree != NULL, RETURN_FAILURE);
    //S_R_ASSERT(p_current_node != NULL, RETURN_FAILURE);

    ENUM_RETURN ret_val = RETURN_SUCCESS;

    ret_val = p_bintree->data_update_handler(p_current_node->p_data_container, p_data_new);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

ENUM_RETURN bintree_create(BINTREE *p_bintree, 
    BINTREE_DATA_ALLOC_PROC data_alloc_handler,
    BINTREE_DATA_FREE_PROC data_free_handler,
    BINTREE_DATA_COMPARE_PROC data_compare_handler,
    BINTREE_DATA_UPDATE_PROC data_update_handler,
    BINTREE_DATA_PRINT_PROC data_print_handler)
{
    S_R_ASSERT(p_bintree != NULL, RETURN_FAILURE);
    S_R_ASSERT(data_alloc_handler != NULL, RETURN_FAILURE);
    S_R_ASSERT(data_free_handler != NULL, RETURN_FAILURE);
    S_R_ASSERT(data_compare_handler != NULL, RETURN_FAILURE);
    S_R_ASSERT(data_update_handler != NULL, RETURN_FAILURE);
    S_R_ASSERT(data_print_handler != NULL, RETURN_FAILURE);
    
    *p_bintree = NULL;
    
    STRU_BINTREE *p_bintree_temp = NULL;

    p_bintree_temp = (STRU_BINTREE*)malloc(sizeof(STRU_BINTREE));
    S_R_ASSERT(p_bintree_temp != NULL, RETURN_FAILURE);

    p_bintree_temp->p_root = NULL;
    p_bintree_temp->data_alloc_handler = data_alloc_handler;
    p_bintree_temp->data_free_handler = data_free_handler;
    p_bintree_temp->data_compare_handler = data_compare_handler;
    p_bintree_temp->data_print_handler = data_print_handler;
    p_bintree_temp->data_update_handler = data_update_handler;

    *p_bintree = p_bintree_temp;

    DEBUG_PRINT("bintree_create: %p", p_bintree_temp);
    
    return RETURN_SUCCESS;
}


ENUM_RETURN bintree_print(BINTREE *p_bintree)
{
    S_R_ASSERT(p_bintree != NULL, RETURN_FAILURE);
    S_R_ASSERT(*p_bintree != NULL, RETURN_FAILURE);
    STRU_BINTREE *p_bintree_temp = (STRU_BINTREE*)*p_bintree;
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    
    if(p_bintree_temp->p_root != NULL)
    {
        DEBUG_PRINT("printing bintree...\n\n");
        ret_val = bintree_print_node(p_bintree_temp, p_bintree_temp->p_root);
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }
    else
    {
        DEBUG_PRINT("bin tree is empty.\n");
    }

    return RETURN_SUCCESS;
}

ENUM_RETURN bintree_delete(BINTREE *p_bintree)
{
    S_R_ASSERT(p_bintree != NULL, RETURN_FAILURE);
    S_R_ASSERT(*p_bintree != NULL, RETURN_FAILURE);
    STRU_BINTREE *p_bintree_temp = (STRU_BINTREE*)*p_bintree;
    ENUM_RETURN ret_val = RETURN_SUCCESS;
    
    if(p_bintree_temp->p_root != NULL)
    {
        ret_val = bintree_free_node(p_bintree_temp, &(p_bintree_temp->p_root));
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }
    
    S_FREE(*p_bintree);

    return RETURN_SUCCESS;
}

ENUM_RETURN bintree_insert_data(BINTREE *p_bintree, void *p_data_new)
{
    
    S_R_ASSERT(p_bintree != NULL, RETURN_FAILURE);
    S_R_ASSERT(*p_bintree != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_data_new != NULL, RETURN_FAILURE);
    
    STRU_BINTREE *p_bintree_temp = (STRU_BINTREE*)*p_bintree;
    STRU_BINTREE_NODE **pp_dest_node = NULL;
    ENUM_RETURN ret_val = RETURN_SUCCESS;

    ret_val = bintree_find_node(p_bintree_temp, &(p_bintree_temp->p_root), &pp_dest_node, p_data_new);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    if(*pp_dest_node == NULL)
    {
        ret_val = bintree_insert_node(p_bintree_temp, pp_dest_node, p_data_new);
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }
    else
    {
        ret_val = bintree_update_node(p_bintree_temp, *pp_dest_node, p_data_new);
        S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }
    
    return RETURN_SUCCESS;
}

ENUM_RETURN bintree_find_data(BINTREE *p_bintree, void *p_data_new, ENUM_BOOLEAN *p_result)
{
    S_R_ASSERT(p_bintree != NULL, RETURN_FAILURE);
    S_R_ASSERT(*p_bintree != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_data_new != NULL, RETURN_FAILURE);

    *p_result = BOOLEAN_FALSE;
    STRU_BINTREE *p_bintree_temp = (STRU_BINTREE*)*p_bintree;
    STRU_BINTREE_NODE **pp_dest_node = NULL;
    ENUM_RETURN ret_val = RETURN_SUCCESS;

    ret_val = bintree_find_node(p_bintree_temp, &(p_bintree_temp->p_root), &pp_dest_node, p_data_new);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    if(*pp_dest_node != NULL)
    {
        *p_result = BOOLEAN_TRUE;
    }
    
    return RETURN_SUCCESS;

}

ENUM_RETURN bintree_add_data_alloc_handler(BINTREE *p_bintree, BINTREE_DATA_ALLOC_PROC handler);
ENUM_RETURN bintree_add_data_free_handler(BINTREE *p_bintree, BINTREE_DATA_FREE_PROC handler);
ENUM_RETURN bintree_add_data_compare_handler(BINTREE *p_bintree, BINTREE_DATA_COMPARE_PROC handler);
ENUM_RETURN bintree_add_data_print_handler(BINTREE *p_bintree, BINTREE_DATA_PRINT_PROC handler);


