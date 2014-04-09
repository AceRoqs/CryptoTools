/* $Id: SKIPJACK.H 1.1 1999/06/27 02:54:03 tjones Rel $ */

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

/*
   Revisions:
         6/24/99 - Change name of Encrypt and Decrypt to be more modular.
         6/10/99 - Fixed directives that did not export functions under C.
                   Added BLOCKLENGTH definition.
                   Made pointers in function prototypes const correct.
                   Removed FEISTEL_ITER macro.
                   Added ITER_PER_FUNC and NUM_FEISTELS macros.
*/

#ifndef __SKIPJACK_H
#define __SKIPJACK_H

#include "platform.h"

#define ROUNDS 32
#define NUM_FEISTELS 4
#define KEYLENGTH 10
#define BLOCKLENGTH 8
#define ITER_PER_FUNC 8

#ifdef __cplusplus
# define _EXTERN extern "C"
#else
# define _EXTERN extern
#endif
   _EXTERN void SJ_Encrypt(unsigned char *text, const unsigned char *key);
   _EXTERN void SJ_Decrypt(unsigned char *text, const unsigned char *key);
   _EXTERN void RuleA(word *w, const unsigned char *key, word counter);
   _EXTERN void RuleB(word *w, const unsigned char *key, word counter);
   _EXTERN void RuleA_1(word *w, const unsigned char *key, word counter);
   _EXTERN void RuleB_1(word *w, const unsigned char *key, word counter);
   _EXTERN word G(word g, const unsigned char *key, int step);
   _EXTERN word G_1(word g, const unsigned char *key, int step);

#endif /* __SKIPJACK_H */
