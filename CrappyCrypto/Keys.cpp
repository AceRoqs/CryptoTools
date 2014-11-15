#include "PreCompile.h"
#include "Keys.h"           // Pick up forward declarations to ensure correctness.

namespace CrappyCrypto
{

void key_vector_from_string(uint8_t* key_vector, size_t key_vector_length, const char* key_string)
{
    memset(key_vector, 0, key_vector_length);

    // NOTE: Since the input is ASCII, most bits have no chance at being set, which limits the range of keys.
    for(size_t ix = 0; key_string[ix] != '\0'; ++ix)
    {
        key_vector[ix % key_vector_length] ^= key_string[ix];
    }
}

}

