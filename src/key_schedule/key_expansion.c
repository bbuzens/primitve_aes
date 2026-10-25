#include <string.h>

#include "key_expansion.h"
#include "rcon.h"
#include "rot_word.h"
#include "sub_word.h"

void KeyExpansion(const uint8_t key[AES_KEY_SIZE],
                  uint8_t w[AES_EXPANDED_KEY_WORDS][AES_WORD_SIZE])
{
    /* Les Nk premiers mots sont la clé, découpée en mots de 4 octets. */
    for (int i = 0; i < AES_NK; i++)
        memcpy(w[i], &key[AES_WORD_SIZE * i], AES_WORD_SIZE);

    /* Chaque mot suivant dépend du mot précédent et du mot situé Nk positions plus tôt. */
    for (int i = AES_NK; i < AES_EXPANDED_KEY_WORDS; i++) {
        uint8_t temp[AES_WORD_SIZE];
        memcpy(temp, w[i - 1], AES_WORD_SIZE);

        if (i % AES_NK == 0) {
            uint8_t round_constant[AES_WORD_SIZE];
            Rcon(i / AES_NK, round_constant);

            RotWord(temp);
            SubWord(temp);
            for (int byte = 0; byte < AES_WORD_SIZE; byte++)
                temp[byte] ^= round_constant[byte];
        }

        for (int byte = 0; byte < AES_WORD_SIZE; byte++)
            w[i][byte] = w[i - AES_NK][byte] ^ temp[byte];
    }
}
