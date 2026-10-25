#include "gmul.h"
#include "xtime.h"

uint8_t gmul(uint8_t a, uint8_t b)
{
    uint8_t product = 0;
    uint8_t a_times_x_power = a; /* a . x^i, en commençant par i = 0 */

    for (int bit = 0; bit < 8; bit++) {
        bool bit_is_set = (b >> bit) & 1;

        if (bit_is_set)
            product ^= a_times_x_power; /* l'addition dans GF(2^8) est un XOR */

        a_times_x_power = xtime(a_times_x_power);
    }

    return product;
}
