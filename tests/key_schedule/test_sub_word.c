#include <stdio.h>

#include "sub_word.h"
#include "test_utils.h"

/* FIPS-197 annexe A.1, après RotWord : i = 4 et i = 8. */
static void test_sub_word_fips_examples(void)
{
    uint8_t word_i4[AES_WORD_SIZE] = { 0xcf, 0x4f, 0x3c, 0x09 };
    static const uint8_t expected_i4[AES_WORD_SIZE] = { 0x8a, 0x84, 0xeb, 0x01 };
    uint8_t word_i8[AES_WORD_SIZE] = { 0x6c, 0x76, 0x05, 0x2a };
    static const uint8_t expected_i8[AES_WORD_SIZE] = { 0x50, 0x38, 0x6b, 0xe5 };

    SubWord(word_i4);
    check_bytes("SubWord(cf4f3c09) = 8a84eb01 (FIPS-197 A.1, i = 4)", word_i4, expected_i4, AES_WORD_SIZE);
    SubWord(word_i8);
    check_bytes("SubWord(6c76052a) = 50386be5 (FIPS-197 A.1, i = 8)", word_i8, expected_i8, AES_WORD_SIZE);
}

/* S({00}) = {63} : un mot nul devient 63636363. */
static void test_sub_word_zero(void)
{
    uint8_t word[AES_WORD_SIZE] = { 0x00, 0x00, 0x00, 0x00 };
    static const uint8_t expected[AES_WORD_SIZE] = { 0x63, 0x63, 0x63, 0x63 };

    SubWord(word);
    check_bytes("SubWord(00000000) = 63636363", word, expected, AES_WORD_SIZE);
}

int main(void)
{
    printf("== sub_word ==\n");
    test_sub_word_fips_examples();
    test_sub_word_zero();
    return test_report();
}
