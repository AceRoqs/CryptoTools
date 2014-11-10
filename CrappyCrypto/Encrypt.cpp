#include "PreCompile.h"
#include "Skipjack.h"

namespace CrappyCrypto
{

namespace Skipjack
{

//int main(int argc, char** argv)
int encrypt_main(int argc, char** argv)
{
    unsigned char keyvector[key_length] = {};
    unsigned char testvector[block_length] = {};

    int i;
    FILE* pInfile;
    FILE* pOutfile;

    if(argc != 4)
    {
        fprintf(stderr, "Usage: %s infile outfile key", argv[0]);
        exit(0);
    }

    // Build key.
    i = 0;
    while(argv[3][i] != 0)
    {
        keyvector[i % key_length] ^= argv[3][i];
        i++;
    }

    // Open input file.
    if((pInfile = fopen(argv[1], "rb")) == 0)
    {
        fprintf(stderr, "%s: error opening %s\n", argv[0], argv[1]);
        exit(1);
    }

    // Open output file.
    if((pOutfile = fopen(argv[2], "wb")) == 0)
    {
        fclose(pInfile);
        fprintf(stderr, "%s: error opening %s\n", argv[0], argv[2]);
        exit(1);
    }

    // Encrypt file.
    while(!feof(pInfile) && !ferror(pInfile) && !ferror(pOutfile))
    {
        i = (int)fread(testvector, 1, block_length, pInfile);
        if(i > 0)
        {
            memset(testvector + i, 0, block_length - i);
            encrypt(testvector, keyvector);
            fwrite(testvector, 1, block_length, pOutfile);
        }
    }

    if(!ferror(pInfile) && !ferror(pOutfile))
    {
        uint8_t final_count = (uint8_t)(i > 0 ? i : block_length );
        fwrite(&final_count, 1, sizeof(final_count), pOutfile);
    }
    else
    {
        perror(nullptr);
    }

    fclose(pInfile);
    fclose(pOutfile);

    return 0;
}

}

}

