#include <stdio.h>

#include "rcon.h"
#include "test_utils.h"

/* FIPS-197 §5.2 et annexe A.1 : les 10 constantes utilisées par AES-128. */
static void test_rcon_values(void)
{
    static const uint8_t expected_first_byte[AES_NR + 1] = {
        0x00, /* i = 0 : non utilisé */
        0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36,
    };

    for (int i = 1; i <= AES_NR; i++) {
        uint8_t word[AES_WORD_SIZE];
        uint8_t expected[AES_WORD_SIZE] = { expected_first_byte[i], 0x00, 0x00, 0x00 };
        char name[64];

        Rcon(i, word);
        snprintf(name, sizeof name, "Rcon(%d) = %02x000000", i, expected_first_byte[i]);
        check_bytes(name, word, expected, AES_WORD_SIZE);
    }
}

int main(void)
{
    printf("== rcon ==\n");
    test_rcon_values();
    return test_report();
}
