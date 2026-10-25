#include <stdio.h>

#include "gf_multiply.h"
#include "xtime.h"
#include "test_utils.h"

/* FIPS-197 §4.2 et §4.2.1. */
static void test_gf_multiply_fips_examples(void)
{
    check_byte("{57} . {83} = {c1}", gf_multiply(0x57, 0x83), 0xc1);
    check_byte("{57} . {13} = {fe}", gf_multiply(0x57, 0x13), 0xfe);
}

/* {53} et {ca} sont inverses l'un de l'autre (FIPS-197 §4.2). */
static void test_gf_multiply_inverse_example(void)
{
    check_byte("{53} . {ca} = {01}", gf_multiply(0x53, 0xca), 0x01);
}

/* Propriétés vérifiées sur tous les octets. */
static void test_gf_multiply_properties(void)
{
    bool identity = true;
    bool zero = true;
    bool times_two_is_xtime = true;
    bool commutative = true;

    for (int a = 0; a < 256; a++) {
        identity           &= gf_multiply(a, 0x01) == a;
        zero               &= gf_multiply(a, 0x00) == 0x00;
        times_two_is_xtime &= gf_multiply(a, 0x02) == xtime(a);
        for (int b = 0; b < 256; b++)
            commutative &= gf_multiply(a, b) == gf_multiply(b, a);
    }

    check_true("a . {01} = a, pour tout a", identity);
    check_true("a . {00} = {00}, pour tout a", zero);
    check_true("a . {02} = xtime(a), pour tout a", times_two_is_xtime);
    check_true("a . b = b . a, pour tous a, b", commutative);
}

int main(void)
{
    printf("== gf_multiply ==\n");
    test_gf_multiply_fips_examples();
    test_gf_multiply_inverse_example();
    test_gf_multiply_properties();
    return test_report();
}
