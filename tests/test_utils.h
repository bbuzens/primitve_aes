#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stddef.h>

#include "aes_config.h"

/* Vérifie une condition (utile pour les propriétés testées sur tous les octets). */
void check_true(const char *name, bool condition);

/* Compare deux octets et les affiche en hexadécimal en cas d'écart. */
void check_byte(const char *name, uint8_t got, uint8_t expected);

/* Compare deux suites d'octets et affiche l'attendu/l'obtenu en cas d'écart. */
void check_bytes(const char *name, const uint8_t *got, const uint8_t *expected, size_t len);

/* Compare deux états 4x4 et les affiche sous forme de grille en cas d'écart. */
void check_state(const char *name,
                 const uint8_t got[AES_ROWS][AES_COLUMNS],
                 const uint8_t expected[AES_ROWS][AES_COLUMNS]);

/* Affiche le bilan et renvoie le code de sortie du programme de test (0 = succès). */
int test_report(void);

#endif
