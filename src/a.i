# 1 "s_parameters.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "s_parameters.c"
# 1 "/usr/include/stdio.h" 1 3 4
# 27 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/features.h" 1 3 4
# 367 "/usr/include/features.h" 3 4
# 1 "/usr/include/i386-linux-gnu/sys/cdefs.h" 1 3 4
# 410 "/usr/include/i386-linux-gnu/sys/cdefs.h" 3 4
# 1 "/usr/include/i386-linux-gnu/bits/wordsize.h" 1 3 4
# 411 "/usr/include/i386-linux-gnu/sys/cdefs.h" 2 3 4
# 368 "/usr/include/features.h" 2 3 4
# 391 "/usr/include/features.h" 3 4
# 1 "/usr/include/i386-linux-gnu/gnu/stubs.h" 1 3 4






# 1 "/usr/include/i386-linux-gnu/gnu/stubs-32.h" 1 3 4
# 8 "/usr/include/i386-linux-gnu/gnu/stubs.h" 2 3 4
# 392 "/usr/include/features.h" 2 3 4
# 28 "/usr/include/stdio.h" 2 3 4





# 1 "/usr/lib/gcc/i686-linux-gnu/5/include/stddef.h" 1 3 4
# 216 "/usr/lib/gcc/i686-linux-gnu/5/include/stddef.h" 3 4

# 216 "/usr/lib/gcc/i686-linux-gnu/5/include/stddef.h" 3 4
typedef unsigned int size_t;
# 34 "/usr/include/stdio.h" 2 3 4

# 1 "/usr/include/i386-linux-gnu/bits/types.h" 1 3 4
# 27 "/usr/include/i386-linux-gnu/bits/types.h" 3 4
# 1 "/usr/include/i386-linux-gnu/bits/wordsize.h" 1 3 4
# 28 "/usr/include/i386-linux-gnu/bits/types.h" 2 3 4


typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;


typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;




__extension__ typedef signed long long int __int64_t;
__extension__ typedef unsigned long long int __uint64_t;







__extension__ typedef long long int __quad_t;
__extension__ typedef unsigned long long int __u_quad_t;
# 121 "/usr/include/i386-linux-gnu/bits/types.h" 3 4
# 1 "/usr/include/i386-linux-gnu/bits/typesizes.h" 1 3 4
# 122 "/usr/include/i386-linux-gnu/bits/types.h" 2 3 4


__extension__ typedef __u_quad_t __dev_t;
__extension__ typedef unsigned int __uid_t;
__extension__ typedef unsigned int __gid_t;
__extension__ typedef unsigned long int __ino_t;
__extension__ typedef __u_quad_t __ino64_t;
__extension__ typedef unsigned int __mode_t;
__extension__ typedef unsigned int __nlink_t;
__extension__ typedef long int __off_t;
__extension__ typedef __quad_t __off64_t;
__extension__ typedef int __pid_t;
__extension__ typedef struct { int __val[2]; } __fsid_t;
__extension__ typedef long int __clock_t;
__extension__ typedef unsigned long int __rlim_t;
__extension__ typedef __u_quad_t __rlim64_t;
__extension__ typedef unsigned int __id_t;
__extension__ typedef long int __time_t;
__extension__ typedef unsigned int __useconds_t;
__extension__ typedef long int __suseconds_t;

__extension__ typedef int __daddr_t;
__extension__ typedef int __key_t;


__extension__ typedef int __clockid_t;


__extension__ typedef void * __timer_t;


__extension__ typedef long int __blksize_t;




__extension__ typedef long int __blkcnt_t;
__extension__ typedef __quad_t __blkcnt64_t;


__extension__ typedef unsigned long int __fsblkcnt_t;
__extension__ typedef __u_quad_t __fsblkcnt64_t;


__extension__ typedef unsigned long int __fsfilcnt_t;
__extension__ typedef __u_quad_t __fsfilcnt64_t;


__extension__ typedef int __fsword_t;

__extension__ typedef int __ssize_t;


__extension__ typedef long int __syscall_slong_t;

__extension__ typedef unsigned long int __syscall_ulong_t;



typedef __off64_t __loff_t;
typedef __quad_t *__qaddr_t;
typedef char *__caddr_t;


__extension__ typedef int __intptr_t;


__extension__ typedef unsigned int __socklen_t;
# 36 "/usr/include/stdio.h" 2 3 4
# 44 "/usr/include/stdio.h" 3 4
struct _IO_FILE;



typedef struct _IO_FILE FILE;





# 64 "/usr/include/stdio.h" 3 4
typedef struct _IO_FILE __FILE;
# 74 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/libio.h" 1 3 4
# 31 "/usr/include/libio.h" 3 4
# 1 "/usr/include/_G_config.h" 1 3 4
# 15 "/usr/include/_G_config.h" 3 4
# 1 "/usr/lib/gcc/i686-linux-gnu/5/include/stddef.h" 1 3 4
# 16 "/usr/include/_G_config.h" 2 3 4




# 1 "/usr/include/wchar.h" 1 3 4
# 82 "/usr/include/wchar.h" 3 4
typedef struct
{
  int __count;
  union
  {

    unsigned int __wch;



    char __wchb[4];
  } __value;
} __mbstate_t;
# 21 "/usr/include/_G_config.h" 2 3 4
typedef struct
{
  __off_t __pos;
  __mbstate_t __state;
} _G_fpos_t;
typedef struct
{
  __off64_t __pos;
  __mbstate_t __state;
} _G_fpos64_t;
# 32 "/usr/include/libio.h" 2 3 4
# 49 "/usr/include/libio.h" 3 4
# 1 "/usr/lib/gcc/i686-linux-gnu/5/include/stdarg.h" 1 3 4
# 40 "/usr/lib/gcc/i686-linux-gnu/5/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 50 "/usr/include/libio.h" 2 3 4
# 144 "/usr/include/libio.h" 3 4
struct _IO_jump_t; struct _IO_FILE;





typedef void _IO_lock_t;





struct _IO_marker {
  struct _IO_marker *_next;
  struct _IO_FILE *_sbuf;



  int _pos;
# 173 "/usr/include/libio.h" 3 4
};


enum __codecvt_result
{
  __codecvt_ok,
  __codecvt_partial,
  __codecvt_error,
  __codecvt_noconv
};
# 241 "/usr/include/libio.h" 3 4
struct _IO_FILE {
  int _flags;




  char* _IO_read_ptr;
  char* _IO_read_end;
  char* _IO_read_base;
  char* _IO_write_base;
  char* _IO_write_ptr;
  char* _IO_write_end;
  char* _IO_buf_base;
  char* _IO_buf_end;

  char *_IO_save_base;
  char *_IO_backup_base;
  char *_IO_save_end;

  struct _IO_marker *_markers;

  struct _IO_FILE *_chain;

  int _fileno;



  int _flags2;

  __off_t _old_offset;



  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];



  _IO_lock_t *_lock;
# 289 "/usr/include/libio.h" 3 4
  __off64_t _offset;







  void *__pad1;
  void *__pad2;
  void *__pad3;
  void *__pad4;

  size_t __pad5;
  int _mode;

  char _unused2[15 * sizeof (int) - 4 * sizeof (void *) - sizeof (size_t)];

};


typedef struct _IO_FILE _IO_FILE;


struct _IO_FILE_plus;

extern struct _IO_FILE_plus _IO_2_1_stdin_;
extern struct _IO_FILE_plus _IO_2_1_stdout_;
extern struct _IO_FILE_plus _IO_2_1_stderr_;
# 333 "/usr/include/libio.h" 3 4
typedef __ssize_t __io_read_fn (void *__cookie, char *__buf, size_t __nbytes);







typedef __ssize_t __io_write_fn (void *__cookie, const char *__buf,
     size_t __n);







typedef int __io_seek_fn (void *__cookie, __off64_t *__pos, int __w);


typedef int __io_close_fn (void *__cookie);
# 385 "/usr/include/libio.h" 3 4
extern int __underflow (_IO_FILE *);
extern int __uflow (_IO_FILE *);
extern int __overflow (_IO_FILE *, int);
# 429 "/usr/include/libio.h" 3 4
extern int _IO_getc (_IO_FILE *__fp);
extern int _IO_putc (int __c, _IO_FILE *__fp);
extern int _IO_feof (_IO_FILE *__fp) __attribute__ ((__nothrow__ , __leaf__));
extern int _IO_ferror (_IO_FILE *__fp) __attribute__ ((__nothrow__ , __leaf__));

extern int _IO_peekc_locked (_IO_FILE *__fp);





extern void _IO_flockfile (_IO_FILE *) __attribute__ ((__nothrow__ , __leaf__));
extern void _IO_funlockfile (_IO_FILE *) __attribute__ ((__nothrow__ , __leaf__));
extern int _IO_ftrylockfile (_IO_FILE *) __attribute__ ((__nothrow__ , __leaf__));
# 459 "/usr/include/libio.h" 3 4
extern int _IO_vfscanf (_IO_FILE * __restrict, const char * __restrict,
   __gnuc_va_list, int *__restrict);
extern int _IO_vfprintf (_IO_FILE *__restrict, const char *__restrict,
    __gnuc_va_list);
extern __ssize_t _IO_padn (_IO_FILE *, int, __ssize_t);
extern size_t _IO_sgetn (_IO_FILE *, void *, size_t);

extern __off64_t _IO_seekoff (_IO_FILE *, __off64_t, int, int);
extern __off64_t _IO_seekpos (_IO_FILE *, __off64_t, int);

extern void _IO_free_backup_area (_IO_FILE *) __attribute__ ((__nothrow__ , __leaf__));
# 75 "/usr/include/stdio.h" 2 3 4




typedef __gnuc_va_list va_list;
# 90 "/usr/include/stdio.h" 3 4
typedef __off_t off_t;
# 102 "/usr/include/stdio.h" 3 4
typedef __ssize_t ssize_t;







typedef _G_fpos_t fpos_t;




# 164 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/i386-linux-gnu/bits/stdio_lim.h" 1 3 4
# 165 "/usr/include/stdio.h" 2 3 4



extern struct _IO_FILE *stdin;
extern struct _IO_FILE *stdout;
extern struct _IO_FILE *stderr;







extern int remove (const char *__filename) __attribute__ ((__nothrow__ , __leaf__));

