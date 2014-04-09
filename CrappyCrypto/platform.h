/* $Id: PLATFORM.H 1.1 1999/06/27 02:54:54 tjones Rel $ */

/* Skipjack - A portable 80-bit block cipher designed by NSA.
   Copyright (C) 1998  Thobias Jones.  All rights reserved.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA  02111-1307, USA.
*/

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
    define NULL ((void *)0)
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
