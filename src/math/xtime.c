#include <stdbool.h>

#include "xtime.h"

uint8_t xtime(uint8_t byte)
{
    bool overflows = (byte & 0x80) != 0;   /* le terme x^7 va devenir x^8 */
    uint8_t shifted = (uint8_t)(byte << 1); /* multiplication par x ; x^8 est perdu */

    if (overflows)
        shifted ^= AES_REDUCTION_POLYNOMIAL; /* x^8 = x^4 + x^3 + x + 1 modulo m(x) */

    return shifted;
}
