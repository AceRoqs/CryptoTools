#ifndef __PLATFORM_H
#define __PLATFORM_H

/*** Boolean definition ***/
#ifndef __cplusplus
  typedef int bool;
# define false 0
# define true  1
#endif

/*** NULL definition ***/
#ifdef USE_NULL
# ifdef __cplusplus
    const int NULL = 0;
# else
#   define NULL ((void *)0)
# endif
#endif

/*** HANDLE definition ***/
#ifdef __cplusplus
  typedef char **HANDLE;        /* pointer to pointer (no void* in C++) */
#else
  typedef void **HANDLE;
#endif

/*** Turbo C++ 3.0 definitions ***/
#ifdef __TCPLUSPLUS__
# define FOUND_TARGET
  enum bool {false, true};
# define pack
  typedef unsigned char BYTE;   /* 8 bits */
  typedef unsigned short WORD;  /* 16 bits */
  typedef unsigned long DWORD;  /* 32 bits */
#endif


/*** GNU C++ (DJGPP) definitions ***/
#ifdef __GNUC__
# define FOUND_TARGET
# define pack __attribute__ ((packed))  /* Pack structures */
  typedef unsigned char BYTE;   /* 8 bits */
  typedef unsigned short WORD;  /* 16 bits */
  typedef unsigned long DWORD;  /* 32 bits */
#endif

/*** Generic compiler ***/
#ifndef FOUND_TARGET
# define FOUND_TARGET
# define pack
  typedef unsigned char BYTE;   /* 8 bits */
  typedef unsigned short WORD;  /* 16 bits */
  typedef unsigned long DWORD;  /* 32 bits */
#endif

/*** Debugging definitions ***/
#ifndef DEBUG
# define DEBUG false
#endif

#if DEBUG == false
# define NDEBUG
#endif

#include <assert.h>

/*** Platform definitions ***/
#define INTEL 0
#define ALPHA 1

#ifndef PLATFORM
# define PLATFORM INTEL
#endif

/*** Cleanup ***/
typedef BYTE byte;              /* same as above */
typedef WORD word;
typedef DWORD dword;

#undef FOUND_TARGET

#endif /* __PLATFORM_H */