extern int rename (const char *__old, const char *__new) __attribute__ ((__nothrow__ , __leaf__));




extern int renameat (int __oldfd, const char *__old, int __newfd,
       const char *__new) __attribute__ ((__nothrow__ , __leaf__));








extern FILE *tmpfile (void) ;
# 209 "/usr/include/stdio.h" 3 4
extern char *tmpnam (char *__s) __attribute__ ((__nothrow__ , __leaf__)) ;





extern char *tmpnam_r (char *__s) __attribute__ ((__nothrow__ , __leaf__)) ;
# 227 "/usr/include/stdio.h" 3 4
extern char *tempnam (const char *__dir, const char *__pfx)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) ;








extern int fclose (FILE *__stream);




extern int fflush (FILE *__stream);

# 252 "/usr/include/stdio.h" 3 4
extern int fflush_unlocked (FILE *__stream);
# 266 "/usr/include/stdio.h" 3 4






extern FILE *fopen (const char *__restrict __filename,
      const char *__restrict __modes) ;




extern FILE *freopen (const char *__restrict __filename,
        const char *__restrict __modes,
        FILE *__restrict __stream) ;
# 295 "/usr/include/stdio.h" 3 4

# 306 "/usr/include/stdio.h" 3 4
extern FILE *fdopen (int __fd, const char *__modes) __attribute__ ((__nothrow__ , __leaf__)) ;
# 319 "/usr/include/stdio.h" 3 4
extern FILE *fmemopen (void *__s, size_t __len, const char *__modes)
  __attribute__ ((__nothrow__ , __leaf__)) ;




extern FILE *open_memstream (char **__bufloc, size_t *__sizeloc) __attribute__ ((__nothrow__ , __leaf__)) ;






extern void setbuf (FILE *__restrict __stream, char *__restrict __buf) __attribute__ ((__nothrow__ , __leaf__));



extern int setvbuf (FILE *__restrict __stream, char *__restrict __buf,
      int __modes, size_t __n) __attribute__ ((__nothrow__ , __leaf__));





extern void setbuffer (FILE *__restrict __stream, char *__restrict __buf,
         size_t __size) __attribute__ ((__nothrow__ , __leaf__));


extern void setlinebuf (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__));








extern int fprintf (FILE *__restrict __stream,
      const char *__restrict __format, ...);




extern int printf (const char *__restrict __format, ...);

extern int sprintf (char *__restrict __s,
      const char *__restrict __format, ...) __attribute__ ((__nothrow__));





extern int vfprintf (FILE *__restrict __s, const char *__restrict __format,
       __gnuc_va_list __arg);




extern int vprintf (const char *__restrict __format, __gnuc_va_list __arg);

extern int vsprintf (char *__restrict __s, const char *__restrict __format,
       __gnuc_va_list __arg) __attribute__ ((__nothrow__));





extern int snprintf (char *__restrict __s, size_t __maxlen,
       const char *__restrict __format, ...)
     __attribute__ ((__nothrow__)) __attribute__ ((__format__ (__printf__, 3, 4)));

extern int vsnprintf (char *__restrict __s, size_t __maxlen,
        const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__nothrow__)) __attribute__ ((__format__ (__printf__, 3, 0)));

# 412 "/usr/include/stdio.h" 3 4
extern int vdprintf (int __fd, const char *__restrict __fmt,
       __gnuc_va_list __arg)
     __attribute__ ((__format__ (__printf__, 2, 0)));
extern int dprintf (int __fd, const char *__restrict __fmt, ...)
     __attribute__ ((__format__ (__printf__, 2, 3)));








extern int fscanf (FILE *__restrict __stream,
     const char *__restrict __format, ...) ;




extern int scanf (const char *__restrict __format, ...) ;

extern int sscanf (const char *__restrict __s,
     const char *__restrict __format, ...) __attribute__ ((__nothrow__ , __leaf__));
# 443 "/usr/include/stdio.h" 3 4
extern int fscanf (FILE *__restrict __stream, const char *__restrict __format, ...) __asm__ ("" "__isoc99_fscanf")

                               ;
extern int scanf (const char *__restrict __format, ...) __asm__ ("" "__isoc99_scanf")
                              ;
extern int sscanf (const char *__restrict __s, const char *__restrict __format, ...) __asm__ ("" "__isoc99_sscanf") __attribute__ ((__nothrow__ , __leaf__))

                      ;
# 463 "/usr/include/stdio.h" 3 4








extern int vfscanf (FILE *__restrict __s, const char *__restrict __format,
      __gnuc_va_list __arg)
     __attribute__ ((__format__ (__scanf__, 2, 0))) ;





extern int vscanf (const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__format__ (__scanf__, 1, 0))) ;


extern int vsscanf (const char *__restrict __s,
      const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__format__ (__scanf__, 2, 0)));
# 494 "/usr/include/stdio.h" 3 4
extern int vfscanf (FILE *__restrict __s, const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("" "__isoc99_vfscanf")



     __attribute__ ((__format__ (__scanf__, 2, 0))) ;
extern int vscanf (const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("" "__isoc99_vscanf")

     __attribute__ ((__format__ (__scanf__, 1, 0))) ;
extern int vsscanf (const char *__restrict __s, const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("" "__isoc99_vsscanf") __attribute__ ((__nothrow__ , __leaf__))



     __attribute__ ((__format__ (__scanf__, 2, 0)));
# 522 "/usr/include/stdio.h" 3 4









extern int fgetc (FILE *__stream);
extern int getc (FILE *__stream);





extern int getchar (void);

# 550 "/usr/include/stdio.h" 3 4
extern int getc_unlocked (FILE *__stream);
extern int getchar_unlocked (void);
# 561 "/usr/include/stdio.h" 3 4
extern int fgetc_unlocked (FILE *__stream);











extern int fputc (int __c, FILE *__stream);
extern int putc (int __c, FILE *__stream);





extern int putchar (int __c);

# 594 "/usr/include/stdio.h" 3 4
extern int fputc_unlocked (int __c, FILE *__stream);







extern int putc_unlocked (int __c, FILE *__stream);
extern int putchar_unlocked (int __c);






extern int getw (FILE *__stream);


extern int putw (int __w, FILE *__stream);








extern char *fgets (char *__restrict __s, int __n, FILE *__restrict __stream)
     ;
# 640 "/usr/include/stdio.h" 3 4

# 665 "/usr/include/stdio.h" 3 4
extern __ssize_t __getdelim (char **__restrict __lineptr,
          size_t *__restrict __n, int __delimiter,
          FILE *__restrict __stream) ;
extern __ssize_t getdelim (char **__restrict __lineptr,
        size_t *__restrict __n, int __delimiter,
        FILE *__restrict __stream) ;







extern __ssize_t getline (char **__restrict __lineptr,
       size_t *__restrict __n,
       FILE *__restrict __stream) ;








extern int fputs (const char *__restrict __s, FILE *__restrict __stream);





extern int puts (const char *__s);






extern int ungetc (int __c, FILE *__stream);






extern size_t fread (void *__restrict __ptr, size_t __size,
       size_t __n, FILE *__restrict __stream) ;




extern size_t fwrite (const void *__restrict __ptr, size_t __size,
        size_t __n, FILE *__restrict __s);

# 737 "/usr/include/stdio.h" 3 4
extern size_t fread_unlocked (void *__restrict __ptr, size_t __size,
         size_t __n, FILE *__restrict __stream) ;
extern size_t fwrite_unlocked (const void *__restrict __ptr, size_t __size,
          size_t __n, FILE *__restrict __stream);








extern int fseek (FILE *__stream, long int __off, int __whence);




extern long int ftell (FILE *__stream) ;




extern void rewind (FILE *__stream);

# 773 "/usr/include/stdio.h" 3 4
extern int fseeko (FILE *__stream, __off_t __off, int __whence);




extern __off_t ftello (FILE *__stream) ;
# 792 "/usr/include/stdio.h" 3 4






extern int fgetpos (FILE *__restrict __stream, fpos_t *__restrict __pos);




extern int fsetpos (FILE *__stream, const fpos_t *__pos);
# 815 "/usr/include/stdio.h" 3 4

# 824 "/usr/include/stdio.h" 3 4


extern void clearerr (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__));

extern int feof (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;

extern int ferror (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;




extern void clearerr_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__));
extern int feof_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;
extern int ferror_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;








extern void perror (const char *__s);






# 1 "/usr/include/i386-linux-gnu/bits/sys_errlist.h" 1 3 4
# 26 "/usr/include/i386-linux-gnu/bits/sys_errlist.h" 3 4
extern int sys_nerr;
extern const char *const sys_errlist[];
# 854 "/usr/include/stdio.h" 2 3 4




extern int fileno (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;




extern int fileno_unlocked (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;
# 872 "/usr/include/stdio.h" 3 4
extern FILE *popen (const char *__command, const char *__modes) ;





extern int pclose (FILE *__stream);





extern char *ctermid (char *__s) __attribute__ ((__nothrow__ , __leaf__));
# 912 "/usr/include/stdio.h" 3 4
extern void flockfile (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__));



extern int ftrylockfile (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__)) ;


extern void funlockfile (FILE *__stream) __attribute__ ((__nothrow__ , __leaf__));
# 942 "/usr/include/stdio.h" 3 4

# 2 "s_parameters.c" 2
# 1 "/usr/include/stdlib.h" 1 3 4
# 32 "/usr/include/stdlib.h" 3 4
# 1 "/usr/lib/gcc/i686-linux-gnu/5/include/stddef.h" 1 3 4
# 328 "/usr/lib/gcc/i686-linux-gnu/5/include/stddef.h" 3 4
typedef long int wchar_t;
# 33 "/usr/include/stdlib.h" 2 3 4








# 1 "/usr/include/i386-linux-gnu/bits/waitflags.h" 1 3 4
# 50 "/usr/include/i386-linux-gnu/bits/waitflags.h" 3 4
typedef enum
{
  P_ALL,
  P_PID,
  P_PGID
} idtype_t;
# 42 "/usr/include/stdlib.h" 2 3 4
# 1 "/usr/include/i386-linux-gnu/bits/waitstatus.h" 1 3 4
# 64 "/usr/include/i386-linux-gnu/bits/waitstatus.h" 3 4
# 1 "/usr/include/endian.h" 1 3 4
# 36 "/usr/include/endian.h" 3 4
# 1 "/usr/include/i386-linux-gnu/bits/endian.h" 1 3 4
# 37 "/usr/include/endian.h" 2 3 4
# 60 "/usr/include/endian.h" 3 4
# 1 "/usr/include/i386-linux-gnu/bits/byteswap.h" 1 3 4
# 28 "/usr/include/i386-linux-gnu/bits/byteswap.h" 3 4
# 1 "/usr/include/i386-linux-gnu/bits/wordsize.h" 1 3 4
# 29 "/usr/include/i386-linux-gnu/bits/byteswap.h" 2 3 4






# 1 "/usr/include/i386-linux-gnu/bits/byteswap-16.h" 1 3 4
# 36 "/usr/include/i386-linux-gnu/bits/byteswap.h" 2 3 4
# 44 "/usr/include/i386-linux-gnu/bits/byteswap.h" 3 4
static __inline unsigned int
__bswap_32 (unsigned int __bsx)
{
  return __builtin_bswap32 (__bsx);
}
# 108 "/usr/include/i386-linux-gnu/bits/byteswap.h" 3 4
static __inline __uint64_t
__bswap_64 (__uint64_t __bsx)
{
  return __builtin_bswap64 (__bsx);
}
# 61 "/usr/include/endian.h" 2 3 4
# 65 "/usr/include/i386-linux-gnu/bits/waitstatus.h" 2 3 4

union wait
  {
    int w_status;
    struct
      {

 unsigned int __w_termsig:7;
 unsigned int __w_coredump:1;
 unsigned int __w_retcode:8;
 unsigned int:16;







      } __wait_terminated;
    struct
      {

 unsigned int __w_stopval:8;
 unsigned int __w_stopsig:8;
 unsigned int:16;






      } __wait_stopped;
  };
# 43 "/usr/include/stdlib.h" 2 3 4
# 67 "/usr/include/stdlib.h" 3 4
typedef union
  {
    union wait *__uptr;
    int *__iptr;
  } __WAIT_STATUS __attribute__ ((__transparent_union__));
# 95 "/usr/include/stdlib.h" 3 4


typedef struct
  {
    int quot;
    int rem;
  } div_t;



typedef struct
  {
    long int quot;
    long int rem;
  } ldiv_t;







__extension__ typedef struct
  {
    long long int quot;
    long long int rem;
  } lldiv_t;


# 139 "/usr/include/stdlib.h" 3 4
extern size_t __ctype_get_mb_cur_max (void) __attribute__ ((__nothrow__ , __leaf__)) ;




extern double atof (const char *__nptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) ;

extern int atoi (const char *__nptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) ;

extern long int atol (const char *__nptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) ;





__extension__ extern long long int atoll (const char *__nptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) ;





extern double strtod (const char *__restrict __nptr,
        char **__restrict __endptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));





extern float strtof (const char *__restrict __nptr,
       char **__restrict __endptr) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

extern long double strtold (const char *__restrict __nptr,
       char **__restrict __endptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));





