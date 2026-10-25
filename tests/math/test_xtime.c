#include <stdio.h>

#include "xtime.h"
#include "test_utils.h"

/* FIPS-197 §4.2.1 : puissances successives de x appliquées à {57}. */
static void test_xtime_fips_example(void)
{
    check_byte("xtime({57}) = {ae}", xtime(0x57), 0xae);
    check_byte("xtime({ae}) = {47}", xtime(0xae), 0x47);
    check_byte("xtime({47}) = {8e}", xtime(0x47), 0x8e);
    check_byte("xtime({8e}) = {07}", xtime(0x8e), 0x07);
}

/* Sans débordement (bit x^7 à 0) : simple décalage à gauche. */
static void test_xtime_without_reduction(void)
{
    check_byte("xtime({01}) = {02}", xtime(0x01), 0x02);
    check_byte("xtime({40}) = {80}", xtime(0x40), 0x80);
}

/* Avec débordement (bit x^7 à 1) : x^8 est remplacé par x^4 + x^3 + x + 1 = {1b}. */
static void test_xtime_with_reduction(void)
{
    check_byte("xtime({80}) = {1b}", xtime(0x80), 0x1b);
    check_byte("xtime({ff}) = {e5}", xtime(0xff), 0xe5);
}

static void test_xtime_zero(void)
{
    check_byte("xtime({00}) = {00}", xtime(0x00), 0x00);
}

int main(void)
{
    printf("== xtime ==\n");
    test_xtime_fips_example();
    test_xtime_without_reduction();
    test_xtime_with_reduction();
    test_xtime_zero();
    return test_report();
}
