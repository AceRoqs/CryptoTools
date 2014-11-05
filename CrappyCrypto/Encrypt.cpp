#include "PreCompile.h"
#include "Skipjack.h"

namespace CrappyCrypto
{

namespace Skipjack
{

static unsigned char keyvector[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static unsigned char testvector[] =
{
    0, 0, 0, 0, 0, 0, 0, 0
};

//int main(int argc, char** argv)
int encrypt_main(int argc, char** argv)
{
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
        keyvector[i % KEYLENGTH] ^= argv[3][i];
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
        i = (int)fread(testvector, 1, BLOCKLENGTH, pInfile);
        if(i > 0)
        {
            memset(testvector + i, 0, BLOCKLENGTH - i);
            SJ_Encrypt(testvector, keyvector);
            fwrite(testvector, 1, BLOCKLENGTH, pOutfile);
        }
    }

    if(!ferror(pInfile) && !ferror(pOutfile))
    {
        uint8_t final_count = (uint8_t)(i > 0 ? i : BLOCKLENGTH );
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

