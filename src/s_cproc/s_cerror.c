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
	
	size_t start = 0;

	struct winsize size;
	ioctl(STDIN_FILENO,TIOCGWINSZ,&size);
	DEBUG_PRINT("%d\n",size.ws_col);
	DEBUG_PRINT("%d\n",size.ws_row);

	size_t column = 0;
	while((p_text_buffer + offset - column) != p_text_buffer 
		&& *(p_text_buffer + offset - column) != '\r' 
		&& *(p_text_buffer + offset - column) != '\n')
	{
		column++;
	};

	if(*(p_text_buffer + offset - column) == '\r' 
		|| *(p_text_buffer + offset - column) == '\n')
	{
		column -- ;
	};
	
	while(column - start > size.ws_col)
	{
		start += 10;
	}

	_S8 c;
	for(_S32 i = 0; i < size.ws_col; i++)
	{
		c = *(p_text_buffer + offset - column + i);
		if(c == '\r' || c == '\n')
		{
			break;
		}
		
		printf("%c", c);
	}

	printf("\n");
	
	for(_S32 i = 0; i < column - start; i++)
	{
		printf(" ");
	}
	printf(LIGHT_GREEN"^"NONE"\n");
	
}

