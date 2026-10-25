#ifndef SBOX_H
#define SBOX_H

#include "aes_config.h"

/*
 * S-box (FIPS-197 §5.1.1) : substitution d'un octet, calculée en deux étapes :
 *   1. on prend l'inverse de l'octet dans GF(2^8) ({00} reste {00}) ;
 *   2. on applique la transformation affine.
 */
uint8_t SBox(uint8_t byte);

#endif
