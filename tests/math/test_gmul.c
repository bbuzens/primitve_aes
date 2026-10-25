#include <stdio.h>

#include "gmul.h"
#include "xtime.h"
#include "test_utils.h"

/* FIPS-197 §4.2 et §4.2.1. */
static void test_gmul_fips_examples(void)
{
    check_byte("{57} . {83} = {c1}", gmul(0x57, 0x83), 0xc1);
    check_byte("{57} . {13} = {fe}", gmul(0x57, 0x13), 0xfe);
}

/* {53} et {ca} sont inverses l'un de l'autre (FIPS-197 §4.2). */
static void test_gmul_inverse_example(void)
{
    check_byte("{53} . {ca} = {01}", gmul(0x53, 0xca), 0x01);
}

/* Propriétés vérifiées sur tous les octets. */
static void test_gmul_properties(void)
{
    bool identity = true;
    bool zero = true;
    bool times_two_is_xtime = true;
    bool commutative = true;

    for (int a = 0; a < 256; a++) {
        identity           &= gmul(a, 0x01) == a;
        zero               &= gmul(a, 0x00) == 0x00;
        times_two_is_xtime &= gmul(a, 0x02) == xtime(a);
        for (int b = 0; b < 256; b++)
            commutative &= gmul(a, b) == gmul(b, a);
    }

    check_true("a . {01} = a, pour tout a", identity);
    check_true("a . {00} = {00}, pour tout a", zero);
    check_true("a . {02} = xtime(a), pour tout a", times_two_is_xtime);
    check_true("a . b = b . a, pour tous a, b", commutative);
}

int main(void)
{
    printf("== gmul ==\n");
    test_gmul_fips_examples();
    test_gmul_inverse_example();
    test_gmul_properties();
    return test_report();
}
