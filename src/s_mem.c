#include <stdlib.h>
#include <stdio.h>

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

void display_mem(void *addr, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        printf("        %p: %02x\n", (char*)addr+i, (char)*((char*)addr+i));
    }
}

void display_var(void *addr, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        int x = ((int)*((char*)addr+ size - i - 1))&0x000000FF;
        printf("%02X", x);
    }
    printf("\n");
}