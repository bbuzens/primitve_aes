#include <stdio.h>

#include "affine_transform.h"
#include "test_utils.h"

/* Avec tous les bits à 0, il ne reste que la constante c = {63}. */
static void test_affine_transform_zero(void)
{
    check_byte("affine({00}) = {63}", affine_transform(0x00), 0x63);
}

/*
 * FIPS-197 §5.1.1 : S({53}) = {ed}, et {53}^-1 = {ca} (§4.2), donc affine({ca}) = {ed}.
 * De même S({01}) = {7c} et {01}^-1 = {01}, donc affine({01}) = {7c}.
 */
static void test_affine_transform_fips_examples(void)
{
    check_byte("affine({ca}) = {ed}", affine_transform(0xca), 0xed);
    check_byte("affine({01}) = {7c}", affine_transform(0x01), 0x7c);
}

/* La transformation est bijective : les 256 images sont toutes différentes. */
static void test_affine_transform_is_bijective(void)
{
    bool seen[256] = { false };
    bool bijective = true;

    for (int byte = 0; byte < 256; byte++) {
        uint8_t image = affine_transform(byte);
        bijective &= !seen[image];
        seen[image] = true;
    }

    check_true("affine est bijective", bijective);
}

int main(void)
{
    printf("== affine_transform ==\n");
    test_affine_transform_zero();
    test_affine_transform_fips_examples();
    test_affine_transform_is_bijective();
    return test_report();
}
