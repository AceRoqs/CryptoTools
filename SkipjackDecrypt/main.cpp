#include "PreCompile.h"
#include <CrappyCrypto/Decrypt.h>
#include <CrappyCrypto/Keys.h>
#include <PortableRuntime/CheckException.h>

namespace SkipjackDecrypt
{

void decrypt_file(const char* input_file_name, const char* output_file_name, const char* key_file_name)
{
    using namespace PortableRuntime;

    // Open input file.
    std::ifstream input_file(input_file_name, std::ios::binary);
    CHECK_EXCEPTION(input_file.good(), u8"Error opening: " + std::string(input_file_name));

    // Open output file.
    std::ofstream output_file(output_file_name, std::ios::binary | std::ios::trunc);
    CHECK_EXCEPTION(output_file.good(), u8"Error opening: " + std::string(output_file_name));

    // Open key file.
    std::ifstream key_file(key_file_name, std::ios::binary);
    CHECK_EXCEPTION(key_file.good(), u8"Error opening: " + std::string(key_file_name));

    // Build key.
    uint8_t key_vector[CrappyCrypto::Skipjack::key_size];
    CrappyCrypto::key_vector_from_key_file(key_vector, sizeof(key_vector), key_file);

    CrappyCrypto::Skipjack::decrypt_istream(input_file, output_file, key_vector);
}

}

int main(int argc, char** argv)
{
    constexpr auto arg_program_name     = 0;
    constexpr auto arg_input_file_name  = 1;
    constexpr auto arg_output_file_name = 2;
    constexpr auto arg_key_file_name    = 3;

    // ERRORLEVEL zero is the success code.
    int error_level = 0;

    try
    {
        if(argc == 4)
        {
            SkipjackDecrypt::decrypt_file(argv[arg_input_file_name], argv[arg_output_file_name], argv[arg_key_file_name]);
        }
        else
        {
            std::fprintf(stderr, u8"Usage: %s infile outfile keyfile\n", argv[arg_program_name]);
            error_level = 1;
        }
    }
    catch(const std::exception& ex)
    {
        std::fprintf(stderr, u8"\n%s\n", ex.what());
        error_level = 1;
    }

    return error_level;
}

