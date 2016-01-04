#include "PreCompile.h"
#include <CrappyCrypto/Decrypt.h>
#include <CrappyCrypto/Keys.h>
#include <PortableRuntime/CheckException.h>
#include <PortableRuntime/Unicode.h>

namespace CrappyCrypto
{

void decrypt_file(_In_z_ const wchar_t* input_file_name, _In_z_ const wchar_t* output_file_name, _In_z_ const wchar_t* key_file_name);

}

int wmain(int argc, _In_reads_(argc) wchar_t** argv)
{
    // Set wprintf output to UTF-8 in Windows console.
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stderr), _O_U8TEXT);

    if(argc != 4)
    {
        std::fwprintf(stderr, L"Usage: %s infile outfile keyfile", argv[0]);
        return 0;
    }

    try
    {
        CrappyCrypto::decrypt_file(argv[1], argv[2], argv[3]);
    }
    catch(const std::exception& ex)
    {
        std::fwprintf(stderr, L"%s\n", PortableRuntime::utf16_from_utf8(ex.what()).c_str());
        return 1;
    }

    return 0;
}

namespace CrappyCrypto
{

void decrypt_file(_In_z_ const wchar_t* input_file_name, _In_z_ const wchar_t* output_file_name, _In_z_ const wchar_t* key_file_name)
{
    using namespace PortableRuntime;

    // Open input file.
    std::basic_ifstream<uint8_t> input_file(input_file_name, std::ios::binary);
    check_exception(input_file.good(), (u8"Error opening: " + utf8_from_utf16(input_file_name)).c_str());

    // Open output file.
    std::basic_ofstream<uint8_t> output_file(output_file_name, std::ios::binary);
    check_exception(output_file.good(), (u8"Error opening: " + utf8_from_utf16(output_file_name)).c_str());

    // Open key file.
    std::basic_ifstream<uint8_t> key_file(key_file_name, std::ios::binary);
    check_exception(key_file.good(), (u8"Error opening: " + utf8_from_utf16(key_file_name)).c_str());

    // Build key.
    uint8_t key_vector[CrappyCrypto::Skipjack::key_size];
    CrappyCrypto::key_vector_from_key_file(key_vector, sizeof(key_vector), key_file);

    CrappyCrypto::Skipjack::decrypt_fstream(input_file, output_file, key_vector);
}

}

