#include "PreCompile.h"
#include <CrappyCrypto/Decrypt.h>
#include <CrappyCrypto/Keys.h>
#include <PortableRuntime/CheckException.h>

namespace CrappyCrypto
{

void decrypt_file(_In_z_ const char* input_file_name, _In_z_ const char* output_file_name, _In_z_ const char* key_file_name);

}

int main(int argc, _In_reads_(argc) char** argv)
{
    if(argc != 4)
    {
        std::fprintf(stderr, "Usage: %s infile outfile keyfile", argv[0]);
        return 0;
    }

    try
    {
        CrappyCrypto::decrypt_file(argv[1], argv[2], argv[3]);
    }
    catch(const std::exception& ex)
    {
        // TODO: Under MSVC, fprintf expects ANSI, not UTF-8.
        std::fprintf(stderr, "%s\n", ex.what());
        return 1;
    }

    return 0;
}

namespace CrappyCrypto
{

void decrypt_file(_In_z_ const char* input_file_name, _In_z_ const char* output_file_name, _In_z_ const char* key_file_name)
{
    // Open input file.
    std::basic_ifstream<uint8_t> input_file(input_file_name, std::ios::binary);
    PortableRuntime::check_exception(input_file.good(), (std::string("Error opening: ") + input_file_name).c_str());

    // Open output file.
    std::basic_ofstream<uint8_t> output_file(output_file_name, std::ios::binary);
    PortableRuntime::check_exception(output_file.good(), (std::string("Error opening: ") + output_file_name).c_str());

     // Open key file.
    std::basic_ifstream<uint8_t> key_file(key_file_name, std::ios::binary);
    PortableRuntime::check_exception(key_file.good(), (std::string("Error opening: ") + key_file_name).c_str());

   // Build key.
    uint8_t key_vector[CrappyCrypto::Skipjack::key_size];
    CrappyCrypto::key_vector_from_key_file(key_vector, sizeof(key_vector), key_file);

    CrappyCrypto::Skipjack::decrypt_fstream(input_file, output_file, key_vector);
}

}

