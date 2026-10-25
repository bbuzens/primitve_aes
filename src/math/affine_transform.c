#include "affine_transform.h"

/* Renvoie le bit d'indice i de l'octet (0 = poids faible), l'indice étant pris modulo 8. */
static uint8_t bit_at(uint8_t byte, int i)
{
    return (byte >> (i % 8)) & 1;
}

uint8_t affine_transform(uint8_t byte)
{
    uint8_t result = 0;

    for (int i = 0; i < 8; i++) {
        uint8_t new_bit = bit_at(byte, i)
                        ^ bit_at(byte, i + 4)
                        ^ bit_at(byte, i + 5)
                        ^ bit_at(byte, i + 6)
                        ^ bit_at(byte, i + 7)
                        ^ bit_at(AES_SBOX_AFFINE_CONSTANT, i);

        result |= (uint8_t)(new_bit << i);
    }

    return result;
}
