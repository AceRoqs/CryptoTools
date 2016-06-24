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

    CHECK_EXCEPTION((padding != 0) && (padding <= block_size), "Invalid input in ciphertext.");

    // Validate all bytes of padding.
    for(unsigned int ix = 0; ix < padding; ++ix)
    {
        CHECK_EXCEPTION(plaintext[plaintext_length - padding + ix] == padding,
                        "Invalid input in ciphertext.");
    }
}

static void write_chunk(std::ostream& output_stream, _In_reads_(chunk_length) const uint8_t* chunk, size_t chunk_length, bool strip_padding)
{
    if(strip_padding)
    {
        uint8_t padding = chunk[chunk_length - 1];
        validate_padding(chunk, chunk_length, padding);

        chunk_length -= padding;
    }

    output_stream.write(reinterpret_cast<const char*>(chunk), chunk_length);
}

static size_t read_chunk(std::istream& input_stream, _Out_writes_(chunk_size) uint8_t* chunk, size_t chunk_size)
{
    input_stream.read(reinterpret_cast<char*>(chunk), chunk_size);

    const auto chunk_length = static_cast<size_t>(input_stream.gcount());

    // Validate length is multiple of block length (or zero).
    CHECK_EXCEPTION((chunk_length % block_size) == 0, "Invalid input in ciphertext.");

    return chunk_length;
}

void decrypt_istream(std::istream& input_stream, std::ostream& output_stream, _In_ uint8_t key_vector[key_size])
{
    // Decrypt file in chunks that are multiples of the block size.
    constexpr size_t chunk_size = block_size * 8192;
    std::vector<uint8_t> current_chunk(chunk_size);
    std::vector<uint8_t> next_chunk(chunk_size);

    auto current_valid_length = read_chunk(input_stream, current_chunk.data(), current_chunk.size());
    while((current_valid_length > 0) && output_stream.good())
    {
        const size_t next_valid_length = read_chunk(input_stream, next_chunk.data(), next_chunk.size());

        decrypt_using_electronic_codebook_mode(current_chunk.data(), current_valid_length, key_vector);

        write_chunk(output_stream, current_chunk.data(), current_valid_length, (next_valid_length == 0));

        std::swap(current_chunk, next_chunk);
        current_valid_length = next_valid_length;
    }

    CHECK_EXCEPTION(!input_stream.fail() || input_stream.eof(), "Error reading input file.");
    CHECK_EXCEPTION(!output_stream.fail(), "Error writing output file.");
}

}

}

