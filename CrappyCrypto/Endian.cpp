#include "PreCompile.h"
#include "Endian.h"         // Pick up forward declarations to ensure correctness.

namespace PortableRuntime
{

// Change byte order from big to little or little to big.
static uint16_t uint16_swap(uint16_t value) NOEXCEPT
{
    return ((value >> 8) & 0x00ff) | ((value << 8) & 0xff00);
}

static uint32_t uint32_swap(uint32_t value) NOEXCEPT
{
    return ((value >> 24) & 0x000000ff) | ((value >> 8) & 0x0000ff00) | ((value << 8) & 0x00ff0000) | ((value << 24) & 0xff000000);
}

// Change byte order from big/little to native format.
uint16_t lswap16(uint16_t value) NOEXCEPT
{
#ifdef BIG_ENDIAN
    return uint16_swap(value);
#else
    return value;
#endif
}

uint16_t bswap16(uint16_t value) NOEXCEPT
{
#ifdef BIG_ENDIAN
    return value;
#else
    return uint16_swap(value);
#endif
}

uint32_t lswap32(uint32_t value) NOEXCEPT
{
#ifdef BIG_ENDIAN
    return uint32_swap(value);
#else
    return value;
#endif
}

uint32_t bswap32(uint32_t value) NOEXCEPT
{
#ifdef BIG_ENDIAN
    return value;
#else
    return uint32_swap(value);
#endif
}

}

