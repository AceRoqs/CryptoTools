#pragma once

namespace CrappyCrypto
{

namespace Skipjack
{

const int key_length = 10;
const int block_length = 8;
const int iter_per_func = 8;

void encrypt(uint8_t* block, const uint8_t* key);
void decrypt(uint8_t* block, const uint8_t* key);
void rule_a(uint8_t* block, const uint8_t* key, uint16_t counter);
void rule_b(uint8_t* block, const uint8_t* key, uint16_t counter);
void rule_a_inverse(uint8_t* block, const uint8_t* key, uint16_t counter);
void rule_b_inverse(uint8_t* block, const uint8_t* key, uint16_t counter);
uint16_t g_permutation(uint16_t w_block, const uint8_t* key, int step);
uint16_t g_permutation_inverse(uint16_t w_block, const uint8_t* key, int step);

}

}

