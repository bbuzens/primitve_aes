#ifndef GF_MULTIPLY_H
#define GF_MULTIPLY_H

#include "aes_config.h"

/*
 * gf_multiply (FIPS-197 §4.2) : multiplie deux octets dans GF(2^8), modulo m(x).
 * La norme ne nomme pas cette opération : elle la note avec le symbole "•" (a • b).
 *
 * Le produit est construit comme une multiplication posée en binaire :
 * pour chaque bit à 1 de b, on ajoute (XOR) a multiplié par la puissance de x correspondante.
 */
uint8_t gf_multiply(uint8_t a, uint8_t b);

#endif
