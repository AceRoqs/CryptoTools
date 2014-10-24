#include "PreCompile.h"
#include "Skipjack.h"

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
    int i, j, k;
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
    i = (int)fread(testvector, 1, BLOCKLENGTH, pInfile);
    j = 0;
    while(!feof(pInfile))
    {
        // TODO: 2014: This loop is problematic, as j can overflow.
        CrappyCrypto::SJ_Decrypt(testvector, keyvector);
        j += i;
        i = (int)fread(testvector2, 1, BLOCKLENGTH, pInfile);
        if(i == BLOCKLENGTH)
        {
            fwrite(testvector, 1, BLOCKLENGTH, pOutfile);
            for(k = 0; k < BLOCKLENGTH; ++k)
            {
                testvector[k] = testvector2[k];
            }
        }
        else
        {
            fwrite(testvector, 1, *(uint32_t *)testvector2 - (j - BLOCKLENGTH), pOutfile);
        }
    }

    fclose(pInfile);
    fclose(pOutfile);

    return 0;
}

