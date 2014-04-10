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

