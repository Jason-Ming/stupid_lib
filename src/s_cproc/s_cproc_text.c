#include <stdio.h>
#include <stdlib.h>

#include "s_log.h"
#include "s_mem.h"
#include "s_limits.h"
#include "s_list.h"
#include "s_text.h"
#include "s_ctoken.h"
#include "s_cerror.h"
#include "s_cproc_stm.h"
#include "s_cproc_text.h"

typedef struct TAG_STRU_C_TEXT
{
    _S8 *p_filename;
    _SL inode;
    _S8 *p_buffer;
    size_t buffer_size;
}STRU_C_TEXT;

typedef struct TAG_STRU_C_TEXT_NODE
{
    STRU_C_TEXT data;
    struct list_head list;
}STRU_C_TEXT_NODE;

PRIVATE STRU_C_TEXT_NODE g_c_text_list_head;

_VOID s_cproc_text_list_init(_VOID)
{
    INIT_LIST_HEAD(&g_c_text_list_head.list);
    g_c_text_list_head.data.p_filename = "text-list-head";
    g_c_text_list_head.data.p_buffer = NULL;
    g_c_text_list_head.data.buffer_size = 0;
    g_c_text_list_head.data.inode = SL_INVALID;
}

PRIVATE _VOID s_cproc_text_release_node(STRU_C_TEXT_NODE **pp_c_text_node)
{
    S_V_ASSERT(pp_c_text_node != NULL);
    S_V_FALSE(*pp_c_text_node != NULL);
    
    FREE((*pp_c_text_node)->data.p_filename);
    FREE((*pp_c_text_node)->data.p_buffer);
    FREE((*pp_c_text_node));
}

PRIVATE ENUM_RETURN s_cproc_text_make_new_node(
    STRU_C_TEXT_NODE **pp_c_text_node, 
    const _S8 *p_filename)
{
    S_R_ASSERT(pp_c_text_node != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_filename != NULL, RETURN_FAILURE);
    *pp_c_text_node = NULL;

    STRU_C_TEXT_NODE *p_c_text_node = NULL;
    p_c_text_node = (STRU_C_TEXT_NODE*)malloc(sizeof(STRU_C_TEXT_NODE));
    S_R_ASSERT(p_c_text_node != NULL, RETURN_FAILURE);
    
    p_c_text_node->data.inode = s_get_inode_by_filename(p_filename);
    S_R_ASSERT(p_c_text_node->data.inode != SL_INVALID, RETURN_FAILURE);
    
    p_c_text_node->data.p_filename = (_S8 *)malloc(strlen(p_filename) + 1);
    S_R_ASSERT(p_c_text_node->data.p_filename != NULL, RETURN_FAILURE);
    strcpy(p_c_text_node->data.p_filename, p_filename);

    FILE *pfr = fopen(p_filename, "r");
    S_R_ASSERT(pfr != NULL, RETURN_FAILURE);
    ENUM_RETURN ret_val = s_save_file_to_text_buffer(pfr, &p_c_text_node->data.p_buffer, &p_c_text_node->data.buffer_size);
    FCLOSE(pfr);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    S_R_ASSERT(p_c_text_node->data.p_buffer != NULL, RETURN_FAILURE);
    S_R_ASSERT(p_c_text_node->data.buffer_size > 0, RETURN_FAILURE);

    *pp_c_text_node = p_c_text_node;

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_cproc_text_add_node_to_list(
    STRU_C_TEXT_NODE *p_c_text_node_new)
{
    S_R_ASSERT(p_c_text_node_new != NULL, RETURN_FAILURE);

    list_add_tail(&(p_c_text_node_new->list), &(g_c_text_list_head.list));

    return RETURN_SUCCESS;
}

ENUM_RETURN s_cproc_text_read_file_to_buffer(
    const _S8 *p_filename,
    const _S8 **pp_text_buffer)
{
    S_R_ASSERT(p_filename != NULL, RETURN_FAILURE);
    S_R_ASSERT(pp_text_buffer != NULL, RETURN_FAILURE);

    //file has already been read to text list
    *pp_text_buffer = s_cproc_text_get_buffer_by_filename(p_filename);
    S_R_FALSE(*pp_text_buffer == NULL, RETURN_SUCCESS);

    STRU_C_TEXT_NODE *p_c_text_node_new = NULL;
    ENUM_RETURN ret_val = s_cproc_text_make_new_node(
        &p_c_text_node_new, 
        p_filename);
    S_R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, s_cproc_text_release_node(&p_c_text_node_new));

    ret_val = s_cproc_text_add_node_to_list(p_c_text_node_new);
    S_R_ASSERT_DO(ret_val == RETURN_SUCCESS, RETURN_FAILURE, s_cproc_text_release_node(&p_c_text_node_new));

    *pp_text_buffer = p_c_text_node_new->data.p_buffer;
    
    return RETURN_SUCCESS;
}

const _S8 * s_cproc_text_get_buffer_by_filename(const _S8 *p_filename)
{
    S_R_ASSERT(p_filename != NULL, NULL);

    _SL inode = s_get_inode_by_filename(p_filename);
    S_R_ASSERT(inode != SL_INVALID, NULL);
    struct list_head *pos;
    STRU_C_TEXT_NODE *p_c_text_temp;

    list_for_each_all(pos, &g_c_text_list_head.list)
    {
        p_c_text_temp = list_entry(pos, STRU_C_TEXT_NODE, list);
        if(p_c_text_temp->data.inode == inode)
        {
            return p_c_text_temp->data.p_buffer;
        }
    } 
    
    return NULL;
}

_VOID s_cproc_text_release_list(_VOID)
{
    STRU_C_TEXT_NODE *p_c_text_node_temp;
    struct list_head *pos, *next;
    list_for_each_all_safe(pos, next, &g_c_text_list_head.list)
    {
        p_c_text_node_temp = list_entry(pos, STRU_C_TEXT_NODE, list); 
        list_del_init(pos); 
        s_cproc_text_release_node(&p_c_text_node_temp); 
    }
}

PRIVATE _VOID s_cproc_text_print_node_debug_info(STRU_C_TEXT_NODE *p_c_text_node)
{
    S_V_ASSERT(p_c_text_node != NULL);

    printf(" >>>%-10s: %s\n", "filename", p_c_text_node->data.p_filename);
    printf("    %-10s: %ld\n", "inode", p_c_text_node->data.inode);
    printf("    %-10s: %p\n", "buffer", p_c_text_node->data.p_buffer);
    printf("    %-10s: %zd\n", "size", p_c_text_node->data.buffer_size);
}
ENUM_RETURN s_cproc_text_print_list_debug_info(_VOID)
{
    STRU_C_TEXT_NODE *p_c_text_node_temp;
    struct list_head *pos;
    printf("\n\n--TEXT LIST DEBUG INFO BEGIN-------------------------------------------------------\n");
    list_for_each_all(pos, &g_c_text_list_head.list)
    {
        p_c_text_node_temp = list_entry(pos, STRU_C_TEXT_NODE, list); 
        s_cproc_text_print_node_debug_info(p_c_text_node_temp);
    }
    printf("--TEXT LIST DEBUG INFO END---------------------------------------------------------\n");

    return RETURN_SUCCESS;
}

