#include "PreCompile.h"
#include "Base64.h"         // Pick up forward declarations to ensure correctness.
#include <PortableRuntime/CheckException.h>

static const char base64_map[] =
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};

union Base64_block
{
    std::array<uint8_t, 3> bytes;
    struct
    {
        // Layout:
        // |00000000 |11111111 |22222222 : bytes[0..2]
        // aaaaaa|bb cccc|dddd ee|ffffff : bits[23..0]

        // Layout on MSVC is least significant bit of each byte first,
        // and this may be vendor defined.
        uint8_t b : 2;
        uint8_t a : 6;
        uint8_t d : 4;
        uint8_t c : 4;
        uint8_t f : 6;
        uint8_t e : 2;
    } bits;

    uint8_t bits_b23_18() const
    {
        return bits.a;
    }
    uint8_t bits_b17_12() const
    {
        return (bits.b << 4) | bits.c;
    }
    uint8_t bits_b11_6() const
    {
        return (bits.d << 2) | bits.e;
    }
    uint8_t bits_b5_0() const
    {
        return bits.f;
    }

    void bits_b23_18(uint8_t six_bits)
    {
        assert(six_bits < 64);
        bits.a = six_bits;
    }
    void bits_b17_12(uint8_t six_bits)
    {
        assert(six_bits < 64);
        bits.b = (six_bits >> 4);
        bits.c = (six_bits & 0xf);
    }
    void bits_b11_6(uint8_t six_bits)
    {
        assert(six_bits < 64);
        bits.d = (six_bits >> 2);
        bits.e = (six_bits & 3);
    }
    void bits_b5_0(uint8_t six_bits)
    {
        assert(six_bits < 64);
        bits.f = six_bits;
    }
};

// Vector does not need to be padded to four character boundary.
static size_t Base64_size_from_array_size(size_t byte_count)
{
    return (byte_count + 2) / 3 * 4;
}

// Base64 is always aligned.
static size_t Array_size_from_base64_size(size_t byte_count)
{
    assert(byte_count % 4 == 0);
    return byte_count / 4 * 3;
}

// Return value is [0..63].
static uint8_t Bits_from_base64_char(char ch)
{
    uint8_t value;

    if(ch >= 'A' && ch <= 'Z')
    {
        value = ch - 'A';
    }
    else if(ch >= 'a' && ch <= 'z')
    {
        value = ch - 'a' + 26;
    }
    else if(ch >= '0' && ch <= '9')
    {
        value = ch - '0' + 52;
    }
    else if(ch == '+')
    {
        value = 62;
    }
    else
    {
        CHECK_EXCEPTION(ch == '/', u8"Invalid base64 character.");
        value = 63;
    }

    return value;
}

// https://en.wikipedia.org/wiki/Base64.
std::vector<uint8_t> Base64_from_vector(const std::vector<uint8_t>& input)
{
    std::vector<uint8_t> output;
    output.reserve(Base64_size_from_array_size(input.size()));

    auto input_iter = input.cbegin();
    while(input_iter != input.cend())
    {
        Base64_block block = {};
        for(auto block_iter = std::begin(block.bytes); (block_iter != std::end(block.bytes)) && (input_iter != input.cend());)
        {
            *block_iter++ = *input_iter++;
        }

        output.push_back(base64_map[block.bits_b23_18()]);
        output.push_back(base64_map[block.bits_b17_12()]);
        output.push_back(base64_map[block.bits_b11_6()]);
        output.push_back(base64_map[block.bits_b5_0()]);
    }

    auto output_iter = output.rbegin();
    for(decltype(input.size()) ix = 0; ix < ((3 - (input.size() % 3)) % 3); ++ix)
    {
        *output_iter++ = '=';
    }

    return output;
}

std::vector<uint8_t> Vector_from_base64(const std::vector<uint8_t>& input)
{
    // TODO: where should these asserts go?  They came from main.
    static_assert(sizeof(base64_map) == 64, "base64_map must be 64 entries long.");
    static_assert(sizeof(Base64_block) == 3, "Base64_block must be 3 bytes long.");

    CHECK_EXCEPTION((input.size() != 0) && (input.size() % 4) == 0, u8"Base64 input size must non-zero and be a multiple of four.");

    std::vector<uint8_t> output;
    output.reserve(Array_size_from_base64_size(input.size()));

    Base64_block block;
    auto base64_char = input.cbegin();
    for(decltype(input.size()) ix = 0; ix < (input.size() / 4) - 1; ++ix)
    {
        block.bits_b23_18(Bits_from_base64_char(*base64_char++));
        block.bits_b17_12(Bits_from_base64_char(*base64_char++));
        block.bits_b11_6(Bits_from_base64_char(*base64_char++));
        block.bits_b5_0(Bits_from_base64_char(*base64_char++));

        output.push_back(block.bytes[0]);
        output.push_back(block.bytes[1]);
        output.push_back(block.bytes[2]);
    }

    block.bits_b23_18(Bits_from_base64_char(*base64_char++));
    block.bits_b17_12(Bits_from_base64_char(*base64_char++));
    output.push_back(block.bytes[0]);

    if(*base64_char != '=')
    {
        block.bits_b11_6(Bits_from_base64_char(*base64_char++));
        output.push_back(block.bytes[1]);

        if(*base64_char != '=')
        {
            block.bits_b5_0(Bits_from_base64_char(*base64_char++));
            output.push_back(block.bytes[2]);
        }
    }

    return output;
}

#if 0
bool Array_to_base64(const uint8_t* bytes, size_t byte_count, uint8_t* output, size_t output_char_count)
{
    if(Base64_size_from_array_size(byte_count) > output_char_count)
    {
        return false;
    }

    const auto total_count = (byte_count + 2) / 3 * 3;
    const auto padding_count = total_count - byte_count;

    // Pad source material to three byte boundary.
    std::unique_ptr<uint8_t[]> source_set(new uint8_t[total_count]);
    //std::copy(bytes, bytes + byte_count, source_set.get());
    memcpy(source_set.get(), bytes, byte_count);
    //memset(source_set.get() + byte_count, 0, padding_count);
    std::fill(source_set.get() + byte_count, source_set.get() + byte_count + padding_count, 0);

    size_t output_ix = 0;
    for(auto ix = 0u; ix < total_count / 3; ++ix)
    {
        const auto byte0 = source_set[ix * 3];
        const auto byte1 = source_set[ix * 3 + 1];
        const auto byte2 = source_set[ix * 3 + 2];

        output[output_ix++] = base64_map[byte0 >> 2];
        output[output_ix++] = base64_map[((byte0 & 3) << 4) | (byte1 >> 4)];
        output[output_ix++] = base64_map[((byte1 & 0x0f) << 2) | (byte2 >> 6)];
        output[output_ix++] = base64_map[byte2 & 0x3f];
    }

    // total_count - byte_count is correct enough for this purpose.
    for(auto ix = 0u; ix < total_count - byte_count; ++ix)
    {
        output[output_ix - ix - 1] = '=';
    }

    return true;
}
#endif

