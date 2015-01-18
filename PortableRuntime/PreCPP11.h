#pragma once

// Define C++11 constructs not available in pre-C++11 compilers.

#ifdef _MSC_VER
#if _MSC_VER > 1800
#error Revisit the NOEXCEPT macro and use the C++11 version if the compiler supports it.
#endif
#endif

// "override" keyword.
// http://en.wikipedia.org/wiki/C++11#Explicit_overrides_and_final
#if defined(_MSC_VER) && _MSC_VER < 1700
#define OVERRIDE
#else
#define OVERRIDE override
#endif

// = delete.
// http://en.wikipedia.org/wiki/C++11#Explicitly_defaulted_and_deleted_special_member_functions
#if defined(_MSC_VER) && _MSC_VER < 1800
#define EQUALS_DELETE
#else
#define EQUALS_DELETE = delete
#endif

#if defined(_MSC_VER)
#if defined(NDEBUG)
#define NOEXCEPT
#else
// MSVC + Debug build: use non-standard empty throw exception specification.
#define NOEXCEPT throw()
#endif
#else
#define NOEXCEPT noexcept
#endif


#if defined(_MSC_VER)
#define UTF8(x) x
#else
#define UTF8(x) u8##x
#endif

