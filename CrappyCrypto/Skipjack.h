#ifndef SKIPJACK_H
#define SKIPJACK_H

namespace CrappyCrypto
{

namespace Skipjack
{

#define KEYLENGTH 10
#define BLOCKLENGTH 8
#define ITER_PER_FUNC 8

void SJ_Encrypt(unsigned char* text, const unsigned char* key);
void SJ_Decrypt(unsigned char* text, const unsigned char* key);
void RuleA(uint16_t* w, const unsigned char* key, uint16_t counter);
void RuleB(uint16_t* w, const unsigned char* key, uint16_t counter);
void RuleA_1(uint16_t* w, const unsigned char* key, uint16_t counter);
void RuleB_1(uint16_t* w, const unsigned char* key, uint16_t counter);
uint16_t G(uint16_t g, const unsigned char* key, int step);
uint16_t G_1(uint16_t g, const unsigned char* key, int step);

}

}

#endif

