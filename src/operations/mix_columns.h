#ifndef MIX_COLUMNS_H
#define MIX_COLUMNS_H

#include "aes_config.h"

/*
 * MixColumns (FIPS-197 §5.1.3) : chaque colonne de l'état est multipliée par une matrice fixe,
 * les additions étant des XOR et les multiplications des produits dans GF(2^8) :
 *
 *     | s'0 |   | 02 03 01 01 |   | s0 |
 *     | s'1 | = | 01 02 03 01 | . | s1 |
 *     | s'2 |   | 01 01 02 03 |   | s2 |
 *     | s'3 |   | 03 01 01 02 |   | s3 |
 *
 * Par exemple : s'0 = ({02} • s0) ^ ({03} • s1) ^ s2 ^ s3.
 */
void MixColumns(uint8_t state[AES_ROWS][AES_COLUMNS]);

#endif
