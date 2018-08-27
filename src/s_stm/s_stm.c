#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "s_defines.h"
#include "s_limits.h"
#include "s_mem.h"
#include "s_text.h"
#include "s_cmd.h"
#include "s_log.h"

#include "s_stm.h"


typedef struct TAG_STRU_STM_PROC
{
    int state;
    STM_PROC handler;
    char *info;
}STRU_STM_PROC;

typedef struct TAG_STRU_STM
{
    int last_state;
    int current_state;
    int start_state;
    int end_state;
    STM_PROC state_notifier;
    STM_PROC prepare_handler;
    STM_PROC clear_handler;
    STM_PROC preproc_handler;
    STM_PROC postproc_handler;
    unsigned int state_num;
    STRU_STM_PROC *state_handlers;
}STRU_STM;

ENUM_RETURN stm_copy(STM *p_stm_dest, STM stm_source)
{
    R_ASSERT(p_stm_dest != NULL, RETURN_FAILURE);
    R_ASSERT(stm_source != NULL, RETURN_FAILURE);
    STRU_STM *p_stm_source_temp = (STRU_STM*)stm_source;
    ENUM_RETURN ret_val = stm_create(p_stm_dest, p_stm_source_temp->state_num);
    S_R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    R_ASSERT(*p_stm_dest != NULL, RETURN_FAILURE);

    memcpy((_VOID*)(*p_stm_dest), (_VOID*)(p_stm_source_temp), sizeof(STRU_STM));
    memcpy((_VOID*)(((STRU_STM*)(*p_stm_dest))->state_handlers), 
        (_VOID*)(p_stm_source_temp->state_handlers), sizeof(STRU_STM_PROC) * p_stm_source_temp->state_num);

    return RETURN_SUCCESS;
}

ENUM_RETURN stm_create(STM *p_stm, unsigned int state_num)
{
    R_ASSERT(p_stm != NULL, RETURN_FAILURE);
    R_ASSERT(state_num > 0, RETURN_FAILURE);
    *p_stm = NULL;
    
    STRU_STM *p_stm_temp = NULL;

    p_stm_temp = (STRU_STM*)malloc(sizeof(STRU_STM));
    R_ASSERT(p_stm_temp != NULL, RETURN_FAILURE);

    p_stm_temp->state_num = state_num;
    p_stm_temp->current_state = SI_INVALID;
    p_stm_temp->start_state = SI_INVALID;
    p_stm_temp->end_state = SI_INVALID;
    p_stm_temp->state_notifier = NULL;
    p_stm_temp->prepare_handler = NULL;
    p_stm_temp->clear_handler = NULL;
    p_stm_temp->preproc_handler = NULL;
    p_stm_temp->postproc_handler = NULL;
    p_stm_temp->state_handlers = (STRU_STM_PROC*)malloc(sizeof(STRU_STM_PROC)*state_num);
    R_ASSERT_DO(p_stm_temp->state_handlers != NULL, RETURN_FAILURE, S_FREE(p_stm_temp));

    for(int i = 0; i < state_num; i++)
    {
        p_stm_temp->state_handlers[i].state = SI_INVALID;
        p_stm_temp->state_handlers[i].handler = NULL;
        p_stm_temp->state_handlers[i].info = NULL;
    }
    
    *p_stm = p_stm_temp;
    return RETURN_SUCCESS;
}

ENUM_RETURN stm_delete(STM *p_stm)
{
    R_ASSERT(p_stm != NULL, RETURN_FAILURE);
    R_ASSERT(*p_stm != NULL, RETURN_FAILURE);
    STRU_STM *p_stm_temp = (STRU_STM*)*p_stm;

    STRU_STM_PROC *handlers = p_stm_temp->state_handlers;
    R_ASSERT(handlers != NULL, RETURN_FAILURE);
    
    for(int i = 0; i < p_stm_temp->state_num; i++)
    {
        R_ASSERT(handlers[i].info != NULL, RETURN_FAILURE);
        S_FREE(handlers[i].info);
    }
    S_FREE(handlers);
    S_FREE(p_stm_temp);
    *p_stm = NULL;

    return RETURN_SUCCESS;
}

PRIVATE ENUM_BOOLEAN whether_stm_state_is_valid(STRU_STM *p_stm, STM_STATE state)
{
    R_ASSERT(p_stm != NULL, BOOLEAN_FALSE);
    return (p_stm->state_num > state && 0 <= state)? BOOLEAN_TRUE:BOOLEAN_FALSE;
}

