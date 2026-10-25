#ifndef SHIFT_ROWS_H
#define SHIFT_ROWS_H

#include "aes_config.h"

/*
 * ShiftRows (FIPS-197 §5.1.2) : décale cycliquement chaque ligne de l'état vers la gauche,
 * d'autant de cases que son numéro :
 *
 *     s'[r][c] = s[r][(c + r) mod 4]
 *
 *     ligne 0 : pas de décalage      s0 s1 s2 s3  ->  s0 s1 s2 s3
 *     ligne 1 : 1 case               s0 s1 s2 s3  ->  s1 s2 s3 s0
 *     ligne 2 : 2 cases              s0 s1 s2 s3  ->  s2 s3 s0 s1
 *     ligne 3 : 3 cases              s0 s1 s2 s3  ->  s3 s0 s1 s2
 */
void ShiftRows(uint8_t state[AES_ROWS][AES_COLUMNS]);

#endif
