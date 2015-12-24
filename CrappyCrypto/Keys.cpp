#include "PreCompile.h"
#include "Keys.h"           // Pick up forward declarations to ensure correctness.
#include <PortableRuntime/CheckException.h>

namespace CrappyCrypto
{

void key_vector_from_key_file(_Out_writes_all_(key_vector_length) uint8_t* key_vector, size_t key_vector_length, std::basic_ifstream<uint8_t>& key_file) noexcept
{
    // Assumes exceptions are not enabled for stream.
    key_file.seekg(0, std::ios::end);
    PortableRuntime::check_exception(key_file.good(), "Error reading the key file.");
    PortableRuntime::check_exception(key_file.tellg() == static_cast<std::streampos>(key_vector_length), "Key file is the wrong size.");

    key_file.seekg(0, std::ios::beg);
    PortableRuntime::check_exception(key_file.good(), "Error reading the key file.");
    key_file.read(key_vector, key_vector_length);
    PortableRuntime::check_exception(key_file.good(), "Error reading the key file.");
}

}

