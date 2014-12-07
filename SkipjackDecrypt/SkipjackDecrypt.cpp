#include "PreCompile.h"
#include "Decrypt.h"

int main(int argc, _In_count_(argc) char** argv)
{
    if(argc != 4)
    {
        fprintf(stderr, "Usage: %s infile outfile key", argv[0]);
        return 0;
    }

    return CrappyCrypto::Skipjack::decrypt_file(argv[1], argv[2], argv[3]);
}

