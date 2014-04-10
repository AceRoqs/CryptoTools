#include "skipjack.h"
#include <stdio.h>
#include <cstdlib>

static unsigned char keyvector[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static unsigned char testvector[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

//int main(int argc, char** argv)
int Encrypt_main(int argc, char** argv)
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

    // Encrypt file.
    j = 0;
    while(!feof(pInfile))
    {
        i = fread(testvector, 1, BLOCKLENGTH, pInfile);
        SJ_Encrypt(testvector, keyvector);
        fwrite(testvector, 1, BLOCKLENGTH, pOutfile);
        j += i;
    }

    // The next line is not portable across endian architectures.  The
    // problem is that j might be in a register, and thus not the same
    // endian as in memory.  The next line is portable across machines
    // whose registers are big-endian (i.e. x86).
    (uint32_t)*(uint32_t *)(testvector) = (uint32_t)j;

    fwrite(testvector, 1, sizeof(uint32_t), pOutfile);

    fclose(pInfile);
    fclose(pOutfile);

    return 0;
}

