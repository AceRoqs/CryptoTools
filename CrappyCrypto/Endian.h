#ifndef ENDIAN_H
#define ENDIAN_H

// TODO: Move implementations to a cpp file.
namespace PortableRuntime
{

// Change byte order from big to little or little to big.
inline uint16_t uint16_swap(uint16_t value)
{
    return ((value >> 8) & 0x00ff) | ((value << 8) & 0xff00);
}

inline uint32_t uint32_swap(uint32_t value)
{
    return ((value >> 24) & 0x000000ff) | ((value >> 8) & 0x0000ff00) | ((value << 8) & 0x00ff0000) | ((value << 24) & 0xff000000);
}

// Change byte order from big/little to native format.
inline uint16_t lswap16(uint16_t value)
{
#ifdef BIG_ENDIAN
    return uint16_swap(value);
#else
    return value;
#endif
}

inline uint16_t bswap16(uint16_t value)
{
#ifdef BIG_ENDIAN
    return value;
#else
    return uint16_swap(value);
#endif
}

inline uint32_t lswap32(uint32_t value)
{
#ifdef BIG_ENDIAN
    return uint32_swap(value);
#else
    return value;
#endif
}

inline uint32_t bswap32(uint32_t value)
{
#ifdef BIG_ENDIAN
    return value;
#else
    return uint32_swap(value);
#endif
}

}

#endif

