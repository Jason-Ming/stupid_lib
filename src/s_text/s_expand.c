#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <ctype.h>

#include "s_defines.h"
#include "s_log.h"
#include "s_text.h"
#include "s_mem.h"
#include "s_alg.h"
#include "s_stm.h"

typedef enum TAG_ENUM_EXPAND_STATE
{
    EXPAND_STATE_CONC = 0,
    EXPAND_STATE_RANGE_BEGIN,
    EXPAND_STATE_RANGE_TO,
    EXPAND_STATE_RANGE_END,
    EXPAND_STATE_END,
    EXPAND_STATE_MAX
}ENUM_EXPAND_STATE;

typedef struct TAG_STRU_EXPAND_STM_RUN_DATA
{
    const _S8 *source;
    _S8 *dest;
    size_t size;
    _S8 begin;
    _S8 end;
    _S8 c;
}STRU_EXPAND_STM_RUN_DATA;

PRIVATE STM s_expand_stm;
PRIVATE STRU_EXPAND_STM_RUN_DATA s_expand_run_data;

PRIVATE ENUM_RETURN s_expand_stm_prepare_proc()
{
    s_expand_run_data.begin = s_expand_run_data.end = s_expand_run_data.c = '\0';
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_expand_stm_clear_proc()
{

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_expand_stm_preproc()
{
    ENUM_RETURN ret_val;
    s_expand_run_data.c = *(s_expand_run_data.source);

    if(s_expand_run_data.c == '\0')
    {
        ret_val = set_current_stm_state(s_expand_stm, EXPAND_STATE_END);
        R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
        
        return RETURN_SUCCESS;
    }
    
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_expand_stm_postproc()
{
    (s_expand_run_data.source)++;
    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_expand_stm_state_notifier()
{

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_expand_stm_state_proc_conc()
{
    _S8 c = s_expand_run_data.c;
    _S8 begin = s_expand_run_data.begin;
    _S8 end = s_expand_run_data.end;

    if(c == '-')
    {
        //原样输出
        OUTPUT_STR(c, s_expand_run_data.dest, s_expand_run_data.size);
    }
    else if(!isdigit(c) && !isupper(c) && !(islower(c))) /* -1-2a-z---B*/
    {
        set_current_stm_state(s_expand_stm, EXPAND_STATE_END);
        R_RET_LOG(RETURN_FAILURE, "unexpected char: %c", c);
    }
    else
    {
        begin = end = c;
        set_current_stm_state(s_expand_stm, EXPAND_STATE_RANGE_BEGIN);
    }
    
    s_expand_run_data.begin = begin;
    s_expand_run_data.end = end;

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_expand_stm_state_proc_range_begin()
{
    _S8 c = s_expand_run_data.c;
    _S8 begin = s_expand_run_data.begin;
    _S8 end = s_expand_run_data.end;

    if(c == '-')
    {
        set_current_stm_state(s_expand_stm, EXPAND_STATE_RANGE_TO);
    }
    else if(!isdigit(c) && !isupper(c) && !(islower(c))) /* -1-2a-z---B*/
    {
        set_current_stm_state(s_expand_stm, EXPAND_STATE_END);
        R_RET_LOG(RETURN_FAILURE, "unexpected char: %c", c);
    }
    else
    {
        OUTPUT_STR_RANGE(begin, end, s_expand_run_data.dest, s_expand_run_data.size);
        begin = end = c;
    }
    
    s_expand_run_data.begin = begin;
    s_expand_run_data.end = end;

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_expand_stm_state_proc_range_to()
{
    _S8 c = s_expand_run_data.c;
    _S8 begin = s_expand_run_data.begin;
    _S8 end = s_expand_run_data.end;

    if(c == '-')
    {
        OUTPUT_STR_RANGE(begin, end, s_expand_run_data.dest, s_expand_run_data.size);
        
        //原样输出
        OUTPUT_STR(c, s_expand_run_data.dest, s_expand_run_data.size);
        OUTPUT_STR(c, s_expand_run_data.dest, s_expand_run_data.size);
        set_current_stm_state(s_expand_stm, EXPAND_STATE_CONC);
    }
    else if(!isdigit(c) && !isupper(c) && !(islower(c))) /* -1-2a-z---B*/
    {
        set_current_stm_state(s_expand_stm, EXPAND_STATE_END);
        R_RET_LOG(RETURN_FAILURE, "unexpected char: %c", c);
    }
    else
    {
        //与之前的begin不形成递增
        if(s_range(end, c) == BOOLEAN_FALSE)
        {
            OUTPUT_STR_RANGE(begin, end, s_expand_run_data.dest, s_expand_run_data.size);
            OUTPUT_STR('-', s_expand_run_data.dest, s_expand_run_data.size);
            begin = end = c;
            set_current_stm_state(s_expand_stm, EXPAND_STATE_RANGE_BEGIN);
        }
        else
        {
            //与之前的begin形成递增
            end = c;
            set_current_stm_state(s_expand_stm, EXPAND_STATE_RANGE_END);
        }
    }
    
    s_expand_run_data.begin = begin;
    s_expand_run_data.end = end;

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_expand_stm_state_proc_range_end()
{
    _S8 c = s_expand_run_data.c;
    _S8 begin = s_expand_run_data.begin;
    _S8 end = s_expand_run_data.end;

    if(c == '-')
    {
        set_current_stm_state(s_expand_stm, EXPAND_STATE_RANGE_TO);
    }
    else if(!isdigit(c) && !isupper(c) && !(islower(c))) /* -1-2a-z---B*/
    {
        set_current_stm_state(s_expand_stm, EXPAND_STATE_END);
        R_RET_LOG(RETURN_FAILURE, "unexpected char: %c", c);
    }
    else
    {
        OUTPUT_STR_RANGE(begin, end, s_expand_run_data.dest, s_expand_run_data.size);
        begin = end = c;
        set_current_stm_state(s_expand_stm, EXPAND_STATE_RANGE_BEGIN);
    }
    
    s_expand_run_data.begin = begin;
    s_expand_run_data.end = end;

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_expand_stm_state_proc_end()
{
    _S8 begin = s_expand_run_data.begin;
    _S8 end = s_expand_run_data.end;
    ENUM_EXPAND_STATE state = get_last_stm_state(s_expand_stm);


    OUTPUT_STR_RANGE(begin, end, s_expand_run_data.dest, s_expand_run_data.size);
    
    if(state == EXPAND_STATE_RANGE_TO)
    {
        OUTPUT_STR('-', s_expand_run_data.dest, s_expand_run_data.size);
    }
    
    *(s_expand_run_data.dest)++ = '\0';
    (s_expand_run_data.size)--;

    return RETURN_SUCCESS;
}

PRIVATE ENUM_RETURN s_expand_init()
{
    ENUM_RETURN ret_val = RETURN_FAILURE;
    ret_val = stm_create(&s_expand_stm, EXPAND_STATE_MAX);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = set_stm_start_state(s_expand_stm, EXPAND_STATE_CONC);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = set_stm_end_state(s_expand_stm, EXPAND_STATE_END);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_prepare_handler(s_expand_stm, s_expand_stm_prepare_proc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_clear_handler(s_expand_stm, s_expand_stm_clear_proc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_preproc_handler(s_expand_stm, s_expand_stm_preproc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_postproc_handler(s_expand_stm, s_expand_stm_postproc);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_state_notifier(s_expand_stm, s_expand_stm_state_notifier);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_state_handler(s_expand_stm, EXPAND_STATE_CONC, s_expand_stm_state_proc_conc, "- state");
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_state_handler(s_expand_stm, EXPAND_STATE_RANGE_BEGIN, s_expand_stm_state_proc_range_begin, "range begin state");
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_state_handler(s_expand_stm, EXPAND_STATE_RANGE_TO, s_expand_stm_state_proc_range_to, "range to state");
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_state_handler(s_expand_stm, EXPAND_STATE_RANGE_END, s_expand_stm_state_proc_range_end, "range end state");
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = add_stm_state_handler(s_expand_stm, EXPAND_STATE_END, s_expand_stm_state_proc_end, "end state");
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    return RETURN_SUCCESS;
}
PRIVATE ENUM_RETURN s_expand_clear()
{
    ENUM_RETURN ret_val = stm_delete(&s_expand_stm);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    
    return RETURN_SUCCESS;
}

ENUM_RETURN s_expand(const _S8 *source, _S8 *dest, size_t size)
{
    R_ASSERT(source != NULL, RETURN_FAILURE);
    R_ASSERT(dest != NULL, RETURN_FAILURE);

    ENUM_RETURN ret_val = s_expand_init();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    s_expand_run_data.source = source;
    s_expand_run_data.dest = dest;
    s_expand_run_data.size = size;
    
    ret_val = stm_run(s_expand_stm);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = s_expand_clear();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
    

    return RETURN_SUCCESS;
}

