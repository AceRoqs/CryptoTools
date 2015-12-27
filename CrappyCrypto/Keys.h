#pragma once

namespace CrappyCrypto
{

void key_vector_from_key_file(_Out_writes_all_(key_vector_length) uint8_t* key_vector, size_t key_vector_length, std::basic_ifstream<uint8_t>& key_file);

}

