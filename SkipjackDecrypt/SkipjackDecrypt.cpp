#include "PreCompile.h"
#include <CrappyCrypto/Decrypt.h>
#include <CrappyCrypto/Keys.h>
#include <WindowsCommon/DebuggerTracing.h>
#include <PortableRuntime/Tracing.h>
#include <PortableRuntime/CheckException.h>
#include <PortableRuntime/Unicode.h>

namespace CrappyCrypto
{

void decrypt_file(_In_z_ const wchar_t* input_file_name, _In_z_ const wchar_t* output_file_name, _In_z_ const wchar_t* key_file_name);

}

int wmain(int argc, _In_reads_(argc) wchar_t** argv)
{
    constexpr auto arg_program_name     = 0;
    constexpr auto arg_input_file_name  = 1;
    constexpr auto arg_output_file_name = 2;
    constexpr auto arg_key_file_name    = 3;

    // ERRORLEVEL zero is the success code.
    int error_level = 0;

    try
    {
        PortableRuntime::set_dprintf(WindowsCommon::debugger_dprintf);

        // Set wprintf output to UTF-8 in Windows console.
        // CHECK_EXCEPTION ensures against the case that the CRT invalid parameter handler
        // routine is set by a global constructor.
        CHECK_EXCEPTION(_setmode(_fileno(stdout), _O_U8TEXT) != -1, u8"Failed to set UTF-8 output mode.");
        CHECK_EXCEPTION(_setmode(_fileno(stderr), _O_U8TEXT) != -1, u8"Failed to set UTF-8 output mode.");

        if(argc == 4)
        {
            CrappyCrypto::decrypt_file(argv[arg_input_file_name], argv[arg_output_file_name], argv[arg_key_file_name]);
        }
        else
        {
            std::fwprintf(stderr, L"Usage: %s infile outfile keyfile\n", argv[arg_program_name]);
            error_level = 1;
        }
    }
    catch(const std::exception& ex)
    {
        std::fwprintf(stderr, L"\n%s\n", PortableRuntime::utf16_from_utf8(ex.what()).c_str());
        error_level = 1;
    }

    return error_level;
}

namespace CrappyCrypto
{

void decrypt_file(_In_z_ const wchar_t* input_file_name, _In_z_ const wchar_t* output_file_name, _In_z_ const wchar_t* key_file_name)
{
    using namespace PortableRuntime;

    // Open input file.
    std::basic_ifstream<uint8_t> input_file(input_file_name, std::ios::binary);
    CHECK_EXCEPTION(input_file.good(), u8"Error opening: " + utf8_from_utf16(input_file_name));

    // Open output file.
    std::basic_ofstream<uint8_t> output_file(output_file_name, std::ios::binary);
    CHECK_EXCEPTION(output_file.good(), u8"Error opening: " + utf8_from_utf16(output_file_name));

    // Open key file.
    std::basic_ifstream<uint8_t> key_file(key_file_name, std::ios::binary);
    CHECK_EXCEPTION(key_file.good(), u8"Error opening: " + utf8_from_utf16(key_file_name));

    // Build key.
    uint8_t key_vector[CrappyCrypto::Skipjack::key_size];
    CrappyCrypto::key_vector_from_key_file(key_vector, sizeof(key_vector), key_file);

    CrappyCrypto::Skipjack::decrypt_fstream(input_file, output_file, key_vector);
}

}

