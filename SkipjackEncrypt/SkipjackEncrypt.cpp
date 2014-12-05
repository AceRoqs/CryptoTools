#include "PreCompile.h"
#include "Encrypt.h"

int main(int argc, _In_count_(argc) char** argv)
{
    if(argc != 4)
    {
        fprintf(stderr, "Usage: SkipjackEncrypt.exe infile outfile key");
        return 0;
    }

    return CrappyCrypto::Skipjack::encrypt_file(argv[1], argv[2], argv[3]);
}

