#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

#include "s_log.h"
#include "s_mem.h"
#include "s_text.h"
#include "s_cproc.h"

#include "s_cerror.h"


_VOID display_token_and_line(const _S8 *p_text_buffer, size_t offset)
{
	S_V_ASSERT(p_text_buffer != NULL);
	//printf("text_buffer:%s\n", p_text_buffer);
    //printf("offset: %zd\n", offset);
    
	size_t start = 0;

	struct winsize size;
	ioctl(STDIN_FILENO,TIOCGWINSZ,&size);
	DEBUG_PRINT("%d\n",size.ws_col);
	DEBUG_PRINT("%d\n",size.ws_row);

	size_t column = 0;

    while((p_text_buffer + offset - column) != p_text_buffer)
	{
        if(( *(p_text_buffer + offset - column) == '\r' 
		    || *(p_text_buffer + offset - column) == '\n')
		    &&(column != 0))
        {
            column--;
            break;
		}
		column++;
	};

    //printf("column: %zd\n", column);
	while(column > size.ws_col + start)
	{
		start += 10;
	}
    //printf("start: %zd\n", start);

	_S8 c;
    printf(" ");
	for(_S32 i = 0; i < size.ws_col - 1; i++)
	{
		c = *(p_text_buffer + offset - column + start + i);
		if(c == '\r' || c == '\n')
		{
			break;
		}
		
		printf("%c"NONE, c);
	}

	printf("\n");
	printf(" ");
	for(_S32 i = 0; i < column - start; i++)
	{
		printf(" ");
	}
	printf(LIGHT_GREEN"^"NONE"\n");
	
}

#ifdef CPPUTEST

_VOID display_token_and_line_to_file(const _S8 *p_text_buffer, size_t offset, FILE *pfw)
{
	S_V_ASSERT(p_text_buffer != NULL);
    S_V_FALSE(pfw != NULL);

	size_t start = 0;
	size_t column = 0;

    while((p_text_buffer + offset - column) != p_text_buffer)
	{
        if(( *(p_text_buffer + offset - column) == '\r' 
		    || *(p_text_buffer + offset - column) == '\n')
		    &&(column != 0))
        {
            column--;
            break;
		}
		column++;
	};

	_S8 c;
    fputc(' ', pfw);
	for(_S32 i = 0; ; i++)
	{
		c = *(p_text_buffer + offset - column + start + i);
        if(c == '\0')
        {
            break;
        }
        
		if(c == '\r' || c == '\n')
		{
			break;
		}
		
		fputc(c, pfw);
	}

	fputc('\n', pfw);
	fputc(' ', pfw);
	for(_S32 i = 0; i < column - start; i++)
	{
		fputc(' ', pfw);
	}
    fputc('^', pfw);
	fputc('\n', pfw);
	
}
#endif

