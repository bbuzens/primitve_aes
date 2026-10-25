#ifndef RCON_H
#define RCON_H

#include "aes_config.h"

/*
 * Rcon (FIPS-197 §5.2) : constante du tour i (i >= 1), le mot [x^(i-1), {00}, {00}, {00}],
 * où x^(i-1) est une puissance de x = {02} dans GF(2^8) :
 *
 *     i       1  2  3  4  5  6  7  8  9  10
 *     x^(i-1) 01 02 04 08 10 20 40 80 1b 36
 */
void Rcon(int i, uint8_t word[AES_WORD_SIZE]);

#endif
