#include "PreCompile.h"
#include "Unicode.h"        // Pick up forward declarations to ensure correctness.
#include "CheckException.h"

namespace PortableRuntime
{

// TODO: char16_t is the correct type for UTF-16.  Revisit the types once there are string types that are templated on char16_t instead of wchar_t.
std::wstring utf16_from_utf8(const std::string& utf8_string)
{
    std::wstring utf16_string;
    utf16_string = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().from_bytes(utf8_string);

    return utf16_string;
}

std::string utf8_from_utf16(const std::wstring& utf16_string)
{
    std::string utf8_string;
    utf8_string = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().to_bytes(utf16_string);

    return utf8_string;
}

// Untested, and not used anywhere.
#if 0
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