extern long int strtol (const char *__restrict __nptr,
   char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

extern unsigned long int strtoul (const char *__restrict __nptr,
      char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));




__extension__
extern long long int strtoq (const char *__restrict __nptr,
        char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

__extension__
extern unsigned long long int strtouq (const char *__restrict __nptr,
           char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));





__extension__
extern long long int strtoll (const char *__restrict __nptr,
         char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

__extension__
extern unsigned long long int strtoull (const char *__restrict __nptr,
     char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));

# 305 "/usr/include/stdlib.h" 3 4
extern char *l64a (long int __n) __attribute__ ((__nothrow__ , __leaf__)) ;


extern long int a64l (const char *__s)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) ;




# 1 "/usr/include/i386-linux-gnu/sys/types.h" 1 3 4
# 27 "/usr/include/i386-linux-gnu/sys/types.h" 3 4






typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;




typedef __loff_t loff_t;



typedef __ino_t ino_t;
# 60 "/usr/include/i386-linux-gnu/sys/types.h" 3 4
typedef __dev_t dev_t;




typedef __gid_t gid_t;




typedef __mode_t mode_t;




typedef __nlink_t nlink_t;




typedef __uid_t uid_t;
# 98 "/usr/include/i386-linux-gnu/sys/types.h" 3 4
typedef __pid_t pid_t;





typedef __id_t id_t;
# 115 "/usr/include/i386-linux-gnu/sys/types.h" 3 4
typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;





typedef __key_t key_t;
# 132 "/usr/include/i386-linux-gnu/sys/types.h" 3 4
# 1 "/usr/include/time.h" 1 3 4
# 57 "/usr/include/time.h" 3 4


typedef __clock_t clock_t;



# 73 "/usr/include/time.h" 3 4


typedef __time_t time_t;



# 91 "/usr/include/time.h" 3 4
typedef __clockid_t clockid_t;
# 103 "/usr/include/time.h" 3 4
typedef __timer_t timer_t;
# 133 "/usr/include/i386-linux-gnu/sys/types.h" 2 3 4
# 146 "/usr/include/i386-linux-gnu/sys/types.h" 3 4
# 1 "/usr/lib/gcc/i686-linux-gnu/5/include/stddef.h" 1 3 4
# 147 "/usr/include/i386-linux-gnu/sys/types.h" 2 3 4



typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;
# 194 "/usr/include/i386-linux-gnu/sys/types.h" 3 4
typedef int int8_t __attribute__ ((__mode__ (__QI__)));
typedef int int16_t __attribute__ ((__mode__ (__HI__)));
typedef int int32_t __attribute__ ((__mode__ (__SI__)));
typedef int int64_t __attribute__ ((__mode__ (__DI__)));


typedef unsigned int u_int8_t __attribute__ ((__mode__ (__QI__)));
typedef unsigned int u_int16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int u_int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int u_int64_t __attribute__ ((__mode__ (__DI__)));

typedef int register_t __attribute__ ((__mode__ (__word__)));
# 219 "/usr/include/i386-linux-gnu/sys/types.h" 3 4
# 1 "/usr/include/i386-linux-gnu/sys/select.h" 1 3 4
# 30 "/usr/include/i386-linux-gnu/sys/select.h" 3 4
# 1 "/usr/include/i386-linux-gnu/bits/select.h" 1 3 4
# 22 "/usr/include/i386-linux-gnu/bits/select.h" 3 4
# 1 "/usr/include/i386-linux-gnu/bits/wordsize.h" 1 3 4
# 23 "/usr/include/i386-linux-gnu/bits/select.h" 2 3 4
# 31 "/usr/include/i386-linux-gnu/sys/select.h" 2 3 4


# 1 "/usr/include/i386-linux-gnu/bits/sigset.h" 1 3 4
# 22 "/usr/include/i386-linux-gnu/bits/sigset.h" 3 4
typedef int __sig_atomic_t;




typedef struct
  {
    unsigned long int __val[(1024 / (8 * sizeof (unsigned long int)))];
  } __sigset_t;
# 34 "/usr/include/i386-linux-gnu/sys/select.h" 2 3 4



typedef __sigset_t sigset_t;





# 1 "/usr/include/time.h" 1 3 4
# 120 "/usr/include/time.h" 3 4
struct timespec
  {
    __time_t tv_sec;
    __syscall_slong_t tv_nsec;
  };
# 44 "/usr/include/i386-linux-gnu/sys/select.h" 2 3 4

# 1 "/usr/include/i386-linux-gnu/bits/time.h" 1 3 4
# 30 "/usr/include/i386-linux-gnu/bits/time.h" 3 4
struct timeval
  {
    __time_t tv_sec;
    __suseconds_t tv_usec;
  };
# 46 "/usr/include/i386-linux-gnu/sys/select.h" 2 3 4


typedef __suseconds_t suseconds_t;





typedef long int __fd_mask;
# 64 "/usr/include/i386-linux-gnu/sys/select.h" 3 4
typedef struct
  {






    __fd_mask __fds_bits[1024 / (8 * (int) sizeof (__fd_mask))];


  } fd_set;






typedef __fd_mask fd_mask;
# 96 "/usr/include/i386-linux-gnu/sys/select.h" 3 4

# 106 "/usr/include/i386-linux-gnu/sys/select.h" 3 4
extern int select (int __nfds, fd_set *__restrict __readfds,
     fd_set *__restrict __writefds,
     fd_set *__restrict __exceptfds,
     struct timeval *__restrict __timeout);
# 118 "/usr/include/i386-linux-gnu/sys/select.h" 3 4
extern int pselect (int __nfds, fd_set *__restrict __readfds,
      fd_set *__restrict __writefds,
      fd_set *__restrict __exceptfds,
      const struct timespec *__restrict __timeout,
      const __sigset_t *__restrict __sigmask);
# 131 "/usr/include/i386-linux-gnu/sys/select.h" 3 4

# 220 "/usr/include/i386-linux-gnu/sys/types.h" 2 3 4


# 1 "/usr/include/i386-linux-gnu/sys/sysmacros.h" 1 3 4
# 24 "/usr/include/i386-linux-gnu/sys/sysmacros.h" 3 4


__extension__
extern unsigned int gnu_dev_major (unsigned long long int __dev)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
__extension__
extern unsigned int gnu_dev_minor (unsigned long long int __dev)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
__extension__
extern unsigned long long int gnu_dev_makedev (unsigned int __major,
            unsigned int __minor)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
# 58 "/usr/include/i386-linux-gnu/sys/sysmacros.h" 3 4

# 223 "/usr/include/i386-linux-gnu/sys/types.h" 2 3 4





typedef __blksize_t blksize_t;






typedef __blkcnt_t blkcnt_t;



typedef __fsblkcnt_t fsblkcnt_t;



typedef __fsfilcnt_t fsfilcnt_t;
# 270 "/usr/include/i386-linux-gnu/sys/types.h" 3 4
# 1 "/usr/include/i386-linux-gnu/bits/pthreadtypes.h" 1 3 4
# 21 "/usr/include/i386-linux-gnu/bits/pthreadtypes.h" 3 4
# 1 "/usr/include/i386-linux-gnu/bits/wordsize.h" 1 3 4
# 22 "/usr/include/i386-linux-gnu/bits/pthreadtypes.h" 2 3 4
# 60 "/usr/include/i386-linux-gnu/bits/pthreadtypes.h" 3 4
typedef unsigned long int pthread_t;


