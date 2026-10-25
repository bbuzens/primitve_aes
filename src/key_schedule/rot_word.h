#ifndef ROT_WORD_H
#define ROT_WORD_H

#include "aes_config.h"

/* RotWord (FIPS-197 §5.2) : rotation cyclique d'un octet vers la gauche, [a0,a1,a2,a3] -> [a1,a2,a3,a0]. */
void RotWord(uint8_t word[AES_WORD_SIZE]);

#endif
