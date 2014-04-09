/* $Id: decrypt.c 1.1 1999/06/26 18:06:00 tjones Exp $ */

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
unsigned char testvector2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

int main(int argc, char **argv)
{
   int i, j, k;
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

/* Decrypt file */
   i = fread(testvector, 1, BLOCKLENGTH, pInfile);
   j = 0;
   while(!feof(pInfile))
   {
      SJ_Decrypt(testvector, keyvector);
      j += i;
      i = fread(testvector2, 1, BLOCKLENGTH, pInfile);
      if(i == BLOCKLENGTH)
      {
         fwrite(testvector, 1, BLOCKLENGTH, pOutfile);
         for(k = 0; k < BLOCKLENGTH; ++k)
            testvector[k] = testvector2[k];
      }
      else
         fwrite(testvector, 1,
           (dword)*(dword *)testvector2 - (j - BLOCKLENGTH), pOutfile);
   } /* while(!feof(pInfile)) */
   
   fclose(pInfile);
   fclose(pOutfile);
   
   return 0;
} /* main */
