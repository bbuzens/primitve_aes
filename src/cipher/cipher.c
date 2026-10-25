#include "cipher.h"
#include "add_round_key.h"
#include "mix_columns.h"
#include "round_key.h"
#include "shift_rows.h"
#include "state.h"
#include "sub_bytes.h"

void Cipher(const uint8_t in[AES_BLOCK_SIZE],
            uint8_t out[AES_BLOCK_SIZE],
            const uint8_t w[AES_EXPANDED_KEY_WORDS][AES_WORD_SIZE])
{
    uint8_t state[AES_ROWS][AES_COLUMNS];
    uint8_t round_key[AES_ROWS][AES_COLUMNS];

    BytesToState(in, state);

    /* Tour 0 : uniquement l'ajout de la clé initiale. */
    GetRoundKey(w, 0, round_key);
    AddRoundKey(state, round_key);

    /* Tours 1 à Nr-1 : les quatre transformations. */
    for (int round = 1; round < AES_NR; round++) {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        GetRoundKey(w, round, round_key);
        AddRoundKey(state, round_key);
    }

    /* Dernier tour (Nr) : sans MixColumns. */
    SubBytes(state);
    ShiftRows(state);
    GetRoundKey(w, AES_NR, round_key);
    AddRoundKey(state, round_key);

    StateToBytes(state, out);
}
