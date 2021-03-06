#ifndef __S_PRINT_H__
#define __S_PRINT_H__

/*
颜色分为背景色和字体色，30~39用来设置字体色，40~49设置背景：
    背景色                        字体色
    40: 黑                          30: 黑
    41: 红                          31: 红
    42: 绿                          32: 绿
    43: 黄                          33: 黄
    44: 蓝                          34: 蓝
    45: 紫                          35: 紫
    46: 深绿                         36: 蓝绿
    47: 白色                         37: 白色 
记得在打印完之后，把颜色恢复成NONE，不然再后面的打印都会跟着变色。
 
另外，还可以加一些ANSI控制码。加颜色只是以下控制码中的一种：
    \033[0m   关闭所有属性   
    \033[1m   设置高亮度   
    \033[4m   下划线   
    \033[5m   闪烁   
    \033[7m   反显   
    \033[8m   消隐   
    \033[30m   --   \033[37m   设置前景色   
    \033[40m   --   \033[47m   设置背景色   
    \033[nA   光标上移n行   
    \033[nB   光标下移n行   
    \033[nC   光标右移n行   
    \033[nD   光标左移n行   
    \033[y;xH设置光标位置   
    \033[2J   清屏   
    \033[K   清除从光标到行尾的内容   
    \033[s   保存光标位置   
    \033[u   恢复光标位置   
    \033[?25l   隐藏光标   
    \033[?25h   显示光标
*/
#define BACKGROUND_WHITE         "\033[47m"
#define BACKGROUND_YELLOW        "\033[43m"

#define REVERSE      "\033[7m"
#define NONE         "\033[m" 

#define BLACK        "\033[0;30m" 
#define LIGHT_BLACK  "\033[1;30m" 
#define DARK_GRAY    "\033[1;30m" 

#define RED          "\033[0;31m" 
#define LIGHT_RED    "\033[1;31m" 

#define GREEN        "\033[0;32m" 
#define LIGHT_GREEN  "\033[1;32m" 

#define YELLOW       "\033[0;33m" 
#define LIGHT_YELLOW "\033[1;33m" 

#define BLUE         "\033[0;34m" 
#define LIGHT_BLUE   "\033[1;34m" 

#define PURPLE       "\033[0;35m" 
#define LIGHT_PURPLE "\033[1;35m" 

#define CYAN         "\033[0;36m" 
#define LIGHT_CYAN   "\033[1;36m" 

#define LIGHT_GRAY   "\033[0;37m" 
#define WHITE        "\033[1;37m"



#define PRINT_COLOR\
    do{\
        printf("       BLACK:" BLACK "BLACK" NONE "\n");\
        printf(" LIGHT_BLACK:" LIGHT_BLACK "LIGHT_BLACK" NONE "\n");\
        printf("   DARK_GRAY:" DARK_GRAY "DARK_GRAY" NONE "\n");\
        \
        printf("         RED:" RED "RED" NONE "\n");\
        printf("   LIGHT_RED:" LIGHT_RED "LIGHT_RED" NONE "\n");\
        \
        printf("       GREEN:" GREEN "GREEN" NONE "\n");\
        printf(" LIGHT_GREEN:" LIGHT_GREEN "LIGHT_GREEN" NONE "\n");\
        \
        printf("      YELLOW:" YELLOW "YELLOW" NONE "\n");\
        printf("LIGHT_YELLOW:" LIGHT_YELLOW "LIGHT_YELLOW" NONE "\n");\
        \
        printf("        BLUE:" BLUE "BLUE" NONE "\n");\
        printf("  LIGHT_BLUE:" LIGHT_BLUE "LIGHT_BLUE" NONE "\n");\
        \
        printf("      PURPLE:" PURPLE "PURPLE" NONE "\n");\
        printf("LIGHT_PURPLE:" LIGHT_PURPLE "LIGHT_PURPLE" NONE "\n");\
        \
        printf("        CYAN:" CYAN "CYAN" NONE "\n");\
        printf("  LIGHT_CYAN:" LIGHT_CYAN "LIGHT_CYAN" NONE "\n");\
        \
        printf("  LIGHT_GRAY:" LIGHT_GRAY "LIGHT_GRAY" NONE "\n");\
        printf("       WHITE:" WHITE "WHITE" NONE "\n");\
    }while(0)
#endif
