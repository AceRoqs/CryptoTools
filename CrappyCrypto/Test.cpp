#include "PreCompile.h"
#include "Skipjack.h"

namespace CrappyCrypto
{

namespace Skipjack
{

static void display_vector(const uint8_t* vector, size_t length)
{
    for(size_t ix = 0; ix < length; ++ix)
    {
        printf("%02x ", vector[ix]);
    }
    printf("\n");
}

static void display_count_and_vector(unsigned int counter, const uint8_t* vector, size_t length)
{
    printf("%u: ", counter);
    display_vector(vector, length);
}

int test_main(int argc, char** argv)
{
    // Unreferenced parameters.
    (argc);
    (argv);

    const uint8_t key_vector[] =
    {
        0x00, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11
    };

    uint8_t test_vector[] =
    {
        0x33, 0x22, 0x11, 0x00, 0xdd, 0xcc, 0xbb, 0xaa
    };
    uint8_t test_vector_copy[sizeof(test_vector)];
    memcpy(test_vector_copy, test_vector, sizeof(test_vector));

    printf("Skipjack test vectors\n\n");

    printf("plaintext: ");
    display_vector(test_vector, sizeof(test_vector));

    printf("key:       ");
    display_vector(key_vector, sizeof(key_vector));
    printf("\n");

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
    printf("\nCiphertext output: ");
    display_vector(test_vector_copy, sizeof(test_vector_copy));

    decrypt(test_vector_copy, key_vector);
    printf("\nPlaintext reverse: ");
    display_vector(test_vector_copy, sizeof(test_vector_copy));

    return 0;
}

}

}

