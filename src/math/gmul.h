#ifndef GMUL_H
#define GMUL_H

#include "aes_config.h"

/*
 * gmul (FIPS-197 §4.2) : multiplie deux octets dans GF(2^8), modulo m(x).
 * Le produit est construit comme une multiplication posée en binaire :
 * pour chaque bit à 1 de b, on ajoute (XOR) a multiplié par la puissance de x correspondante.
 */
uint8_t gmul(uint8_t a, uint8_t b);

#endif
