#include "gf_inverse.h"
#include "gf_multiply.h"

/*
 * Les 255 octets non nuls forment un groupe pour la multiplication, donc a^255 = {01}
 * pour tout a non nul (petit théorème de Fermat). On en déduit :
 *
 *     a^254 . a = {01}   donc   a^-1 = a^254
 *
 * Pour a = {00}, a^254 vaut {00} : la convention de la S-box est respectée sans cas particulier.
 */
uint8_t gf_inverse(uint8_t a)
{
    uint8_t power = a; /* a^1 */

    for (int exponent = 2; exponent <= 254; exponent++)
        power = gf_multiply(power, a); /* a^(exponent-1) . a = a^exponent */

    return power;
}
