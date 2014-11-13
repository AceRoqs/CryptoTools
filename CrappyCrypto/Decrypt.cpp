#include "PreCompile.h"
#include "Skipjack.h"

namespace CrappyCrypto
{

namespace Skipjack
{

int decrypt_main(int argc, char** argv)
{
    if(argc != 4)
    {
        fprintf(stderr, "Usage: %s infile outfile key", argv[0]);
        exit(0);
    }

    // Open input file.
    FILE* input_file = fopen(argv[1], "rb");
    if(input_file == nullptr)
    {
        fprintf(stderr, "%s: error opening %s\n", argv[0], argv[1]);
        exit(1);
    }

    // Open output file.
    FILE* output_file = fopen(argv[2], "wb");
    if(output_file == nullptr)
    {
        fclose(input_file);
        fprintf(stderr, "%s: error opening %s\n", argv[0], argv[2]);
        exit(1);
    }

    // Build key.
    // This needs to match the code in Encrypt.cpp.
    // TODO: Share this code.
    // NOTE: Since the input is ASCII, most bits have no chance at being set, which limits the range of keys.
    uint8_t keyvector[key_length] = {};
    for(size_t ix = 0; argv[3][ix] != '\0'; ++ix)
    {
        keyvector[ix % key_length] ^= argv[3][ix];
    }

    // Decrypt file.
    uint8_t current_block[block_length];
    uint8_t next_block[block_length];
    size_t current_block_length = fread(next_block, 1, block_length, input_file);
    if(current_block_length == block_length)
    {
        memcpy(current_block, next_block, block_length);
        size_t next_block_length = fread(next_block, 1, block_length, input_file);

        while((current_block_length > 0) && !ferror(input_file) && !ferror(output_file))
        {
            decrypt(current_block, keyvector);
            if(next_block_length == block_length)
            {
                fwrite(current_block, 1, block_length, output_file);

                current_block_length = next_block_length;
                memcpy(current_block, next_block, block_length);
                next_block_length = fread(next_block, 1, block_length, input_file);
            }
            else
            {
                if(next_block_length > 0)
                {
                    fwrite(current_block, 1, next_block[0] <= block_length ? next_block[0] : 0, output_file);
                }
                break;
            }
        }
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}

}

}