union pthread_attr_t
{
  char __size[36];
  long int __align;
};

typedef union pthread_attr_t pthread_attr_t;
# 81 "/usr/include/i386-linux-gnu/bits/pthreadtypes.h" 3 4
typedef struct __pthread_internal_slist
{
  struct __pthread_internal_slist *__next;
} __pthread_slist_t;





typedef union
{
  struct __pthread_mutex_s
  {
    int __lock;
    unsigned int __count;
    int __owner;





    int __kind;
# 111 "/usr/include/i386-linux-gnu/bits/pthreadtypes.h" 3 4
    unsigned int __nusers;
    __extension__ union
    {
      struct
      {
 short __espins;
 short __elision;



      } __elision_data;
      __pthread_slist_t __list;
    };

  } __data;
  char __size[24];
  long int __align;
} pthread_mutex_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_mutexattr_t;




typedef union
{
  struct
  {
    int __lock;
    unsigned int __futex;
    __extension__ unsigned long long int __total_seq;
    __extension__ unsigned long long int __wakeup_seq;
    __extension__ unsigned long long int __woken_seq;
    void *__mutex;
    unsigned int __nwaiters;
    unsigned int __broadcast_seq;
  } __data;
  char __size[48];
  __extension__ long long int __align;
} pthread_cond_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_condattr_t;



typedef unsigned int pthread_key_t;



typedef int pthread_once_t;





typedef union
{
# 202 "/usr/include/i386-linux-gnu/bits/pthreadtypes.h" 3 4
  struct
  {
    int __lock;
    unsigned int __nr_readers;
    unsigned int __readers_wakeup;
    unsigned int __writer_wakeup;
    unsigned int __nr_readers_queued;
    unsigned int __nr_writers_queued;


    unsigned char __flags;
    unsigned char __shared;
    signed char __rwelision;

    unsigned char __pad2;
    int __writer;
  } __data;

  char __size[32];
  long int __align;
} pthread_rwlock_t;

typedef union
{
  char __size[8];
  long int __align;
} pthread_rwlockattr_t;





typedef volatile int pthread_spinlock_t;




typedef union
{
  char __size[20];
  long int __align;
} pthread_barrier_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_barrierattr_t;
# 271 "/usr/include/i386-linux-gnu/sys/types.h" 2 3 4



# 315 "/usr/include/stdlib.h" 2 3 4






extern long int random (void) __attribute__ ((__nothrow__ , __leaf__));


extern void srandom (unsigned int __seed) __attribute__ ((__nothrow__ , __leaf__));





extern char *initstate (unsigned int __seed, char *__statebuf,
   size_t __statelen) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));



extern char *setstate (char *__statebuf) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));







struct random_data
  {
    int32_t *fptr;
    int32_t *rptr;
    int32_t *state;
    int rand_type;
    int rand_deg;
    int rand_sep;
    int32_t *end_ptr;
  };

extern int random_r (struct random_data *__restrict __buf,
       int32_t *__restrict __result) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));

extern int srandom_r (unsigned int __seed, struct random_data *__buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));

extern int initstate_r (unsigned int __seed, char *__restrict __statebuf,
   size_t __statelen,
   struct random_data *__restrict __buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2, 4)));

extern int setstate_r (char *__restrict __statebuf,
         struct random_data *__restrict __buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));






extern int rand (void) __attribute__ ((__nothrow__ , __leaf__));

extern void srand (unsigned int __seed) __attribute__ ((__nothrow__ , __leaf__));




extern int rand_r (unsigned int *__seed) __attribute__ ((__nothrow__ , __leaf__));







extern double drand48 (void) __attribute__ ((__nothrow__ , __leaf__));
extern double erand48 (unsigned short int __xsubi[3]) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern long int lrand48 (void) __attribute__ ((__nothrow__ , __leaf__));
extern long int nrand48 (unsigned short int __xsubi[3])
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern long int mrand48 (void) __attribute__ ((__nothrow__ , __leaf__));
extern long int jrand48 (unsigned short int __xsubi[3])
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern void srand48 (long int __seedval) __attribute__ ((__nothrow__ , __leaf__));
extern unsigned short int *seed48 (unsigned short int __seed16v[3])
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
extern void lcong48 (unsigned short int __param[7]) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));





struct drand48_data
  {
    unsigned short int __x[3];
    unsigned short int __old_x[3];
    unsigned short int __c;
    unsigned short int __init;
    __extension__ unsigned long long int __a;

  };


extern int drand48_r (struct drand48_data *__restrict __buffer,
        double *__restrict __result) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
extern int erand48_r (unsigned short int __xsubi[3],
        struct drand48_data *__restrict __buffer,
        double *__restrict __result) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern int lrand48_r (struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
extern int nrand48_r (unsigned short int __xsubi[3],
        struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern int mrand48_r (struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
extern int jrand48_r (unsigned short int __xsubi[3],
        struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern int srand48_r (long int __seedval, struct drand48_data *__buffer)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));

extern int seed48_r (unsigned short int __seed16v[3],
       struct drand48_data *__buffer) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));

extern int lcong48_r (unsigned short int __param[7],
        struct drand48_data *__buffer)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));









extern void *malloc (size_t __size) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) ;

extern void *calloc (size_t __nmemb, size_t __size)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) ;










extern void *realloc (void *__ptr, size_t __size)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__warn_unused_result__));

extern void free (void *__ptr) __attribute__ ((__nothrow__ , __leaf__));




extern void cfree (void *__ptr) __attribute__ ((__nothrow__ , __leaf__));



# 1 "/usr/include/alloca.h" 1 3 4
# 24 "/usr/include/alloca.h" 3 4
# 1 "/usr/lib/gcc/i686-linux-gnu/5/include/stddef.h" 1 3 4
# 25 "/usr/include/alloca.h" 2 3 4







extern void *alloca (size_t __size) __attribute__ ((__nothrow__ , __leaf__));






# 493 "/usr/include/stdlib.h" 2 3 4





extern void *valloc (size_t __size) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) ;




extern int posix_memalign (void **__memptr, size_t __alignment, size_t __size)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;




extern void *aligned_alloc (size_t __alignment, size_t __size)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) __attribute__ ((__alloc_size__ (2))) ;




extern void abort (void) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));



extern int atexit (void (*__func) (void)) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));







extern int at_quick_exit (void (*__func) (void)) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));







extern int on_exit (void (*__func) (int __status, void *__arg), void *__arg)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));






extern void exit (int __status) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));





extern void quick_exit (int __status) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));







extern void _Exit (int __status) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));






extern char *getenv (const char *__name) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;

# 578 "/usr/include/stdlib.h" 3 4
extern int putenv (char *__string) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));





extern int setenv (const char *__name, const char *__value, int __replace)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));


extern int unsetenv (const char *__name) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));






extern int clearenv (void) __attribute__ ((__nothrow__ , __leaf__));
# 606 "/usr/include/stdlib.h" 3 4
extern char *mktemp (char *__template) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));
# 619 "/usr/include/stdlib.h" 3 4
extern int mkstemp (char *__template) __attribute__ ((__nonnull__ (1))) ;
# 641 "/usr/include/stdlib.h" 3 4
extern int mkstemps (char *__template, int __suffixlen) __attribute__ ((__nonnull__ (1))) ;
# 662 "/usr/include/stdlib.h" 3 4
extern char *mkdtemp (char *__template) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;
# 711 "/usr/include/stdlib.h" 3 4





extern int system (const char *__command) ;

# 733 "/usr/include/stdlib.h" 3 4
extern char *realpath (const char *__restrict __name,
         char *__restrict __resolved) __attribute__ ((__nothrow__ , __leaf__)) ;






typedef int (*__compar_fn_t) (const void *, const void *);
# 751 "/usr/include/stdlib.h" 3 4



extern void *bsearch (const void *__key, const void *__base,
        size_t __nmemb, size_t __size, __compar_fn_t __compar)
     __attribute__ ((__nonnull__ (1, 2, 5))) ;







extern void qsort (void *__base, size_t __nmemb, size_t __size,
     __compar_fn_t __compar) __attribute__ ((__nonnull__ (1, 4)));
# 774 "/usr/include/stdlib.h" 3 4
extern int abs (int __x) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;
extern long int labs (long int __x) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;



__extension__ extern long long int llabs (long long int __x)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;







extern div_t div (int __numer, int __denom)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;
extern ldiv_t ldiv (long int __numer, long int __denom)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;




__extension__ extern lldiv_t lldiv (long long int __numer,
        long long int __denom)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__)) ;

# 811 "/usr/include/stdlib.h" 3 4
extern char *ecvt (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4))) ;




extern char *fcvt (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4))) ;




extern char *gcvt (double __value, int __ndigit, char *__buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3))) ;




extern char *qecvt (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4))) ;
extern char *qfcvt (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4))) ;
extern char *qgcvt (long double __value, int __ndigit, char *__buf)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3))) ;




extern int ecvt_r (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign, char *__restrict __buf,
     size_t __len) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4, 5)));
extern int fcvt_r (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign, char *__restrict __buf,
     size_t __len) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4, 5)));

extern int qecvt_r (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign,
      char *__restrict __buf, size_t __len)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4, 5)));
extern int qfcvt_r (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign,
      char *__restrict __buf, size_t __len)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (3, 4, 5)));






extern int mblen (const char *__s, size_t __n) __attribute__ ((__nothrow__ , __leaf__));


extern int mbtowc (wchar_t *__restrict __pwc,
     const char *__restrict __s, size_t __n) __attribute__ ((__nothrow__ , __leaf__));


extern int wctomb (char *__s, wchar_t __wchar) __attribute__ ((__nothrow__ , __leaf__));



extern size_t mbstowcs (wchar_t *__restrict __pwcs,
   const char *__restrict __s, size_t __n) __attribute__ ((__nothrow__ , __leaf__));

extern size_t wcstombs (char *__restrict __s,
   const wchar_t *__restrict __pwcs, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__));








extern int rpmatch (const char *__response) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1))) ;
# 898 "/usr/include/stdlib.h" 3 4
extern int getsubopt (char **__restrict __optionp,
        char *const *__restrict __tokens,
        char **__restrict __valuep)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2, 3))) ;
