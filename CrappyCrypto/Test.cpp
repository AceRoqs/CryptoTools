#include "PreCompile.h"
#include "Skipjack.h"

namespace CrappyCrypto
{

namespace Skipjack
{

static void display_vector(unsigned char* vector)
{
    int i;

    for(i = 0; i < block_length; ++i)
    {
        printf("%02x ", vector[i]);
    }
    printf("\n");
}

static void display_vector_and_count(unsigned char* vector, int counter)
{
    int i;

    printf("%d\t", counter);
    for(i = 0; i < block_length; ++i)
    {
        printf("%02x ", vector[i]);
    }
    printf("\n");
}

int test_main(int argc, char** argv)
{
    // Unreferenced parameters.
    (argc);
    (argv);

    const uint8_t keyvector[] =
    {
        0x00, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11
    };

    uint8_t testvector[] =
    {
        0x33, 0x22, 0x11, 0x00, 0xdd, 0xcc, 0xbb, 0xaa
    };

    uint8_t testvector2[] =
    {
        0x33, 0x22, 0x11, 0x00, 0xdd, 0xcc, 0xbb, 0xaa
    };

    uint16_t i, counter;

    printf("Skipjack test vectors\n\n");

    printf("plaintext\t");
    display_vector(testvector);

    printf("key      \t");
    for(i = 0; i < key_length; ++i)
    {
        printf("%02x ", keyvector[i]);
    }
    printf("\n\n");

    display_vector_and_count(testvector, 0);
    for(counter = 1; counter <= iter_per_func * 1; ++counter)
    {
        rule_a(testvector, keyvector, counter);
        display_vector_and_count(testvector, counter);
    }
    for(; counter <= iter_per_func * 2; ++counter)
    {
        rule_b(testvector, keyvector, counter);
        display_vector_and_count(testvector, counter);
    }
    for(; counter <= iter_per_func * 3; ++counter)
    {
        rule_a(testvector, keyvector, counter);
        display_vector_and_count(testvector, counter);
    }
    for(; counter <= iter_per_func * 4; ++counter)
    {
        rule_b(testvector, keyvector, counter);
        display_vector_and_count(testvector, counter);
    }

    encrypt(testvector2, keyvector);
    printf("\nCiphertext output: ");
    display_vector(testvector2);

    decrypt(testvector2, keyvector);
    printf("\nPlaintext reverse: ");
    display_vector(testvector2);

    return 0;
}

}

}

