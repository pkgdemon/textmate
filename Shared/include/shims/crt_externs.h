#ifndef CRT_EXTERNS_H_SHIM
#define CRT_EXTERNS_H_SHIM
/* Linux/FreeBSD shim for Darwin's <crt_externs.h> */
#ifdef __cplusplus
extern "C" {
#endif
extern char** environ;
static inline char*** _NSGetEnviron(void) { return &environ; }
#ifdef __cplusplus
}
#endif
#endif
