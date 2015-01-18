#pragma once

namespace PortableRuntime
{

// TODO: Move implementation to .cpp file.
inline void check_exception(bool result)
{
    if(!result)
    {
        assert(false);
        throw std::exception();
    }
}

// This macro should only be used to work around static analysis warnings.
// TODO: Passing a function as expr is not valid.  Check this.
#define CHECK_EXCEPTION(expr) { PortableRuntime::check_exception(expr); _Analysis_assume_(expr); }

}

