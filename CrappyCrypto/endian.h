/* $Id: ENDIAN.H 1.1 1999/06/27 02:54:20 tjones Rel $ */

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
          6/4/99 - Changed format of preprocessor directives so warnings
                    are avoided in Tru64 Unix compilers.
        12/23/98 - Repackaged and re-released under GPL.
         5/30/98 - Added defines to auto change endian
                 - Removed inline C functions and replaced with
                   defines
*/

#ifndef __ENDIAN_H
#define __ENDIAN_H

#define LITTLE_ENDIAN 1234
#define BIG_ENDIAN 4321
#define PDP_ENDIAN 3412
#define BYTE_ORDER LITTLE_ENDIAN

/* Change byte order from big to little or little to big */
#define EndianInt16(x) ((((x) >> 8) & 0x00ff) | (((x) << 8) & 0xff00))
#define EndianInt32(x) ((((x) >> 24) & 0x000000ff) | (((x) >> 8) & 0x0000ff00) | (((x) << 8) & 0x00ff0000) | (((x) << 24) & 0xff000000))

/* Change byte order from big/little to native format */
#if (BYTEORDER==LITTLE_ENDIAN)
# define lswap16(x) (x)
# define bswap16(x) EndianInt16(x)
# define lswap32(x) (x)
# define bswap32(x) EndianInt32(x)

#elif (BYTEORDER==BIGENDIAN)
# define lswap16(x) EndianInt16(x)
# define bswap16(x) (x)
# define lswap32(x) EndianInt32(x)
# define bswap32(x) (x)

#else
# error Unsupported endian set
#endif

#endif /* __ENDIAN_H */
