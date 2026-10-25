#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H

#include "aes_config.h"

/*
 * affine_transform (FIPS-197 §5.1.1, équation 5.1) : transformation affine sur les bits d'un octet.
 * Pour chaque bit i (de 0 à 7), indices pris modulo 8 :
 *
 *     b'[i] = b[i] ^ b[i+4] ^ b[i+5] ^ b[i+6] ^ b[i+7] ^ c[i]     avec c = {63}
 */
uint8_t affine_transform(uint8_t byte);

#endif
