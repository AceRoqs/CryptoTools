#pragma once

#include "Skipjack.h"

namespace CrappyCrypto
{

namespace Skipjack
{

void decrypt_fstream(std::basic_ifstream<uint8_t>& input_file, std::basic_ofstream<uint8_t>& output_file, _In_ uint8_t key_vector[key_size]);

}

}

