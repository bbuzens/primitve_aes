#ifndef AES_H
#define AES_H

/*
 * Interface publique de la primitive AES-128 (FIPS-197).
 *
 * Implémentation pédagogique, non durcie : ne pas l'utiliser pour protéger de vraies données
 * (voir « Limites connues » dans le README).
 *
 * Utilisation :
 *
 *     uint8_t w[AES_EXPANDED_KEY_WORDS][AES_WORD_SIZE];
 *     KeyExpansion(key, w);              // une fois par clé
 *     Cipher(plaintext, ciphertext, w);  // un bloc de 16 octets
 *
 * Compilation d'un programme utilisateur :
 *
 *     cc -std=c23 -I<projet>/include mon_programme.c -L<projet>/build -laes
 *
 * Les fonctions gardent les noms de la FIPS-197. Les fonctions internes de la bibliothèque
 * (SubBytes, xtime, Rcon...) ne sont pas déclarées ici, mais leurs symboles sont présents
 * dans libaes.a : un programme utilisateur ne doit pas définir de fonctions de mêmes noms.
 */

#include <stdint.h>

#define AES_BLOCK_SIZE          16  /* octets par bloc */
#define AES_KEY_SIZE            16  /* octets de clé (AES-128) */
#define AES_WORD_SIZE           4   /* octets par mot */
#define AES_EXPANDED_KEY_WORDS  44  /* mots de la clé étendue : 4 x (10 tours + 1) */

/* KeyExpansion (FIPS-197 §5.2) : étend la clé en 44 mots w[0..43]. */
void KeyExpansion(const uint8_t key[AES_KEY_SIZE],
                  uint8_t w[AES_EXPANDED_KEY_WORDS][AES_WORD_SIZE]);

/* Cipher (FIPS-197 §5.1) : chiffre un bloc de 16 octets avec la clé étendue w. */
void Cipher(const uint8_t in[AES_BLOCK_SIZE],
            uint8_t out[AES_BLOCK_SIZE],
            const uint8_t w[AES_EXPANDED_KEY_WORDS][AES_WORD_SIZE]);

#endif
