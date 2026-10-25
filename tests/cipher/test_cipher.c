#include <stdio.h>
#include <string.h>

#include "cipher.h"
#include "key_expansion.h"
#include "fips197_vectors.h"
#include "test_utils.h"

/* Étend la clé, chiffre le bloc et compare au chiffré attendu. */
static void check_encryption(const char *name,
                             const uint8_t key[AES_KEY_SIZE],
                             const uint8_t plaintext[AES_BLOCK_SIZE],
                             const uint8_t expected[AES_BLOCK_SIZE])
{
    uint8_t w[AES_EXPANDED_KEY_WORDS][AES_WORD_SIZE];
    uint8_t ciphertext[AES_BLOCK_SIZE];

    KeyExpansion(key, w);
    Cipher(plaintext, ciphertext, w);
    check_bytes(name, ciphertext, expected, AES_BLOCK_SIZE);
}

/* FIPS-197 annexe B : le vecteur suivi pas à pas depuis le début du projet. */
static void test_cipher_fips_appendix_b(void)
{
    check_encryption("Cipher : FIPS-197 annexe B",
                     FIPS_KEY_BYTES, FIPS_PLAINTEXT_BYTES, FIPS_CIPHERTEXT_BYTES);
}

/* FIPS-197 annexe C.1 : un second vecteur officiel, indépendant du premier. */
static void test_cipher_fips_appendix_c1(void)
{
    check_encryption("Cipher : FIPS-197 annexe C.1",
                     FIPS_C1_KEY_BYTES, FIPS_C1_PLAINTEXT_BYTES, FIPS_C1_CIPHERTEXT_BYTES);
}

/* Clé nulle et clair nul : valeur de référence obtenue avec OpenSSL (aes-128-ecb). */
static void test_cipher_all_zero(void)
{
    static const uint8_t zero[AES_BLOCK_SIZE] = { 0 };
    static const uint8_t expected[AES_BLOCK_SIZE] = {
        0x66, 0xe9, 0x4b, 0xd4, 0xef, 0x8a, 0x2c, 0x3b,
        0x88, 0x4c, 0xfa, 0x59, 0xca, 0x34, 0x2b, 0x2e,
    };

    check_encryption("Cipher : clé nulle, clair nul", zero, zero, expected);
}

/* Le bloc d'entrée ne doit pas être modifié par le chiffrement. */
static void test_cipher_does_not_modify_input(void)
{
    uint8_t w[AES_EXPANDED_KEY_WORDS][AES_WORD_SIZE];
    uint8_t plaintext[AES_BLOCK_SIZE];
    uint8_t ciphertext[AES_BLOCK_SIZE];

    memcpy(plaintext, FIPS_PLAINTEXT_BYTES, AES_BLOCK_SIZE);
    KeyExpansion(FIPS_KEY_BYTES, w);
    Cipher(plaintext, ciphertext, w);
    check_bytes("Cipher : bloc d'entrée inchangé", plaintext, FIPS_PLAINTEXT_BYTES, AES_BLOCK_SIZE);
}

int main(void)
{
    printf("== cipher ==\n");
    test_cipher_fips_appendix_b();
    test_cipher_fips_appendix_c1();
    test_cipher_all_zero();
    test_cipher_does_not_modify_input();
    return test_report();
}
