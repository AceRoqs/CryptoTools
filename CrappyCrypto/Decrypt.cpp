#include "PreCompile.h"
#include "Skipjack.h"

namespace CrappyCrypto
{

namespace Skipjack
{

//int main(int argc, char** argv)
int decrypt_main(int argc, char** argv)
{
    unsigned char keyvector[key_length] = {};
    unsigned char testvector[block_length] = {};
    unsigned char testvector2[block_length] = {};

    int i, j;
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

    // Decrypt file.
    i = (int)fread(testvector2, 1, block_length, pInfile);
    if(i == block_length)
    {
        memcpy(testvector, testvector2, block_length);
        j = (int)fread(testvector2, 1, block_length, pInfile);

        while((i > 0) && !ferror(pInfile) && !ferror(pOutfile))
        {
            SJ_Decrypt(testvector, keyvector);
            if(j == block_length)
            {
                fwrite(testvector, 1, block_length, pOutfile);

                i = j;
                memcpy(testvector, testvector2, block_length);
                j = (int)fread(testvector2, 1, block_length, pInfile);
            }
            else
            {
                fwrite(testvector, 1, testvector2[0] <= block_length ? testvector2[0] : 0, pOutfile);
                break;
            }
        }
    }

    fclose(pInfile);
    fclose(pOutfile);

    return 0;
}

}

}

