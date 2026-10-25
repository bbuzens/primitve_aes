#ifndef XTIME_H
#define XTIME_H

#include "aes_config.h"

/*
 * xtime (FIPS-197 §4.2.1) : multiplie un octet par x, c'est-à-dire par {02}, dans GF(2^8).
 * On décale d'un bit vers la gauche ; si le bit x^7 était à 1, le résultat déborde
 * en x^8 et on le réduit modulo m(x).
 */
uint8_t xtime(uint8_t byte);

#endif
