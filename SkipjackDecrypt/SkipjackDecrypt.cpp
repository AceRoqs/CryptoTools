#include "PreCompile.h"
#include "Decrypt.h"

int main(int argc, _In_count_(argc) char** argv)
{
    return CrappyCrypto::Skipjack::decrypt_main(argc, argv);
}

