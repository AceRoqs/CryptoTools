#include "PreCompile.h"
#include "Keys.h"           // Pick up forward declarations to ensure correctness.

namespace CrappyCrypto
{

void key_vector_from_string(_Out_writes_all_(key_vector_length) uint8_t* key_vector, size_t key_vector_length, _In_z_ const char* key_string) NOEXCEPT
{
    std::fill(key_vector, key_vector + key_vector_length, 0);

    // NOTE: Since the input is ASCII, most bits have no chance at being set, which limits the range of keys.
    for(size_t ix = 0; key_string[ix] != '\0'; ++ix)
    {
        key_vector[ix % key_vector_length] ^= key_string[ix];
    }
}

}

