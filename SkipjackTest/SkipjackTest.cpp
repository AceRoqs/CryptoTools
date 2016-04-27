#include "PreCompile.h"
#include <CrappyCrypto/Test.h>

int main(int argc, _In_reads_(argc) char** argv)
{
    (void)argc;     // Unreferenced parameter.
    (void)argv;

    CrappyCrypto::Skipjack::output_test_vectors();

    return 0;
}

