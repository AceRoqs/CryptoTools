#include "PreCompile.h"
#include "Decrypt.h"        // Pick up forward declarations to ensure correctness.
#include "Skipjack.h"
#include "Keys.h"

namespace CrappyCrypto
{

namespace Skipjack
{

int decrypt_main(int argc, _In_count_(argc) char** argv)
{
    if(argc != 4)
    {
        fprintf(stderr, "Usage: %s infile outfile key", argv[0]);
        return 0;
    }

    // Open input file.
    std::basic_ifstream<uint8_t> input_file(argv[1], std::ios::binary);
    if(!input_file.good())
    {
        fprintf(stderr, "%s: error opening %s\n", argv[0], argv[1]);
        return 1;
    }

    // Open output file.
    std::basic_ofstream<uint8_t> output_file(argv[2], std::ios::binary);
    if(!output_file.good())
    {
        fprintf(stderr, "%s: error opening %s\n", argv[0], argv[2]);
        return 1;
    }

    // Build key.
    uint8_t key_vector[key_length];
    key_vector_from_string(key_vector, sizeof(key_vector), argv[3]);

    // Decrypt file in electronic codebook (ECB) mode.
    uint8_t next_block[block_length];
    input_file.read(next_block, block_length);
    auto current_block_length = static_cast<size_t>(input_file.gcount());
    if(current_block_length == block_length)
    {
        uint8_t current_block[block_length];
        memcpy(current_block, next_block, block_length);
        input_file.read(next_block, block_length);
        auto next_block_length = static_cast<size_t>(input_file.gcount());

        while((current_block_length > 0) && !input_file.bad() && !output_file.bad())
        {
            decrypt(current_block, key_vector);
            if(next_block_length == block_length)
            {
                output_file.write(current_block, block_length);

                current_block_length = next_block_length;
                memcpy(current_block, next_block, block_length);
                input_file.read(next_block, block_length);
                next_block_length = static_cast<size_t>(input_file.gcount());
            }
            else
            {
                if(next_block_length > 0)
                {
                    output_file.write(current_block, next_block[0] <= block_length ? next_block[0] : 0);
                }
                break;
            }
        }
    }

    return 0;
}

}

}

