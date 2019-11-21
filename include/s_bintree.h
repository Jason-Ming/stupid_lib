#ifndef __S_BINTREE_H__
#define __S_BINTREE_H__

#include "s_clinkage.h"
#include "s_defines.h"

typedef enum TAG_ENUM_BINTREE_DATA_COMPARE_RESULT
{
    ENUM_BINTREE_DATA_COMPARE_SMALL = -1,
    ENUM_BINTREE_DATA_COMPARE_EQUAL = 0,
    ENUM_BINTREE_DATA_COMPARE_BIG = 1,
}ENUM_BINTREE_DATA_COMPARE_RESULT;

typedef void* BINTREE;

typedef ENUM_RETURN (*BINTREE_DATA_ALLOC_PROC)(void** pp_data_container, void *p_data_new);
typedef ENUM_RETURN (*BINTREE_DATA_FREE_PROC)(void** pp_data_container);
typedef ENUM_RETURN (*BINTREE_DATA_PRINT_PROC)(void *p_data);
typedef ENUM_BINTREE_DATA_COMPARE_RESULT (*BINTREE_DATA_COMPARE_PROC)(void *p_data_container, void *p_data_new);
typedef ENUM_RETURN (*BINTREE_DATA_UPDATE_PROC)(void *p_data_container, void *p_data_new);


__BEGIN_C_DECLS
ENUM_RETURN bintree_create(BINTREE *p_bintree, 
    BINTREE_DATA_ALLOC_PROC data_alloc_handler,
    BINTREE_DATA_FREE_PROC data_free_handler,
    BINTREE_DATA_COMPARE_PROC data_compare_handler,
    BINTREE_DATA_UPDATE_PROC data_update_handler,
    BINTREE_DATA_PRINT_PROC data_print_handler);
ENUM_RETURN bintree_delete(BINTREE *p_bintree);
ENUM_RETURN bintree_insert_data(BINTREE *p_bintree, void *p_data_new);
ENUM_RETURN bintree_find_data(BINTREE *p_bintree, void *p_data_new, ENUM_BOOLEAN *p_result);

ENUM_RETURN bintree_print(BINTREE *p_bintree);

ENUM_RETURN bintree_add_data_alloc_handler(BINTREE *p_bintree, BINTREE_DATA_ALLOC_PROC handler);
ENUM_RETURN bintree_add_data_free_handler(BINTREE *p_bintree, BINTREE_DATA_FREE_PROC handler);
ENUM_RETURN bintree_add_data_compare_handler(BINTREE *p_bintree, BINTREE_DATA_COMPARE_PROC handler);
ENUM_RETURN bintree_add_data_print_handler(BINTREE *p_bintree, BINTREE_DATA_PRINT_PROC handler);


__END_C_DECLS

#endif

