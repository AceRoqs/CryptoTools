#include "PreCompile.h"
#include "Encrypt.h"        // Pick up forward declarations to ensure correctness.
#include "Skipjack.h"
#include <PortableRuntime/CheckException.h>

namespace CrappyCrypto
{

namespace Skipjack
{

static void encrypt_using_electronic_codebook_mode(_Inout_updates_all_(plaintext_length) uint8_t* plaintext, size_t plaintext_length, _In_reads_(key_size) const uint8_t* key_vector) noexcept
{
    assert((plaintext_length % block_size) == 0);

    // Encrypt in electronic codebook (ECB) mode.
    for(size_t offset = 0; offset < plaintext_length; offset += block_size)
    {
        encrypt(plaintext + offset, key_vector);
    }
}

// plaintext_size is the full size of the buffer, which is the amount that can be written.
// read_length is the amount readable, which is <= plaintext_size.
static size_t pad_plaintext_if_needed(_Inout_updates_(plaintext_size) uint8_t* plaintext, size_t plaintext_size, size_t read_length)
{
    assert((plaintext_size % block_size) == 0);
    assert(read_length <= plaintext_size);

    // Pad up to the nearest block_size.
    if(read_length < plaintext_size)
    {
        const uint8_t padding = block_size - (read_length % block_size);

        // Pad blocks per Schneier.
        std::fill(&plaintext[read_length], &plaintext[read_length + padding], padding);
        read_length += padding;
    }

    return read_length;
}

void encrypt_istream(std::istream& input_stream, std::ostream& output_stream, _In_ uint8_t key_vector[key_size])
{
    // Encrypt file in chunks that are multiples of the block size.
    const size_t chunk_size = block_size * 8192;
    std::vector<uint8_t> chunk(chunk_size);

    size_t read_length = chunk.size();
    while((read_length == chunk.size()) && output_stream.good())
    {
        input_stream.read(reinterpret_cast<char*>(chunk.data()), chunk.size());
        read_length = static_cast<size_t>(input_stream.gcount());

        const auto valid_length = pad_plaintext_if_needed(chunk.data(), chunk.size(), read_length);

        encrypt_using_electronic_codebook_mode(chunk.data(), valid_length, key_vector);

        output_stream.write(reinterpret_cast<const char*>(chunk.data()), valid_length);
    }

    CHECK_EXCEPTION(!input_stream.fail() || input_stream.eof(), "Error reading input file.");
    CHECK_EXCEPTION(!output_stream.fail(), "Error writing output file.");
}

}

}

