#include <stdio.h>
#include <string.h>

#include "test_utils.h"

static int failures = 0;

static void print_bytes(const uint8_t *bytes, size_t len)
{
    for (size_t i = 0; i < len; i++)
        printf("%02x ", bytes[i]);
    printf("\n");
}

static void print_state_row(const uint8_t state[AES_ROWS][AES_COLUMNS], int row)
{
    for (int column = 0; column < AES_COLUMNS; column++)
        printf("%02x ", state[row][column]);
}

void check_true(const char *name, bool condition)
{
    if (condition) {
        printf("[OK]   %s\n", name);
        return;
    }
    failures++;
    printf("[FAIL] %s\n", name);
}

void check_byte(const char *name, uint8_t got, uint8_t expected)
{
    if (got == expected) {
        printf("[OK]   %s\n", name);
        return;
    }
    failures++;
    printf("[FAIL] %s\n", name);
    printf("       attendu : %02x\n", expected);
    printf("       obtenu  : %02x\n", got);
}

void check_bytes(const char *name, const uint8_t *got, const uint8_t *expected, size_t len)
{
    if (memcmp(got, expected, len) == 0) {
        printf("[OK]   %s\n", name);
        return;
    }
    failures++;
    printf("[FAIL] %s\n", name);
    printf("       attendu : ");
    print_bytes(expected, len);
    printf("       obtenu  : ");
    print_bytes(got, len);
}

void check_state(const char *name,
                 const uint8_t got[AES_ROWS][AES_COLUMNS],
                 const uint8_t expected[AES_ROWS][AES_COLUMNS])
{
    if (memcmp(got, expected, AES_BLOCK_SIZE) == 0) {
        printf("[OK]   %s\n", name);
        return;
    }
    failures++;
    printf("[FAIL] %s\n", name);
    printf("       attendu        obtenu\n");
    for (int row = 0; row < AES_ROWS; row++) {
        printf("       ");
        print_state_row(expected, row);
        printf("   ");
        print_state_row(got, row);
        printf("\n");
    }
}

int test_report(void)
{
    if (failures) {
        printf("%d test(s) en échec\n\n", failures);
        return 1;
    }
    printf("Tous les tests passent\n\n");
    return 0;
}
