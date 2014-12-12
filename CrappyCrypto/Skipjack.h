#pragma once

namespace CrappyCrypto
{

namespace Skipjack
{

const unsigned int key_size = 10;
const unsigned int block_size = 8;
const unsigned int iterations_per_rule = 8;

void encrypt(_Inout_updates_all_(block_size) uint8_t* block, _In_reads_(key_size) const uint8_t* key) NOEXCEPT;
void decrypt(_Inout_updates_all_(block_size) uint8_t* block, _In_reads_(key_size) const uint8_t* key) NOEXCEPT;
void rule_a(_Inout_updates_all_(block_size) uint8_t* block, _In_reads_(key_size) const uint8_t* key, uint16_t counter) NOEXCEPT;
void rule_b(_Inout_updates_all_(block_size) uint8_t* block, _In_reads_(key_size) const uint8_t* key, uint16_t counter) NOEXCEPT;
void rule_a_inverse(_Inout_updates_all_(block_size) uint8_t* block, _In_reads_(key_size) const uint8_t* key, uint16_t counter) NOEXCEPT;
void rule_b_inverse(_Inout_updates_all_(block_size) uint8_t* block, _In_reads_(key_size) const uint8_t* key, uint16_t counter) NOEXCEPT;
uint16_t g_permutation(uint16_t w_block, _In_reads_(key_size) const uint8_t* key, uint16_t step) NOEXCEPT;
uint16_t g_permutation_inverse(uint16_t w_block, _In_reads_(key_size) const uint8_t* key, uint16_t step) NOEXCEPT;

}

}

