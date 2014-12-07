#include "PreCompile.h"
#include "Decrypt.h"        // Pick up forward declarations to ensure correctness.
#include "Skipjack.h"
#include "Keys.h"

namespace CrappyCrypto
{

namespace Skipjack
{

int decrypt_file(_In_z_ const char* input_file_name, _In_z_ const char* output_file_name, _In_z_ const char* key_string)
{
    // Open input file.
    std::basic_ifstream<uint8_t> input_file(input_file_name, std::ios::binary);
    if(!input_file.good())
    {
        fprintf(stderr, "Error opening %s\n", input_file_name);
        return 1;
    }

    // Open output file.
    std::basic_ofstream<uint8_t> output_file(output_file_name, std::ios::binary);
    if(!output_file.good())
    {
        fprintf(stderr, "Error opening %s\n", output_file_name);
        return 1;
    }

    // Build key.
    uint8_t key_vector[key_length];
    key_vector_from_string(key_vector, sizeof(key_vector), key_string);

    // Decrypt file in electronic codebook (ECB) mode.
    uint8_t current_block[block_length];
    input_file.read(current_block, block_length);
    auto current_block_length = static_cast<size_t>(input_file.gcount());
    while(current_block_length == block_length)
    {
        decrypt(current_block, key_vector);

        uint8_t next_block[block_length];
        input_file.read(next_block, block_length);
        auto next_block_length = static_cast<size_t>(input_file.gcount());

        unsigned int write_length = block_length;
        if(next_block_length != block_length)
        {
            if(next_block_length != 0)
            {
                return 1;   // Invalid input.
            }

            uint8_t padding = current_block[block_length - 1];
            if((padding == 0) || (padding > block_length))
            {
                return 1;   // Invalid input.
            }

            write_length -= padding;

            // Validate all bytes of padding.
            for(unsigned int ix = 0; ix < padding; ++ix)
            {
                if(current_block[write_length + ix] != padding)
                {
                    return 1;   // Invalid input.
                }
            }
        }

        output_file.write(current_block, write_length);

        memcpy(current_block, next_block, block_length);
        current_block_length = next_block_length;
    }
    if(current_block_length != 0)
    {
        return 1;   // Invalid input.
    }

    return 0;
}

}

}

