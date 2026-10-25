#include <stdio.h>
#include <string.h>

#include "state.h"
#include "fips197_vectors.h"
#include "test_utils.h"

/* Le bloc doit être rangé colonne par colonne : in[r + 4c] -> state[r][c]. */
static void test_bytes_to_state(void)
{
    uint8_t state[AES_ROWS][AES_COLUMNS];

    BytesToState(FIPS_PLAINTEXT_BYTES, state);
    check_state("BytesToState : clair FIPS-197 B", state, FIPS_PLAINTEXT_STATE);
}

static void test_state_to_bytes(void)
{
    uint8_t out[AES_BLOCK_SIZE];

    StateToBytes(FIPS_PLAINTEXT_STATE, out);
    check_bytes("StateToBytes : clair FIPS-197 B", out, FIPS_PLAINTEXT_BYTES, AES_BLOCK_SIZE);
}

/* Les deux conversions sont inverses l'une de l'autre. */
static void test_round_trip(void)
{
    uint8_t state[AES_ROWS][AES_COLUMNS];
    uint8_t out[AES_BLOCK_SIZE];

    BytesToState(FIPS_PLAINTEXT_BYTES, state);
    StateToBytes(state, out);
    check_bytes("BytesToState puis StateToBytes : aller-retour", out, FIPS_PLAINTEXT_BYTES, AES_BLOCK_SIZE);
}

int main(void)
{
    printf("== state ==\n");
    test_bytes_to_state();
    test_state_to_bytes();
    test_round_trip();
    return test_report();
}
