#ifndef ROUND_KEY_H
#define ROUND_KEY_H

#include "aes_config.h"

/*
 * GetRoundKey : extrait la clé du tour `round` de la clé étendue, sous forme d'état 4x4.
 * La clé du tour r est formée des mots w[Nb.r] à w[Nb.r + 3] (FIPS-197 §5.1.4) ;
 * le mot w[Nb.r + c] remplit la colonne c :
 *
 *     round_key[ligne][c] = w[Nb.r + c][ligne]
 */
void GetRoundKey(const uint8_t w[AES_EXPANDED_KEY_WORDS][AES_WORD_SIZE],
                 int round,
                 uint8_t round_key[AES_ROWS][AES_COLUMNS]);

#endif
