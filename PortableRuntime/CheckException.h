#pragma once

namespace PortableRuntime
{

// TODO: Move implementation to .cpp file.
// TODO: Consider replacing all calls to this function with calls to the function that accepts a message.
inline void check_exception(bool result)
{
    if(!result)
    {
#if defined(_MSC_VER) && !defined(NDEBUG)
        // Work around MSVC issue where the abort() message box is displayed instead of the assert messagebox.
        // This makes the abort/retry/ignore of the dialog not work for the "ignore" case.  Since the abort dialog is called
        // even for console apps where blocking for a GUI response is not ideal, just mimic the behavior as it is no worse.
        // This also has the nice side effect of printing error information to the debugger window.
        _set_error_mode(_OUT_TO_MSGBOX);
#endif
        // Force break.
        assert(false);
        throw std::exception();
    }
}

// TODO: Move implementation to .cpp file.
inline void check_exception(bool result, const char* message)
{
    if(!result)
    {
#if defined(_MSC_VER) && !defined(NDEBUG)
        // Work around MSVC issue where the abort() message box is displayed instead of the assert messagebox.
        // This makes the abort/retry/ignore of the dialog not work for the "ignore" case.  Since the abort dialog is called
        // even for console apps where blocking for a GUI response is not ideal, just mimic the behavior as it is no worse.
        // This also has the nice side effect of printing error information to the debugger window.
        _set_error_mode(_OUT_TO_MSGBOX);
#endif
        // Force break.
        assert(false);
        throw std::exception(message);
    }
}

// This macro should only be used to work around static analysis warnings.
// TODO: Passing a function as expr is not valid.  Check this.
#define CHECK_EXCEPTION(expr) { PortableRuntime::check_exception(expr); _Analysis_assume_(expr); }

}

