#ifndef SUB_BYTES_H
#define SUB_BYTES_H

#include "aes_config.h"

/* SubBytes (FIPS-197 §5.1.1) : remplace chaque octet de l'état par son image par la S-box. */
void SubBytes(uint8_t state[AES_ROWS][AES_COLUMNS]);

#endif
