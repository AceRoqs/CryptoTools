#include "PreCompile.h"
#include "Unicode.h"        // Pick up forward declarations to ensure correctness.
#include "CheckException.h"

namespace PortableRuntime
{

std::wstring utf16_from_utf8(const std::string& utf8_string)
{
    std::wstring utf16_string;
    utf16_string = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().from_bytes(utf8_string);

    return utf16_string;
}

// Untested, and not used anywhere.
#if 0
std::string utf8_from_utf16(const std::wstring& utf16_string)
{
    std::string utf8_string;
    utf8_string = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().to_bytes(utf16_string);

    return utf8_string;
}

bool utf8_is_ascii(_In_z_ const char* utf8_string)
{
    bool is_ascii = true;

    while(*utf8_string != '\0')
    {
        if(*utf8_string < 0)
        {
            is_ascii = false;
            break;
        }
        ++utf8_string;
    }

    return is_ascii;
}
#endif

}

