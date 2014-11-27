#include "PreCompile.h"
#include "Encrypt.h"        // Pick up forward declarations to ensure correctness.
#include "Skipjack.h"
#include "Keys.h"

namespace CrappyCrypto
{

namespace Skipjack
{

int encrypt_main(int argc, _In_count_(argc) char** argv)
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

    // Encrypt file.
    size_t count = 0;
    while(!feof(input_file.get()) && !ferror(input_file.get()) && !ferror(output_file.get()))
    {
        uint8_t block[block_length] = {};
        count = fread(block, 1, block_length, input_file.get());
        if(count > 0)
        {
            encrypt(block, key_vector);
            fwrite(block, 1, block_length, output_file.get());
        }
    }

    if(!ferror(input_file.get()) && !ferror(output_file.get()))
    {
        uint8_t final_count = static_cast<uint8_t>(count > 0 ? count : block_length );
        fwrite(&final_count, 1, sizeof(final_count), output_file.get());
    }
    else
    {
        perror(nullptr);
    }

    return 0;
}

}

}

