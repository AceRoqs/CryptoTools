#pragma once

#include "Skipjack.h"

namespace CrappyCrypto
{

namespace Skipjack
{

void encrypt_istream(std::basic_istream<uint8_t>& input_stream, std::basic_ostream<uint8_t>& output_stream, _In_ uint8_t key_vector[key_size]);

}

}