# 950 "/usr/include/stdlib.h" 3 4
extern int getloadavg (double __loadavg[], int __nelem)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


# 1 "/usr/include/i386-linux-gnu/bits/stdlib-float.h" 1 3 4
# 955 "/usr/include/stdlib.h" 2 3 4
# 967 "/usr/include/stdlib.h" 3 4

# 3 "s_parameters.c" 2
# 1 "/usr/include/string.h" 1 3 4
# 27 "/usr/include/string.h" 3 4





# 1 "/usr/lib/gcc/i686-linux-gnu/5/include/stddef.h" 1 3 4
# 33 "/usr/include/string.h" 2 3 4









extern void *memcpy (void *__restrict __dest, const void *__restrict __src,
       size_t __n) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern void *memmove (void *__dest, const void *__src, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));






extern void *memccpy (void *__restrict __dest, const void *__restrict __src,
        int __c, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));





extern void *memset (void *__s, int __c, size_t __n) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern int memcmp (const void *__s1, const void *__s2, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 92 "/usr/include/string.h" 3 4
extern void *memchr (const void *__s, int __c, size_t __n)
      __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));


# 123 "/usr/include/string.h" 3 4


extern char *strcpy (char *__restrict __dest, const char *__restrict __src)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));

extern char *strncpy (char *__restrict __dest,
        const char *__restrict __src, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern char *strcat (char *__restrict __dest, const char *__restrict __src)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));

extern char *strncat (char *__restrict __dest, const char *__restrict __src,
        size_t __n) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern int strcmp (const char *__s1, const char *__s2)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));

extern int strncmp (const char *__s1, const char *__s2, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));


extern int strcoll (const char *__s1, const char *__s2)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));

extern size_t strxfrm (char *__restrict __dest,
         const char *__restrict __src, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));






# 1 "/usr/include/xlocale.h" 1 3 4
# 27 "/usr/include/xlocale.h" 3 4
typedef struct __locale_struct
{

  struct __locale_data *__locales[13];


  const unsigned short int *__ctype_b;
  const int *__ctype_tolower;
  const int *__ctype_toupper;


  const char *__names[13];
} *__locale_t;


typedef __locale_t locale_t;
# 160 "/usr/include/string.h" 2 3 4


extern int strcoll_l (const char *__s1, const char *__s2, __locale_t __l)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2, 3)));

extern size_t strxfrm_l (char *__dest, const char *__src, size_t __n,
    __locale_t __l) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2, 4)));




extern char *strdup (const char *__s)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) __attribute__ ((__nonnull__ (1)));






extern char *strndup (const char *__string, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__malloc__)) __attribute__ ((__nonnull__ (1)));
# 206 "/usr/include/string.h" 3 4

# 231 "/usr/include/string.h" 3 4
extern char *strchr (const char *__s, int __c)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));
# 258 "/usr/include/string.h" 3 4
extern char *strrchr (const char *__s, int __c)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));


# 277 "/usr/include/string.h" 3 4



extern size_t strcspn (const char *__s, const char *__reject)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));


extern size_t strspn (const char *__s, const char *__accept)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 310 "/usr/include/string.h" 3 4
extern char *strpbrk (const char *__s, const char *__accept)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 337 "/usr/include/string.h" 3 4
extern char *strstr (const char *__haystack, const char *__needle)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));




extern char *strtok (char *__restrict __s, const char *__restrict __delim)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2)));




extern char *__strtok_r (char *__restrict __s,
    const char *__restrict __delim,
    char **__restrict __save_ptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2, 3)));

extern char *strtok_r (char *__restrict __s, const char *__restrict __delim,
         char **__restrict __save_ptr)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (2, 3)));
# 392 "/usr/include/string.h" 3 4


extern size_t strlen (const char *__s)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));





extern size_t strnlen (const char *__string, size_t __maxlen)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));





extern char *strerror (int __errnum) __attribute__ ((__nothrow__ , __leaf__));

# 422 "/usr/include/string.h" 3 4
extern int strerror_r (int __errnum, char *__buf, size_t __buflen) __asm__ ("" "__xpg_strerror_r") __attribute__ ((__nothrow__ , __leaf__))

                        __attribute__ ((__nonnull__ (2)));
# 440 "/usr/include/string.h" 3 4
extern char *strerror_l (int __errnum, __locale_t __l) __attribute__ ((__nothrow__ , __leaf__));





extern void __bzero (void *__s, size_t __n) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));



extern void bcopy (const void *__src, void *__dest, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));


extern void bzero (void *__s, size_t __n) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1)));


extern int bcmp (const void *__s1, const void *__s2, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 484 "/usr/include/string.h" 3 4
extern char *index (const char *__s, int __c)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));
# 512 "/usr/include/string.h" 3 4
extern char *rindex (const char *__s, int __c)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));




extern int ffs (int __i) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
# 529 "/usr/include/string.h" 3 4
extern int strcasecmp (const char *__s1, const char *__s2)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));


extern int strncasecmp (const char *__s1, const char *__s2, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 552 "/usr/include/string.h" 3 4
extern char *strsep (char **__restrict __stringp,
       const char *__restrict __delim)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));




extern char *strsignal (int __sig) __attribute__ ((__nothrow__ , __leaf__));


extern char *__stpcpy (char *__restrict __dest, const char *__restrict __src)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
extern char *stpcpy (char *__restrict __dest, const char *__restrict __src)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));



extern char *__stpncpy (char *__restrict __dest,
   const char *__restrict __src, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
extern char *stpncpy (char *__restrict __dest,
        const char *__restrict __src, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
# 658 "/usr/include/string.h" 3 4

# 4 "s_parameters.c" 2
# 1 "/usr/include/ctype.h" 1 3 4
# 28 "/usr/include/ctype.h" 3 4

# 46 "/usr/include/ctype.h" 3 4
enum
{
  _ISupper = ((0) < 8 ? ((1 << (0)) << 8) : ((1 << (0)) >> 8)),
  _ISlower = ((1) < 8 ? ((1 << (1)) << 8) : ((1 << (1)) >> 8)),
  _ISalpha = ((2) < 8 ? ((1 << (2)) << 8) : ((1 << (2)) >> 8)),
  _ISdigit = ((3) < 8 ? ((1 << (3)) << 8) : ((1 << (3)) >> 8)),
  _ISxdigit = ((4) < 8 ? ((1 << (4)) << 8) : ((1 << (4)) >> 8)),
  _ISspace = ((5) < 8 ? ((1 << (5)) << 8) : ((1 << (5)) >> 8)),
  _ISprint = ((6) < 8 ? ((1 << (6)) << 8) : ((1 << (6)) >> 8)),
  _ISgraph = ((7) < 8 ? ((1 << (7)) << 8) : ((1 << (7)) >> 8)),
  _ISblank = ((8) < 8 ? ((1 << (8)) << 8) : ((1 << (8)) >> 8)),
  _IScntrl = ((9) < 8 ? ((1 << (9)) << 8) : ((1 << (9)) >> 8)),
  _ISpunct = ((10) < 8 ? ((1 << (10)) << 8) : ((1 << (10)) >> 8)),
  _ISalnum = ((11) < 8 ? ((1 << (11)) << 8) : ((1 << (11)) >> 8))
};
# 79 "/usr/include/ctype.h" 3 4
extern const unsigned short int **__ctype_b_loc (void)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
extern const __int32_t **__ctype_tolower_loc (void)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
extern const __int32_t **__ctype_toupper_loc (void)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__const__));
# 104 "/usr/include/ctype.h" 3 4






extern int isalnum (int) __attribute__ ((__nothrow__ , __leaf__));
extern int isalpha (int) __attribute__ ((__nothrow__ , __leaf__));
extern int iscntrl (int) __attribute__ ((__nothrow__ , __leaf__));
extern int isdigit (int) __attribute__ ((__nothrow__ , __leaf__));
extern int islower (int) __attribute__ ((__nothrow__ , __leaf__));
extern int isgraph (int) __attribute__ ((__nothrow__ , __leaf__));
extern int isprint (int) __attribute__ ((__nothrow__ , __leaf__));
extern int ispunct (int) __attribute__ ((__nothrow__ , __leaf__));
extern int isspace (int) __attribute__ ((__nothrow__ , __leaf__));
extern int isupper (int) __attribute__ ((__nothrow__ , __leaf__));
extern int isxdigit (int) __attribute__ ((__nothrow__ , __leaf__));



extern int tolower (int __c) __attribute__ ((__nothrow__ , __leaf__));


extern int toupper (int __c) __attribute__ ((__nothrow__ , __leaf__));








extern int isblank (int) __attribute__ ((__nothrow__ , __leaf__));


# 150 "/usr/include/ctype.h" 3 4
extern int isascii (int __c) __attribute__ ((__nothrow__ , __leaf__));



extern int toascii (int __c) __attribute__ ((__nothrow__ , __leaf__));



extern int _toupper (int) __attribute__ ((__nothrow__ , __leaf__));
extern int _tolower (int) __attribute__ ((__nothrow__ , __leaf__));
# 271 "/usr/include/ctype.h" 3 4
extern int isalnum_l (int, __locale_t) __attribute__ ((__nothrow__ , __leaf__));
extern int isalpha_l (int, __locale_t) __attribute__ ((__nothrow__ , __leaf__));
extern int iscntrl_l (int, __locale_t) __attribute__ ((__nothrow__ , __leaf__));
extern int isdigit_l (int, __locale_t) __attribute__ ((__nothrow__ , __leaf__));
extern int islower_l (int, __locale_t) __attribute__ ((__nothrow__ , __leaf__));
extern int isgraph_l (int, __locale_t) __attribute__ ((__nothrow__ , __leaf__));
extern int isprint_l (int, __locale_t) __attribute__ ((__nothrow__ , __leaf__));
extern int ispunct_l (int, __locale_t) __attribute__ ((__nothrow__ , __leaf__));
extern int isspace_l (int, __locale_t) __attribute__ ((__nothrow__ , __leaf__));
extern int isupper_l (int, __locale_t) __attribute__ ((__nothrow__ , __leaf__));
extern int isxdigit_l (int, __locale_t) __attribute__ ((__nothrow__ , __leaf__));

extern int isblank_l (int, __locale_t) __attribute__ ((__nothrow__ , __leaf__));



