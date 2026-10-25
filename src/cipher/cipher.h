#ifndef CIPHER_H
#define CIPHER_H

#include "aes_config.h"

/*
 * Cipher (FIPS-197 §5.1, figure 5) : chiffre un bloc de 16 octets avec la clé étendue w.
 *
 *     state = in
 *     AddRoundKey(state, clé du tour 0)
 *     pour round = 1 .. Nr-1 :
 *         SubBytes, ShiftRows, MixColumns, AddRoundKey(clé du tour round)
 *     dernier tour (round = Nr), sans MixColumns :
 *         SubBytes, ShiftRows, AddRoundKey(clé du tour Nr)
 *     out = state
 *
 * w est produit par KeyExpansion.
 */
void Cipher(const uint8_t in[AES_BLOCK_SIZE],
            uint8_t out[AES_BLOCK_SIZE],
            const uint8_t w[AES_EXPANDED_KEY_WORDS][AES_WORD_SIZE]);

#endif
