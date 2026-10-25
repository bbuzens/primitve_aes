#include <stdio.h>

#include "sbox.h"
#include "fips197_vectors.h"
#include "test_utils.h"

/* FIPS-197 §5.1.1 : exemple du texte, et quelques valeurs remarquables de la figure 7. */
static void test_sbox_examples(void)
{
    check_byte("S({53}) = {ed}", SBox(0x53), 0xed);
    check_byte("S({00}) = {63}", SBox(0x00), 0x63);
    check_byte("S({01}) = {7c}", SBox(0x01), 0x7c);
    check_byte("S({ff}) = {16}", SBox(0xff), 0x16);
}

/* La S-box calculée doit être identique, case par case, à la figure 7 de la FIPS-197. */
static void test_sbox_matches_fips_table(void)
{
    int mismatches = 0;

    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            uint8_t byte = (uint8_t)(x << 4 | y);
            uint8_t got = SBox(byte);
            uint8_t expected = FIPS_SBOX[x][y];

            if (got != expected) {
                if (mismatches < 5)
                    printf("       S({%02x}) : attendu %02x, obtenu %02x\n", byte, expected, got);
                mismatches++;
            }
        }
    }

    check_true("S-box identique à la figure 7 (256 valeurs)", mismatches == 0);
}

/* Propriétés de conception de la S-box : pas de point fixe ni de point fixe opposé. */
static void test_sbox_has_no_fixed_points(void)
{
    bool no_fixed_point = true;
    bool no_opposite_fixed_point = true;

    for (int a = 0; a < 256; a++) {
        no_fixed_point          &= SBox(a) != a;
        no_opposite_fixed_point &= SBox(a) != (uint8_t)~a;
    }

    check_true("S(a) != a, pour tout a", no_fixed_point);
    check_true("S(a) != ~a, pour tout a", no_opposite_fixed_point);
}

int main(void)
{
    printf("== sbox ==\n");
    test_sbox_examples();
    test_sbox_matches_fips_table();
    test_sbox_has_no_fixed_points();
    return test_report();
}
