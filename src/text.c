#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "defines.h"
#include "text.h"

#define IN 1 /* inside a word */
#define OUT 0 /* outside a word */

#define SWITCH_ENABLE 1
#define SWITCH_DISABLE 0

int output_switch = SWITCH_DISABLE;

void enable_output(void)
{
    output_switch = SWITCH_ENABLE;
}

void disable_output(void)
{
    output_switch = SWITCH_DISABLE;
}

/* count lines, words, and charactors in input */
int process_words(const char* filename, const char *separator)
{
    assert(filename != NULL);
    assert((separator != NULL && output_switch == SWITCH_ENABLE) 
        || (separator == NULL && output_switch == SWITCH_DISABLE));
    
    FILE* fpr = NULL;
    fpr = fopen(filename, "r");
	assert(fpr != NULL);
    FILE *fpw = NULL;
    char *filename_ouput = NULL;


    if(output_switch == SWITCH_ENABLE)
    {

        filename_ouput = malloc(strlen(filename) + strlen(".output") + 1);
        assert(filename_ouput != NULL);
        filename_ouput[0] = '\0';
        strcat(filename_ouput, filename);
        strcat(filename_ouput, ".output");
        fpw = fopen(filename_ouput, "w");
        assert(fpw != NULL);
    }
    
    int c, nl, nw, nc, state;

    state = OUT;
    nl = nw = nc = 0;

    while((c = fgetc(fpr)) != EOF)
    {
        ++nc;

        if(c == '\n')
        {
            ++nl;
        }

        if(c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f')
        {
            if(state == IN)
            {
                if(output_switch == SWITCH_ENABLE)
                {
                    fputs(separator, fpw);
                }
            }
            
            state = OUT;
        }
        else 
        {
            if(state == OUT)
            {
                state = IN;
                ++nw;
            }
            if(output_switch == SWITCH_ENABLE)
            {
                fputc(c, fpw);
            }
        }
    }

    //printf("%d, %d, %d\n", nl, nw, nc);
	fclose(fpr);
    if(output_switch == SWITCH_ENABLE)
    {
        fclose(fpw);
    }

    return nw;
}
int count_words(const char* filename)
{
    int retval;
    assert(filename != NULL);
    disable_output();

    retval = process_words(filename, NULL);

    return retval;
}

int format_words(const char* filename, const char *separator)
{
    int retval;
    assert(filename != NULL);
    assert(separator != NULL);

    enable_output();
    
    retval = process_words(filename, separator);

    disable_output();
    return retval;
}

