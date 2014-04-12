#include "PreCompile.h"
#include "Skipjack.h"

static const unsigned char keyvector[] =
{
    0x00, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11
};

static unsigned char testvector[] =
{
    0x33, 0x22, 0x11, 0x00, 0xdd, 0xcc, 0xbb, 0xaa
};

static unsigned char testvector2[] =
{
    0x33, 0x22, 0x11, 0x00, 0xdd, 0xcc, 0xbb, 0xaa
};

static void DispVector(unsigned char* vector)
{
    int i;

    for(i = 0; i < BLOCKLENGTH; ++i)
    {
        printf("%02x ", vector[i]);
    }
    printf("\n");
}

static void DispVectorAndCount(int counter)
{
    int i;

    printf("%d\t", counter);
    for(i = 0; i < BLOCKLENGTH; ++i)
    {
        printf("%02x ", testvector[i]);
    }
    printf("\n");
}

int main()
{
    int i, counter;

    printf("Skipjack test vectors\n\n");

    printf("plaintext\t");
    DispVector(testvector);

    printf("key      \t");
    for(i = 0; i < KEYLENGTH; ++i)
    {
        printf("%02x ", keyvector[i]);
    }
    printf("\n\n");

    DispVectorAndCount(0);
    for(counter = 1; counter <= ITER_PER_FUNC * 1; ++counter)
    {
        CrappyCrypto::RuleA((uint16_t *)testvector, keyvector, counter);
        DispVectorAndCount(counter);
    }
    for(; counter <= ITER_PER_FUNC * 2; ++counter)
    {
        CrappyCrypto::RuleB((uint16_t *)testvector, keyvector, counter);
        DispVectorAndCount(counter);
    }
    for(; counter <= ITER_PER_FUNC * 3; ++counter)
    {
        CrappyCrypto::RuleA((uint16_t *)testvector, keyvector, counter);
        DispVectorAndCount(counter);
    }
    for(; counter <= ITER_PER_FUNC * 4; ++counter)
    {
        CrappyCrypto::RuleB((uint16_t *)testvector, keyvector, counter);
        DispVectorAndCount(counter);
    }

    CrappyCrypto::SJ_Encrypt(testvector2, keyvector);
    printf("\nCiphertext output: ");
    DispVector(testvector2);

    CrappyCrypto::SJ_Decrypt(testvector2, keyvector);
    printf("\nPlaintext reverse: ");
    DispVector(testvector2);

    return 0;
}

