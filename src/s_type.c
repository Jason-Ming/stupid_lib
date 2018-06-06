#include <stdio.h>
#include <limits.h>
#include "s_mem.h"
#include "s_text.h"
#include "s_type.h"

#define SET_VAR_MEM_LONG_DOUBLE(dest, value)\
    {\
        long long int x = CONC(value, _L64);\
        memcpy(((void*)&(dest)), (void*)&(x), 8);\
        x = CONC(value, _U64);\
        memcpy(((void*)&(dest)) + 8, (void*)&(x), sizeof(dest) - 8);\
    }

struct T {
    char a;
    int b;
    char c;
};

struct E {
};

void print_types(void)
{
    struct T t = {0x12,0x3456789a,0xbc};
    struct E e;
    printf("Size of struct T               sizeof(struct T)    %24zd\n", sizeof(t));
    DISPLAY_VAR_MEM(t);

    printf("Size of struct E               sizeof(struct E)    %24zd\n", sizeof(e));
    DISPLAY_VAR_MEM(e);
    
    printf("Size of char                   sizeof(char)        %24zd\n", sizeof(char));
    printf("Value of char                  CHAR_BIT            %24d    %24X\n", CHAR_BIT, CHAR_BIT);
    printf("Value of char max              CHAR_MAX            %24d    %24X\n", CHAR_MAX, CHAR_MAX);
    printf("Value of char min              CHAR_MIN            %24d    %24X\n", CHAR_MIN, CHAR_MIN);
    printf("Value of signed char max       SCHAR_MAX           %24d    %24X\n", SCHAR_MAX, SCHAR_MAX);
    printf("Value of signed char min       SCHAR_MIN           %24d    %24X\n", SCHAR_MIN, SCHAR_MIN);
    printf("Value of unsigned char max     UCHAR_MAX           %24u    %24X\n", UCHAR_MAX, UCHAR_MAX);

    printf("Size of short int              sizeof(short int)   %24zd\n", sizeof(short int));
    printf("Value of short min             SHRT_MIN            %24d    %24X\n", SHRT_MIN, SHRT_MIN);
    printf("Value of short max             SHRT_MAX            %24d    %24X\n", SHRT_MAX, SHRT_MAX);
    printf("Value of unsigned short        USHRT_MAX           %24u    %24X\n", USHRT_MAX, USHRT_MAX);

    printf("Size of int                    sizeof(int)         %24zd\n", sizeof(int)); 
    printf("Value of int min               INT_MIN             %24d    %24X\n", INT_MIN, INT_MIN);
    printf("Value of int max               INT_MAX             %24d    %24X\n", INT_MAX, INT_MAX);
    printf("Value of unsigned int          UINT_MAX            %24u    %24X\n", UINT_MAX, UINT_MAX);

    printf("Size of long int               sizeof(long)        %24zd\n", sizeof(long));
    printf("Value of long min              LONG_MIN            %24ld    %24lX\n", LONG_MIN, LONG_MIN);
    printf("Value of long max              LONG_MAX            %24ld    %24lX\n", LONG_MAX, LONG_MAX);
    printf("Value of unsigned long         ULONG_MAX           %24lu    %24lX\n", ULONG_MAX, ULONG_MAX);

    printf("Size of long long int          sizeof(long long)   %24zd\n", sizeof(long long));  
    printf("Value of long long min         LLONG_MIN           %24lld    %24llX\n", LLONG_MIN, LLONG_MIN);
    printf("Value of long long max         LLONG_MAX           %24lld    %24llX\n", LLONG_MAX, LLONG_MAX);
    printf("Value of unsigned long long    ULLONG_MAX          %24llu    %24llX\n", ULLONG_MAX, ULLONG_MAX);


    float f;

    printf("Size of float                  sizeof(float)       %24zd\n", sizeof(float));

    SET_VAR_MEM(f, FLOAT_MIN_MINUS);
    printf("Value of float min minus                           %24e    %24lX\n", f, VALUE_LONG_UINT_OF_ADDR(&f)); 
    SET_VAR_MEM(f, FLOAT_MAX_MINUS);
    printf("Value of float max minus                           %24e    %24lX\n", f, VALUE_LONG_UINT_OF_ADDR(&f));
    SET_VAR_MEM(f, FLOAT_MIN_PLUS);
    printf("Value of float min plus                            %24e    %24lX\n", f, VALUE_LONG_UINT_OF_ADDR(&f)); 
    SET_VAR_MEM(f, FLOAT_MAX_PLUS);
    printf("Value of float max plus                            %24e    %24lX\n", f, VALUE_LONG_UINT_OF_ADDR(&f));
    SET_VAR_MEM(f, FLOAT_ZERO_MINUS);
    printf("Value of float zero minus                          %24e    %24lX\n", f, VALUE_LONG_UINT_OF_ADDR(&f));
    SET_VAR_MEM(f, FLOAT_ZERO_PLUS);
    printf("Value of float zero plus                           %24e    %24lX\n", f, VALUE_LONG_UINT_OF_ADDR(&f));
    SET_VAR_MEM(f, FLOAT_INFINITY_MINUS);
    printf("Value of float infinity minus                      %24e    %24lX\n", f, VALUE_LONG_UINT_OF_ADDR(&f));
    SET_VAR_MEM(f, FLOAT_INFINITY_PLUS);
    printf("Value of float infinity plus                       %24e    %24lX\n", f, VALUE_LONG_UINT_OF_ADDR(&f));
    SET_VAR_MEM(f, FLOAT_NAN_MINUS);
    printf("Value of float NaN minus                           %24e    %24lX\n", f, VALUE_LONG_UINT_OF_ADDR(&f));
    SET_VAR_MEM(f, FLOAT_NAN_PLUS);
    printf("Value of float NaN plus                            %24e    %24lX\n", f, VALUE_LONG_UINT_OF_ADDR(&f));


    double d;
    printf("Size of double                 sizeof(double)      %24zd\n", sizeof(double)); 
    
    SET_VAR_MEM(d, DOUBLE_MIN_MINUS);
    printf("Value of double min minus                          %24e    %24llX\n", d, VALUE_LONG_LONG_UINT_OF_ADDR(&d)); 
    //DISPLAY_VAR_MEM(d);

    SET_VAR_MEM(d, DOUBLE_MAX_MINUS);
    printf("Value of double max minus                          %24e    %24llX\n", d, VALUE_LONG_LONG_UINT_OF_ADDR(&d));
    //DISPLAY_VAR_MEM(d);

    SET_VAR_MEM(d, DOUBLE_MIN_PLUS);
    printf("Value of double min plus                           %24e    %24llX\n", d, VALUE_LONG_LONG_UINT_OF_ADDR(&d)); 
    //DISPLAY_VAR_MEM(d);

    SET_VAR_MEM(d, DOUBLE_MAX_PLUS);
    printf("Value of double max plus                           %24e    %24llX\n", d, VALUE_LONG_LONG_UINT_OF_ADDR(&d));
    //DISPLAY_VAR_MEM(d);

    SET_VAR_MEM(d, DOUBLE_ZERO_MINUS);
    printf("Value of double zero minus                         %24e    %24llX\n", d, VALUE_LONG_LONG_UINT_OF_ADDR(&d));
    //DISPLAY_VAR_MEM(d);

    SET_VAR_MEM(d, DOUBLE_ZERO_PLUS);
    printf("Value of double zero plus                          %24e    %24llX\n", d, VALUE_LONG_LONG_UINT_OF_ADDR(&d));
    //DISPLAY_VAR_MEM(d);

    SET_VAR_MEM(d, DOUBLE_INFINITY_MINUS);
    printf("Value of double infinity minus                     %24e    %24llX\n", d, VALUE_LONG_LONG_UINT_OF_ADDR(&d));
    //DISPLAY_VAR_MEM(d);

    SET_VAR_MEM(d, DOUBLE_INFINITY_PLUS);
    printf("Value of double infinity plus                      %24e    %24llX\n", d, VALUE_LONG_LONG_UINT_OF_ADDR(&d));
    //DISPLAY_VAR_MEM(d);

    SET_VAR_MEM(d, DOUBLE_NAN_MINUS);
    printf("Value of double NaN minus                          %24e    %24llX\n", d, VALUE_LONG_LONG_UINT_OF_ADDR(&d));
    //DISPLAY_VAR_MEM(d);

    SET_VAR_MEM(d, DOUBLE_NAN_PLUS);
    printf("Value of double NaN plus                           %24e    %24llX\n", d, VALUE_LONG_LONG_UINT_OF_ADDR(&d));
    //DISPLAY_VAR_MEM(d);

    long double ld;

    printf("Size of long double            sizeof(long double) %24zd\n", sizeof(long double));

    SET_VAR_MEM_LONG_DOUBLE(ld, LONG_DOUBLE_MIN_MINUS);
    printf("Value of long double min minus                     %24Le    %08X%016llX\n", ld, VALUE_UINT_OF_ADDR((void*)&ld + 8), VALUE_LONG_LONG_UINT_OF_ADDR(&ld)); 
    //DISPLAY_VAR_MEM(ld);

    SET_VAR_MEM_LONG_DOUBLE(ld, LONG_DOUBLE_MAX_MINUS);
    printf("Value of long double max minus                     %24Le    %08X%016llX\n", ld, VALUE_UINT_OF_ADDR((void*)&ld + 8), VALUE_LONG_LONG_UINT_OF_ADDR(&ld));
    //DISPLAY_VAR_MEM(ld);

    SET_VAR_MEM_LONG_DOUBLE(ld, LONG_DOUBLE_MIN_PLUS);
    printf("Value of long double min plus                      %24Le    %08X%016llX\n", ld, VALUE_UINT_OF_ADDR((void*)&ld + 8), VALUE_LONG_LONG_UINT_OF_ADDR(&ld)); 
    //DISPLAY_VAR_MEM(ld);

    SET_VAR_MEM_LONG_DOUBLE(ld, LONG_DOUBLE_MAX_PLUS);
    printf("Value of long double max plus                      %24Le    %08X%016llX\n", ld, VALUE_UINT_OF_ADDR((void*)&ld + 8), VALUE_LONG_LONG_UINT_OF_ADDR(&ld));
    //DISPLAY_VAR_MEM(ld);

    SET_VAR_MEM_LONG_DOUBLE(ld, LONG_DOUBLE_ZERO_MINUS);
    printf("Value of long double zero minus                    %24Le    %08X%016llX\n", ld, VALUE_UINT_OF_ADDR((void*)&ld + 8), VALUE_LONG_LONG_UINT_OF_ADDR(&ld));
    //DISPLAY_VAR_MEM(ld);

    SET_VAR_MEM_LONG_DOUBLE(ld, LONG_DOUBLE_ZERO_PLUS);
    printf("Value of long double zero plus                     %24Le    %08X%016llX\n", ld, VALUE_UINT_OF_ADDR((void*)&ld + 8), VALUE_LONG_LONG_UINT_OF_ADDR(&ld));
    //DISPLAY_VAR_MEM(ld);

    SET_VAR_MEM_LONG_DOUBLE(ld, LONG_DOUBLE_INFINITY_MINUS);
    printf("Value of long double infinity minus                %24Le    %08X%016llX\n", ld, VALUE_UINT_OF_ADDR((void*)&ld + 8), VALUE_LONG_LONG_UINT_OF_ADDR(&ld));
    //DISPLAY_VAR_MEM(ld);

    SET_VAR_MEM_LONG_DOUBLE(ld, LONG_DOUBLE_INFINITY_PLUS);
    printf("Value of long double infinity plus                 %24Le    %08X%016llX\n", ld, VALUE_UINT_OF_ADDR((void*)&ld + 8), VALUE_LONG_LONG_UINT_OF_ADDR(&ld));
    //DISPLAY_VAR_MEM(ld);

    SET_VAR_MEM_LONG_DOUBLE(ld, LONG_DOUBLE_NAN_MINUS);
    printf("Value of long double NaN minus                     %24Le    %08X%016llX\n", ld, VALUE_UINT_OF_ADDR((void*)&ld + 8), VALUE_LONG_LONG_UINT_OF_ADDR(&ld));
    //DISPLAY_VAR_MEM(ld);

    SET_VAR_MEM_LONG_DOUBLE(ld, LONG_DOUBLE_NAN_PLUS);
    printf("Value of long double NaN plus                      %24Le    %08X%016llX\n", ld, VALUE_UINT_OF_ADDR((void*)&ld + 8), VALUE_LONG_LONG_UINT_OF_ADDR(&ld));
    //DISPLAY_VAR_MEM(ld);
    
    printf("Size of void                   sizeof(void)        %24zd    %18zX\n", sizeof(void), sizeof(void));         //x86 1   x64 1   
    printf("Size of void*                  sizeof(void*)       %24zd    %18zX\n", sizeof(void*), sizeof(void*));        //x86 4   x64 8   
    
    printf("Size of size_t                 sizeof(size_t)      %24zd    %18zX\n", sizeof(size_t), sizeof(size_t));       //x86 4   x64 8   
	
}

