#include <stdio.h>
#include <string.h>

#include "sub_bytes.h"
#include "fips197_vectors.h"
#include "test_utils.h"

/* FIPS-197 annexe B : état du tour 1 avant et après SubBytes. */
static void test_sub_bytes_fips_round1(void)
{
    uint8_t state[AES_ROWS][AES_COLUMNS];

    memcpy(state, FIPS_ROUND1_START, sizeof state);
    SubBytes(state);
    check_state("SubBytes : tour 1 (FIPS-197 B)", state, FIPS_ROUND1_AFTER_SUB_BYTES);
}

/* Chaque case est substituée indépendamment : un état rempli de {00} devient rempli de {63}. */
static void test_sub_bytes_all_zero(void)
{
    uint8_t state[AES_ROWS][AES_COLUMNS];
    uint8_t expected[AES_ROWS][AES_COLUMNS];

    memset(state, 0x00, sizeof state);
    memset(expected, 0x63, sizeof expected);
    SubBytes(state);
    check_state("SubBytes : état nul -> {63} partout", state, expected);
}

int main(void)
{
    printf("== sub_bytes ==\n");
    test_sub_bytes_fips_round1();
    test_sub_bytes_all_zero();
    return test_report();
}
