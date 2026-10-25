#include <stdio.h>

#include "key_expansion.h"
#include "fips197_vectors.h"
#include "test_utils.h"

/* Les 4 premiers mots sont la clé elle-même. */
static void test_key_expansion_copies_key(void)
{
    uint8_t w[AES_EXPANDED_KEY_WORDS][AES_WORD_SIZE];

    KeyExpansion(FIPS_KEY_BYTES, w);
    check_bytes("w[0..3] = clé", &w[0][0], FIPS_KEY_BYTES, AES_KEY_SIZE);
}

/* FIPS-197 annexe A.1 : premier mot calculé, w[4] = w[0] ^ (SubWord(RotWord(w[3])) ^ Rcon(1)). */
static void test_key_expansion_first_derived_word(void)
{
    uint8_t w[AES_EXPANDED_KEY_WORDS][AES_WORD_SIZE];

    KeyExpansion(FIPS_KEY_BYTES, w);
    check_bytes("w[4] = a0fafe17 (FIPS-197 A.1)", w[4], FIPS_EXPANDED_KEY[4], AES_WORD_SIZE);
}

/* FIPS-197 annexe A.1 : les 44 mots, comparés un par un pour localiser une erreur éventuelle. */
static void test_key_expansion_all_words(void)
{
    uint8_t w[AES_EXPANDED_KEY_WORDS][AES_WORD_SIZE];

    KeyExpansion(FIPS_KEY_BYTES, w);
    for (int i = 0; i < AES_EXPANDED_KEY_WORDS; i++) {
        char name[64];
        snprintf(name, sizeof name, "w[%d] (FIPS-197 A.1)", i);
        check_bytes(name, w[i], FIPS_EXPANDED_KEY[i], AES_WORD_SIZE);
    }
}

int main(void)
{
    printf("== key_expansion ==\n");
    test_key_expansion_copies_key();
    test_key_expansion_first_derived_word();
    test_key_expansion_all_words();
    return test_report();
}
