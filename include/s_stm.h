#ifndef __S_STM_H__
#define __S_STM_H__

#include "s_clinkage.h"
#include "s_defines.h"

typedef void* STM;
typedef int STM_STATE;
typedef ENUM_RETURN (*STM_PROC)(void);


__BEGIN_C_DECLS
ENUM_RETURN stm_create(STM *p_stm, unsigned int state_num);
ENUM_RETURN stm_delete(STM *p_stm);

ENUM_RETURN add_stm_state_handler(STM p_stm, STM_STATE state, STM_PROC handler, const char *info);
ENUM_RETURN add_stm_preproc_handler(STM p_stm, STM_PROC handler);
ENUM_RETURN add_stm_postproc_handler(STM p_stm, STM_PROC handler);
ENUM_RETURN add_stm_prepare_handler(STM p_stm, STM_PROC handler);
ENUM_RETURN add_stm_clear_handler(STM p_stm, STM_PROC handler);
ENUM_RETURN add_stm_state_notifier(STM p_stm, STM_PROC handler);


ENUM_RETURN set_stm_start_state(STM p_stm, STM_STATE state);
ENUM_RETURN set_stm_end_state(STM p_stm, STM_STATE state);

ENUM_RETURN stm_run(STM p_stm);

STM_STATE get_current_stm_state(STM p_stm);
ENUM_RETURN set_current_stm_state(STM p_stm, STM_STATE state);

__END_C_DECLS

#endif

