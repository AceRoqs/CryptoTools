#include "PreCompile.h"
#include "Encrypt.h"

int main(int argc, _In_count_(argc) char** argv)
{
    return CrappyCrypto::Skipjack::encrypt_main(argc, argv);
}

