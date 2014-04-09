/* $Id: ENCRYPT.C 1.1 1999/06/27 02:54:37 tjones Rel $ */

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
         6/24/99 - Added encryption program.
*/

#include "skipjack.h"
#include <stdio.h>

unsigned char keyvector[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

unsigned char testvector[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

int main(int argc, char **argv)
{
   int i, j;
   FILE *pInfile, *pOutfile;
   
   if(argc != 4)
   {
      fprintf(stderr, "Usage: %s infile outfile key", argv[0]);
      exit(0);
   }

/* Build key */
   i = 0;
   while(argv[3][i] != 0)
   {
      keyvector[i % KEYLENGTH] ^= argv[3][i];
      i++;
   }

/* Open input file */
   if((pInfile = fopen(argv[1], "rb")) == 0)
   {
      fprintf(stderr, "%s: error opening %s\n", argv[0], argv[1]);
      exit(1);
   }

/* Open output file */
   if((pOutfile = fopen(argv[2], "wb")) == 0)
   {
      fclose(pInfile);
      fprintf(stderr, "%s: error opening %s\n", argv[0], argv[2]);
      exit(1);
   }

/* Encrypt file */
   j = 0;
   while(!feof(pInfile))
   {
      i = fread(testvector, 1, BLOCKLENGTH, pInfile);
      SJ_Encrypt(testvector, keyvector);
      fwrite(testvector, 1, BLOCKLENGTH, pOutfile);
      j += i;
   }

/* The next line is not portable across endian architectures.  The
   problem is that j might be in a register, and thus not the same
   endian as in memory.  The next line is portable across machines
   whose registers are big-endian (i.e. x86). */
   (dword)*(dword *)(testvector) = (dword)j;

   fwrite(testvector, 1, sizeof(dword), pOutfile);

   fclose(pInfile);
   fclose(pOutfile);
   
   return 0;
} /* main */

