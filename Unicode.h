#pragma once

namespace PortableRuntime
{

std::wstring utf16_from_utf8(const std::string& utf8_string);
std::string utf8_from_utf16(const std::wstring& utf16_string);

}

