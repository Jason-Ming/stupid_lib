#if defined(__cplusplus) && \
    (!defined(__AN_ALIEN_IMPL__) || \
        (defined(__AN_ALIEN_IMPL__) && __cplusplus))
  #define __BEGIN_C_DECLS extern "C" {
  #define __END_C_DECLS }
#else
  #define __BEGIN_C_DECLS
  #define __END_C_DECLS
#endif

