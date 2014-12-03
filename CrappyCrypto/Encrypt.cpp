#include "PreCompile.h"
#include "Encrypt.h"        // Pick up forward declarations to ensure correctness.
#include "Skipjack.h"
#include "Keys.h"

namespace CrappyCrypto
{

namespace Skipjack
{

static int encrypt_file(_In_z_ const char* input_file_name, _In_z_ const char* output_file_name, _In_z_ const char* key_string)
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


#if 0
TODO:
Take pointers instead of files.
Fix decrypt error handling and error reporting across the board.
decrypt.cpp/encrypt.cpp : extract function
Move out of lib into driver functions? - Depends on how the final version looks.
Remove readme/makefile.  Update readme.md.
Noexcept added where it makes sense.
#endif

#if 1
    // Build key.
    uint8_t key_vector[key_length];
    key_vector_from_string(key_vector, sizeof(key_vector), key_string);

    uint8_t padding = 0;
    const size_t chunk_length = block_length * 8192;
    std::vector<uint8_t> chunk(chunk_length);
    do
    {
        input_file.read(&chunk[0], chunk.size());
        auto valid_length = static_cast<size_t>(input_file.gcount());
        if(valid_length < chunk.size())
        {
            padding = block_length - valid_length % block_length;

            // Pad blocks per Schneier.
            std::fill(&chunk[valid_length], &chunk[valid_length + padding], padding);
            valid_length += padding;
        }

        // Encrypt in electronic codebook (ECB) mode.
        for(size_t ix = 0; ix < chunk.size(); ix += block_length)
        {
            encrypt(&chunk[ix], key_vector);
        }

        output_file.write(&chunk[0], valid_length);
    } while(padding == 0);

    // TODO: error handle, check reads/writes.

#endif

#if 0
// TODO: support greater than size_t files.
// ifstream - no file_close lambda required.
// naked for loop - std::transform?
    auto file_size = get_file_size(input_file.get());
    const auto encrypted_length = round_up<size_t>(file_size + 1, block_length);
    const auto padding = static_cast<unsigned int>(encrypted_length - file_size);
    std::vector<uint8_t> v;
    v.resize(encrypted_length);
    size_t count = fread(&v[0], 1, file_size, input_file.get());
    memset(&v[count], padding, padding);

    // Build key.
    uint8_t key_vector[key_length];
    key_vector_from_string(key_vector, sizeof(key_vector), argv[3]);

    // Encrypt file in electronic codebook (ECB) mode.
    for(size_t offset = 0; offset < encrypted_length; offset += block_length)
    {
        encrypt(&v[offset], key_vector);
    }

    fwrite(&v[0], 1, v.size(), output_file.get());
#endif
#if 0
    // TODO: test failbit
    std::ifstream input_file1(argv[1], std::ios::in | std::ifstream::binary | std::ios::ate);
    if(input_file1.good())
    {
        return 1;
    }

    const auto file_size = input_file1.tellg();
    input_file1.seekg(0, std::ios::beg);

    const auto encrypted_length = round_up<decltype(file_size)>(file_size, block_length);
    const int padding = encrypted_length - file_size;

    std::vector<uint8_t> v;
    v.reserve(encrypted_length);

    std::copy(std::istreambuf_iterator<char>(input_file1), std::istreambuf_iterator<char>(), std::back_inserter(v));
    std::fill_n(std::back_inserter(v), padding, padding);   // Pad blocks per Schneier.

    // Build key.
    uint8_t key_vector[key_length];
    key_vector_from_string(key_vector, sizeof(key_vector), argv[3]);

    // Encrypt file in electronic codebook (ECB) mode.
    auto block_count = encrypted_length / block_length;
    for(decltype(block_count) ix = 0; ix < block_count; ++ix)
    {
        encrypt(&v[0] + ix * block_length, key_vector);
    }

    fwrite(&v[0], 1, v.size(), output_file.get());
#endif
#if 0
get file size
get alloc size = file size rounded to the next block
read in file
pad file
encrypt block
write file
#endif

    return 0;
}

int encrypt_main(int argc, _In_count_(argc) char** argv)
{
    if(argc != 4)
    {
        fprintf(stderr, "Usage: %s infile outfile key", argv[0]);
        return 0;
    }

    return encrypt_file(argv[1], argv[2], argv[3]);
}

}

}

