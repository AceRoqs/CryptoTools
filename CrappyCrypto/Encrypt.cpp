#include "PreCompile.h"
#include "Encrypt.h"        // Pick up forward declarations to ensure correctness.
#include "Skipjack.h"
#include "Keys.h"

namespace CrappyCrypto
{

namespace Skipjack
{

static void encrypt_using_electronic_codebook_mode(_In_count_(plaintext_length) uint8_t* plaintext, size_t plaintext_length, const uint8_t* key_vector) NOEXCEPT
{
    // Encrypt in electronic codebook (ECB) mode.
    for(size_t offset = 0; offset < plaintext_length; offset += block_length)
    {
        encrypt(plaintext + offset, key_vector);
    }
}

void encrypt_file(_In_z_ const char* input_file_name, _In_z_ const char* output_file_name, _In_z_ const char* key_string)
{
    // Open input file.
    std::basic_ifstream<uint8_t> input_file(input_file_name, std::ios::binary);
    if(!input_file.good())
    {
        throw std::exception((std::string("Error opening ") + input_file_name).c_str());
    }

    // Open output file.
    std::basic_ofstream<uint8_t> output_file(output_file_name, std::ios::binary);
    if(!output_file.good())
    {
        throw std::exception((std::string("Error opening ") + output_file_name).c_str());
    }

    // Build key.
    uint8_t key_vector[key_length];
    key_vector_from_string(key_vector, sizeof(key_vector), key_string);

    // Encrypt file in chunks that are multiples of the block size.
    const size_t chunk_length = block_length * 8192;
    std::vector<uint8_t> chunk(chunk_length);

    uint8_t padding = 0;
    while((padding == 0) && input_file.good() && output_file.good())
    {
        input_file.read(&chunk[0], chunk.size());
        auto valid_length = static_cast<size_t>(input_file.gcount());
        if(valid_length < chunk.size())
        {
            padding = block_length - (valid_length % block_length);

            // Pad blocks per Schneier.
            std::fill(&chunk[valid_length], &chunk[valid_length + padding], padding);
            valid_length += padding;
        }

        encrypt_using_electronic_codebook_mode(chunk.data(), valid_length, key_vector);

        output_file.write(&chunk[0], valid_length);
    }

    if(input_file.fail() && !input_file.eof())
    {
        throw std::exception("Error reading input file");
    }
    else if(output_file.fail())
    {
        throw std::exception("Error writing output file");
    }
}

}

}

