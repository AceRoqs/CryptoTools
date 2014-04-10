#ifndef PLATFORM_H
#define PLATFORM_H

// Boolean definition.
#ifndef __cplusplus
  typedef int bool;
# define false 0
# define true  1
#endif

// NULL definition.
#ifdef USE_NULL
# ifdef __cplusplus
    const int NULL = 0;
# else
#   define NULL ((void *)0)
# endif
#endif

#include <cassert>
#include <cstdint>

#undef FOUND_TARGET

#endif

