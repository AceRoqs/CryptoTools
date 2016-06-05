#pragma once

#include "Skipjack.h"

namespace CrappyCrypto
{

namespace Skipjack
{

void decrypt_istream(std::istream& input_stream, std::ostream& output_stream, _In_ uint8_t key_vector[key_size]);

}

}

