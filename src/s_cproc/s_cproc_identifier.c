#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "s_log.h"
#include "s_mem.h"
#include "s_text.h"
#include "s_bintree.h"
#include "s_ctoken.h"

#include "s_cproc_identifier.h"

typedef struct TAG_STRU_IDENTIFIER
{
    PSTR string;
    _S32 count;
}STRU_IDENTIFIER;

PRIVATE BINTREE g_identifiers;

PRIVATE ENUM_RETURN s_cproc_identifier_bintree_data_print(void *p_data)
{
    STRU_IDENTIFIER *p_identifier = (STRU_IDENTIFIER*)p_data;
    printf("%20s, %6d\n", p_identifier->string, p_identifier->count);
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_cproc_identifier_bintree_data_alloc(void** pp_data_container, _VOID *p_data_new)
{
    S_R_ASSERT(pp_data_container != NULL, RETURN_FAILURE);
    S_R_ASSERT(*pp_data_container == NULL, RETURN_FAILURE);

    STRU_IDENTIFIER* p_identifier = (STRU_IDENTIFIER*)malloc(sizeof(STRU_IDENTIFIER));
    S_R_ASSERT(p_identifier != NULL, RETURN_FAILURE);

    p_identifier->string = NULL;
    p_identifier->string = malloc(strlen(p_data_new) + 1);
    S_R_ASSERT_DO(p_identifier->string != NULL, RETURN_FAILURE, S_FREE(p_identifier));

    strcpy(p_identifier->string, p_data_new);
    p_identifier->count = 1;

    *pp_data_container = p_identifier;
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_cproc_identifier_bintree_data_free(void** pp_data_container)
{
    S_R_ASSERT(pp_data_container != NULL, RETURN_FAILURE);
    S_R_ASSERT(*pp_data_container != NULL, RETURN_FAILURE);

    STRU_IDENTIFIER *p_identifier = (STRU_IDENTIFIER*)*pp_data_container;
    
    S_FREE(p_identifier->string);
    S_FREE(p_identifier);
    *pp_data_container = NULL;
    return RETURN_SUCCESS;
}

PRIVATE ENUM_BINTREE_DATA_COMPARE_RESULT s_cproc_identifier_bintree_data_compare(void *p_data_origin, void *p_data_new)
{
    S_R_ASSERT(p_data_origin != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_data_new != NULL, RETURN_FAILURE);

    STRU_IDENTIFIER *p_identifier = (STRU_IDENTIFIER*)p_data_origin;

    _S32 result = strcmp(p_identifier->string, p_data_new);
    if(result > 0)
    {
        return ENUM_BINTREE_DATA_COMPARE_BIG;
    }
    else if(result < 0)
    {
        return ENUM_BINTREE_DATA_COMPARE_SMALL;
    }
    else
    {
        return ENUM_BINTREE_DATA_COMPARE_EQUAL;
    }
}


PRIVATE ENUM_RETURN s_cproc_identifier_bintree_data_update(void *p_data_origin, void *p_data_new)
{
    S_R_ASSERT(p_data_origin != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_data_new != NULL, RETURN_FAILURE);


    STRU_IDENTIFIER *p_identifier = (STRU_IDENTIFIER*)p_data_origin;

    p_identifier->count++;
    
    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_identifier_init(_VOID)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;

    ret_val = bintree_create(&g_identifiers, 
        s_cproc_identifier_bintree_data_alloc, 
        s_cproc_identifier_bintree_data_free, 
        s_cproc_identifier_bintree_data_compare, 
        s_cproc_identifier_bintree_data_update, 
        s_cproc_identifier_bintree_data_print);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_identifier_release(_VOID)
{
    ENUM_RETURN ret_val = RETURN_SUCCESS;

    ret_val = bintree_delete(&g_identifiers);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_identifier_insert(PSTR p_string)
{
    S_R_ASSERT(p_string != NULL, RETURN_FAILURE);
    
    ENUM_RETURN ret_val = RETURN_SUCCESS;

    ret_val = bintree_insert_data(&g_identifiers, p_string);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_identifier_print(_VOID)
{
    return bintree_print(&g_identifiers);
}

