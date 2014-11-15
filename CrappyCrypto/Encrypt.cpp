#include "PreCompile.h"
#include "Skipjack.h"
#include "Keys.h"

namespace CrappyCrypto
{

namespace Skipjack
{

int encrypt_main(int argc, char** argv)
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
    uint8_t key_vector[key_length];
    key_vector_from_string(key_vector, sizeof(key_vector), argv[3]);

    // Encrypt file.
    size_t count = 0;
    while(!feof(input_file) && !ferror(input_file) && !ferror(output_file))
    {
        uint8_t block[block_length];
        count = fread(block, 1, block_length, input_file);
        if(count > 0)
        {
            memset(block + count, 0, block_length - count);
            encrypt(block, key_vector);
            fwrite(block, 1, block_length, output_file);
        }
    }

    if(!ferror(input_file) && !ferror(output_file))
    {
        uint8_t final_count = static_cast<uint8_t>(count > 0 ? count : block_length );
        fwrite(&final_count, 1, sizeof(final_count), output_file);
    }
    else
    {
        perror(nullptr);
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}

}

}

