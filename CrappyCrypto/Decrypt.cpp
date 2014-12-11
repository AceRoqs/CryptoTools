#include "PreCompile.h"
#include "Decrypt.h"        // Pick up forward declarations to ensure correctness.
#include "Skipjack.h"
#include "Keys.h"

namespace CrappyCrypto
{

namespace Skipjack
{

static size_t read_block(std::basic_ifstream<uint8_t>& input_file, _Out_writes_(block_length) uint8_t* block)
{
    input_file.read(block, block_length);
    if(input_file.fail() && !input_file.eof())
    {
        throw std::exception("Error reading input file");
    }

    auto read_block_length = static_cast<size_t>(input_file.gcount());
    if((read_block_length != block_length) && (read_block_length != 0))
    {
        throw std::exception("Invalid input in ciphertext");
    }

    // TODO: This name doesn't work.  Change style to use _size for constants (i.e. max length), and _length for what is valid.
    return read_block_length;
}

static void validate_padding(_In_count_(block_length) const uint8_t* current_block, uint8_t padding)
{
    if((padding == 0) || (padding > block_length))
    {
        throw std::exception("Invalid input in ciphertext");
    }

    // Validate all bytes of padding.
    for(unsigned int ix = 0; ix < padding; ++ix)
    {
        if(current_block[block_length - padding + ix] != padding)
        {
            throw std::exception("Invalid input in ciphertext");
        }
    }
}

void decrypt_file(_In_z_ const char* input_file_name, _In_z_ const char* output_file_name, _In_z_ const char* key_string)
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

    // TODO: Decrypt in chunks.  Extract functions that make sense.

    // Decrypt file in electronic codebook (ECB) mode.
    uint8_t current_block[block_length];
    auto current_block_length = read_block(input_file, current_block);
    if(current_block_length != block_length)
    {
        throw std::exception("Invalid input in ciphertext");
    }
    while(current_block_length == block_length)
    {
        decrypt(current_block, key_vector);

        uint8_t next_block[block_length];
        auto next_block_length = read_block(input_file, next_block);

        unsigned int write_length = block_length;
        if(next_block_length == 0)
        {
            uint8_t padding = current_block[block_length - 1];
            validate_padding(current_block, padding);

            write_length -= padding;
        }

        output_file.write(current_block, write_length);
        if(output_file.fail())
        {
            throw std::exception("Error writing output file");
        }

        std::copy(next_block, next_block + block_length, current_block);
        current_block_length = next_block_length;
    }
}

}

}

