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
static unsigned char testvector2[] =
{
    0, 0, 0, 0, 0, 0, 0, 0
};

//int main(int argc, char** argv)
int decrypt_main(int argc, char** argv)
{
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

    // Decrypt file.
    i = (int)fread(testvector2, 1, BLOCKLENGTH, pInfile);
    if(i == BLOCKLENGTH)
    {
        memcpy(testvector, testvector2, BLOCKLENGTH);
        j = (int)fread(testvector2, 1, BLOCKLENGTH, pInfile);

        while((i > 0) && !ferror(pInfile) && !ferror(pOutfile))
        {
            SJ_Decrypt(testvector, keyvector);
            if(j == BLOCKLENGTH)
            {
                fwrite(testvector, 1, BLOCKLENGTH, pOutfile);

                i = j;
                memcpy(testvector, testvector2, BLOCKLENGTH);
                j = (int)fread(testvector2, 1, BLOCKLENGTH, pInfile);
            }
            else
            {
                fwrite(testvector, 1, testvector2[0] <= BLOCKLENGTH ? testvector2[0] : 0, pOutfile);
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

