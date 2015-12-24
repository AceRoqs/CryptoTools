#include "PreCompile.h"
#include <CrappyCrypto/Decrypt.h>

int main(int argc, _In_reads_(argc) char** argv)
{
    if(argc != 4)
    {
        std::fprintf(stderr, "Usage: %s infile outfile keyfile", argv[0]);
        return 0;
    }

    try
    {
        CrappyCrypto::Skipjack::decrypt_file(argv[1], argv[2], argv[3]);
    }
    catch(const std::exception& ex)
    {
        // TODO: Under MSVC, fprintf expects ANSI, not UTF-8.
        std::fprintf(stderr, "%s\n", ex.what());
        return 1;
    }

    return 0;
}

