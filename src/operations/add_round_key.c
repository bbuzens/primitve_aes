#include "add_round_key.h"

void AddRoundKey(uint8_t state[AES_ROWS][AES_COLUMNS],
                 const uint8_t round_key[AES_ROWS][AES_COLUMNS])
{
    for (int row = 0; row < AES_ROWS; row++)
        for (int column = 0; column < AES_COLUMNS; column++)
            state[row][column] ^= round_key[row][column];
}
