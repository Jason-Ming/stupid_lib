#ifndef __S_PRINT_H__
#define __S_PRINT_H__

/*
��ɫ��Ϊ����ɫ������ɫ��30~39������������ɫ��40~49���ñ�����
    ����ɫ                        ����ɫ
    40: ��                          30: ��
    41: ��                          31: ��
    42: ��                          32: ��
    43: ��                          33: ��
    44: ��                          34: ��
    45: ��                          35: ��
    46: ����                         36: ����
    47: ��ɫ                         37: ��ɫ 
�ǵ��ڴ�ӡ��֮�󣬰���ɫ�ָ���NONE����Ȼ�ٺ���Ĵ�ӡ������ű�ɫ��
 
���⣬�����Լ�һЩANSI�����롣����ɫֻ�����¿������е�һ�֣�
    \033[0m   �ر���������   
    \033[1m   ���ø�����   
    \033[4m   �»���   
    \033[5m   ��˸   
    \033[7m   ����   
    \033[8m   ����   
    \033[30m   --   \033[37m   ����ǰ��ɫ   
    \033[40m   --   \033[47m   ���ñ���ɫ   
    \033[nA   �������n��   
    \033[nB   �������n��   
    \033[nC   �������n��   
    \033[nD   �������n��   
    \033[y;xH���ù��λ��   
    \033[2J   ����   
    \033[K   ����ӹ�굽��β������   
    \033[s   ������λ��   
    \033[u   �ָ����λ��   
    \033[?25l   ���ع��   
    \033[?25h   ��ʾ���
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
