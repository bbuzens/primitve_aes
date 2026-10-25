#include <stdio.h>
#include <string.h>

#include "shift_rows.h"
#include "fips197_vectors.h"
#include "test_utils.h"

/* FIPS-197 annexe B : état du tour 1 avant et après ShiftRows. */
static void test_shift_rows_fips_round1(void)
{
    uint8_t state[AES_ROWS][AES_COLUMNS];

    memcpy(state, FIPS_ROUND1_AFTER_SUB_BYTES, sizeof state);
    ShiftRows(state);
    check_state("ShiftRows : tour 1 (FIPS-197 B)", state, FIPS_ROUND1_AFTER_SHIFT_ROWS);
}

/*
 * Chaque case vaut {rc} (r = ligne, c = colonne) : on voit directement où chaque octet est parti.
 * La ligne 0 ne bouge pas, la ligne r tourne de r cases vers la gauche.
 */
static void test_shift_rows_positions(void)
{
    uint8_t state[AES_ROWS][AES_COLUMNS] = {
        { 0x00, 0x01, 0x02, 0x03 },
        { 0x10, 0x11, 0x12, 0x13 },
        { 0x20, 0x21, 0x22, 0x23 },
        { 0x30, 0x31, 0x32, 0x33 },
    };
    static const uint8_t expected[AES_ROWS][AES_COLUMNS] = {
        { 0x00, 0x01, 0x02, 0x03 },
        { 0x11, 0x12, 0x13, 0x10 },
        { 0x22, 0x23, 0x20, 0x21 },
        { 0x33, 0x30, 0x31, 0x32 },
    };

    ShiftRows(state);
    check_state("ShiftRows : ligne r décalée de r cases", state, expected);
}

/* Après 4 applications, chaque ligne a fait un nombre entier de tours : l'état est inchangé. */
static void test_shift_rows_four_times_is_identity(void)
{
    uint8_t state[AES_ROWS][AES_COLUMNS];

    memcpy(state, FIPS_ROUND1_AFTER_SUB_BYTES, sizeof state);
    for (int i = 0; i < 4; i++)
        ShiftRows(state);
    check_state("ShiftRows appliqué 4 fois = identité", state, FIPS_ROUND1_AFTER_SUB_BYTES);
}

int main(void)
{
    printf("== shift_rows ==\n");
    test_shift_rows_fips_round1();
    test_shift_rows_positions();
    test_shift_rows_four_times_is_identity();
    return test_report();
}
