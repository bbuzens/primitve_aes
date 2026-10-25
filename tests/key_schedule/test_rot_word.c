#include <stdio.h>

#include "rot_word.h"
#include "test_utils.h"

/* FIPS-197 annexe A.1, i = 4 : temp = w[3] = 09cf4f3c, RotWord -> cf4f3c09. */
static void test_rot_word_fips_example(void)
{
    uint8_t word[AES_WORD_SIZE] = { 0x09, 0xcf, 0x4f, 0x3c };
    static const uint8_t expected[AES_WORD_SIZE] = { 0xcf, 0x4f, 0x3c, 0x09 };

    RotWord(word);
    check_bytes("RotWord(09cf4f3c) = cf4f3c09 (FIPS-197 A.1)", word, expected, AES_WORD_SIZE);
}

/* Des octets numérotés montrent directement la rotation. */
static void test_rot_word_positions(void)
{
    uint8_t word[AES_WORD_SIZE] = { 0x00, 0x01, 0x02, 0x03 };
    static const uint8_t expected[AES_WORD_SIZE] = { 0x01, 0x02, 0x03, 0x00 };

    RotWord(word);
    check_bytes("RotWord([00,01,02,03]) = [01,02,03,00]", word, expected, AES_WORD_SIZE);
}

/* 4 rotations d'une case ramènent le mot à son état initial. */
static void test_rot_word_four_times_is_identity(void)
{
    uint8_t word[AES_WORD_SIZE] = { 0x09, 0xcf, 0x4f, 0x3c };
    static const uint8_t expected[AES_WORD_SIZE] = { 0x09, 0xcf, 0x4f, 0x3c };

    for (int i = 0; i < 4; i++)
        RotWord(word);
    check_bytes("RotWord appliqué 4 fois = identité", word, expected, AES_WORD_SIZE);
}

int main(void)
{
    printf("== rot_word ==\n");
    test_rot_word_fips_example();
    test_rot_word_positions();
    test_rot_word_four_times_is_identity();
    return test_report();
}
