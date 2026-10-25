#include <stdio.h>
#include <string.h>

#include "add_round_key.h"
#include "fips197_vectors.h"
#include "test_utils.h"

/* FIPS-197 annexe B : état au début du tour 1 = clair XOR clé du tour 0. */
static void test_add_round_key(void)
{
    uint8_t state[AES_ROWS][AES_COLUMNS];

    memcpy(state, FIPS_PLAINTEXT_STATE, sizeof state);
    AddRoundKey(state, FIPS_KEY_STATE);
    check_state("AddRoundKey : clair XOR clé (FIPS-197 B)", state, FIPS_ROUND1_START);
}

/* Le XOR est une involution : appliquer deux fois la même clé redonne l'état initial. */
static void test_add_round_key_involution(void)
{
    uint8_t state[AES_ROWS][AES_COLUMNS];

    memcpy(state, FIPS_PLAINTEXT_STATE, sizeof state);
    AddRoundKey(state, FIPS_KEY_STATE);
    AddRoundKey(state, FIPS_KEY_STATE);
    check_state("AddRoundKey : involution", state, FIPS_PLAINTEXT_STATE);
}

int main(void)
{
    printf("== add_round_key ==\n");
    test_add_round_key();
    test_add_round_key_involution();
    return test_report();
}
