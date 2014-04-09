/* $Id: TEST.C 1.1 1999/06/27 02:55:36 tjones Rel $ */

/* Skipjack - A portable 80-bit block cipher designed by NSA.
   Copyright (C) 1998-1999  Thobias Jones.  All rights reserved.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public
   License along with this program; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA  02111-1307, USA.
*/

/*
   Revisions:
         6/10/99 - Added test vectors to release.
*/

#include "skipjack.h"
#include <stdio.h>

const unsigned char keyvector[] = {
0x00, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 };

unsigned char testvector[] = {
0x33, 0x22, 0x11, 0x00, 0xdd, 0xcc, 0xbb, 0xaa };

unsigned char testvector2[] = {
0x33, 0x22, 0x11, 0x00, 0xdd, 0xcc, 0xbb, 0xaa };

void DispVector(unsigned char *);
void DispVectorAndCount(int);

int main()
{
   int i, counter;

   printf("Skipjack test vectors\n\n");

   printf("plaintext\t");
   DispVector(testvector);

   printf("key      \t");
   for(i = 0; i < KEYLENGTH; ++i)
      printf("%02x ", keyvector[i]);
   printf("\n\n");
   
   DispVectorAndCount(0);
   for(counter = 1; counter <= ITER_PER_FUNC * 1; ++counter)
   {
      RuleA((word *)testvector, keyvector, counter);
      DispVectorAndCount(counter);
   }
   for(; counter <= ITER_PER_FUNC * 2; ++counter)
   {
      RuleB((word *)testvector, keyvector, counter);
      DispVectorAndCount(counter);
   }
   for(; counter <= ITER_PER_FUNC * 3; ++counter)
   {
      RuleA((word *)testvector, keyvector, counter);
      DispVectorAndCount(counter);
   }
   for(; counter <= ITER_PER_FUNC * 4; ++counter)
   {
      RuleB((word *)testvector, keyvector, counter);
      DispVectorAndCount(counter);
   }

   SJ_Encrypt(testvector2, keyvector);
   printf("\nCiphertext output: ");
   DispVector(testvector2);

   SJ_Decrypt(testvector2, keyvector);
   printf("\nPlaintext reverse: ");
   DispVector(testvector2);

   return 0;
} /* main */

void DispVector(unsigned char *vector)
{
   int i;

   for(i = 0; i < BLOCKLENGTH; ++i)
      printf("%02x ", vector[i]);
   printf("\n");
} /* DispVector */

void DispVectorAndCount(int counter)
{
   int i;

   printf("%d\t", counter);
   for(i = 0; i < BLOCKLENGTH; ++i)
      printf("%02x ", testvector[i]);
   printf("\n");
} /* DispVectorAndCount */
