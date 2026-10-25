#ifndef STATE_H
#define STATE_H

#include "aes_config.h"

/* Range un bloc de 16 octets dans l'état, colonne par colonne (FIPS-197 §3.4). */
void BytesToState(const uint8_t in[AES_BLOCK_SIZE], uint8_t state[AES_ROWS][AES_COLUMNS]);

/* Opération inverse : relit l'état colonne par colonne vers un bloc de 16 octets. */
void StateToBytes(const uint8_t state[AES_ROWS][AES_COLUMNS], uint8_t out[AES_BLOCK_SIZE]);

#endif
