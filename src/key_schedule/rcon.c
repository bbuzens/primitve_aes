#include "rcon.h"
#include "xtime.h"

void Rcon(int i, uint8_t word[AES_WORD_SIZE])
{
    uint8_t x_power = 0x01; /* x^0 */

    for (int exponent = 1; exponent < i; exponent++)
        x_power = xtime(x_power); /* x^(exponent-1) . x = x^exponent */

    word[0] = x_power;
    word[1] = 0x00;
    word[2] = 0x00;
    word[3] = 0x00;
}