ENUM_RETURN add_stm_state_handler(STM p_stm, STM_STATE state, STM_PROC handler, const char *info)
{
    R_ASSERT(p_stm != NULL, RETURN_FAILURE);
    R_ASSERT(handler != NULL, RETURN_FAILURE);
    R_ASSERT(info != NULL, RETURN_FAILURE);

    STRU_STM *p_stm_temp = (STRU_STM*)p_stm;
    R_ASSERT_LOG(whether_stm_state_is_valid(p_stm_temp, state) == BOOLEAN_TRUE, RETURN_FAILURE,
        "state = %d", state);

    STRU_STM_PROC *handlers = p_stm_temp->state_handlers;
    R_ASSERT(handlers != NULL, RETURN_FAILURE);
    R_ASSERT_LOG(handlers[state].handler == NULL, RETURN_FAILURE, "state = %d", state);
    handlers[state].handler = handler;
    handlers[state].state = state;
    handlers[state].info = malloc(strlen(info) + 1);
    R_ASSERT(handlers[state].info != NULL, RETURN_FAILURE);
    strcpy(handlers[state].info, info);

    return RETURN_SUCCESS;
}

ENUM_RETURN add_stm_state_notifier(STM p_stm, STM_PROC handler)
{
    R_ASSERT(p_stm != NULL, RETURN_FAILURE);
    R_ASSERT(handler != NULL, RETURN_FAILURE);

    STRU_STM *p_stm_temp = (STRU_STM*)p_stm;

    p_stm_temp->state_notifier = handler;
    
    return RETURN_SUCCESS;
}

ENUM_RETURN add_stm_preproc_handler(STM p_stm, STM_PROC handler)
{
    R_ASSERT(p_stm != NULL, RETURN_FAILURE);
    R_ASSERT(handler != NULL, RETURN_FAILURE);

    STRU_STM *p_stm_temp = (STRU_STM*)p_stm;

    p_stm_temp->preproc_handler = handler;
    
    return RETURN_SUCCESS;
}

ENUM_RETURN add_stm_postproc_handler(STM p_stm, STM_PROC handler)
{
    R_ASSERT(p_stm != NULL, RETURN_FAILURE);
    R_ASSERT(handler != NULL, RETURN_FAILURE);

    STRU_STM *p_stm_temp = (STRU_STM*)p_stm;

    p_stm_temp->postproc_handler = handler;
    
    return RETURN_SUCCESS;
}

ENUM_RETURN add_stm_prepare_handler(STM p_stm, STM_PROC handler)
{
    R_ASSERT(p_stm != NULL, RETURN_FAILURE);
    R_ASSERT(handler != NULL, RETURN_FAILURE);

    STRU_STM *p_stm_temp = (STRU_STM*)p_stm;

    p_stm_temp->prepare_handler = handler;
    
    return RETURN_SUCCESS;
}

ENUM_RETURN add_stm_clear_handler(STM p_stm, STM_PROC handler)
{
    R_ASSERT(p_stm != NULL, RETURN_FAILURE);
    R_ASSERT(handler != NULL, RETURN_FAILURE);

    STRU_STM *p_stm_temp = (STRU_STM*)p_stm;

    p_stm_temp->clear_handler = handler;
    
    return RETURN_SUCCESS;
}

ENUM_RETURN set_stm_start_state(STM p_stm, STM_STATE state)
{
    R_ASSERT(p_stm != NULL, RETURN_FAILURE);

    STRU_STM *p_stm_temp = (STRU_STM*)p_stm;
    R_ASSERT_LOG(whether_stm_state_is_valid(p_stm_temp, state) == BOOLEAN_TRUE, RETURN_FAILURE,
        "state = %d", state);

    p_stm_temp->start_state = state;
    p_stm_temp->current_state = state;
    p_stm_temp->last_state = state;

    return RETURN_SUCCESS;
}

ENUM_RETURN set_stm_end_state(STM p_stm, STM_STATE state)
{
    R_ASSERT(p_stm != NULL, RETURN_FAILURE);

    STRU_STM *p_stm_temp = (STRU_STM*)p_stm;
    R_ASSERT_LOG(whether_stm_state_is_valid(p_stm_temp, state) == BOOLEAN_TRUE, RETURN_FAILURE,
        "state = %d", state);

    p_stm_temp->end_state = state;

    return RETURN_SUCCESS;
}

PRIVATE const char* get_stm_state_info(STRU_STM *p_stm, STM_STATE state)
{
    R_ASSERT(whether_stm_state_is_valid(p_stm, state) == BOOLEAN_TRUE, NULL);

    return p_stm->state_handlers[state].info;
}

