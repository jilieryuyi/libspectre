#ifndef SPECTRE_MACROS_H
#define SPECTRE_MACROS_H

#ifdef  __cplusplus
#  define SPECTRE_BEGIN_DECLS  extern "C" {
#  define SPECTRE_END_DECLS    }
#else
#  define SPECTRE_BEGIN_DECLS
#  define SPECTRE_END_DECLS
#endif

#ifndef TRUE
#  define TRUE 1
#endif
#ifndef FALSE
#  define FALSE 0
#endif

#ifndef NULL
#  ifdef __cplusplus
#    define NULL        (0L)
#  else /* !__cplusplus */
#    define NULL        ((void*) 0)
#  endif /* !__cplusplus */
#endif

#endif /* SPECTRE_MACROS_H */
