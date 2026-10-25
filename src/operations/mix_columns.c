#include "mix_columns.h"
#include "gf_multiply.h"

void MixColumns(uint8_t state[AES_ROWS][AES_COLUMNS])
{
    for (int column = 0; column < AES_COLUMNS; column++) {
        /*
         * On travaille sur une copie de la colonne : chaque nouvel octet dépend
         * des 4 octets d'origine, qu'il ne faut pas écraser en cours de calcul.
         */
        uint8_t original_column[AES_ROWS];
        for (int row = 0; row < AES_ROWS; row++)
            original_column[row] = state[row][column];

        /* Produit matrice x colonne : ligne de la matrice "scalaire" colonne d'origine. */
        for (int row = 0; row < AES_ROWS; row++) {
            uint8_t sum = 0;

            for (int k = 0; k < AES_ROWS; k++)
                sum ^= gf_multiply(AES_MIX_COLUMNS_MATRIX[row][k], original_column[k]);

            state[row][column] = sum;
        }
    }
}
