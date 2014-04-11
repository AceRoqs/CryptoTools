#ifndef SKIPJACK_H
#define SKIPJACK_H

namespace CrappyCrypto
{

#define KEYLENGTH 10
#define BLOCKLENGTH 8
#define ITER_PER_FUNC 8

#ifdef __cplusplus
# define _EXTERN extern "C"
#else
# define _EXTERN extern
#endif
    _EXTERN void SJ_Encrypt(unsigned char* text, const unsigned char* key);
    _EXTERN void SJ_Decrypt(unsigned char* text, const unsigned char* key);
    _EXTERN void RuleA(uint16_t* w, const unsigned char* key, uint16_t counter);
    _EXTERN void RuleB(uint16_t* w, const unsigned char* key, uint16_t counter);
    _EXTERN void RuleA_1(uint16_t* w, const unsigned char* key, uint16_t counter);
    _EXTERN void RuleB_1(uint16_t* w, const unsigned char* key, uint16_t counter);
    _EXTERN uint16_t G(uint16_t g, const unsigned char* key, int step);
    _EXTERN uint16_t G_1(uint16_t g, const unsigned char* key, int step);

#endif

}

