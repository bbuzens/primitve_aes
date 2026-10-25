#ifndef AES_CONFIG_H
#define AES_CONFIG_H

#include <stdint.h>

/*
 * L'état AES est une matrice de 4 lignes x 4 colonnes d'octets,
 * indexée state[ligne][colonne], comme s[r][c] dans la FIPS-197 (§3.4).
 *
 * Un bloc de 16 octets remplit l'état colonne par colonne :
 *
 *     in[0]  in[4]  in[8]  in[12]
 *     in[1]  in[5]  in[9]  in[13]
 *     in[2]  in[6]  in[10] in[14]
 *     in[3]  in[7]  in[11] in[15]
 *
 * Les clés de tour utilisent la même disposition que l'état.
 */

#define AES_ROWS       4
#define AES_COLUMNS    4
#define AES_BLOCK_SIZE (AES_ROWS * AES_COLUMNS)

/*
 * Paramètres de la norme (FIPS-197 §5, figure 4), pour AES-128 :
 *   Nb : nombre de colonnes (mots de 4 octets) de l'état, toujours 4 ;
 *   Nk : nombre de mots de la clé : 4 (AES-128), 6 (AES-192), 8 (AES-256) ;
 *   Nr : nombre de tours         : 10 (AES-128), 12 (AES-192), 14 (AES-256).
 */
#define AES_WORD_SIZE 4
#define AES_NB        AES_COLUMNS
#define AES_NK        4
#define AES_NR        10

#define AES_KEY_SIZE           (AES_WORD_SIZE * AES_NK)  /* 16 octets */
#define AES_EXPANDED_KEY_WORDS (AES_NB * (AES_NR + 1))   /* 44 mots : une clé par tour, plus la clé initiale */

/*
 * Polynôme irréductible de GF(2^8) choisi par AES (FIPS-197 §4.2) :
 *     m(x) = x^8 + x^4 + x^3 + x + 1  ->  0x11b
 * Un octet ne contient que les termes x^7..x^0 : lors d'une réduction, le terme x^8
 * disparaît de lui-même et il ne reste qu'à ajouter (XOR) les termes bas, 0x1b.
 */
#define AES_REDUCTION_POLYNOMIAL 0x1b

/* Constante c = {63} = 01100011 de la transformation affine de la S-box (FIPS-197 §5.1.1). */
#define AES_SBOX_AFFINE_CONSTANT 0x63

/*
 * Matrice de MixColumns (FIPS-197 §5.1.3, équation 5.6).
 * Chaque colonne de l'état est multipliée par cette matrice, dans GF(2^8).
 */
static const uint8_t AES_MIX_COLUMNS_MATRIX[AES_ROWS][AES_ROWS] = {
    { 0x02, 0x03, 0x01, 0x01 },
    { 0x01, 0x02, 0x03, 0x01 },
    { 0x01, 0x01, 0x02, 0x03 },
    { 0x03, 0x01, 0x01, 0x02 },
};

#endif
