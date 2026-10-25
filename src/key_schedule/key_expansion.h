#ifndef KEY_EXPANSION_H
#define KEY_EXPANSION_H

#include "aes_config.h"

/*
 * KeyExpansion (FIPS-197 §5.2, figure 11) : étend la clé en Nb x (Nr + 1) = 44 mots w[0..43].
 *
 *   - w[0..Nk-1] : les mots de la clé, recopiés ;
 *   - puis, pour i >= Nk :   w[i] = w[i - Nk] ^ temp,   avec temp = w[i - 1],
 *     transformé quand i est un multiple de Nk :
 *         temp = SubWord(RotWord(temp)) ^ Rcon(i / Nk)
 *
 * Limite : seul le cas AES-128 (Nk = 4) est implémenté. AES-256 (Nk = 8) demande
 * une étape SubWord supplémentaire quand i mod Nk = 4.
 */
void KeyExpansion(const uint8_t key[AES_KEY_SIZE],
                  uint8_t w[AES_EXPANDED_KEY_WORDS][AES_WORD_SIZE]);

#endif
