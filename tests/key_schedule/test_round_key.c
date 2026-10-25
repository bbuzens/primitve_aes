#include <stdio.h>

#include "round_key.h"
#include "fips197_vectors.h"
#include "test_utils.h"

/* Clé du tour 0 = la clé de chiffrement elle-même. */
static void test_round_key_0(void)
{
    uint8_t round_key[AES_ROWS][AES_COLUMNS];

    GetRoundKey(FIPS_EXPANDED_KEY, 0, round_key);
    check_state("GetRoundKey : tour 0 = clé (FIPS-197 B)", round_key, FIPS_KEY_STATE);
}

/* Clé du tour 1 = w[4..7], disposée en colonnes comme dans la figure de l'annexe B. */
static void test_round_key_1(void)
{
    uint8_t round_key[AES_ROWS][AES_COLUMNS];

    GetRoundKey(FIPS_EXPANDED_KEY, 1, round_key);
    check_state("GetRoundKey : tour 1 (FIPS-197 B)", round_key, FIPS_ROUND1_KEY_STATE);
}

/* Clé du dernier tour = w[40..43]. */
static void test_round_key_last(void)
{
    uint8_t round_key[AES_ROWS][AES_COLUMNS];

    GetRoundKey(FIPS_EXPANDED_KEY, AES_NR, round_key);
    check_state("GetRoundKey : tour 10 (FIPS-197 B)", round_key, FIPS_ROUND10_KEY_STATE);
}

int main(void)
{
    printf("== round_key ==\n");
    test_round_key_0();
    test_round_key_1();
    test_round_key_last();
    return test_report();
}
