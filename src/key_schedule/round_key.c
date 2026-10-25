#include "round_key.h"

void GetRoundKey(const uint8_t w[AES_EXPANDED_KEY_WORDS][AES_WORD_SIZE],
                 int round,
                 uint8_t round_key[AES_ROWS][AES_COLUMNS])
{
    int first_word = AES_NB * round;

    for (int column = 0; column < AES_COLUMNS; column++)
        for (int row = 0; row < AES_ROWS; row++)
            round_key[row][column] = w[first_word + column][row];
}
