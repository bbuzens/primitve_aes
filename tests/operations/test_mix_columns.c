#include <stdio.h>
#include <string.h>

#include "mix_columns.h"
#include "fips197_vectors.h"
#include "test_utils.h"

/* FIPS-197 annexe B : état du tour 1 avant et après MixColumns. */
static void test_mix_columns_fips_round1(void)
{
    uint8_t state[AES_ROWS][AES_COLUMNS];

    memcpy(state, FIPS_ROUND1_AFTER_SHIFT_ROWS, sizeof state);
    MixColumns(state);
    check_state("MixColumns : tour 1 (FIPS-197 B)", state, FIPS_ROUND1_AFTER_MIX_COLUMNS);
}

/*
 * Colonnes de test classiques, une par colonne de l'état.
 * Les colonnes sont indépendantes : chacune doit donner son résultat, quelle que soit sa voisine.
 */
static void test_mix_columns_known_columns(void)
{
    uint8_t state[AES_ROWS][AES_COLUMNS] = {
        { 0xdb, 0xf2, 0xd4, 0x2d },
        { 0x13, 0x0a, 0xd4, 0x26 },
        { 0x53, 0x22, 0xd4, 0x31 },
        { 0x45, 0x5c, 0xd5, 0x4c },
    };
    static const uint8_t expected[AES_ROWS][AES_COLUMNS] = {
        { 0x8e, 0x9f, 0xd5, 0x4d },
        { 0x4d, 0xdc, 0xd5, 0x7e },
        { 0xa1, 0x58, 0xd7, 0xbd },
        { 0xbc, 0x9d, 0xd6, 0xf8 },
    };

    MixColumns(state);
    check_state("MixColumns : colonnes de test classiques", state, expected);
}

/*
 * Une colonne de 4 octets identiques est inchangée : chaque ligne de la matrice
 * donne ({02} ^ {03} ^ {01} ^ {01}) • a = {01} • a = a.
 */
static void test_mix_columns_constant_columns(void)
{
    uint8_t state[AES_ROWS][AES_COLUMNS] = {
        { 0x01, 0xc6, 0x00, 0xff },
        { 0x01, 0xc6, 0x00, 0xff },
        { 0x01, 0xc6, 0x00, 0xff },
        { 0x01, 0xc6, 0x00, 0xff },
    };
    uint8_t expected[AES_ROWS][AES_COLUMNS];

    memcpy(expected, state, sizeof expected);
    MixColumns(state);
    check_state("MixColumns : colonnes constantes inchangées", state, expected);
}

int main(void)
{
    printf("== mix_columns ==\n");
    test_mix_columns_fips_round1();
    test_mix_columns_known_columns();
    test_mix_columns_constant_columns();
    return test_report();
}
