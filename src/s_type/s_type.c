#include <stdio.h>
#include <limits.h>
#include "s_mem.h"
#include "s_text.h"
#include "s_type.h"
#include "s_limits.h"
struct T {
    char a;
    int b;
    char c;
};

struct E {
};
#define _SET_LONG_DOUBLE_VALUE(ld, value) SET_LONG_DOUBLE_VALUE(ld, CONC_(value, U64), CONC_(value, L64))

void s_print_types(void)
{
    struct T t = {0x12,0x3456789a,0xbc};
    struct E e;
    printf("Size of struct T               sizeof(struct T)    %20zd\n", sizeof(t));
    DISPLAY_VAR_MEM_MULTI_LINES(t);

    printf("Size of struct E               sizeof(struct E)    %20zd\n", sizeof(e));
    DISPLAY_VAR_MEM_MULTI_LINES(e);
    
    printf("Size of char                   sizeof(char)        %20zd\n", sizeof(_SC));
    printf("Value of char                  CHAR_BIT            %20d    %20X\n", CHAR_BIT, CHAR_BIT);
    printf("Value of char max              CHAR_MAX            %20d    %20X\n", CHAR_MAX, CHAR_MAX);
    printf("Value of char min              CHAR_MIN            %20d    %20X\n", CHAR_MIN, CHAR_MIN);
    printf("Value of signed char max       SCHAR_MAX           %20d    %20X\n", SCHAR_MAX, SCHAR_MAX);
    printf("Value of signed char min       SCHAR_MIN           %20d    %20X\n", SCHAR_MIN, SCHAR_MIN);
    printf("Value of unsigned char max     UCHAR_MAX           %20u    %20X\n", UCHAR_MAX, UCHAR_MAX);

    printf("Size of short int              sizeof(short int)   %20zd\n", sizeof(_SS));
    printf("Value of short min             SHRT_MIN            %20d    %20X\n", SHRT_MIN, SHRT_MIN);
    printf("Value of short max             SHRT_MAX            %20d    %20X\n", SHRT_MAX, SHRT_MAX);
    printf("Value of unsigned short        USHRT_MAX           %20u    %20X\n", USHRT_MAX, USHRT_MAX);

    printf("Size of int                    sizeof(int)         %20zd\n", sizeof(_SI)); 
    printf("Value of int min               INT_MIN             %20d    %20X\n", INT_MIN, INT_MIN);
    printf("Value of int max               INT_MAX             %20d    %20X\n", INT_MAX, INT_MAX);
    printf("Value of unsigned int          UINT_MAX            %20u    %20X\n", UINT_MAX, UINT_MAX);

    printf("Size of long int               sizeof(long)        %20zd\n", sizeof(_SL));
    printf("Value of long min              LONG_MIN            %20ld    %20lX\n", LONG_MIN, LONG_MIN);
    printf("Value of long max              LONG_MAX            %20ld    %20lX\n", LONG_MAX, LONG_MAX);
    printf("Value of unsigned long         ULONG_MAX           %20lu    %20lX\n", ULONG_MAX, ULONG_MAX);

    printf("Size of long long int          sizeof(long long)   %20zd\n", sizeof(_SLL));  
    printf("Value of long long min         LLONG_MIN           %20lld    %20llX\n", LLONG_MIN, LLONG_MIN);
    printf("Value of long long max         LLONG_MAX           %20lld    %20llX\n", LLONG_MAX, LLONG_MAX);
    printf("Value of unsigned long long    ULLONG_MAX          %20llu    %20llX\n", ULLONG_MAX, ULLONG_MAX);


    _SF f;

    printf("Size of float                  sizeof(float)       %20zd\n", sizeof(_SF));

    SET_VAR_VALUE(f, FLOAT_MIN_MINUS);
    printf("Value of float min minus                           %20e    ", f); 
    DISPLAY_VAR_MEM_ONE_LINE(f);
    
    SET_VAR_VALUE(f, FLOAT_MAX_MINUS);
    printf("Value of float max minus                           %20e    ", f); 
    DISPLAY_VAR_MEM_ONE_LINE(f);
    
    SET_VAR_VALUE(f, FLOAT_MIN_PLUS);
    printf("Value of float min plus                            %20e    ", f); 
    DISPLAY_VAR_MEM_ONE_LINE(f);
    
    SET_VAR_VALUE(f, FLOAT_MAX_PLUS);
    printf("Value of float max plus                            %20e    ", f); 
    DISPLAY_VAR_MEM_ONE_LINE(f);
    
    SET_VAR_VALUE(f, FLOAT_ZERO_MINUS);
    printf("Value of float zero minus                          %20e    ", f); 
    DISPLAY_VAR_MEM_ONE_LINE(f);
    
    SET_VAR_VALUE(f, FLOAT_ZERO_PLUS);
    printf("Value of float zero plus                           %20e    ", f); 
    DISPLAY_VAR_MEM_ONE_LINE(f);
    
    SET_VAR_VALUE(f, FLOAT_INFINITY_MINUS);
    printf("Value of float infinity minus                      %20e    ", f); 
    DISPLAY_VAR_MEM_ONE_LINE(f);
    
    SET_VAR_VALUE(f, FLOAT_INFINITY_PLUS);
    printf("Value of float infinity plus                       %20e    ", f); 
    DISPLAY_VAR_MEM_ONE_LINE(f);
    
    SET_VAR_VALUE(f, FLOAT_NAN_MINUS);
    printf("Value of float NaN minus                           %20e    ", f); 
    DISPLAY_VAR_MEM_ONE_LINE(f);
    
    SET_VAR_VALUE(f, FLOAT_NAN_PLUS);
    printf("Value of float NaN plus                            %20e    ", f); 
    DISPLAY_VAR_MEM_ONE_LINE(f);
    


    double d;
    printf("Size of double                 sizeof(double)      %20zd\n", sizeof(double)); 
    
    SET_VAR_VALUE(d, DOUBLE_MIN_MINUS);
    printf("Value of double min minus                          %20e    ", d); 
    DISPLAY_VAR_MEM_ONE_LINE(d);

    SET_VAR_VALUE(d, DOUBLE_MAX_MINUS);
    printf("Value of double max minus                          %20e    ", d);
    DISPLAY_VAR_MEM_ONE_LINE(d);

    SET_VAR_VALUE(d, DOUBLE_MIN_PLUS);
    printf("Value of double min plus                           %20e    ", d); 
    DISPLAY_VAR_MEM_ONE_LINE(d);

    SET_VAR_VALUE(d, DOUBLE_MAX_PLUS);
    printf("Value of double max plus                           %20e    ", d);
    DISPLAY_VAR_MEM_ONE_LINE(d);

    SET_VAR_VALUE(d, DOUBLE_ZERO_MINUS);
    printf("Value of double zero minus                         %20e    ", d);
    DISPLAY_VAR_MEM_ONE_LINE(d);

    SET_VAR_VALUE(d, DOUBLE_ZERO_PLUS);
    printf("Value of double zero plus                          %20e    ", d);
    DISPLAY_VAR_MEM_ONE_LINE(d);

    SET_VAR_VALUE(d, DOUBLE_INFINITY_MINUS);
    printf("Value of double infinity minus                     %20e    ", d);
    DISPLAY_VAR_MEM_ONE_LINE(d);

    SET_VAR_VALUE(d, DOUBLE_INFINITY_PLUS);
    printf("Value of double infinity plus                      %20e    ", d);
    DISPLAY_VAR_MEM_ONE_LINE(d);

    SET_VAR_VALUE(d, DOUBLE_NAN_MINUS);
    printf("Value of double NaN minus                          %20e    ", d);
    DISPLAY_VAR_MEM_ONE_LINE(d);

    SET_VAR_VALUE(d, DOUBLE_NAN_PLUS);
    printf("Value of double NaN plus                           %20e    ", d);
    DISPLAY_VAR_MEM_ONE_LINE(d);

    long double ld;

    printf("Size of long double            sizeof(long double) %20zd\n", sizeof(long double));

    _SET_LONG_DOUBLE_VALUE(ld, LONG_DOUBLE_MIN_MINUS);
    printf("Value of long double min minus                     %20Le    ", ld); 
    DISPLAY_VAR_MEM_ONE_LINE(ld);

    _SET_LONG_DOUBLE_VALUE(ld, LONG_DOUBLE_MAX_MINUS);
    printf("Value of long double max minus                     %20Le    ", ld);;
    DISPLAY_VAR_MEM_ONE_LINE(ld);

    _SET_LONG_DOUBLE_VALUE(ld, LONG_DOUBLE_MIN_PLUS);
    printf("Value of long double min plus                      %20Le    ", ld);; 
    DISPLAY_VAR_MEM_ONE_LINE(ld);

    _SET_LONG_DOUBLE_VALUE(ld, LONG_DOUBLE_MAX_PLUS);
    printf("Value of long double max plus                      %20Le    ", ld);;
    DISPLAY_VAR_MEM_ONE_LINE(ld);

    _SET_LONG_DOUBLE_VALUE(ld, LONG_DOUBLE_ZERO_MINUS);
    printf("Value of long double zero minus                    %20Le    ", ld);;
    DISPLAY_VAR_MEM_ONE_LINE(ld);

    _SET_LONG_DOUBLE_VALUE(ld, LONG_DOUBLE_ZERO_PLUS);
    printf("Value of long double zero plus                     %20Le    ", ld);;
    DISPLAY_VAR_MEM_ONE_LINE(ld);

    _SET_LONG_DOUBLE_VALUE(ld, LONG_DOUBLE_INFINITY_MINUS);
    printf("Value of long double infinity minus                %20Le    ", ld);;
    DISPLAY_VAR_MEM_ONE_LINE(ld);

    _SET_LONG_DOUBLE_VALUE(ld, LONG_DOUBLE_INFINITY_PLUS);
    printf("Value of long double infinity plus                 %20Le    ", ld);;
    DISPLAY_VAR_MEM_ONE_LINE(ld);

    _SET_LONG_DOUBLE_VALUE(ld, LONG_DOUBLE_NAN_MINUS);
    printf("Value of long double NaN minus                     %20Le    ", ld);;
    DISPLAY_VAR_MEM_ONE_LINE(ld);

    _SET_LONG_DOUBLE_VALUE(ld, LONG_DOUBLE_NAN_PLUS);
    printf("Value of long double NaN plus                      %20Le    ", ld);;
    DISPLAY_VAR_MEM_ONE_LINE(ld);
    
    printf("Size of void                   sizeof(void)        %20zd    %18zX\n", sizeof(void), sizeof(void));         //x86 1   x64 1   
    printf("Size of void*                  sizeof(void*)       %20zd    %18zX\n", sizeof(void*), sizeof(void*));        //x86 4   x64 8   
    
    printf("Size of size_t                 sizeof(size_t)      %20zd    %18zX\n", sizeof(size_t), sizeof(size_t));       //x86 4   x64 8   
	
}

