#include "sbox.h"
#include "affine_transform.h"
#include "gf_inverse.h"

uint8_t SBox(uint8_t byte)
{
    uint8_t inverse = gf_inverse(byte);

    return affine_transform(inverse);
}
