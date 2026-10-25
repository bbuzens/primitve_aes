#include "state.h"

void BytesToState(const uint8_t in[AES_BLOCK_SIZE], uint8_t state[AES_ROWS][AES_COLUMNS])
{
    for (int column = 0; column < AES_COLUMNS; column++)
        for (int row = 0; row < AES_ROWS; row++)
            state[row][column] = in[row + AES_ROWS * column];
}

void StateToBytes(const uint8_t state[AES_ROWS][AES_COLUMNS], uint8_t out[AES_BLOCK_SIZE])
{
    for (int column = 0; column < AES_COLUMNS; column++)
        for (int row = 0; row < AES_ROWS; row++)
            out[row + AES_ROWS * column] = state[row][column];
}
