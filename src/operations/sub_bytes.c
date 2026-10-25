#include "sub_bytes.h"
#include "sbox.h"

void SubBytes(uint8_t state[AES_ROWS][AES_COLUMNS])
{
    for (int row = 0; row < AES_ROWS; row++)
        for (int column = 0; column < AES_COLUMNS; column++)
            state[row][column] = SBox(state[row][column]);
}
