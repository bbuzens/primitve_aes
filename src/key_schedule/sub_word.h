#ifndef SUB_WORD_H
#define SUB_WORD_H

#include "aes_config.h"

/* SubWord (FIPS-197 §5.2) : applique la S-box à chacun des 4 octets du mot. */
void SubWord(uint8_t word[AES_WORD_SIZE]);

#endif
