#include "PreCompile.h"
#include "Test.h"

int main(int argc, _In_count_(argc) char** argv)
{
    // Unreferenced parameters.
    (argc);
    (argv);

    CrappyCrypto::Skipjack::output_test_vectors();

    return 0;
}

