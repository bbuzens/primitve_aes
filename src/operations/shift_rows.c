#include <string.h>

#include "shift_rows.h"

void ShiftRows(uint8_t state[AES_ROWS][AES_COLUMNS])
{
    for (int row = 0; row < AES_ROWS; row++) {
        /*
         * On travaille sur une copie de la ligne : en écrivant directement dans l'état,
         * on écraserait des octets qui n'ont pas encore été déplacés.
         */
        uint8_t original_row[AES_COLUMNS];
        memcpy(original_row, state[row], sizeof original_row);

        int shift = row; /* la ligne r est décalée de r cases */

        for (int column = 0; column < AES_COLUMNS; column++)
            state[row][column] = original_row[(column + shift) % AES_COLUMNS];
    }
}
