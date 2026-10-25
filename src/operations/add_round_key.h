#ifndef ADD_ROUND_KEY_H
#define ADD_ROUND_KEY_H

#include "aes_config.h"

/*
 * AddRoundKey (FIPS-197 §5.1.4) : ajoute la clé de tour à l'état.
 * L'addition dans GF(2^8) est un XOR, appliqué octet par octet.
 */
void AddRoundKey(uint8_t state[AES_ROWS][AES_COLUMNS],
                 const uint8_t round_key[AES_ROWS][AES_COLUMNS]);

#endif
