#include <stdlib.h>
#include <stdio.h>

#include "s_mem.h"
//void set_mem(void *addr, size_t size, const char *str)
//{
//    size_t str_len = strlen(str);
//    assert(str_len % 2 = 0 && str_len != 0);
//
//    size_t min_len = MIN(size, str_len);
//
//    for(size_t i = 0; i < min_len; i++)
//    {
//        assert(IS_HEX(str[i] && IS_HEX(str[i+1]))
//    }
//}


void display_mem(void *addr, size_t size, ENUM_BOOLEAN bit_switch)
{
    for(size_t i = 0; i < size; i++)
    {
        int x = ((int)*((char*)addr+ i))&(~(~0 << 8));
        printf("        %p: %02x ", (char*)addr+i, x);

        if(bit_switch == BOOLEAN_FALSE)
        {
            printf("\n");
            continue;
        }
        
        for(size_t j = 0; j < sizeof(char)*8; j++)
        {
            printf("%d", (x&(0x01 << (sizeof(char)*8 - j - 1)))?1:0);
        }

        printf("\n");
    }
}

void display_var(void *addr, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        int x = ((int)*((char*)addr+ size - i - 1))&(~(~0 << 8));
        printf("%02X", x);

    }
    printf("\n");
    for(size_t i = 0; i < size; i++)
    {
        int x = ((int)*((char*)addr+ size - i - 1))&(~(~0 << 8));
        for(size_t j = 0; j < sizeof(char)*8; j++)
        {
            printf("%d", (x&(0x01 << (sizeof(char)*8 - j - 1)))?1:0);
        }
    }
    
    printf("\n");
}