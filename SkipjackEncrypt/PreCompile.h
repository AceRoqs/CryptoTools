#pragma once

#include <cassert>
#include <cstdio>
#include <exception>
#include <fstream>
#include <memory>

#ifdef _MSC_VER

// APIs for MSVCRT UTF-8 output.
#include <fcntl.h>
#include <io.h>

#endif

