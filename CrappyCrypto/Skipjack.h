#ifndef SKIPJACK_H
#define SKIPJACK_H

namespace CrappyCrypto
{

namespace Skipjack
{

const int key_length = 10;
const int block_length = 8;
const int iter_per_func = 8;

void encrypt(unsigned char* text, const unsigned char* key);
void decrypt(unsigned char* text, const unsigned char* key);
void rule_a(uint16_t* w, const unsigned char* key, uint16_t counter);
void rule_b(uint16_t* w, const unsigned char* key, uint16_t counter);
void rule_a_inverse(uint16_t* w, const unsigned char* key, uint16_t counter);
void rule_b_inverse(uint16_t* w, const unsigned char* key, uint16_t counter);
uint16_t g_permutation(uint16_t g, const unsigned char* key, int step);
uint16_t g_permutation_inverse(uint16_t g, const unsigned char* key, int step);

}

}

#endif

