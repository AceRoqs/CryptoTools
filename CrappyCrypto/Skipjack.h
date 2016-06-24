#pragma once

namespace CrappyCrypto
{

namespace Skipjack
{

constexpr unsigned int key_size = 10;
constexpr unsigned int block_size = 8;
constexpr unsigned int iterations_per_rule = 8;

void encrypt(_Inout_updates_all_(block_size) uint8_t* block, _In_reads_(key_size) const uint8_t* key) noexcept;
void decrypt(_Inout_updates_all_(block_size) uint8_t* block, _In_reads_(key_size) const uint8_t* key) noexcept;
void rule_a(_Inout_updates_all_(block_size) uint8_t* block, _In_reads_(key_size) const uint8_t* key, uint16_t counter) noexcept;
void rule_b(_Inout_updates_all_(block_size) uint8_t* block, _In_reads_(key_size) const uint8_t* key, uint16_t counter) noexcept;
void rule_a_inverse(_Inout_updates_all_(block_size) uint8_t* block, _In_reads_(key_size) const uint8_t* key, uint16_t counter) noexcept;
void rule_b_inverse(_Inout_updates_all_(block_size) uint8_t* block, _In_reads_(key_size) const uint8_t* key, uint16_t counter) noexcept;
uint16_t g_permutation(uint16_t w_block, _In_reads_(key_size) const uint8_t* key, uint16_t step) noexcept;
uint16_t g_permutation_inverse(uint16_t w_block, _In_reads_(key_size) const uint8_t* key, uint16_t step) noexcept;

}

}

