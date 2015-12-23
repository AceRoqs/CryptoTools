#include "PreCompile.h"
#include "Decrypt.h"        // Pick up forward declarations to ensure correctness.
#include "Skipjack.h"
#include "Keys.h"
#include <PortableRuntime/CheckException.h>

namespace CrappyCrypto
{

namespace Skipjack
{

static void decrypt_using_electronic_codebook_mode(_Inout_updates_all_(plaintext_length) uint8_t* plaintext, size_t plaintext_length, _In_reads_(key_size) const uint8_t* key_vector) noexcept
{
    assert((plaintext_length % block_size) == 0);

    // Decrypt in electronic codebook (ECB) mode.
    for(size_t offset = 0; offset < plaintext_length; offset += block_size)
    {
        decrypt(plaintext + offset, key_vector);
    }
}

static void validate_padding(_In_reads_(plaintext_length) const uint8_t* plaintext, size_t plaintext_length, uint8_t padding)
{
    assert(plaintext_length >= block_size);

    PortableRuntime::check_exception((padding != 0) && (padding <= block_size), "Invalid input in ciphertext.");

    // Validate all bytes of padding.
    for(unsigned int ix = 0; ix < padding; ++ix)
    {
        PortableRuntime::check_exception(plaintext[plaintext_length - padding + ix] == padding,
                                         "Invalid input in ciphertext.");
    }
}

static void write_chunk(std::basic_ofstream<uint8_t>& output_file, _In_reads_(chunk_length) const uint8_t* chunk, size_t chunk_length, bool strip_padding)
{
    if(strip_padding)
    {
        uint8_t padding = chunk[chunk_length - 1];
        validate_padding(chunk, chunk_length, padding);

        chunk_length -= padding;
    }

    output_file.write(chunk, chunk_length);
}

static size_t read_chunk(std::basic_ifstream<uint8_t>& input_file, _Out_writes_(chunk_size) uint8_t* chunk, size_t chunk_size)
{
    input_file.read(chunk, chunk_size);

    const auto chunk_length = static_cast<size_t>(input_file.gcount());

    // Validate length is multiple of block length (or zero).
    PortableRuntime::check_exception((chunk_length % block_size) == 0, "Invalid input in ciphertext.");

    return chunk_length;
}

void decrypt_file(_In_z_ const char* input_file_name, _In_z_ const char* output_file_name, _In_z_ const char* key_string)
{
    // Open input file.
    std::basic_ifstream<uint8_t> input_file(input_file_name, std::ios::binary);
    PortableRuntime::check_exception(input_file.good(), (std::string("Error opening: ") + input_file_name).c_str());

    // Open output file.
    std::basic_ofstream<uint8_t> output_file(output_file_name, std::ios::binary);
    PortableRuntime::check_exception(output_file.good(), (std::string("Error opening: ") + output_file_name).c_str());

    // Build key.
    uint8_t key_vector[key_size];
    key_vector_from_string(key_vector, sizeof(key_vector), key_string);

    // Decrypt file in chunks that are multiples of the block size.
    const size_t chunk_size = block_size * 8192;
    std::vector<uint8_t> current_chunk(chunk_size);
    std::vector<uint8_t> next_chunk(chunk_size);

    auto current_valid_length = read_chunk(input_file, current_chunk.data(), current_chunk.size());
    while((current_valid_length > 0) && output_file.good())
    {
        const size_t next_valid_length = read_chunk(input_file, next_chunk.data(), next_chunk.size());

        decrypt_using_electronic_codebook_mode(current_chunk.data(), current_valid_length, key_vector);

        write_chunk(output_file, current_chunk.data(), current_valid_length, (next_valid_length == 0));

        std::swap(current_chunk, next_chunk);
        current_valid_length = next_valid_length;
    }

    PortableRuntime::check_exception(!input_file.fail() || input_file.eof(), "Error reading input file.");
    PortableRuntime::check_exception(!output_file.fail(), "Error writing output file.");
}

}

}

