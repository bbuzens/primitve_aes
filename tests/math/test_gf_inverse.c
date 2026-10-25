#include <stdio.h>

#include "gf_inverse.h"
#include "gf_multiply.h"
#include "test_utils.h"

/* FIPS-197 §4.2 : {53} et {ca} sont inverses l'un de l'autre. */
static void test_gf_inverse_fips_example(void)
{
    check_byte("{53}^-1 = {ca}", gf_inverse(0x53), 0xca);
    check_byte("{ca}^-1 = {53}", gf_inverse(0xca), 0x53);
}

static void test_gf_inverse_special_values(void)
{
    check_byte("{00} -> {00} (convention de la S-box)", gf_inverse(0x00), 0x00);
    check_byte("{01}^-1 = {01}", gf_inverse(0x01), 0x01);
}

/* Propriétés vérifiées sur tous les octets non nuls. */
static void test_gf_inverse_properties(void)
{
    bool is_inverse = true;
    bool is_involution = true;

    for (int a = 1; a < 256; a++) {
        is_inverse    &= gf_multiply(a, gf_inverse(a)) == 0x01;
        is_involution &= gf_inverse(gf_inverse(a)) == a;
    }

    check_true("a . a^-1 = {01}, pour tout a non nul", is_inverse);
    check_true("(a^-1)^-1 = a, pour tout a non nul", is_involution);
}

int main(void)
{
    printf("== gf_inverse ==\n");
    test_gf_inverse_fips_example();
    test_gf_inverse_special_values();
    test_gf_inverse_properties();
    return test_report();
}
