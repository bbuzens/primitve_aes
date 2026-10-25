/*
 * Test de l'interface publique, écrit comme un programme utilisateur :
 * il n'inclut que "aes.h", est compilé avec -Iinclude seulement et lié à libaes.a.
 * Il n'utilise donc pas test_utils (qui dépend des en-têtes internes).
 */
#include <stdio.h>
#include <string.h>

#include "aes.h"

static int failures = 0;

static void check_encryption(const char *name,
                             const uint8_t key[AES_KEY_SIZE],
                             const uint8_t plaintext[AES_BLOCK_SIZE],
                             const uint8_t expected[AES_BLOCK_SIZE])
{
    uint8_t w[AES_EXPANDED_KEY_WORDS][AES_WORD_SIZE];
    uint8_t ciphertext[AES_BLOCK_SIZE];

    KeyExpansion(key, w);
    Cipher(plaintext, ciphertext, w);

    if (memcmp(ciphertext, expected, AES_BLOCK_SIZE) == 0) {
        printf("[OK]   %s\n", name);
        return;
    }
    failures++;
    printf("[FAIL] %s\n       obtenu  : ", name);
    for (int i = 0; i < AES_BLOCK_SIZE; i++)
        printf("%02x ", ciphertext[i]);
    printf("\n");
}

/* FIPS-197 annexe B. */
static void test_public_api_fips_appendix_b(void)
{
    static const uint8_t key[AES_KEY_SIZE] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,
    };
    static const uint8_t plaintext[AES_BLOCK_SIZE] = {
        0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
        0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34,
    };
    static const uint8_t expected[AES_BLOCK_SIZE] = {
        0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb,
        0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32,
    };

    check_encryption("aes.h : FIPS-197 annexe B", key, plaintext, expected);
}

/* FIPS-197 annexe C.1. */
static void test_public_api_fips_appendix_c1(void)
{
    static const uint8_t key[AES_KEY_SIZE] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    };
    static const uint8_t plaintext[AES_BLOCK_SIZE] = {
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
    };
    static const uint8_t expected[AES_BLOCK_SIZE] = {
        0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30,
        0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a,
    };

    check_encryption("aes.h : FIPS-197 annexe C.1", key, plaintext, expected);
}

int main(void)
{
    printf("== public_api (aes.h + libaes.a) ==\n");
    test_public_api_fips_appendix_b();
    test_public_api_fips_appendix_c1();

    if (failures) {
        printf("%d test(s) en échec\n\n", failures);
        return 1;
    }
    printf("Tous les tests passent\n\n");
    return 0;
}
