#include "PreCompile.h"
#include "Test.h"           // Pick up forward declarations to ensure correctness.
#include "Skipjack.h"

namespace CrappyCrypto
{

namespace Skipjack
{

static void display_vector(_In_reads_(length) const uint8_t* vector, size_t length) noexcept
{
    for(size_t ix = 0; ix < length; ++ix)
    {
        std::printf("%02x ", vector[ix]);
    }
    std::printf("\n");
}

static void display_count_and_vector(unsigned int counter, _In_reads_(length) const uint8_t* vector, size_t length) noexcept
{
    std::printf("%u: ", counter);
    display_vector(vector, length);
}

void output_test_vectors()
{
    // Test vectors taken from the Skipjack specification at:
    // http://csrc.nist.gov/groups/ST/toolkit/documents/skipjack/skipjack.pdf
    constexpr uint8_t key_vector[] =
    {
        0x00, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11
    };

    constexpr uint8_t expected_ciphertext[] =
    {
        0x25, 0x87, 0xca, 0xe2, 0x7a, 0x12, 0xd3, 0x00
    };

    uint8_t test_vector[] =
    {
        0x33, 0x22, 0x11, 0x00, 0xdd, 0xcc, 0xbb, 0xaa
    };
    uint8_t test_vector_copy[sizeof(test_vector)];
    std::copy(test_vector, test_vector + sizeof(test_vector), test_vector_copy);

    std::printf("Skipjack test vectors\n\n");

    std::printf("plaintext: ");
    display_vector(test_vector, sizeof(test_vector));

    std::printf("key:       ");
    display_vector(key_vector, sizeof(key_vector));
    std::printf("\n");

    display_count_and_vector(0, test_vector, sizeof(test_vector));
    uint16_t counter;
    for(counter = 1; counter <= iterations_per_rule * 1; ++counter)
    {
        rule_a(test_vector, key_vector, counter);
        display_count_and_vector(counter, test_vector, sizeof(test_vector));
    }
    for(; counter <= iterations_per_rule * 2; ++counter)
    {
        rule_b(test_vector, key_vector, counter);
        display_count_and_vector(counter, test_vector, sizeof(test_vector));
    }
    for(; counter <= iterations_per_rule * 3; ++counter)
    {
        rule_a(test_vector, key_vector, counter);
        display_count_and_vector(counter, test_vector, sizeof(test_vector));
    }
    for(; counter <= iterations_per_rule * 4; ++counter)
    {
        rule_b(test_vector, key_vector, counter);
        display_count_and_vector(counter, test_vector, sizeof(test_vector));
    }

    encrypt(test_vector_copy, key_vector);
    std::printf("\nCiphertext output: ");
    display_vector(test_vector_copy, sizeof(test_vector_copy));
    std::printf("Expected:          ");
    display_vector(expected_ciphertext, sizeof(expected_ciphertext));

    decrypt(test_vector_copy, key_vector);
    std::printf("\nPlaintext reverse: ");
    display_vector(test_vector_copy, sizeof(test_vector_copy));
}

}

}

