/*
1.[�ַ���ת��]
    �����Ҫ�Ļ�,������Դ�ļ��Ķ��ֽ��ַ�(multibyte characters)��,��ʵ�֢޶���ķ�ʽӳ��
    ��Դ�ַ���(source character set),�����뻻�з�(new-line characters)��Ϊ�н���ָʾ��.
    ������(trigraph)�߱���Ӧ�ĵ��ַ��ڲ���ʾ�滻.

2.[��������]
    ÿ������һ���з��ķ�б���ַ�(/,backslash),��ͬ����Ļ��з�һ��ɾ��,�Խ������ϵ�Դ
    �����нӺ�����,�γ��߼��ϵ�Դ������.���κ�����Դ��������,ֻ�����һ����б���ַ�,����
    ���ֽӺϲŷ���������.

3.[����ע�ͺͿհ�]
    Դ�ļ����ֽ������Ԥ����ǺŢ�6)�����ɿհ׷��ݵ�����(����ע��).һ��Դ�ļ���Ӧ��ĳ��Ԥ
    ����Ǻŵ�һ���֩p��ĳ��ע�͵�һ���ֽ���.ÿ��ע�ͱ�һ���ո�����滻��.���з��Ա���.
    ����ÿ�οհ׷�(�������з�)�ķǿ�����,�Ǳ��������滻��һ���ո��,����ʵ�ֶ���.

4.[Ԥ����]
   Ԥ����ָ�ִ��,����ñ�չ��,_PragmaһԪ���������ʽ��ִ��.���һ��ƥ����ַ���
   (universal character name)���﷨���ַ��������ɼǺ�����(6.10.3.3)������,����Ϊδ�����.
   һ��#includeԤ����ָ���ָ����ͷ�ļ���Դ�ļ��ӽ׶�1���׶�4�ݹ�ر�����.���ɾ������
   ��Ԥ����ָ���.

5.[����ת���ַ�]
    �ַ��������ַ����������е�ÿ��Դ�ַ�����Ա��ת�����б�ת����ִ���ַ���(execution 
    character set)����Ӧ�ĳ�Ա.���û����Ӧ�ĳ�Ա,��ת������ʵ�ֶ���ĳ�Ա,�����ǿ�
    (���)�ַ�.7)

6.[�ϲ��ڽ����ַ���������]
    �ڽ������ɸ��ַ����������Ǻű�������һ���Ϊһ��.

7.[�ʷ�����,�﷨����,�������,�м���������]
    �ָ��ǺŵĿհ׷����پ�������.ÿ��Ԥ����Ǻű�ת����һ���Ǻ�.���н���Ǻ���Ϊһ����
    �뵥λ,���﷨�Ϻ������Ͻ��з����ͷ���.

8.[�����ⲿ��,���ɿ�ִ�г���]
    ������ж��ⲿ����ͺ���������.���ӿⲿ��,��ȷ����ǰ���뵥λ��δ����ĺ����Ͷ����
    �ⲿ����.������Щ�������������г�һ�������ӳ��,֮�к�����Ҫ���������л�����ִ��
    ����Ϣ.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "s_log.h"
#include "s_mem.h"
#include "s_text.h"
#include "s_cproc.h"
#include "s_ctoken.h"
#include "s_cpp.h"
#include "s_cdcl.h"
#include "s_cc.h"
#include "s_cproc_macro.h"

ENUM_RETURN s_cget_statement(_S8 ** pp_text_buffer, _S8 statement_buffer[], size_t buffer_size, size_t *len)
{
    R_ASSERT(pp_text_buffer != NULL, RETURN_FAILURE);
    R_ASSERT(*pp_text_buffer != NULL, RETURN_FAILURE);
    R_ASSERT(statement_buffer != NULL, RETURN_FAILURE);
    R_ASSERT(buffer_size > 0, RETURN_FAILURE);
    R_ASSERT(len != NULL, RETURN_FAILURE);
    _S32 brace_num = 0;
    ENUM_BOOLEAN whether_c_can_be_ignored = BOOLEAN_TRUE;
    
    _S8 *temp = statement_buffer;
    *len = 0;
    _S32 c;
    
    /* skip white space */
    while((c = *(*pp_text_buffer)++) != '\0')
    {
        if(isspace(c) && whether_c_can_be_ignored)
        {
            continue;
        }

        if(c == ';')
        {
            if(whether_c_can_be_ignored)
            {
                continue;
            }
            else if(brace_num == 0)
            {
                break;
            }
        }

        whether_c_can_be_ignored = BOOLEAN_FALSE;

        if(c == '{')
        {
            brace_num++;
        }

        if(c == '}')
        {
            brace_num--;
        }

        OUTPUT_C(c, statement_buffer, buffer_size);

        
    }

    OUTPUT_END(statement_buffer, buffer_size);

    *len = strlen(temp);
    return RETURN_SUCCESS;
}

ENUM_RETURN s_cc(FILE * pfr, FILE * pfw)
{
	R_ASSERT(pfr != NULL, RETURN_FAILURE);

	ENUM_RETURN ret_val = RETURN_SUCCESS;
    _S8 *p_text_buffer = NULL;
    size_t text_buffer_size = 0;

	STRU_C_TOKEN_NODE *p_token_list_head = NULL;
	STRU_C_TOKEN_NODE *p_token_list_tail = NULL;

    ret_val = s_cpp(pfr, &p_text_buffer, &text_buffer_size, &p_token_list_head, &p_token_list_tail);
    S_ASSERT(ret_val == RETURN_SUCCESS);

	s_cpp_print(pfw, p_token_list_head, p_token_list_tail);

	ret_val = s_cdcl(p_text_buffer, p_token_list_head, p_token_list_tail);
    S_ASSERT(ret_val == RETURN_SUCCESS);

	FREE(p_text_buffer);
	
	ret_val = release_token_list(&p_token_list_head, &p_token_list_tail);
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);

    ret_val = s_cproc_macro_release_list();
    R_ASSERT(ret_val == RETURN_SUCCESS, RETURN_FAILURE);
	return RETURN_SUCCESS;
}

