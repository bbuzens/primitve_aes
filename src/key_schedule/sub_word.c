#include "sub_word.h"
#include "sbox.h"

void SubWord(uint8_t word[AES_WORD_SIZE])
{
    for (int i = 0; i < AES_WORD_SIZE; i++)
        word[i] = SBox(word[i]);
}
