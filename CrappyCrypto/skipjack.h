#ifndef __SKIPJACK_H
#define __SKIPJACK_H

#include "platform.h"

#define ROUNDS 32
#define NUM_FEISTELS 4
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
    _EXTERN void RuleA(word* w, const unsigned char* key, word counter);
    _EXTERN void RuleB(word* w, const unsigned char* key, word counter);
    _EXTERN void RuleA_1(word* w, const unsigned char* key, word counter);
    _EXTERN void RuleB_1(word* w, const unsigned char* key, word counter);
    _EXTERN word G(word g, const unsigned char* key, int step);
    _EXTERN word G_1(word g, const unsigned char* key, int step);

#endif /* __SKIPJACK_H */

