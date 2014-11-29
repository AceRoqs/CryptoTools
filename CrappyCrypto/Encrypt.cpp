#include "PreCompile.h"
#include "Encrypt.h"        // Pick up forward declarations to ensure correctness.
#include "Skipjack.h"
#include "Keys.h"

namespace CrappyCrypto
{

namespace Skipjack
{

#if 0
static int get_file_size(FILE* file)
{
    int current_position = ftell(file);

    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);

    fseek(file, current_position, SEEK_SET);

    return file_size;
}

template <typename Ty>
Ty round_up(Ty number, Ty multiple) /* noexcept */
{
    return ((number + multiple - 1) / multiple) * multiple;
}
#endif

int encrypt_main(int argc, _In_count_(argc) char** argv)
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


#if 0
TODO:
Take pointers instead of files.
Fix decrypt error handling.
Change padding to remove leaky last byte.
decrypt.cpp/encrypt.cpp : extract function
Move out of lib into driver functions? - Depends on how the final version looks.
Remove readme/makefile.
Noexcept added where it makes sense.
#endif

#if 1
    // Build key.
    uint8_t key_vector[key_length];
    key_vector_from_string(key_vector, sizeof(key_vector), argv[3]);

    // Encrypt file in electronic codebook (ECB) mode.
    size_t count = 0;
    while(input_file.good() && output_file.good())
    {
        // NOTE: Blocks are zero padded, which possibly leaks information.
        uint8_t block[block_length] = {};
        input_file.read(block, block_length);
        count = static_cast<size_t>(input_file.gcount());
        if(count > 0)
        {
            encrypt(block, key_vector);
            output_file.write(block, block_length);
        }
    }

    if(input_file.eof() && output_file.good())
    {
        uint8_t final_count = static_cast<uint8_t>(count > 0 ? count : block_length );
        output_file.write(&final_count, sizeof(final_count));
    }
    else
    {
        // TODO:
        //perror(nullptr);
    }

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

}

}