PRIVATE STM_PROC get_stm_state_handler(STRU_STM *p_stm, STM_STATE state)
{
    R_ASSERT(whether_stm_state_is_valid(p_stm, state) == BOOLEAN_TRUE, NULL);

    return p_stm->state_handlers[state].handler;
}

STM_STATE get_current_stm_state(STM p_stm)
{
    R_ASSERT(p_stm != NULL, SI_INVALID);

    STRU_STM *p_stm_temp = (STRU_STM*)p_stm;
    return p_stm_temp->current_state;
}

STM_STATE get_last_stm_state(STM p_stm)
{
    R_ASSERT(p_stm != NULL, SI_INVALID);

    STRU_STM *p_stm_temp = (STRU_STM*)p_stm;
    return p_stm_temp->last_state;
}

ENUM_RETURN set_current_stm_state(STM p_stm, STM_STATE state)
{
    R_ASSERT(p_stm != NULL, RETURN_FAILURE);

    ENUM_RETURN ret_val = RETURN_SUCCESS;
    
    STRU_STM *p_stm_temp = (STRU_STM*)p_stm;
    R_ASSERT_LOG(whether_stm_state_is_valid(p_stm_temp, state) == BOOLEAN_TRUE, RETURN_FAILURE,
        "state = %d", state);
    
    R_ASSERT(p_stm_temp->current_state != state, RETURN_FAILURE);
    
    DEBUG_PRINT("state change from %s to %s", 
        get_stm_state_info(p_stm_temp, p_stm_temp->current_state),
        get_stm_state_info(p_stm_temp, state));

    p_stm_temp->last_state = p_stm_temp->current_state;
    p_stm_temp->current_state = state;
    ret_val = p_stm_temp->state_notifier();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

PRIVATE ENUM_BOOLEAN whether_stm_is_ready(STRU_STM *p_stm)
{
    R_ASSERT(p_stm != NULL, BOOLEAN_FALSE);
    R_ASSERT_LOG(p_stm->current_state != SI_INVALID 
        && p_stm->start_state != SI_INVALID 
        && p_stm->end_state != SI_INVALID
        && p_stm->state_num != 0, 
        BOOLEAN_FALSE,
        "current_state = %d, start_state = %d, end_state = %d, state_num = %d",
        p_stm->current_state, p_stm->start_state, p_stm->end_state, p_stm->state_num);

    R_ASSERT(p_stm->prepare_handler != NULL, BOOLEAN_FALSE); 
    R_ASSERT(p_stm->clear_handler != NULL, BOOLEAN_FALSE); 
    R_ASSERT(p_stm->state_notifier != NULL, BOOLEAN_FALSE); 
    R_ASSERT(p_stm->preproc_handler != NULL, BOOLEAN_FALSE);   
    R_ASSERT(p_stm->postproc_handler != NULL, BOOLEAN_FALSE); 

    STRU_STM_PROC *state_handlers = p_stm->state_handlers;
    R_ASSERT(state_handlers != NULL, BOOLEAN_FALSE);

    for(int i = 0; i < p_stm->state_num; i++)
    {
        R_ASSERT(state_handlers[i].state != SI_INVALID, BOOLEAN_FALSE);
        R_ASSERT(state_handlers[i].handler != NULL, BOOLEAN_FALSE);
        R_ASSERT(state_handlers[i].info != NULL, BOOLEAN_FALSE);
    }

    return BOOLEAN_TRUE;
}

ENUM_RETURN stm_run(STM p_stm)
{
    R_ASSERT(p_stm != NULL, RETURN_FAILURE);

    STRU_STM *p_stm_temp = (STRU_STM*)p_stm;
    ENUM_RETURN ret_val = RETURN_SUCCESS;

    R_ASSERT(whether_stm_is_ready(p_stm_temp) == BOOLEAN_TRUE, RETURN_FAILURE);
    
    ret_val = p_stm_temp->prepare_handler();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    while(p_stm_temp->end_state != p_stm_temp->current_state)
    {
        ret_val = p_stm_temp->preproc_handler();
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
        STM_PROC handler = get_stm_state_handler(p_stm_temp, p_stm_temp->current_state);
        R_ASSERT(handler != NULL, RETURN_FAILURE);

        ret_val = handler();
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

        ret_val = p_stm_temp->postproc_handler();
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    }

    ret_val = p_stm_temp->clear_handler();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}

