#pragma once

namespace CrappyCrypto
{

void key_vector_from_string(_Out_cap_(key_vector_length) uint8_t* key_vector, size_t key_vector_length, _In_z_ const char* key_string) NOEXCEPT;

}

