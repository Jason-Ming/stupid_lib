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
#include <limits.h>
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
#include "s_cproc_text.h"
#include "s_cproc_token.h"

#ifdef CPPUTEST
FILE *g_pfw_errors;
#endif

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
#ifdef CPPUTEST
ENUM_RETURN s_cc(const _S8 * file_name, FILE * pfw, FILE * pfw_errors)
{
    g_pfw_errors = pfw_errors;

#else
ENUM_RETURN s_cc(const _S8 * file_name, FILE * pfw)
{

#endif

	R_ASSERT(file_name != NULL, RETURN_FAILURE);

    _S8 *path = realpath(file_name, NULL);
    printf("file name: %s, file real path: %s\n", file_name, path);

	ENUM_RETURN ret_val = RETURN_FAILURE;
    ENUM_RETURN result = RETURN_FAILURE;
    s_cproc_text_list_init();
    s_cproc_macro_list_init();
    s_cproc_token_init_list();
    
    ret_val = s_cpp(path, &result);
    S_ASSERT(ret_val == RETURN_SUCCESS);
    
    s_cproc_text_print_list_debug_info();
    s_cproc_token_print_list_debug_info();
    s_cproc_macro_print_list_debug_info();

	s_cproc_token_print_list(pfw);

    s_cproc_token_delete_blanks_and_newline();
    s_cproc_token_print_list_debug_info();

    if(result == RETURN_SUCCESS)
    {
    	ret_val = s_cdcl();
        S_ASSERT(ret_val == RETURN_SUCCESS);
    }
    
	s_cproc_text_release_list();
    s_cproc_token_release_list();
    s_cproc_macro_release_list();
#ifndef CPPUTEST
    S_FREE(path);
#endif
	return result;
}