extern int __tolower_l (int __c, __locale_t __l) __attribute__ ((__nothrow__ , __leaf__));
extern int tolower_l (int __c, __locale_t __l) __attribute__ ((__nothrow__ , __leaf__));


extern int __toupper_l (int __c, __locale_t __l) __attribute__ ((__nothrow__ , __leaf__));
extern int toupper_l (int __c, __locale_t __l) __attribute__ ((__nothrow__ , __leaf__));
# 347 "/usr/include/ctype.h" 3 4

# 5 "s_parameters.c" 2

# 1 "../include/s_log.h" 1




# 1 "../include/s_clinkage.h" 1
# 6 "../include/s_log.h" 2
# 1 "../include/s_time.h" 1



# 1 "../include/s_clinkage.h" 1
# 5 "../include/s_time.h" 2




# 8 "../include/s_time.h"
char* get_date_string(void);
char* get_time_string(void);
char* get_time_stamp(void);


# 7 "../include/s_log.h" 2

char* log_getfn(void);
# 7 "s_parameters.c" 2
# 1 "../include/s_parameters.h" 1


# 1 "../include/s_clinkage.h" 1
# 4 "../include/s_parameters.h" 2
# 1 "../include/s_defines.h" 1


# 1 "/usr/lib/gcc/i686-linux-gnu/5/include-fixed/limits.h" 1 3 4
# 34 "/usr/lib/gcc/i686-linux-gnu/5/include-fixed/limits.h" 3 4
# 1 "/usr/lib/gcc/i686-linux-gnu/5/include-fixed/syslimits.h" 1 3 4






# 1 "/usr/lib/gcc/i686-linux-gnu/5/include-fixed/limits.h" 1 3 4
# 168 "/usr/lib/gcc/i686-linux-gnu/5/include-fixed/limits.h" 3 4
# 1 "/usr/include/limits.h" 1 3 4
# 143 "/usr/include/limits.h" 3 4
# 1 "/usr/include/i386-linux-gnu/bits/posix1_lim.h" 1 3 4
# 160 "/usr/include/i386-linux-gnu/bits/posix1_lim.h" 3 4
# 1 "/usr/include/i386-linux-gnu/bits/local_lim.h" 1 3 4
# 38 "/usr/include/i386-linux-gnu/bits/local_lim.h" 3 4
# 1 "/usr/include/linux/limits.h" 1 3 4
# 39 "/usr/include/i386-linux-gnu/bits/local_lim.h" 2 3 4
# 161 "/usr/include/i386-linux-gnu/bits/posix1_lim.h" 2 3 4
# 144 "/usr/include/limits.h" 2 3 4



# 1 "/usr/include/i386-linux-gnu/bits/posix2_lim.h" 1 3 4
# 148 "/usr/include/limits.h" 2 3 4
# 169 "/usr/lib/gcc/i686-linux-gnu/5/include-fixed/limits.h" 2 3 4
# 8 "/usr/lib/gcc/i686-linux-gnu/5/include-fixed/syslimits.h" 2 3 4
# 35 "/usr/lib/gcc/i686-linux-gnu/5/include-fixed/limits.h" 2 3 4
# 4 "../include/s_defines.h" 2


# 1 "../include/s_clinkage.h" 1
# 7 "../include/s_defines.h" 2

typedef enum TAG_ENUM_RETURN
{
    RETURN_SUCCESS = 0,
    RETURN_FAILURE = -1,
}ENUM_RETURN;

typedef enum TAG_ENUM_BOOLEAN
{
    BOOLEAN_TURE = 1,
    BOOLEAN_FALSE = 0,
}ENUM_BOOLEAN;
# 27 "../include/s_defines.h"
typedef int INT;
typedef unsigned int UINT;
typedef char* PSTR;
# 5 "../include/s_parameters.h" 2


typedef enum TAG_ENUM_OPTION_TYPE
{
    OPTION_TYPE_MANDATORY = 0,
    OPTION_TYPE_OPTIONAL,
    OPTION_TYPE_MAX,
}ENUM_OPTION_TYPE;

typedef enum TAG_ENUM_VALUE_TYPE
{
    VALUE_TYPE_SWITCH = 0,
    VALUE_TYPE_DATA,
    VALUE_TYPE_MAX,
}ENUM_VALUE_TYPE;

typedef struct TAG_STRU_VALUE
{
    char* value;
    struct TAG_STRU_VALUE *next;
} STRU_VALUE;

typedef ENUM_RETURN (*FUNC_OPTION_PROC)(struct TAG_STRU_VALUE *value);


ENUM_RETURN register_error_info(int code, const char * info);

ENUM_RETURN register_introduction(const char *introduction);
ENUM_RETURN register_usage(const char *usage);

ENUM_RETURN register_option(
    const char* option_name,
    ENUM_OPTION_TYPE option_type,
    ENUM_VALUE_TYPE value_type,
    FUNC_OPTION_PROC handler,
    const char* help_info);


ENUM_RETURN process(int argc, char **argv);

# 8 "s_parameters.c" 2



typedef struct TAG_STRU_ERROR_INFO
{
    int code;
    const char *info;
} STRU_ERROR_INFO;

typedef struct TAG_STRU_OPTION_PROC
{
    char* option;
    ENUM_OPTION_TYPE option_type;
    ENUM_BOOLEAN need_process;
    struct TAG_STRU_VALUE *value;
    ENUM_VALUE_TYPE value_type;
    FUNC_OPTION_PROC handler;
    char* help_info;
    struct TAG_STRU_OPTION_PROC *next;
} STRU_OPTION_PROC;

static STRU_OPTION_PROC *p_list_head = 
# 29 "s_parameters.c" 3 4
                                       ((void *)0)
# 29 "s_parameters.c"
                                           ;
static STRU_OPTION_PROC *p_list_tail = 
# 30 "s_parameters.c" 3 4
                                       ((void *)0)
# 30 "s_parameters.c"
                                           ;

static const char *p_bin = 
# 32 "s_parameters.c" 3 4
                           ((void *)0)
# 32 "s_parameters.c"
                               ;
static char *p_usage = 
# 33 "s_parameters.c" 3 4
                       ((void *)0)
# 33 "s_parameters.c"
                           ;
static char *p_introduction = 
# 34 "s_parameters.c" 3 4
                              ((void *)0)
# 34 "s_parameters.c"
                                  ;
static int error_code = 0;


static const char* error_info_array[200] = {
# 38 "s_parameters.c" 3 4
                                                              ((void *)0)
# 38 "s_parameters.c"
                                                                  };


static int num_of_input_files = 0;
static char* input_files[64] = {
# 42 "s_parameters.c" 3 4
                                                    ((void *)0)
# 42 "s_parameters.c"
                                                        };

static STRU_OPTION_PROC *get_option_proc_list_head(void)
{
    return p_list_head;
}

