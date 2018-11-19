/*
1.[字符集转换]
    如果必要的话,将物理源文件的多字节字符(multibyte characters)⑤,以实现⑥定义的方式映射
    到源字符集(source character set),并引入换行符(new-line characters)作为行结束指示符.
    三联符(trigraph)⑦被相应的单字符内部表示替换.

2.[断行连接]
    每个紧跟一换行符的反斜线字符(/,backslash),连同后跟的换行符一起被删除,以将物理上的源
    代码行接合起来,形成逻辑上的源代码行.在任何物理源代码行中,只有最后一个反斜线字符,用作
    这种接合才符合条件⑧.

3.[处理注释和空白]
    源文件被分解成若干预处理记号④6)和若干空白符⑤的序列(包括注释).一个源文件不应以某个预
    处理记号的一部分﹑或某个注释的一部分结束.每个注释被一个空格符所替换⑥.换行符仍保留.
    对于每段空白符(不含换行符)的非空序列,是保留还是替换成一个空格符,则由实现定义.

4.[预处理]
   预处理指令被执行,宏调用被展开,_Pragma一元操作符表达式被执行.如果一段匹配大字符集
   (universal character name)⑦语法的字符序列是由记号连接(6.10.3.3)产生的,则行为未定义⑧.
   一条#include预处理指令导致指定的头文件或源文件从阶段1到阶段4递归地被处理.最后删除所有
   的预处理指令⑨.

5.[处理转义字符]
    字符常量和字符串文字量中的每个源字符集成员与转义序列被转换成执行字符集(execution 
    character set)中相应的成员.如果没有相应的成员,则转换成由实现定义的成员,而不是空
    (或宽)字符.7)

6.[合并邻近的字符串文字量]
    邻近的若干个字符串文字量记号被连接在一起成为一个.

7.[词法分析,语法分析,语义分析,中间代码产生等]
    分隔记号的空白符不再具有意义.每个预处理记号被转换成一个记号.所有结果记号作为一个编
    译单位,从语法上和语义上进行分析和翻译.

8.[链接外部库,生成可执行程序]
    解决所有对外部对象和函数的引用.链接库部分,以确保当前编译单位中未定义的函数和对象的
    外部引用.所有这些翻译的输出被集中成一个程序的映象,之中含有需要在它的运行环境中执行
    的信息.

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

