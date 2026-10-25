#ifndef GF_INVERSE_H
#define GF_INVERSE_H

#include "aes_config.h"

/*
 * gf_inverse (FIPS-197 §4.2 et §5.1.1) : inverse multiplicatif d'un octet dans GF(2^8),
 * c'est-à-dire l'octet b tel que a • b = {01}.
 * {00} n'a pas d'inverse ; par convention de la S-box, il est associé à lui-même.
 */
uint8_t gf_inverse(uint8_t a);

#endif
