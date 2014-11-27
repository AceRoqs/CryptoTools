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

    const auto file_close = [](_In_ FILE* open_file)
    {
        if(open_file != nullptr)
        {
            fclose(open_file);
        }
    };

    // Open input file.
    typedef std::unique_ptr<FILE, std::function<void (_In_ FILE* open_file)>> File;
    const File input_file(fopen(argv[1], "rb"), file_close);
    if(input_file == nullptr)
    {
        fprintf(stderr, "%s: error opening %s\n", argv[0], argv[1]);
        return 1;
    }

    // Open output file.
    const File output_file(fopen(argv[2], "wb"), file_close);
    if(output_file == nullptr)
    {
        fprintf(stderr, "%s: error opening %s\n", argv[0], argv[2]);
        return 1;
    }

    // Build key.
    uint8_t key_vector[key_length];
    key_vector_from_string(key_vector, sizeof(key_vector), argv[3]);

    // Decrypt file in electronic codebook (ECB) mode.
    uint8_t next_block[block_length];
    size_t current_block_length = fread(next_block, 1, block_length, input_file.get());
    if(current_block_length == block_length)
    {
        uint8_t current_block[block_length];
        memcpy(current_block, next_block, block_length);
        size_t next_block_length = fread(next_block, 1, block_length, input_file.get());

        while((current_block_length > 0) && !ferror(input_file.get()) && !ferror(output_file.get()))
        {
            decrypt(current_block, key_vector);
            if(next_block_length == block_length)
            {
                fwrite(current_block, 1, block_length, output_file.get());

                current_block_length = next_block_length;
                memcpy(current_block, next_block, block_length);
                next_block_length = fread(next_block, 1, block_length, input_file.get());
            }
            else
            {
                if(next_block_length > 0)
                {
                    fwrite(current_block, 1, next_block[0] <= block_length ? next_block[0] : 0, output_file.get());
                }
                break;
            }
        }
    }

    return 0;
}

}

}

