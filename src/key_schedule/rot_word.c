#include "rot_word.h"

void RotWord(uint8_t word[AES_WORD_SIZE])
{
    uint8_t first = word[0]; /* sauvegardé : il sera écrasé par le décalage */

    for (int i = 0; i < AES_WORD_SIZE - 1; i++)
        word[i] = word[i + 1];

    word[AES_WORD_SIZE - 1] = first;
}
