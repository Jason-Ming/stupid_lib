/*
1. 条件包含
2. 源文件包含
3. 宏替换
3.1 实参的替换
3.2 #操作符
3.3 ##操作符
3.4 重复扫描和进一步替换
3.5 宏定义的作用范围
4. Line指令
5. Error指令
6. Pragma指令
7. 空指令
8. 预定义的宏名
9. _Pragma操作符
10. 处理转义字符
11. 合并邻近的字符串文字量
12. 去除分隔记号空白符，生成词法分析token, 宏定义列表(可能嵌套包含供包含者使用)
*/
# include<stdio.h>   得分  /* DSAFASD */ DDD
//this is a test comment
//test snippet begin========================================================================================
#define /*这是单个字啧啧啧啧啧啧*/ STRING "checkpair//\   \ 
/**/\t  'this //is a string' \" good! \"\n"//a line comment\ 
#define CALC(x, y, z) (x/y/*this is a comment"a common\ 
string" 'a' // x/y/z*//z)
#define STRING1 'this is a single quote// /* lalala\
*/ \" "hehe" \'sssss\''//a line comment
//a line comment

#define a\   
bc "abc /*\  
*/abc" STRING

#define STR1 "STR1\
#define STR2 "
#define STR12 STR1 //STR2

//test snippet end==========================================================================================