static ENUM_RETURN add_node_to_option_proc_list(STRU_OPTION_PROC *p_new)
{
    if(!(p_new != 
# 51 "s_parameters.c" 3 4
   ((void *)0)
# 51 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 51 "s_parameters.c" 3 4
   ((void *)0)
# 51 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""p_new != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 51, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""p_new != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 51, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };

    if(p_list_tail == 
# 53 "s_parameters.c" 3 4
                     ((void *)0)
# 53 "s_parameters.c"
                         )
    {
        p_list_head = p_list_tail = p_new;
    }
    else
    {
        p_list_tail->next = p_new;
        p_list_tail = p_new;
    }

    return RETURN_SUCCESS;
}

static void save_bin_name(const char *bin_name)
{
    p_bin = bin_name;
}

static const char *get_bin_name(void)
{
    return p_bin;
}

static ENUM_RETURN get_a_new_node_do(STRU_OPTION_PROC **pp_new,
    const char* option_name,
    ENUM_OPTION_TYPE option_type,
    ENUM_VALUE_TYPE value_type,
    FUNC_OPTION_PROC handler,
    const char* help_info)
{
    if(!(pp_new != 
# 83 "s_parameters.c" 3 4
   ((void *)0)
# 83 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 83 "s_parameters.c" 3 4
   ((void *)0)
# 83 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""pp_new != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 83, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""pp_new != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 83, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };

    *pp_new = (STRU_OPTION_PROC*)malloc(sizeof(STRU_OPTION_PROC));
    if(!(*pp_new != 
# 86 "s_parameters.c" 3 4
   ((void *)0)
# 86 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 86 "s_parameters.c" 3 4
   ((void *)0)
# 86 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""*pp_new != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 86, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""*pp_new != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 86, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };

    (*pp_new)->option = 
# 88 "s_parameters.c" 3 4
                       ((void *)0)
# 88 "s_parameters.c"
                           ;
    (*pp_new)->value = 
# 89 "s_parameters.c" 3 4
                      ((void *)0)
# 89 "s_parameters.c"
                          ;
    (*pp_new)->need_process = BOOLEAN_FALSE;
    (*pp_new)->handler = handler;
    (*pp_new)->help_info = 
# 92 "s_parameters.c" 3 4
                          ((void *)0)
# 92 "s_parameters.c"
                              ;
    (*pp_new)->next = 
# 93 "s_parameters.c" 3 4
                     ((void *)0)
# 93 "s_parameters.c"
                         ;

    (*pp_new)->option = (char*)malloc(strlen(option_name) + 1);
    if(!((*pp_new)->option != 
# 96 "s_parameters.c" 3 4
   ((void *)0)
# 96 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 96 "s_parameters.c" 3 4
   ((void *)0)
# 96 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""(*pp_new)->option != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 96, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""(*pp_new)->option != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 96, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };
    strcpy((*pp_new)->option, option_name);

    (*pp_new)->option_type = option_type;

    (*pp_new)->value = 
# 101 "s_parameters.c" 3 4
                      ((void *)0)
# 101 "s_parameters.c"
                          ;
    (*pp_new)->value_type = value_type;

    (*pp_new)->help_info = (char*)malloc(strlen(help_info) + 1);
    if(!((*pp_new)->help_info != 
# 105 "s_parameters.c" 3 4
   ((void *)0)
# 105 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 105 "s_parameters.c" 3 4
   ((void *)0)
# 105 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""(*pp_new)->help_info != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 105, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""(*pp_new)->help_info != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 105, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };
    strcpy((*pp_new)->help_info, help_info);

    return RETURN_SUCCESS;
}

static void get_a_new_node_do_error(STRU_OPTION_PROC *p_new)
{
    if(p_new == 
# 113 "s_parameters.c" 3 4
               ((void *)0)
# 113 "s_parameters.c"
                   )
    {
        return;
    }

    do{ if(p_new->option != 
# 118 "s_parameters.c" 3 4
   ((void *)0)
# 118 "s_parameters.c"
   ){free(p_new->option);}}while(0);;
    do{ if(p_new->help_info != 
# 119 "s_parameters.c" 3 4
   ((void *)0)
# 119 "s_parameters.c"
   ){free(p_new->help_info);}}while(0);;
    free(p_new);
}

static STRU_OPTION_PROC *get_a_new_option_proc_node(
    const char* option_name,
    ENUM_OPTION_TYPE option_type,
    ENUM_VALUE_TYPE value_type,
    FUNC_OPTION_PROC handler,
    const char* help_info)
{
    ENUM_RETURN ret_val;
    STRU_OPTION_PROC *p_new = 
# 131 "s_parameters.c" 3 4
                             ((void *)0)
# 131 "s_parameters.c"
                                 ;
    ret_val = get_a_new_node_do(&p_new, option_name, option_type, value_type, handler, help_info);
    if(!(ret_val == RETURN_SUCCESS)) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 133 "s_parameters.c" 3 4
   ((void *)0)
# 133 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 133, __FUNCTION__, (INT)
# 133 "s_parameters.c" 3 4
   ((void *)0)
# 133 "s_parameters.c"
   ); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 133, __FUNCTION__, (INT)
# 133 "s_parameters.c" 3 4
   ((void *)0)
# 133 "s_parameters.c"
   ); } }while(0);; get_a_new_node_do_error(p_new); return 
# 133 "s_parameters.c" 3 4
   ((void *)0)
# 133 "s_parameters.c"
   ; };

    return p_new;
}

static ENUM_RETURN print_help_info(struct TAG_STRU_VALUE *value)
{
    printf("       %s\n", p_introduction);
    printf("Usage: %s %s\n", get_bin_name(), p_usage);

    STRU_OPTION_PROC *p = get_option_proc_list_head();

    while(p != 
# 145 "s_parameters.c" 3 4
              ((void *)0)
# 145 "s_parameters.c"
                  )
    {
        printf("    %10s: %s\n", p->option, p->help_info);
        p = p->next;

    }

    return RETURN_SUCCESS;
}

static const char * get_error(void)
{
    return error_info_array[error_code];
}

static void print_error_info(void)
{
    printf("Error: %s\n", get_error());
}

ENUM_RETURN set_error(int code)
{
    if(!(code >= 0 && code < 200)) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 167 "s_parameters.c" 3 4
   ((void *)0)
# 167 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""code >= 0 && code < MAX_NUM_OF_ERROR_INFO"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 167, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""code >= 0 && code < MAX_NUM_OF_ERROR_INFO"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 167, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };
    if(!(error_info_array[code] != 
# 168 "s_parameters.c" 3 4
   ((void *)0)
# 168 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 168 "s_parameters.c" 3 4
   ((void *)0)
# 168 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""error_info_array[code] != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 168, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""error_info_array[code] != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 168, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };

    error_code = code;

    return RETURN_SUCCESS;
}

ENUM_RETURN register_error_info(int code, const char * info)
{
    if(!(code >= 0 && code < 200)) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 177 "s_parameters.c" 3 4
   ((void *)0)
# 177 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""code >= 0 && code < MAX_NUM_OF_ERROR_INFO"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 177, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""code >= 0 && code < MAX_NUM_OF_ERROR_INFO"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 177, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };

    if(!(error_info_array[code] == 
# 179 "s_parameters.c" 3 4
   ((void *)0)
# 179 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 179 "s_parameters.c" 3 4
   ((void *)0)
# 179 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""error_info_array[code] == NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 179, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""error_info_array[code] == NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 179, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };

    error_info_array[code] = info;

    return RETURN_SUCCESS;
}

ENUM_BOOLEAN is_option_valid(const char* option_name)
{

    do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 189 "s_parameters.c" 3 4
   ((void *)0)
# 189 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "option = %s""\n", get_time_stamp(), "s_parameters.c", 189, __FUNCTION__, option_name); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "option = %s""\n", get_time_stamp(), "s_parameters.c", 189, __FUNCTION__, option_name); } }while(0);;;

    int position = 0;

    if(!(option_name[position] == '-')) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 193 "s_parameters.c" 3 4
   ((void *)0)
# 193 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""option_name[position] == '-'"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 193, __FUNCTION__, (INT)BOOLEAN_FALSE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""option_name[position] == '-'"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 193, __FUNCTION__, (INT)BOOLEAN_FALSE); } }while(0);; return BOOLEAN_FALSE; };
    position++;

    while(option_name[position] != '\0')
    {
        if(!((((option_name[position] >= 'a' && option_name[position] <= 'z') || (option_name[position] >= 'A' && option_name[position] <= 'Z'))?(BOOLEAN_TURE):(BOOLEAN_FALSE)) == BOOLEAN_TURE)) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 198 "s_parameters.c" 3 4
       ((void *)0)
# 198 "s_parameters.c"
       ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""IS_ALPHABET(option_name[position]) == BOOLEAN_TURE"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 198, __FUNCTION__, (INT)BOOLEAN_FALSE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""IS_ALPHABET(option_name[position]) == BOOLEAN_TURE"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 198, __FUNCTION__, (INT)BOOLEAN_FALSE); } }while(0);; return BOOLEAN_FALSE; };
        position++;
    }

    return BOOLEAN_TURE;
}

ENUM_BOOLEAN is_option_type_valid(ENUM_OPTION_TYPE type)
{
    return (0 <= type && type < OPTION_TYPE_MAX)?BOOLEAN_TURE:BOOLEAN_FALSE;
}

ENUM_BOOLEAN is_value_type_valid(ENUM_VALUE_TYPE type)
{
    return (0 <= type && type < VALUE_TYPE_MAX)?BOOLEAN_TURE:BOOLEAN_FALSE;
}

ENUM_RETURN register_option(
    const char* option_name,
    ENUM_OPTION_TYPE option_type,
    ENUM_VALUE_TYPE value_type,
    FUNC_OPTION_PROC handler,
    const char* help_info)
{
    if(!(option_name != 
# 222 "s_parameters.c" 3 4
   ((void *)0)
# 222 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 222 "s_parameters.c" 3 4
   ((void *)0)
# 222 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""option_name != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 222, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""option_name != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 222, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };
    if(!(BOOLEAN_TURE == is_option_valid(option_name))) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 223 "s_parameters.c" 3 4
   ((void *)0)
# 223 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""BOOLEAN_TURE == is_option_valid(option_name)"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 223, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""BOOLEAN_TURE == is_option_valid(option_name)"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 223, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };
    if(!(BOOLEAN_TURE == is_option_type_valid(option_type))) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 224 "s_parameters.c" 3 4
   ((void *)0)
# 224 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""BOOLEAN_TURE == is_option_type_valid(option_type)"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 224, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""BOOLEAN_TURE == is_option_type_valid(option_type)"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 224, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };
    if(!(BOOLEAN_TURE == is_value_type_valid(value_type))) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 225 "s_parameters.c" 3 4
   ((void *)0)
# 225 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""BOOLEAN_TURE == is_value_type_valid(value_type)"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 225, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""BOOLEAN_TURE == is_value_type_valid(value_type)"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 225, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };
    if(!(handler != 
# 226 "s_parameters.c" 3 4
   ((void *)0)
# 226 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 226 "s_parameters.c" 3 4
   ((void *)0)
# 226 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""handler != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 226, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""handler != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 226, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };
    if(!(help_info != 
# 227 "s_parameters.c" 3 4
   ((void *)0)
# 227 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 227 "s_parameters.c" 3 4
   ((void *)0)
# 227 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""help_info != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 227, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""help_info != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 227, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };

    STRU_OPTION_PROC *p_new = 
# 229 "s_parameters.c" 3 4
                             ((void *)0)
# 229 "s_parameters.c"
                                 ;
    p_new = get_a_new_option_proc_node(option_name, option_type, value_type, handler, help_info);
    if(!(p_new != 
# 231 "s_parameters.c" 3 4
   ((void *)0)
# 231 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 231 "s_parameters.c" 3 4
   ((void *)0)
# 231 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""p_new != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 231, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""p_new != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 231, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };

    ENUM_RETURN ret_val;
    ret_val = add_node_to_option_proc_list(p_new);
    if(!(ret_val == RETURN_SUCCESS)) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 235 "s_parameters.c" 3 4
   ((void *)0)
# 235 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 235, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 235, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };



    return RETURN_SUCCESS;
}

ENUM_RETURN register_introduction(const char *introduction)
{
    if(!(introduction != 
# 244 "s_parameters.c" 3 4
   ((void *)0)
# 244 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 244 "s_parameters.c" 3 4
   ((void *)0)
# 244 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""introduction != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 244, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""introduction != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 244, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };

    p_introduction = malloc(strlen(introduction) + 1);
    if(!(p_introduction != 
# 247 "s_parameters.c" 3 4
   ((void *)0)
# 247 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 247 "s_parameters.c" 3 4
   ((void *)0)
# 247 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""p_introduction != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 247, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""p_introduction != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 247, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };
    strcpy(p_introduction, introduction);

    return RETURN_SUCCESS;
}

ENUM_RETURN register_usage(const char *usage)
{
    if(!(usage != 
# 255 "s_parameters.c" 3 4
   ((void *)0)
# 255 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 255 "s_parameters.c" 3 4
   ((void *)0)
# 255 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""usage != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 255, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""usage != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 255, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };

    p_usage = malloc(strlen(usage) + 1);
    if(!(p_usage != 
# 258 "s_parameters.c" 3 4
   ((void *)0)
# 258 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 258 "s_parameters.c" 3 4
   ((void *)0)
# 258 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""p_usage != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 258, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""p_usage != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 258, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };
    strcpy(p_usage, usage);

    return RETURN_SUCCESS;
}

static ENUM_RETURN register_option_help(void)
{
    ENUM_RETURN ret_val = register_option("-h", OPTION_TYPE_OPTIONAL, VALUE_TYPE_SWITCH, print_help_info, "print help info");
    if(!(ret_val == RETURN_SUCCESS)) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 267 "s_parameters.c" 3 4
   ((void *)0)
# 267 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 267, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 267, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };
    return RETURN_SUCCESS;
}

static STRU_OPTION_PROC *get_option_proc_block(char* option)
{
    STRU_OPTION_PROC *p = get_option_proc_list_head();

    while(p != 
# 275 "s_parameters.c" 3 4
              ((void *)0)
# 275 "s_parameters.c"
                  )
    {
        if(strcmp(option, p->option) == 0)
        {
            return p;
        }
        p = p->next;

    }

    return 
# 285 "s_parameters.c" 3 4
          ((void *)0)
# 285 "s_parameters.c"
              ;
}
static ENUM_RETURN add_option_value(STRU_OPTION_PROC *p, const char *value)
{
    STRU_VALUE* temp_value = (STRU_VALUE*)malloc(sizeof(STRU_VALUE));
    if(!(temp_value != 
# 290 "s_parameters.c" 3 4
   ((void *)0)
# 290 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 290 "s_parameters.c" 3 4
   ((void *)0)
# 290 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""temp_value != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 290, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""temp_value != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 290, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };

    temp_value->value = (char *)malloc(strlen(value) + 1);
    if(!(temp_value->value != 
# 293 "s_parameters.c" 3 4
   ((void *)0)
# 293 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 293 "s_parameters.c" 3 4
   ((void *)0)
# 293 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""temp_value->value != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 293, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""temp_value->value != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 293, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };

    strcpy(temp_value->value, value);

    temp_value->next = 
# 297 "s_parameters.c" 3 4
                      ((void *)0)
# 297 "s_parameters.c"
                          ;

    if(p->value != 
# 299 "s_parameters.c" 3 4
                  ((void *)0)
# 299 "s_parameters.c"
                      )
    {
        p->value->next = temp_value;
    }
    else
    {
        p->value = temp_value;
    }

    return RETURN_SUCCESS;
}

static ENUM_RETURN add_input_file(const char *file)
{
    if(!(file != 
# 313 "s_parameters.c" 3 4
   ((void *)0)
# 313 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 313 "s_parameters.c" 3 4
   ((void *)0)
# 313 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""file != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 313, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""file != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 313, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };
    do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 314 "s_parameters.c" 3 4
   ((void *)0)
# 314 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "add file: %s""\n", get_time_stamp(), "s_parameters.c", 314, __FUNCTION__, file); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "add file: %s""\n", get_time_stamp(), "s_parameters.c", 314, __FUNCTION__, file); } }while(0);;;

    input_files[num_of_input_files] = malloc(strlen(file) + 1);
    if(!(input_files[num_of_input_files] != 
# 317 "s_parameters.c" 3 4
   ((void *)0)
# 317 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 317 "s_parameters.c" 3 4
   ((void *)0)
# 317 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""input_files[num_of_input_files] != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 317, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""input_files[num_of_input_files] != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 317, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };
    strcpy(input_files[num_of_input_files], file);
    num_of_input_files++;

    return RETURN_SUCCESS;
}

static int get_input_file_num(void)
{
    return num_of_input_files;
}

static ENUM_RETURN process_input_files(int argc, char **argv)
{
    int i = 1;
    ENUM_RETURN ret_val;

    while(i < argc)
    {
        if(argv[i][0] == '-')
        {
            break;
        }


        ret_val = add_input_file(argv[i]);
        if(!(ret_val == RETURN_SUCCESS)) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 343 "s_parameters.c" 3 4
       ((void *)0)
# 343 "s_parameters.c"
       ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 343, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 343, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };
        i++;
    }

    return RETURN_SUCCESS;
}

static ENUM_RETURN process_options(int argc, char **argv)
{
    STRU_OPTION_PROC *p = 
# 352 "s_parameters.c" 3 4
                         ((void *)0)
# 352 "s_parameters.c"
                             ;
    ENUM_RETURN ret_val;
    int i = 1 + get_input_file_num();

    while(i < argc)
    {
        p = get_option_proc_block(argv[i]);
        if(p == 
# 359 "s_parameters.c" 3 4
               ((void *)0)
# 359 "s_parameters.c"
                   )
        {
            printf("error: unrecognized command line option ¡®%s¡¯\n", argv[i]);
            return RETURN_FAILURE;
        }

        p->need_process = BOOLEAN_TURE;

        if(p->value_type == VALUE_TYPE_SWITCH)
        {
            if(!(p->value == 
# 369 "s_parameters.c" 3 4
           ((void *)0)
# 369 "s_parameters.c"
           )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 369 "s_parameters.c" 3 4
           ((void *)0)
# 369 "s_parameters.c"
           ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""p->value == NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 369, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""p->value == NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 369, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };

            ret_val = add_option_value(p, "enable");
            if(!(ret_val == RETURN_SUCCESS)) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 372 "s_parameters.c" 3 4
           ((void *)0)
# 372 "s_parameters.c"
           ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 372, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 372, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };
            i++;
            continue;
        }
        else
        {
            i++;

            while(i < argc)
            {
                if(argv[i][0] == '-')
                {
                    break;
                }

                ret_val = add_option_value(p, argv[i]);
                if(!(ret_val == RETURN_SUCCESS)) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 388 "s_parameters.c" 3 4
               ((void *)0)
# 388 "s_parameters.c"
               ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 388, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 388, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };
                i++;
            }
        }

        if(p->value == 
# 393 "s_parameters.c" 3 4
                      ((void *)0)
# 393 "s_parameters.c"
                          )
        {
            printf("fatal error: [%s] need vales\n", p->option);
            return RETURN_FAILURE;
        }

    }

    return RETURN_SUCCESS;
}

static ENUM_BOOLEAN is_option_need_process(STRU_OPTION_PROC *p)
{
    return p->need_process;
}

static ENUM_RETURN process_do(void)
{
    STRU_OPTION_PROC *p = get_option_proc_list_head();
    ENUM_RETURN ret_val;
    while(p != 
# 413 "s_parameters.c" 3 4
              ((void *)0)
# 413 "s_parameters.c"
                  )
    {
        if(is_option_need_process(p) == BOOLEAN_TURE)
        {
            ret_val = p->handler(p->value);
            if(!(ret_val == RETURN_SUCCESS)) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 418 "s_parameters.c" 3 4
           ((void *)0)
# 418 "s_parameters.c"
           ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "FALSE(""ret_val == RETURN_SUCCESS"")!, ""process option [%s] failed!\n""\n", get_time_stamp(), "s_parameters.c", 418, __FUNCTION__, p->option); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "FALSE(""ret_val == RETURN_SUCCESS"")!, ""process option [%s] failed!\n""\n", get_time_stamp(), "s_parameters.c", 418, __FUNCTION__, p->option); } }while(0);; };
        }

        p = p->next;

    }

    return RETURN_SUCCESS;
}
static ENUM_RETURN register_default_data(void)
{
    ENUM_RETURN ret_val;
    ret_val = register_option_help();
    if(!(ret_val == RETURN_SUCCESS)) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 431 "s_parameters.c" 3 4
   ((void *)0)
# 431 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 431, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 431, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };

    ret_val = register_error_info(0, "Program run succeed!");
    if(!(ret_val == RETURN_SUCCESS)) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 434 "s_parameters.c" 3 4
   ((void *)0)
# 434 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 434, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 434, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };

    return RETURN_SUCCESS;
}

static void debug_print_option_proc_block(void)
{

    STRU_OPTION_PROC *p = get_option_proc_list_head();

    while(p != 
# 444 "s_parameters.c" 3 4
              ((void *)0)
# 444 "s_parameters.c"
                  )
    {
        printf("%20s = %s\n", "option", p->option);
        printf("%20s = %d\n", "option type", p->option_type);
        printf("%20s = %s\n", "value", p->value!= 
# 448 "s_parameters.c" 3 4
                                                 ((void *)0)
# 448 "s_parameters.c"
                                                     ? p->value->value: "NULL");
        printf("%20s = %d\n", "value type", p->value_type);
        printf("%20s = %p\n", "handler", p->handler);
        printf("%20s = %s\n", "help_info", p->help_info);

        p = p->next;

    }
}

ENUM_RETURN process(int argc, char **argv)
{
    save_bin_name(argv[0]);

    register_default_data();

    if(argc < 2)
    {
        print_help_info(
# 466 "s_parameters.c" 3 4
                       ((void *)0)
# 466 "s_parameters.c"
                           );
        return RETURN_FAILURE;
    }

    if(!(argv[1] != 
# 470 "s_parameters.c" 3 4
   ((void *)0)
# 470 "s_parameters.c"
   )) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 470 "s_parameters.c" 3 4
   ((void *)0)
# 470 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""argv[1] != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 470, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""argv[1] != NULL"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 470, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };

    do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 472 "s_parameters.c" 3 4
   ((void *)0)
# 472 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" " %d parameters: \n""\n", get_time_stamp(), "s_parameters.c", 472, __FUNCTION__, argc); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" " %d parameters: \n""\n", get_time_stamp(), "s_parameters.c", 472, __FUNCTION__, argc); } }while(0);;;

    ENUM_RETURN ret_val;
    ret_val = process_input_files(argc, argv);
    if(!(ret_val == RETURN_SUCCESS)) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 476 "s_parameters.c" 3 4
   ((void *)0)
# 476 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 476, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 476, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };

    ret_val = process_options(argc, argv);
    if(!(ret_val == RETURN_SUCCESS)) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 479 "s_parameters.c" 3 4
   ((void *)0)
# 479 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 479, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 479, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };



    ret_val = process_do();
    if(!(ret_val == RETURN_SUCCESS)) { do { FILE *fp = fopen(log_getfn(), "a"); if(fp != 
# 484 "s_parameters.c" 3 4
   ((void *)0)
# 484 "s_parameters.c"
   ) { fprintf(fp, "[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 484, __FUNCTION__, (INT)RETURN_FAILURE); fclose(fp); } else { printf("[%s] [file: %s, line: %u] [function: %s]" "ASSERT(""ret_val == RETURN_SUCCESS"")!, return  = %d.\n", get_time_stamp(), "s_parameters.c", 484, __FUNCTION__, (INT)RETURN_FAILURE); } }while(0);; return RETURN_FAILURE; };

    print_error_info();

    return RETURN_SUCCESS;

}
